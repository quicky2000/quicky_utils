/*    This file is part of quicky_utils
      Copyright (C) 2017  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifdef QUICKY_UTILS_SELF_TEST
#include "type_string.h"
#include "quicky_exception.h"
#include "quicky_bitfield.h"
#include "safe_types.h"
#include "ext_uint.h"
#include "fract.h"
#include <iostream>
#include <functional>
#include <sstream>

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

/**
 * Method regrouping tests of type_string class
 */
void test_type_string();

template <typename FRACT_INTERNAL_TYPE>
void test_fract(void);

void test_safe_types(void);

template <typename SAFE_TYPE, typename REFERENCE_TYPE>
void test_safe_type(void);

template <typename SOURCE_TYPE, typename TARGET_TYPE>
void test_type_conversion(const typename SOURCE_TYPE::base_type & p_value,
                          bool p_exception_expected
                         );

/**
 * Method regrouping tests of ext_uint class
 */
void test_ext_uint();

//------------------------------------------------------------------------------
int main(int argc,char ** argv)
{
    try
    {
        test_ext_uint();
        test_type_string();
        test_fract<uint32_t>();
        test_fract<quicky_utils::safe_int<int32_t>>();
        test_safe_types();
    }
    catch (quicky_exception::quicky_runtime_exception & e)
    {
        std::cout << "ERROR : " << e.what() << " " << e.get_file() << ":" << e.get_line() << std::endl;
        return (-1);
    }
    catch (quicky_exception::quicky_logic_exception & e)
    {
        std::cout << "ERROR : " << e.what() << " " << e.get_file() << ":" << e.get_line() << std::endl;
        return (-1);
    }
    return 0;
}

/**
 * Method checking type string representation against expected
 * @tparam T
 */
template <typename T>
void check_a_type_string(const std::string & p_reference)
{
    std::stringstream l_result;
    l_result <<  quicky_utils::type_string<T>::name();
    std::cout << "Reference \"" + p_reference + "\"\t Result \"" << l_result.str() << "\"" << std::endl;
    if(p_reference != l_result.str())
    {
        throw quicky_exception::quicky_logic_exception("type_string result don't match with refence", __LINE__, __FILE__);
    }

}
#define check(type) check_a_type_string<type>(#type)
#define check_name(type,type_name) check_a_type_string<type>(type_name)
#define check_prefix(prefix,type) check_a_type_string<prefix::type>(#type)

//-----------------------------------------------------------------------------
void test_type_string()
{
    check(uint8_t);
    check(uint16_t);
    check(uint32_t);
    check(uint64_t);

    check(int8_t);
    check(int16_t);
    check(int32_t);
    check(int64_t);

    check(char);
    check(double);
    check(float);
    check(std::string);

    check_name(quicky_utils::safe_uint32_t,"safe_uint<uint32_t>");
    check_name(quicky_utils::safe_uint64_t,"safe_uint<uint64_t>");

    check_prefix(quicky_utils,safe_uint<uint8_t>);
    check_name(quicky_utils::safe_uint<uint8_t>,"safe_uint<uint8_t>");
    check_prefix(quicky_utils,safe_uint<uint16_t>);
    check_name(quicky_utils::safe_uint<uint16_t>,"safe_uint<uint16_t>");

    check_name(quicky_utils::safe_int8_t,"safe_int<int8_t>");
    check_name(quicky_utils::safe_int16_t,"safe_int<int16_t>");
    check_name(quicky_utils::safe_int32_t,"safe_int<int32_t>");
    check_name(quicky_utils::safe_int64_t,"safe_int<int64_t>");
}

#if __cplusplus==201402L
auto g_lambda = [] (auto x, auto y, char op)
        {
            switch(op)
            {
                case '+':return x + y;
                case '-':return x - y;
                case '/':return x / y;
                case '*':return x * y;
                case '%':return x % y;
                default: throw quicky_exception::quicky_logic_exception("Unsupported operator '" + std::string(1,op) +"'",__LINE__,__FILE__);
            }
        };
#else
#define g_lambda(x,y,op) (op == '+' ? x + y :(op == '-' ? x - y :(op == '*' ? x * y : (op == '/' ? x / y : (op == '%' ? x % y : (decltype(x))0)))))
#endif // __cplusplus

/**
 * Methods converting infinite type to integer type if possible
 * in contrary case an exception is raised
 * @tparam EXT_INT_TYPE infinite type like infinite_uint
 * @tparam INT_TYPE integer type like uint32_t
 * @param p_ext_int_type extensible integer type variable to convert to integer type
 * @return integer type representing extensible integer type if possible
 */
template <typename INT_TYPE,
          typename EXT_INT_TYPE
          >
INT_TYPE convert(const EXT_INT_TYPE & p_ext_int_type)
{
    if(p_ext_int_type.get_nb_bytes() <= sizeof(INT_TYPE))
    {
        INT_TYPE l_result = 0;
        for(unsigned int l_index = 0;
            l_index < p_ext_int_type.get_nb_words();
            ++l_index
           )
        {
            l_result |= ((INT_TYPE) p_ext_int_type.get_word(l_index)) << (l_index * 8 * sizeof(typename EXT_INT_TYPE::base_type));
        }
        return l_result;
    }
    throw quicky_exception::quicky_logic_exception("Infinite type size is greater than integer type size", __LINE__, __FILE__);
}

