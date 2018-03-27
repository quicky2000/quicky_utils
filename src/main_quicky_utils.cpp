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
#include "quicky_test.h"
#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

using namespace quicky_utils;

/**
 * Method regrouping test of test utilites
 */
bool
check_test_utilities(void);

/**
 * Method regrouping tests of type_string class
 */
bool
test_type_string();

template <typename FRACT_INTERNAL_TYPE>
bool
test_fract(void);

bool
test_safe_types(void);

template <typename SAFE_TYPE, typename REFERENCE_TYPE>
bool test_safe_type(void);

template <typename SOURCE_TYPE, typename TARGET_TYPE>
bool test_type_conversion(const typename SOURCE_TYPE::base_type & p_value,
                          bool p_exception_expected
                         );

/**
 * Method regrouping tests of ext_uint class
 */
bool
test_ext_uint();

//------------------------------------------------------------------------------
int main(int argc,char ** argv)
{
    bool l_ok = true;
    try
    {
        std::ofstream l_report_file;
        l_report_file.open("report.log");
        if(!l_report_file.is_open())
        {
            throw quicky_exception::quicky_runtime_exception("Unable to create file report.log", __LINE__, __FILE__);
        }
        quicky_test::set_ostream(l_report_file);

        l_ok &= check_test_utilities();
        l_ok &= test_ext_uint();
        l_ok &= test_type_string();
        l_ok &= test_fract<uint32_t>();
        l_ok &= test_fract<quicky_utils::safe_int<int32_t>>();
        l_ok &= test_safe_types();
        l_report_file.close();

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
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "- TEST " << (l_ok ? "PASSED" : "FAILED") << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    return !l_ok;
}

//-----------------------------------------------------------------------------
bool
check_test_utilities(void)
{
    quicky_test::get_ostream() << "----------------------------------------------" << std::endl;
    quicky_test::get_ostream() << "| CHECK test_utilities" << std::endl;
    quicky_test::get_ostream() << "----------------------------------------------" << std::endl;
    bool l_ok = true;
    l_ok &= quicky_test::check_expected(quicky_test::check_expected_quiet(false,true),false, "Failed expected verification");
    l_ok &= quicky_test::check_expected(quicky_test::check_expected_quiet(true,true),true,"Passed expected verification");
    l_ok &= quicky_test::check_expected(quicky_test::check_exception_quiet<quicky_exception::quicky_logic_exception>([]{
                                                                                                                         throw quicky_exception::quicky_logic_exception("Volontary exception",
                                                                                                                                                                        __LINE__,
                                                                                                                                                                        __FILE__
                                                                                                                                                                       );
                                                                                                                       }
                                                                                                               ),
                                        true,
                                        "Exception detection verification"
                                       );
    l_ok &= quicky_test::check_expected(quicky_test::check_exception_quiet<quicky_exception::quicky_logic_exception>([]{},
                                                                                                                     false // No exception expected
                                                                                                                    ),
                                        true,
                                        "No exception detection verification"
                                       );
    l_ok &= quicky_test::check_expected(quicky_test::check_ostream_operator_quiet<uint32_t>(12,"23"),false,"Failed ostream operator verification");
    l_ok &= quicky_test::check_expected(quicky_test::check_ostream_operator_quiet<uint32_t>(12,"12"),true,"Passed ostream operator verification");
    return l_ok;
}

/**
 * Method checking type string representation against expected
 * @tparam T
 * @return true if test is successfull
 */
template <typename T>
bool check_a_type_string(const std::string & p_reference)
{
    return quicky_test::check_expected(quicky_utils::type_string<T>::name(), p_reference, "Check name of " + quicky_utils::type_string<T>::name());
}
#define check(type) check_a_type_string<type>(#type)
#define check_name(type,type_name) check_a_type_string<type>(type_name)
#define check_prefix(prefix,type) check_a_type_string<prefix::type>(#type)

//-----------------------------------------------------------------------------
bool
test_type_string()
{
    bool l_ok = true;
    l_ok &= check(uint8_t);
    l_ok &= check(uint16_t);
    l_ok &= check(uint32_t);
    l_ok &= check(uint64_t);

    l_ok &= check(int8_t);
    l_ok &= check(int16_t);
    l_ok &= check(int32_t);
    l_ok &= check(int64_t);

    l_ok &= check(char);
    l_ok &= check(double);
    l_ok &= check(float);
    l_ok &= check(std::string);

    l_ok &= check_name(quicky_utils::safe_uint32_t,"safe_uint<uint32_t>");
    l_ok &= check_name(quicky_utils::safe_uint64_t,"safe_uint<uint64_t>");

    l_ok &= check_prefix(quicky_utils,safe_uint<uint8_t>);
    l_ok &= check_name(quicky_utils::safe_uint<uint8_t>,"safe_uint<uint8_t>");
    l_ok &= check_prefix(quicky_utils,safe_uint<uint16_t>);
    l_ok &= check_name(quicky_utils::safe_uint<uint16_t>,"safe_uint<uint16_t>");

    l_ok &= check_name(quicky_utils::safe_int8_t,"safe_int<int8_t>");
    l_ok &= check_name(quicky_utils::safe_int16_t,"safe_int<int16_t>");
    l_ok &= check_name(quicky_utils::safe_int32_t,"safe_int<int32_t>");
    l_ok &= check_name(quicky_utils::safe_int64_t,"safe_int<int64_t>");

    return l_ok;
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
bool check_convert(const EXT_INT_TYPE & p_ext_type,
                   const INT_TYPE & p_expected
                  )
{
    INT_TYPE l_result = convert<INT_TYPE,EXT_INT_TYPE>(p_ext_type);
    std::stringstream l_stream;
    l_stream << p_ext_type;
    return quicky_test::check_expected<INT_TYPE>(l_result, p_expected, "Check conversion of " + l_stream.str() + " to " + quicky_utils::type_string<INT_TYPE>::name());
}

//------------------------------------------------------------------------------
bool
test_ext_uint()
{
    bool l_ok = true;
    quicky_utils::ext_uint<uint8_t> l_zero;
    quicky_utils::ext_uint<uint8_t> l_zero_list_init({0});
    quicky_utils::ext_uint<uint8_t> l_un({1});
    quicky_utils::ext_uint<uint8_t> l_256({0,1});
    quicky_utils::ext_uint<uint8_t> l_max({0xFF, 0xFF, 0xFF, 0xFF});

    quicky_test::get_ostream() << std::endl << "Test ext_uint output stream operator:" << std::endl;
    l_ok &= quicky_test::check_ostream_operator(l_zero,"0x00");
    l_ok &= quicky_test::check_ostream_operator(l_un,"0x01");
    l_ok &= quicky_test::check_ostream_operator(l_256,"0x0100");

    quicky_test::get_ostream() << std::endl << "Test conversion to integer type" << std::endl;
    l_ok &= check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_256, 256);
    l_ok &= check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_zero, 0);
    l_ok &= check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_un, 1);
    l_ok &= check_convert<uint32_t,quicky_utils::ext_uint<uint8_t>>(l_max, std::numeric_limits<uint32_t>::max());

    quicky_test::get_ostream() << std::endl << "Test == operator" << std::endl;
    l_ok &= quicky_test::check_expected(l_zero == l_zero, true, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_zero_list_init == l_zero_list_init, true, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_zero == l_zero_list_init, true, quicky_test::auto_message(__FILE__, __LINE__));

    l_ok &= quicky_test::check_expected(l_un == l_un, true, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_zero == l_un, false, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_zero_list_init == l_un, false, quicky_test::auto_message(__FILE__, __LINE__));

    l_ok &= quicky_test::check_expected(l_zero < l_un, true, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_zero < l_zero, false, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_un < l_zero, false, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_zero < l_256, true, quicky_test::auto_message(__FILE__, __LINE__));
    l_ok &= quicky_test::check_expected(l_256 < l_zero, false, quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test + operator" << std::endl;
    quicky_utils::ext_uint<uint8_t> l_result;
    l_result = l_zero + l_zero;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_zero + l_un;
    l_ok &= quicky_test::check_expected(l_result, l_un, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255}) + l_un;
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0, 1}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) + l_un;
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0, 0, 1}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) + quicky_utils::ext_uint<uint8_t>({0,1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({255, 0, 1}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255}) + quicky_utils::ext_uint<uint8_t>({255});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({254, 1}), quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test - operator" << std::endl;
    l_result = l_zero - l_zero;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_un - l_un;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_256 - l_256;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_256 - l_un;
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({255}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result =quicky_utils::ext_uint<uint8_t>({255, 255}) - quicky_utils::ext_uint<uint8_t>({0,1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({255, 254}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result =quicky_utils::ext_uint<uint8_t>({255, 255}) - quicky_utils::ext_uint<uint8_t>({1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({254, 255}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result =quicky_utils::ext_uint<uint8_t>({0, 0, 1}) - quicky_utils::ext_uint<uint8_t>({1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({255, 255}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result =quicky_utils::ext_uint<uint8_t>({0, 0, 2}) - quicky_utils::ext_uint<uint8_t>({1, 1, 1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({255, 254}), quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test * operator" << std::endl;
    l_result = l_zero * l_zero;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_zero * l_un;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_un * l_zero;
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_un * l_un;
    l_ok &= quicky_test::check_expected(l_result, l_un, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({128}) * quicky_utils::ext_uint<uint8_t>({42});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0x0, 0x15}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) * quicky_utils::ext_uint<uint8_t>({255});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0x1, 0xFF, 0xFE}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255, 255}) * quicky_utils::ext_uint<uint8_t>({255, 255});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0x1, 0x0, 0xFE, 0xFF}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t>({255, 255, 255, 255}) * quicky_utils::ext_uint<uint8_t>({255, 255, 255, 255});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0x1, 0x0, 0x0, 0x0, 0xFE, 0xFF, 0xFF, 0xFF}), quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test >> operator" << std::endl;
    l_result = l_256 >> quicky_utils::ext_uint<uint8_t>({1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({128}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_256 >> quicky_utils::ext_uint<uint8_t>({20});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>(), quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test << operator" << std::endl;
    l_result = l_un << quicky_utils::ext_uint<uint8_t>({1});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({2}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_un << quicky_utils::ext_uint<uint8_t>({8});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0, 1}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = l_un << quicky_utils::ext_uint<uint8_t>({16});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0, 0, 1}), quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test / operator" << std::endl;
    l_result = quicky_utils::ext_uint<uint8_t >({12}) / quicky_utils::ext_uint<uint8_t>({6});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({2}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t >({13}) / quicky_utils::ext_uint<uint8_t>({6});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({2}), quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t >({0xFF,0x1}) / quicky_utils::ext_uint<uint8_t>({0x12});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({0x1C}), quicky_test::auto_message(__FILE__, __LINE__));

    quicky_test::get_ostream() << std::endl << "Test % operator" << std::endl;
    l_result = quicky_utils::ext_uint<uint8_t >({12}) % quicky_utils::ext_uint<uint8_t>({6});
    l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t >({13}) % quicky_utils::ext_uint<uint8_t>({6});
    l_ok &= quicky_test::check_expected(l_result, l_un, quicky_test::auto_message(__FILE__, __LINE__));

    l_result = quicky_utils::ext_uint<uint8_t >({0xFF,0x1}) % quicky_utils::ext_uint<uint8_t>({0x12});
    l_ok &= quicky_test::check_expected(l_result, ext_uint<uint8_t>({7}), quicky_test::auto_message(__FILE__, __LINE__));

    return l_ok;
}

//------------------------------------------------------------------------------
template <typename TYPE>
bool check_safe_equal(const TYPE & p_op1,
                      const TYPE & p_op2,
                      bool p_expected
                     )
{
    return quicky_test::check_expected(p_op1 == p_op2, p_expected);
}

//------------------------------------------------------------------------------
template <typename TYPE>
bool check_safe_not_equal(const TYPE & p_op1,
                          const TYPE & p_op2,
                          bool p_expected
                         )
{
    return quicky_test::check_expected(p_op1 != p_op2, p_expected);
}

//------------------------------------------------------------------------------
bool
test_safe_types(void)
{
    bool l_ok = true;
    l_ok &= test_safe_type<typename quicky_utils::safe_uint<uint8_t>, uint32_t>();
    l_ok &= test_safe_type<typename quicky_utils::safe_int<int8_t>, int32_t>();

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
    l_ok &= test_type_conversion<quicky_utils::safe_int<int8_t>, quicky_utils::safe_uint<uint8_t> >(-128,
                                                                                                    true
                                                                                                   );
    l_ok &= test_type_conversion<quicky_utils::safe_int<int8_t>, quicky_utils::safe_uint<uint8_t> >(-1,
                                                                                                    true
                                                                                                   );
    l_ok &= test_type_conversion<quicky_utils::safe_int<int8_t>, quicky_utils::safe_uint<uint8_t> >(0,
                                                                                                    false
                                                                                                   );

    // Try to change a safe_uint in a safe_int
    l_ok &= test_type_conversion<quicky_utils::safe_uint<uint8_t>, quicky_utils::safe_int<int8_t> >(0,
                                                                                                    false
                                                                                                   );
    l_ok &= test_type_conversion<quicky_utils::safe_uint<uint8_t>, quicky_utils::safe_int<int8_t> >(127,
                                                                                                    false
                                                                                                   );
    l_ok &= test_type_conversion<quicky_utils::safe_uint<uint8_t>, quicky_utils::safe_int<int8_t> >(128,
                                                                                                    true
                                                                                                   );
    // Test equality operator
    l_ok &= check_safe_equal<quicky_utils::safe_uint<uint8_t>>(1,1,true);
    l_ok &= check_safe_equal<quicky_utils::safe_uint<uint8_t>>(1,2,false);
    l_ok &= check_safe_not_equal<quicky_utils::safe_uint<uint8_t>>(1,1,false);
    l_ok &= check_safe_not_equal<quicky_utils::safe_uint<uint8_t>>(1,2,true);

    return l_ok;
}

template<unsigned int NB>
void display_NB(void)
{
    std::cout << NB << std::endl;
}

template <typename FRACT_INTERNAL_TYPE>
bool
test_fract(void)
{
    bool l_ok = true;
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

    return l_ok;
}

template <typename SOURCE_TYPE, typename TARGET_TYPE>
bool test_type_conversion(const typename SOURCE_TYPE::base_type & p_value,
                          bool p_exception_expected
                         )
{
    std::string l_message("test type conversion ");
    l_message += type_string<SOURCE_TYPE>::name() + " -> " + type_string<TARGET_TYPE>::name();
    SOURCE_TYPE l_source(p_value);
    return quicky_test::check_exception<quicky_exception::quicky_logic_exception>([&]{TARGET_TYPE l_target(l_source);},p_exception_expected, l_message);
}

template <typename SAFE_TYPE, typename REFERENCE_TYPE>
bool test_safe_operator(const REFERENCE_TYPE l_op1,
                        const REFERENCE_TYPE l_op2,
                        char p_operator
                       )
{
    bool l_ok = true;
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
        l_ok &= quicky_test::check_expected(l_safe_result.get_value() == l_result, true);
    }
    l_ok &= quicky_test::check_expected(l_coherent_result == l_compute_ok, true);
    return l_ok;
}

template <typename SAFE_TYPE, typename REFERENCE_TYPE>
bool test_safe_type(void)
{
    bool l_ok = true;
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
                l_ok &= test_safe_operator<SAFE_TYPE, REFERENCE_TYPE >(l_x,
                                                                       l_y,
                                                                       l_operator
                                                                      );
            }
        }
    }
    return l_ok;
}

#endif // QUICKY_UTILS_SELF_TEST
//EOF