/**
 * Check if conversion from extensible integer type to integer type provide the expected result
 * @tparam INT_TYPE integer type
 * @tparam EXT_INT_TYPE extensible integer type
 * @param p_ext_type extensible integer variable
 * @param p_expected expected integer value
 */
template <typename INT_TYPE,
          typename EXT_INT_TYPE
         >
void check_convert(const EXT_INT_TYPE & p_ext_type,
                   const INT_TYPE & p_expected
                  )
{
    INT_TYPE l_result = convert<INT_TYPE,EXT_INT_TYPE>(p_ext_type);
    std::cout << "Reference " << p_expected << "\tResult " << l_result << std::endl;
    if(p_expected != l_result)
    {
        throw quicky_exception::quicky_logic_exception("type_string to integer result don't match with refence", __LINE__, __FILE__);
    }
}

//------------------------------------------------------------------------------
void
test_ext_uint()
{
    quicky_utils::ext_uint<uint8_t> l_zero;
    quicky_utils::ext_uint<uint8_t> l_zero_list_init({0});
    quicky_utils::ext_uint<uint8_t> l_un({1});
    quicky_utils::ext_uint<uint8_t> l_256({0,1});
    quicky_utils::ext_uint<uint8_t> l_max({0xFF, 0xFF, 0xFF, 0xFF});

    std::cout << "Test ext_uint output stream operator" << std::endl;
    std::cout << l_zero << std::endl;
    std::cout << l_un << std::endl;
    std::cout << l_256 << std::endl;

    std::cout << "Test conversion to integer type" << std::endl;
    check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_256, 256);
    check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_zero, 0);
    check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_un, 1);
    check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_max, std::numeric_limits<uint32_t>::max());

    assert(l_zero == l_zero);
    assert(l_zero_list_init == l_zero_list_init);
    assert(l_zero == l_zero_list_init);

    assert(l_un == l_un);
    assert(!(l_zero == l_un));
    assert(!(l_zero_list_init == l_un));

    assert(l_zero < l_un);
    assert(!(l_zero < l_zero));
    assert(!(l_un < l_zero));
    assert(l_zero < l_256);
    assert(!(l_256 < l_zero));

    std::cout << "Test + operator" << std::endl;
    quicky_utils::ext_uint<uint8_t> l_result;
    l_result = l_zero + l_zero;
    std::cout << l_result << std::endl;
    l_result = l_zero + l_un;
    std::cout << l_result << std::endl;
    l_result = quicky_utils::ext_uint<uint8_t>({255}) + l_un;
    std::cout << l_result << std::endl;
    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) + l_un;
    std::cout << l_result << std::endl;
    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) + quicky_utils::ext_uint<uint8_t>({0,1});
    std::cout << l_result << std::endl;
    l_result = quicky_utils::ext_uint<uint8_t>({255}) + quicky_utils::ext_uint<uint8_t>({255});
    std::cout << l_result << std::endl;

    std::cout << "Test - operator" << std::endl;
    l_result = l_zero - l_zero;
    std::cout << l_result << std::endl;
    l_result = l_un - l_un;
    std::cout << l_result << std::endl;
    l_result = l_256 - l_256;
    std::cout << l_result << std::endl;
    l_result = l_256 - l_un;
    std::cout << l_result << std::endl;
    l_result =quicky_utils::ext_uint<uint8_t>({255, 255}) - quicky_utils::ext_uint<uint8_t>({0,1});
    std::cout << l_result << std::endl;
    l_result =quicky_utils::ext_uint<uint8_t>({255, 255}) - quicky_utils::ext_uint<uint8_t>({1});
    std::cout << l_result << std::endl;
    l_result =quicky_utils::ext_uint<uint8_t>({0, 0, 1}) - quicky_utils::ext_uint<uint8_t>({1});
    std::cout << l_result << std::endl;
    l_result =quicky_utils::ext_uint<uint8_t>({0, 0, 2}) - quicky_utils::ext_uint<uint8_t>({1, 1, 1});
    std::cout << l_result << std::endl;

    std::cout << "Test * operator" << std::endl;
    l_result = l_zero * l_zero;
    l_result = l_zero * l_un;
    l_result = l_un * l_zero;
    l_result = l_un * l_un;
    l_result = quicky_utils::ext_uint<uint8_t>({128}) * quicky_utils::ext_uint<uint8_t>({42});
    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) * quicky_utils::ext_uint<uint8_t>({255});
    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) * quicky_utils::ext_uint<uint8_t>({255, 255});
    l_result = quicky_utils::ext_uint<uint8_t>({255, 255, 255, 255}) * quicky_utils::ext_uint<uint8_t>({255, 255, 255, 255});

    l_result = l_256 >> quicky_utils::ext_uint<uint8_t>({1});
     std::cout << l_result << std::endl;
    l_result = l_256 >> quicky_utils::ext_uint<uint8_t>({20});

    l_result = l_un << quicky_utils::ext_uint<uint8_t>({1});
    std::cout << l_result << std::endl;
    l_result = l_un << quicky_utils::ext_uint<uint8_t>({8});
    std::cout << l_result << std::endl;
    l_result = l_un << quicky_utils::ext_uint<uint8_t>({16});
    std::cout << l_result << std::endl;

}

//------------------------------------------------------------------------------
template <typename TYPE>
void check_safe_equal(const TYPE & p_op1,
                      const TYPE & p_op2,
                      bool p_expected
                     )
{
    assert((p_op1 == p_op2) == p_expected);
}

template <typename TYPE>
void check_safe_not_equal(const TYPE & p_op1,
                          const TYPE & p_op2,
                          bool p_expected
                         )
{
    assert((p_op1 != p_op2) == p_expected);
}

void test_safe_types(void)
{
    test_safe_type<typename quicky_utils::safe_uint<uint8_t>, uint32_t>();
    test_safe_type<typename quicky_utils::safe_int<int8_t>, int32_t>();

    quicky_utils::safe_int<int16_t> l_safe_int(-128);
    std::cout << l_safe_int << std::endl;
    l_safe_int = abs(l_safe_int);
    l_safe_int = std::abs(l_safe_int);
    std::cout << l_safe_int << std::endl;
    quicky_utils::safe_uint<uint16_t> l_safe_uint(128);
    std::cout << l_safe_uint << std::endl;
    std::cout << abs(l_safe_int) << std::endl;
    assert(l_safe_uint);
    assert(l_safe_int);
    quicky_utils::fract<quicky_utils::safe_uint<uint16_t>> l_fract(1,
                                                                   12
                                                                  );
    quicky_utils::fract<quicky_utils::safe_uint<uint16_t>> l_fract2(1,
                                                                    4
                                                                   );
    std::cout << (l_fract + l_fract2) << std::endl;

    // Try to change a safe_int in a safe_uint
    test_type_conversion<quicky_utils::safe_int<int8_t>, quicky_utils::safe_uint<uint8_t> >(-128,
                                                                                            true
                                                                                           );
    test_type_conversion<quicky_utils::safe_int<int8_t>, quicky_utils::safe_uint<uint8_t> >(-1,
                                                                                            true
                                                                                           );
    test_type_conversion<quicky_utils::safe_int<int8_t>, quicky_utils::safe_uint<uint8_t> >(0,
                                                                                            false
                                                                                           );

    // Try to change a safe_uint in a safe_int
    test_type_conversion<quicky_utils::safe_uint<uint8_t>, quicky_utils::safe_int<int8_t> >(0,
                                                                                            false
                                                                                           );
    test_type_conversion<quicky_utils::safe_uint<uint8_t>, quicky_utils::safe_int<int8_t> >(127,
                                                                                            false
                                                                                           );
    test_type_conversion<quicky_utils::safe_uint<uint8_t>, quicky_utils::safe_int<int8_t> >(128,
                                                                                            true
                                                                                           );
    // Test equality operator
    check_safe_equal<quicky_utils::safe_uint<uint8_t>>(1,1,true);
    check_safe_equal<quicky_utils::safe_uint<uint8_t>>(1,2,false);
    check_safe_not_equal<quicky_utils::safe_uint<uint8_t>>(1,1,false);
    check_safe_not_equal<quicky_utils::safe_uint<uint8_t>>(1,2,true);
}

template<unsigned int NB>
void display_NB(void)
{
    std::cout << NB << std::endl;
}

template <typename FRACT_INTERNAL_TYPE>
void test_fract(void)
{
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "- PGCD" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den l_pos_num = 3;
    typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_num l_neg_num = -3;
    typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den l_pos_den = 6;
    typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_num l_neg_den = -6;

    typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD( l_pos_num, l_pos_den);
    std::cout << "PGCD( 3, 6) = " << l_result << std::endl;
    assert(3 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(l_neg_num, l_pos_den);
    std::cout << "PGCD(-3, 6) = " << l_result << std::endl;
    assert(3 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD( l_pos_num,l_neg_den);
    std::cout << "PGCD( 3,-6) = " << l_result << std::endl;
    assert(3 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(l_neg_num,l_neg_den);
    std::cout << "PGCD(-3,-6) = " << l_result << std::endl;
    assert(3 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(1071,1029);
    std::cout << "PGCD(1071,1029) = " << l_result << std::endl;
    assert(21 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(1029,1071);
    std::cout << "PGCD(1029,1071) = " << l_result << std::endl;
    assert(21 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(60,168);
    std::cout << "PGCD(60,168) = " << l_result << std::endl;
    assert(12 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(168,60);
    std::cout << "PGCD(168,60) = " << l_result << std::endl;
    assert(12 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(6,9);
    std::cout << "PGCD(6,9) = " << l_result << std::endl;
    assert(3 == l_result);

    std::cout << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "- PPCM" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM( l_pos_num, l_pos_den);
    std::cout << "PPCM( 3, 6) = " << l_result << std::endl;
    assert(6 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_neg_num, l_pos_den);
    std::cout << "PPCM(-3, 6) = " << l_result << std::endl;
    assert(6 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_pos_num,l_neg_den);
    std::cout << "PPCM( 3,-6) = " << l_result << std::endl;
    assert(6 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_neg_num,l_neg_den);
    std::cout << "PPCM(-3,-6) = " << l_result << std::endl;
    assert(6 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(60,168);
    std::cout << "PPCM(60,168) = " << l_result << std::endl;
    assert(840 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(168,60);
    std::cout << "PPCM(168,60) = " << l_result << std::endl;
    assert(840 == l_result);

    l_result = quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(6,9);
    std::cout << "PPCM(6,9) = " << l_result << std::endl;
    assert(18 == l_result);

    std::cout << "---------------------------------------" << std::endl;

    std::cout << quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,1) * quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,1) << std::endl;
    std::cout << quicky_utils::fract<FRACT_INTERNAL_TYPE>(-21,1) / quicky_utils::fract<FRACT_INTERNAL_TYPE>(6,1) << std::endl;
    std::cout << quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,1) - quicky_utils::fract<FRACT_INTERNAL_TYPE>(-21,6) << std::endl;

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_nb1(1,2);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_nb2(2);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_nb3(10,2);
    std::cout << l_nb1 << std::endl;
    std::cout << l_nb2 << std::endl;
    std::cout << l_nb3 << std::endl;

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_a(1,3);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_b(1,4);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_c = l_a + l_b;
    std::cout << l_a << " + " << l_b << " => " << l_c << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,12) == l_c);

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_d = l_a - l_b;
    std::cout << l_a << " - " << l_b << " => " << l_d << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,12) == l_d);

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_e = l_a * l_b;
    std::cout << l_a << " * " << l_b << " => " << l_e << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,12) == l_e);

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_f = l_a / l_b;
    std::cout << l_a << " / " << l_b << " => " << l_f << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,3) == l_f);

    std::cout << 1 << " + " << l_a << " => " << 1 + l_a << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,3) == 1 + l_a);

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_x(1,2);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_y(2,-3);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_z(-3,5);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_x_bis(-1,-2);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_y_bis(-2,3);
    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_z_bis(3,-5);

    // Check equality independantly of - position ( num or den)
    std::cout << l_x << std::endl;
    std::cout << l_x_bis << std::endl;
    assert(l_x == l_x_bis);
    std::cout << l_y << std::endl;
    std::cout << l_y_bis << std::endl;
    assert(l_y == l_y_bis);
    std::cout << l_z << std::endl;
    std::cout << l_z_bis << std::endl;
    assert(l_z == l_z_bis);
    assert(l_x != l_y);

    //------------------------------------------------------------------------
    std::cout << l_x << " + " << l_x << " => " << l_x + l_x << std::endl;
    assert(1 == l_x + l_x);

    std::cout << l_x << " + " << l_y << " => " << l_x + l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_x + l_y);

    std::cout << l_x << " + " << l_z << " => " << l_x + l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_x + l_z);

    std::cout << l_x << " + " << l_x_bis << " => " << l_x + l_x_bis << std::endl;
    assert(1 == l_x + l_x_bis);

    std::cout << l_x << " + " << l_y_bis << " => " << l_x + l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_x + l_y_bis);

    std::cout << l_x << " + " << l_z_bis << " => " << l_x + l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_x + l_z_bis);


    //------------------------------------------------------------------------
    std::cout << l_y << " + " << l_x << " => " << l_y + l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_y + l_x);

    std::cout << l_y << " + " << l_y << " => " << l_y + l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y + l_y);

    std::cout << l_y << " + " << l_z << " => " << l_y + l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_y + l_z);

    std::cout << l_y << " + " << l_x_bis << " => " << l_y + l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_y + l_x_bis);

    std::cout << l_y << " + " << l_y_bis << " => " << l_y + l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y + l_y_bis);

    std::cout << l_y << " + " << l_z_bis << " => " << l_y + l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_y + l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z << " + " << l_x << " => " << l_z + l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_z + l_x);

    std::cout << l_z << " + " << l_y << " => " << l_z + l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_z + l_y);

    std::cout << l_z << " + " << l_z << " => " << l_z + l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z + l_z);

    std::cout << l_z << " + " << l_x_bis << " => " << l_z + l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_z + l_x_bis);

    std::cout << l_z << " + " << l_y_bis << " => " << l_z + l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_z + l_y_bis);

    std::cout << l_z << " + " << l_z_bis << " => " << l_z + l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z + l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_x_bis << " + " << l_x << " => " << l_x_bis + l_x << std::endl;
    assert(1 == l_x_bis + l_x);

    std::cout << l_x_bis << " + " << l_y << " => " << l_x_bis + l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_x_bis + l_y);

    std::cout << l_x_bis << " + " << l_z << " => " << l_x_bis + l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_x_bis + l_z);

    std::cout << l_x_bis << " + " << l_x_bis << " => " << l_x_bis + l_x_bis << std::endl;
    assert(1 == l_x_bis + l_x_bis);

    std::cout << l_x_bis << " + " << l_y_bis << " => " << l_x_bis + l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_x_bis + l_y_bis);

    std::cout << l_x_bis << " + " << l_z_bis << " => " << l_x_bis + l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_x_bis + l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_y_bis << " + " << l_x << " => " << l_y_bis + l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_y_bis + l_x);

    std::cout << l_y_bis << " + " << l_y << " => " << l_y_bis + l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y_bis + l_y);

    std::cout << l_y_bis << " + " << l_z << " => " << l_y_bis + l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_y_bis + l_z);

    std::cout << l_y_bis << " + " << l_x_bis << " => " << l_y_bis + l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6) == l_y_bis + l_x_bis);

    std::cout << l_y_bis << " + " << l_y_bis << " => " << l_y_bis + l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y_bis + l_y_bis);

    std::cout << l_y_bis << " + " << l_z_bis << " => " << l_y_bis + l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_y_bis + l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z_bis << " + " << l_x << " => " << l_z_bis + l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_z_bis + l_x);

    std::cout << l_z_bis << " + " << l_y << " => " << l_z_bis + l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_z_bis + l_y);

    std::cout << l_z_bis << " + " << l_z << " => " << l_z_bis + l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z_bis + l_z);

    std::cout << l_z_bis << " + " << l_x_bis << " => " << l_z_bis + l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10) == l_z_bis + l_x_bis);

    std::cout << l_z_bis << " + " << l_y_bis << " => " << l_z_bis + l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15) == l_z_bis + l_y_bis);

    std::cout << l_z_bis << " + " << l_z_bis << " => " << l_z_bis + l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z_bis + l_z_bis);

    //----------------------------------------
    // Operator -
    //----------------------------------------
    //------------------------------------------------------------------------
    std::cout << l_x << " - " << l_x << " => " << l_x - l_x << std::endl;
    assert(0 == l_x - l_x);

    std::cout << l_x << " - " << l_y << " => " << l_x - l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6) == l_x - l_y);

    std::cout << l_x << " - " << l_z << " => " << l_x - l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10) == l_x - l_z);

    std::cout << l_x << " - " << l_x_bis << " => " << l_x - l_x_bis << std::endl;
    assert(0 == l_x - l_x_bis);

    std::cout << l_x << " - " << l_y_bis << " => " << l_x - l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6) == l_x - l_y_bis);

    std::cout << l_x << " - " << l_z_bis << " => " << l_x - l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10) == l_x - l_z_bis);


    //------------------------------------------------------------------------
    std::cout << l_y << " - " << l_x << " => " << l_y - l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6) == l_y - l_x);

    std::cout << l_y << " - " << l_y << " => " << l_y - l_y << std::endl;
    assert(0 == l_y - l_y);

    std::cout << l_y << " - " << l_z << " => " << l_y - l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15) == l_y - l_z);

    std::cout << l_y << " - " << l_x_bis << " => " << l_y - l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6) == l_y - l_x_bis);

    std::cout << l_y << " - " << l_y_bis << " => " << l_y - l_y_bis << std::endl;
    assert(0 == l_y - l_y_bis);

    std::cout << l_y << " - " << l_z_bis << " => " << l_y - l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15) == l_y - l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z << " - " << l_x << " => " << l_z - l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10) == l_z - l_x);

    std::cout << l_z << " - " << l_y << " => " << l_z - l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15) == l_z - l_y);

    std::cout << l_z << " - " << l_z << " => " << l_z - l_z << std::endl;
    assert(0 == l_z - l_z);

    std::cout << l_z << " - " << l_x_bis << " => " << l_z - l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10) == l_z - l_x_bis);

    std::cout << l_z << " - " << l_y_bis << " => " << l_z - l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15) == l_z - l_y_bis);

    std::cout << l_z << " - " << l_z_bis << " => " << l_z - l_z_bis << std::endl;
    assert(0 == l_z - l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_x_bis << " - " << l_x << " => " << l_x_bis - l_x << std::endl;
    assert(0 == l_x_bis - l_x);

    std::cout << l_x_bis << " - " << l_y << " => " << l_x_bis - l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6) == l_x_bis - l_y);

    std::cout << l_x_bis << " - " << l_z << " => " << l_x_bis - l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10) == l_x_bis - l_z);

    std::cout << l_x_bis << " - " << l_x_bis << " => " << l_x_bis - l_x_bis << std::endl;
    assert(0 == l_x_bis - l_x_bis);

    std::cout << l_x_bis << " - " << l_y_bis << " => " << l_x_bis - l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6) == l_x_bis - l_y_bis);

    std::cout << l_x_bis << " - " << l_z_bis << " => " << l_x_bis - l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10) == l_x_bis - l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_y_bis << " - " << l_x << " => " << l_y_bis - l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6) == l_y_bis - l_x);

    std::cout << l_y_bis << " - " << l_y << " => " << l_y_bis - l_y << std::endl;
    assert(0 == l_y_bis - l_y);

    std::cout << l_y_bis << " - " << l_z << " => " << l_y_bis - l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15) == l_y_bis - l_z);

    std::cout << l_y_bis << " - " << l_x_bis << " => " << l_y_bis - l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6) == l_y_bis - l_x_bis);

    std::cout << l_y_bis << " - " << l_y_bis << " => " << l_y_bis - l_y_bis << std::endl;
    assert(0 == l_y_bis - l_y_bis);

    std::cout << l_y_bis << " - " << l_z_bis << " => " << l_y_bis - l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15) == l_y_bis - l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z_bis << " - " << l_x << " => " << l_z_bis - l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10) == l_z_bis - l_x);

    std::cout << l_z_bis << " - " << l_y << " => " << l_z_bis - l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15) == l_z_bis - l_y);

    std::cout << l_z_bis << " - " << l_z << " => " << l_z_bis - l_z << std::endl;
    assert(0 == l_z_bis - l_z);

    std::cout << l_z_bis << " - " << l_x_bis << " => " << l_z_bis - l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10) == l_z_bis - l_x_bis);

    std::cout << l_z_bis << " - " << l_y_bis << " => " << l_z_bis - l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15) == l_z_bis - l_y_bis);

    std::cout << l_z_bis << " - " << l_z_bis << " => " << l_z_bis - l_z_bis << std::endl;
    assert(0 == l_z_bis - l_z_bis);


    //----------------------------------------
    // Operator *
    //----------------------------------------
    //------------------------------------------------------------------------
    std::cout << l_x << " * " << l_x << " => " << l_x * l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4) == l_x * l_x);

    std::cout << l_x << " * " << l_y << " => " << l_x * l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6) == l_x * l_y);

    std::cout << l_x << " * " << l_z << " => " << l_x * l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_x * l_z);

    std::cout << l_x << " * " << l_x_bis << " => " << l_x * l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4) == l_x * l_x_bis);

    std::cout << l_x << " * " << l_y_bis << " => " << l_x * l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6) == l_x * l_y_bis);

    std::cout << l_x << " * " << l_z_bis << " => " << l_x * l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_x * l_z_bis);


    //------------------------------------------------------------------------
    std::cout << l_y << " * " << l_x << " => " << l_y * l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6) == l_y * l_x);

    std::cout << l_y << " * " << l_y << " => " << l_y * l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9) == l_y * l_y);

    std::cout << l_y << " * " << l_z << " => " << l_y * l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_y * l_z);

    std::cout << l_y << " * " << l_x_bis << " => " << l_y * l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6) == l_y * l_x_bis);

    std::cout << l_y << " * " << l_y_bis << " => " << l_y * l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9) == l_y * l_y_bis);

    std::cout << l_y << " * " << l_z_bis << " => " << l_y * l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_y * l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z << " * " << l_x << " => " << l_z * l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_z * l_x);

    std::cout << l_z << " * " << l_y << " => " << l_z * l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_z * l_y);

    std::cout << l_z << " * " << l_z << " => " << l_z * l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25) == l_z * l_z);

    std::cout << l_z << " * " << l_x_bis << " => " << l_z * l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_z * l_x_bis);

    std::cout << l_z << " * " << l_y_bis << " => " << l_z * l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_z * l_y_bis);

    std::cout << l_z << " * " << l_z_bis << " => " << l_z * l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25) == l_z * l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_x_bis << " * " << l_x << " => " << l_x_bis * l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4) == l_x_bis * l_x);

    std::cout << l_x_bis << " * " << l_y << " => " << l_x_bis * l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3) == l_x_bis * l_y);

    std::cout << l_x_bis << " * " << l_z << " => " << l_x_bis * l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_x_bis * l_z);

    std::cout << l_x_bis << " * " << l_x_bis << " => " << l_x_bis * l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4) == l_x_bis * l_x_bis);

    std::cout << l_x_bis << " * " << l_y_bis << " => " << l_x_bis * l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3) == l_x_bis * l_y_bis);

    std::cout << l_x_bis << " * " << l_z_bis << " => " << l_x_bis * l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_x_bis * l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_y_bis << " * " << l_x << " => " << l_y_bis * l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3) == l_y_bis * l_x);

    std::cout << l_y_bis << " * " << l_y << " => " << l_y_bis * l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9) == l_y_bis * l_y);

    std::cout << l_y_bis << " * " << l_z << " => " << l_y_bis * l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_y_bis * l_z);

    std::cout << l_y_bis << " * " << l_x_bis << " => " << l_y_bis * l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3) == l_y_bis * l_x_bis);

    std::cout << l_y_bis << " * " << l_y_bis << " => " << l_y_bis * l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9) == l_y_bis * l_y_bis);

    std::cout << l_y_bis << " * " << l_z_bis << " => " << l_y_bis * l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_y_bis * l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z_bis << " * " << l_x << " => " << l_z_bis * l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_z_bis * l_x);

    std::cout << l_z_bis << " * " << l_y << " => " << l_z_bis * l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_z_bis * l_y);

    std::cout << l_z_bis << " * " << l_z << " => " << l_z_bis * l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25) == l_z_bis * l_z);

    std::cout << l_z_bis << " * " << l_x_bis << " => " << l_z_bis * l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10) == l_z_bis * l_x_bis);

    std::cout << l_z_bis << " * " << l_y_bis << " => " << l_z_bis * l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5) == l_z_bis * l_y_bis);

    std::cout << l_z_bis << " * " << l_z_bis << " => " << l_z_bis * l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25) == l_z_bis * l_z_bis);

    //----------------------------------------
    // Operator /
    //----------------------------------------
    //------------------------------------------------------------------------
    std::cout << l_x << " / " << l_x << " => " << l_x / l_x << std::endl;
    assert(1 == l_x / l_x);

    std::cout << l_x << " / " << l_y << " => " << l_x / l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4) == l_x / l_y);

    std::cout << l_x << " / " << l_z << " => " << l_x / l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6) == l_x / l_z);

    std::cout << l_x << " / " << l_x_bis << " => " << l_x / l_x_bis << std::endl;
    assert(1 == l_x / l_x_bis);

    std::cout << l_x << " / " << l_y_bis << " => " << l_x / l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4) == l_x / l_y_bis);

    std::cout << l_x << " / " << l_z_bis << " => " << l_x / l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6) == l_x / l_z_bis);


    //------------------------------------------------------------------------
    std::cout << l_y << " / " << l_x << " => " << l_y / l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y / l_x);

    std::cout << l_y << " / " << l_y << " => " << l_y / l_y << std::endl;
    assert(1 == l_y / l_y);

    std::cout << l_y << " / " << l_z << " => " << l_y / l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9) == l_y / l_z);

    std::cout << l_y << " / " << l_x_bis << " => " << l_y / l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y / l_x_bis);

    std::cout << l_y << " / " << l_y_bis << " => " << l_y / l_y_bis << std::endl;
    assert(1 == l_y / l_y_bis);

    std::cout << l_y << " / " << l_z_bis << " => " << l_y / l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9) == l_y / l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z << " / " << l_x << " => " << l_z / l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z / l_x);

    std::cout << l_z << " / " << l_y << " => " << l_z / l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10) == l_z / l_y);

    std::cout << l_z << " / " << l_z << " => " << l_z / l_z << std::endl;
    assert(1 == l_z / l_z);

    std::cout << l_z << " / " << l_x_bis << " => " << l_z / l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z / l_x_bis);

    std::cout << l_z << " / " << l_y_bis << " => " << l_z / l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10) == l_z / l_y_bis);

    std::cout << l_z << " / " << l_z_bis << " => " << l_z / l_z_bis << std::endl;
    assert(1 == l_z / l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_x_bis << " / " << l_x << " => " << l_x_bis / l_x << std::endl;
    assert(1 == l_x_bis / l_x);

    std::cout << l_x_bis << " / " << l_y << " => " << l_x_bis / l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4) == l_x_bis / l_y);

    std::cout << l_x_bis << " / " << l_z << " => " << l_x_bis / l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6) == l_x_bis / l_z);

    std::cout << l_x_bis << " / " << l_x_bis << " => " << l_x_bis / l_x_bis << std::endl;
    assert(1 == l_x_bis / l_x_bis);

    std::cout << l_x_bis << " / " << l_y_bis << " => " << l_x_bis / l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4) == l_x_bis / l_y_bis);

    std::cout << l_x_bis << " / " << l_z_bis << " => " << l_x_bis / l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6) == l_x_bis / l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_y_bis << " / " << l_x << " => " << l_y_bis / l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y_bis / l_x);

    std::cout << l_y_bis << " / " << l_y << " => " << l_y_bis / l_y << std::endl;
    assert(1 == l_y_bis / l_y);

    std::cout << l_y_bis << " / " << l_z << " => " << l_y_bis / l_z << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9) == l_y_bis / l_z);

    std::cout << l_y_bis << " / " << l_x_bis << " => " << l_y_bis / l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3) == l_y_bis / l_x_bis);

    std::cout << l_y_bis << " / " << l_y_bis << " => " << l_y_bis / l_y_bis << std::endl;
    assert(1 == l_y_bis / l_y_bis);

    std::cout << l_y_bis << " / " << l_z_bis << " => " << l_y_bis / l_z_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9) == l_y_bis / l_z_bis);

    //------------------------------------------------------------------------
    std::cout << l_z_bis << " / " << l_x << " => " << l_z_bis / l_x << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z_bis / l_x);

    std::cout << l_z_bis << " / " << l_y << " => " << l_z_bis / l_y << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10) == l_z_bis / l_y);

    std::cout << l_z_bis << " / " << l_z << " => " << l_z_bis / l_z << std::endl;
    assert(1 == l_z_bis / l_z);

    std::cout << l_z_bis << " / " << l_x_bis << " => " << l_z_bis / l_x_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5) == l_z_bis / l_x_bis);

    std::cout << l_z_bis << " / " << l_y_bis << " => " << l_z_bis / l_y_bis << std::endl;
    assert(quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10) == l_z_bis / l_y_bis);

    std::cout << l_z_bis << " / " << l_z_bis << " => " << l_z_bis / l_z_bis << std::endl;
    assert(1 == l_z_bis / l_z_bis);

    std::cout << "Operator ++" << std::endl;
    std::cout << l_x << std::endl;
    ++l_x;
    std::cout << l_x << std::endl;
    l_x++;
    std::cout << l_x << std::endl;
    std::cout << "Operator --" << std::endl;
    std::cout << l_x << std::endl;
    --l_x;
    std::cout << l_x << std::endl;
    l_x--;
    std::cout << l_x << std::endl;

    quicky_utils::fract<FRACT_INTERNAL_TYPE> l_zero(1,2);
    assert(l_zero);
    l_zero -= quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,2);
    assert(!l_zero);

    l_x = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,2);
    l_y = quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5);
    std::cout << l_x << " < " << l_y << " => " << (l_x < l_y ?"true":"false") << std::endl;
    assert(!(l_x < l_y));
    std::cout << l_x << " > " << l_y << " => " << (l_x > l_y ?"true":"false") << std::endl;
    assert(l_x > l_y);
    std::cout << l_x << " <= " << l_y << " => " << (l_x <= l_y ?"true":"false") << std::endl;
    assert(!(l_x <= l_y));
    std::cout << l_x << " >= " << l_y << " => " << (l_x >= l_y ?"true":"false") << std::endl;
    assert(l_x >= l_y);
    l_y = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,5);
    std::cout << l_x << " != " << l_y << " => " << (l_x != l_y ?"true":"false") << std::endl;
    assert(l_x != l_y);
    std::cout << l_x << " == " << l_y << " => " << (l_x == l_y ?"true":"false") << std::endl;
    assert(!(l_x == l_y));


    l_y = - quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,5);
    std::cout << l_y << std::endl;
    l_y = + quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,5);
    std::cout << l_y << std::endl;
    l_y += (quicky_utils::fract<FRACT_INTERNAL_TYPE>)3;
    std::cout << l_y << std::endl;

    std::cout << l_x << " < " << 2 << " => " << (l_x < 2 ? "true" : "false") << std::endl;
    assert(l_x < 2);
    std::cout << l_x << " <= " << 2 << " => " << (l_x <= 2 ? "true" : "false") << std::endl;
    assert(l_x <= 2);
    std::cout << l_x << " > " << 2 << " => " << (l_x > 2 ? "true" : "false") << std::endl;
    assert(!(l_x > 2));
    std::cout << l_x << " >= " << 2 << " => " << (l_x >= 2 ? "true" : "false") << std::endl;
    assert(!(l_x >= 2));

    /* Test for GCC > 4.9.4 */
#if GCC_VERSION >= 40904
    display_NB<quicky_utils::fract<uint32_t>::PGCD(20,15,nullptr)>();
    display_NB<quicky_utils::fract<uint32_t>::PPCM(20,15,nullptr)>();
#endif // GCC_VERSION

    std::cout << (1 != l_x) << std::endl;
    std::cout << (l_x != 1) << std::endl;

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "- to_float and to double" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    l_a = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,2);
    l_b = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,3);
    std::cout << l_a << " == " << l_a.to_float() << std::endl;
    std::cout << l_b << " == " << l_b.to_float() << std::endl;
    std::cout << l_a << " == " << l_a.to_double() << std::endl;
    std::cout << l_b << " == " << l_b.to_double() << std::endl;
}

template <typename SOURCE_TYPE, typename TARGET_TYPE>
void test_type_conversion(const typename SOURCE_TYPE::base_type & p_value,
                          bool p_exception_expected
                         )
{
    SOURCE_TYPE l_source(p_value);
    bool l_ok = !p_exception_expected;
    try
    {
        TARGET_TYPE l_target(l_source);
    }
    catch(quicky_exception::quicky_logic_exception & e)
    {
        l_ok = p_exception_expected;
    }
    assert(l_ok);
}

template <typename SAFE_TYPE, typename REFERENCE_TYPE>
void test_safe_operator(const REFERENCE_TYPE l_op1,
                        const REFERENCE_TYPE l_op2,
                        char p_operator
                       )
{
    typename SAFE_TYPE::base_type l_base_x = (typename SAFE_TYPE::base_type) l_op1;
    typename SAFE_TYPE::base_type l_base_y = (typename SAFE_TYPE::base_type) l_op2;
    SAFE_TYPE l_safe_x(l_base_x);
    SAFE_TYPE l_safe_y(l_base_y);

    REFERENCE_TYPE l_ref_result = g_lambda(l_base_x, l_base_y,p_operator);
    typename SAFE_TYPE::base_type l_result = g_lambda(l_base_x, l_base_y, p_operator);
    SAFE_TYPE l_safe_result;
    bool l_compute_ok = true;
    try
    {
       l_safe_result = g_lambda(l_safe_x,l_safe_y, p_operator);
    }
    catch(quicky_utils::safe_type_exception & e)
    {
        l_compute_ok = false;
    }
    bool l_coherent_result = l_ref_result == (REFERENCE_TYPE)l_result;
    if(l_coherent_result)
    {
        assert(l_safe_result.get_value() == l_result);
    }
    assert(l_coherent_result == l_compute_ok);
}

template <typename SAFE_TYPE, typename REFERENCE_TYPE>
void test_safe_type(void)
{
    static_assert(std::is_signed<SAFE_TYPE>::value == std::is_signed<REFERENCE_TYPE>::value,"Check sign coherency between safe_type and reference type");
    std::array<char,5> l_operators = {'+', '-', '*', '/','%'};
    for(auto l_operator: l_operators)
    {
        std::cout << "Operator '" << l_operator << "'" << std::endl;
        typename SAFE_TYPE::base_type l_min = std::numeric_limits<typename SAFE_TYPE::base_type>::min();
        typename SAFE_TYPE::base_type l_max = std::numeric_limits<typename SAFE_TYPE::base_type>::max();
        for (REFERENCE_TYPE l_x = l_min ;
             l_x < l_max;
             ++l_x
                )
        {
            for (REFERENCE_TYPE l_y = l_min;
                 l_y < l_max;
                 ++l_y
                )
            {
                if(('/' == l_operator  || '%' == l_operator) && !l_y) break;
                test_safe_operator<SAFE_TYPE, REFERENCE_TYPE >(l_x,
                                            l_y,
                                            l_operator
                                           );
            }
        }
    }
}

#endif // QUICKY_UTILS_SELF_TEST
//EOF
