/*    This file is part of quicky_utils
      Copyright (C) 2018  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#include "ext_uint.h"
#include "ext_int.h"
#include "quicky_test.h"
#include <cassert>
#include <cmath>

namespace quicky_utils
{
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
        auto l_result = convert<INT_TYPE,EXT_INT_TYPE>(p_ext_type);
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
        quicky_utils::ext_uint<uint8_t> l_zero_bis;
        quicky_utils::ext_uint<uint8_t> l_zero_list_init({0});
        quicky_utils::ext_uint<uint8_t> l_zero_list_init_bis({0});
        quicky_utils::ext_uint<uint8_t> l_un({1});
        quicky_utils::ext_uint<uint8_t> l_un_bis({1});
        quicky_utils::ext_uint<uint8_t> l_256({0,1});
        quicky_utils::ext_uint<uint8_t> l_256_bis({0,1});
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
        l_ok &= quicky_test::check_expected(l_zero == l_zero_bis, true, quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_expected(l_zero_list_init == l_zero_list_init_bis, true, quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_expected(l_zero == l_zero_list_init_bis, true, quicky_test::auto_message(__FILE__, __LINE__));

        l_ok &= quicky_test::check_expected(l_un == l_un_bis, true, quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_expected(l_zero == l_un, false, quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_expected(l_zero_list_init == l_un, false, quicky_test::auto_message(__FILE__, __LINE__));

        l_ok &= quicky_test::check_expected(l_zero < l_un, true, quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_expected(l_zero < l_zero_bis, false, quicky_test::auto_message(__FILE__, __LINE__));
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
        l_result = l_zero - l_zero_bis;
        l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

        l_result = l_un - l_un_bis;
        l_ok &= quicky_test::check_expected(l_result, l_zero, quicky_test::auto_message(__FILE__, __LINE__));

        l_result = l_256 - l_256_bis;
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
    bool
    test_ext_int()
    {
        bool l_ok = true;

        std::vector<uint8_t> l_significative_bytes = {0x0, 0x1, 0x7F, 0x80, 0xFE, 0xFF};
        typedef std::pair<int32_t, quicky_utils::ext_int<int8_t> > test_value_t;
        std::vector<test_value_t> l_test_values;

        // Compute meaningfull numbers that will be used for test
        int32_t l_int32 = 0;
        uint8_t * l_uint32_ptr = (uint8_t*)&l_int32;
        for(unsigned int l_i = 0;
            l_i < pow(l_significative_bytes.size(),4);
            ++l_i
                )
        {
            for(unsigned int l_j = 3;
                l_j <= 3;
                --l_j
               )
            {
                size_t l_byte_index = (l_i / ((uint32_t)pow(l_significative_bytes.size(),l_j))) % l_significative_bytes.size();
                l_uint32_ptr[l_j] = l_significative_bytes[l_byte_index];
#ifdef DEBUG
                std::cout << l_byte_index;
#endif // DEBUG
            }
#ifdef DEBUG
            std::cout << std::endl;
            std::cout << "0x" << std::hex << (uint32_t)l_int32 << std::dec << "\t" << l_int32 << std::endl;
#endif // DEBUG
            l_test_values.push_back(test_value_t(l_int32,
                                                 quicky_utils::ext_int<int8_t>(l_int32)
                                                )
                                   );
        }

        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<uint32_t>::max()), quicky_utils::ext_int<int8_t>(0,{0xFF,0xFF,0xFF,0xFF}), "uint32_t max");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<uint32_t>::min()), quicky_utils::ext_int<int8_t>(0,{}), "uint32_t min");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<int32_t>::max()), quicky_utils::ext_int<int8_t>(0x7F,{0xFF,0xFF,0xFF}), "int32_t max");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<int32_t>::min()), quicky_utils::ext_int<int8_t>((int8_t)0x80,{0x0,0x0,0x0}), "int32_t min");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<uint64_t>::max()), quicky_utils::ext_int<int8_t>(0,{0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF}), "uint64_t max");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<uint64_t>::min()), quicky_utils::ext_int<int8_t>(0,{}), "uint64_t min");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<int64_t>::max()), quicky_utils::ext_int<int8_t>(0x7F,{0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF}), "int64_t max");
        l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(std::numeric_limits<int64_t>::min()), quicky_utils::ext_int<int8_t>((int8_t)0x80,{0x0,0x0,0x0, 0x0, 0x0, 0x0, 0x0}), "int64_t min");
        
        // Check ostream operator
        for(auto l_iter: l_test_values)
        {
            // Build reference string from integer
            bool l_neg = l_iter.first < 0;
            uint32_t l_abs_value = l_neg ? - l_iter.first : l_iter.first;
            std::stringstream l_stream;
            l_stream << std::hex << std::uppercase << l_abs_value << std::dec;
            std::string l_reference = l_stream.str();
            if(l_reference.size() % 2)
            {
                l_reference.insert(l_reference.begin(),'0');
            }
            l_reference.insert(0,"0x");
            if(l_neg)
            {
                l_reference.insert(0,"-");
            }
            // Compare reference string with ext_int string representation
            l_ok &= quicky_test::check_ostream_operator(l_iter.second, l_reference, quicky_test::auto_message(__FILE__, __LINE__));
        }

        // Check == operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                l_ok &= quicky_test::check_expected(l_iter1.second == l_iter2.second,
                                                    l_iter1.first == l_iter2.first,
                                                    (std::string)l_iter1.second + " == " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Check != operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                l_ok &= quicky_test::check_expected(l_iter1.second != l_iter2.second,
                                                    l_iter1.first != l_iter2.first,
                                                    (std::string)l_iter1.second + " != " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Check < operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                l_ok &= quicky_test::check_expected(l_iter1.second < l_iter2.second,
                                                    l_iter1.first < l_iter2.first,
                                                    (std::string)l_iter1.second + " < " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Check <= operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                l_ok &= quicky_test::check_expected(l_iter1.second <= l_iter2.second,
                                                    l_iter1.first <= l_iter2.first,
                                                    (std::string)l_iter1.second + " <= " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Check > operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                l_ok &= quicky_test::check_expected(l_iter1.second > l_iter2.second,
                                                    l_iter1.first > l_iter2.first,
                                                    (std::string)l_iter1.second + " > " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Check >= operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                l_ok &= quicky_test::check_expected(l_iter1.second >= l_iter2.second,
                                                    l_iter1.first >= l_iter2.first,
                                                    (std::string)l_iter1.second + " >= " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Check + operator
        for(auto l_iter1: l_test_values)
        {
            for(auto l_iter2: l_test_values)
            {
                int64_t l_result = (int64_t)l_iter1.first + (int64_t)l_iter2.first;
                l_ok &= quicky_test::check_expected(quicky_utils::ext_int<int8_t>(l_result),
                                                    l_iter1.second + l_iter2.second,
                                                    (std::string)l_iter1.second + " + " + (std::string)l_iter2.second
                                                   );
            }
        }

        // Experiments around int type
        int16_t l_int = -1;
        for(unsigned int l_index = 0; l_index < 16; ++ l_index)
        {
            std::cout << "SHIFT=" << l_index << std::endl;
            std::cout << std::dec << l_int << std::endl;
            std::cout << std::hex << l_int << std::dec << std::endl;
            l_int = l_int << 1;
        }

        int16_t l_int16 = -1;
        auto l_uint16 = (uint16_t) l_int16;
        std::cout << l_int16 << "\t" << l_uint16 << std::endl;

        // ||-128 |  64 |  32 |  16 |   8 |   4 |   2 |   1 |
        //-||-----------------------------------------------|
        // ||   0 |   1 |   1 |   1 |   1 |   1 |   1 |   1 | =>  127
        // ||   0 |   0 |   0 |   0 |   0 |   0 |   0 |   1 | =>    1
        // ||   1 |   1 |   1 |   1 |   1 |   1 |   1 |   1 | => -  1
        // ||   0 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | =>    0
        // ||   1 |   0 |   0 |   0 |   0 |   0 |   0 |   1 | => -127
        // ||   1 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | => -128

        // -512 | 256 || 128 |  64 |  32 |  16 |   8 |   4 |   2 |   1 |
        //------------||-----------------------------------------------|
        //    0 |   0 ||   1 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | =>  128
        //    1 |   1 ||   1 |   0 |   0 |   0 |   0 |   0 |   0 |   0 | => -128
        //    1 |   1 ||   1 |   1 |   1 |   1 |   1 |   1 |   1 |   1 | => -  1
        //    1 |   1 ||   0 |   1 |   1 |   1 |   1 |   1 |   1 |   1 | => -129
        //    0 |   1 ||   0 |   0 |   0 |   0 |   0 |   0 |   0 |   1 | =>  257
        //    1 |   0 ||   1 |   1 |   1 |   1 |   1 |   1 |   1 |   1 | => -257

        l_ok &= quicky_test::check_exception<quicky_exception::quicky_logic_exception>([]{ext_int<int8_t> l_int8_t(0, {0x0});},true,"Check zero bad construct");
        l_ok &= quicky_test::check_exception<quicky_exception::quicky_logic_exception>([]{ext_int<int8_t> l_int8_t(0, {});},false,"Check zero good construct");
        l_ok &= quicky_test::check_exception<quicky_exception::quicky_logic_exception>([]{ext_int<int8_t> l_int8_t(-1, {});},false,"Check -1 good construct");
        l_ok &= quicky_test::check_exception<quicky_exception::quicky_logic_exception>([]{ext_int<int8_t> l_int8_t(-1, {0xFF});},true,"Check -1 bad construct");

        ext_int<int8_t> l_0(0,{});
        ext_int<int8_t> l_0_bis(0,{});
        ext_int<int8_t> l_1(1,{});
        ext_int<int8_t> l_m1(-1,{});
        ext_int<int8_t> l_127(127,{});
        ext_int<int8_t> l_m128(-128,{});
        ext_int<int8_t> l_128(0,{128});
        ext_int<int8_t> l_m256(-1,{0});
        ext_int<int8_t> l_256(1,{0});
        ext_int<int8_t> l_257(1,{1});
        ext_int<int8_t> l_257_bis(1,{1});

        l_ok &= quicky_test::check_ostream_operator(l_0,"0x00",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_1,"0x01",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_m1,"-0x01",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_127,"0x7F",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_m128,"-0x80",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_128,"0x80",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_256,"0x0100",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_257,"0x0101",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(l_m256,"-0x0100",quicky_test::auto_message(__FILE__, __LINE__));
        l_ok &= quicky_test::check_ostream_operator(ext_int<int8_t>(-2,{255}),"-0x0101",quicky_test::auto_message(__FILE__, __LINE__));

        l_ok &= quicky_test::check_expected(l_0 == l_0_bis, true, "ext_int::== self comparison");
        l_ok &= quicky_test::check_expected(l_1 == l_0, false, "ext_int::== root comparison");
        l_ok &= quicky_test::check_expected(l_1 == l_256, false, "ext_int::== ext size comparison");
        l_ok &= quicky_test::check_expected(l_257 == l_256, false, "ext_int::== ext content comparison");
        l_ok &= quicky_test::check_expected(l_257 == l_257_bis, true, "ext_int::== ext content comparison");

        l_ok &= quicky_test::check_expected(l_0 != l_0_bis, false, "ext_int::!= self comparison");
        l_ok &= quicky_test::check_expected(l_1 != l_0, true, "ext_int::!= root comparison");
        l_ok &= quicky_test::check_expected(l_1 != l_256, true, "ext_int::!= ext size comparison");
        l_ok &= quicky_test::check_expected(l_257 != l_256, true, "ext_int::!= ext content comparison");
        l_ok &= quicky_test::check_expected(l_257 != l_257_bis, false, "ext_int::!= ext content comparison");

        l_ok &= quicky_test::check_expected(l_0 < l_0, false, "0 < 0");
        l_ok &= quicky_test::check_expected(l_0 < l_1, true, "0 < 1");
        l_ok &= quicky_test::check_expected(l_0 < l_m1, false, "0 < -1");
        l_ok &= quicky_test::check_expected(l_0 < l_127, true, "0 < 127");
        l_ok &= quicky_test::check_expected(l_0 < l_128, true, "0 < 128");
        l_ok &= quicky_test::check_expected(l_0 < l_m128, false, "0 < -128");
        l_ok &= quicky_test::check_expected(l_0 < l_256, true, "0 < 256");
        l_ok &= quicky_test::check_expected(l_0 < l_m256, false, "0 < -256");
        l_ok &= quicky_test::check_expected(l_0 < l_257, true, "0 < 257");

        l_ok &= quicky_test::check_expected(l_1 < l_0, false, "1 < 0");
        l_ok &= quicky_test::check_expected(l_1 < l_1, false, "1 < 1");
        l_ok &= quicky_test::check_expected(l_1 < l_m1, false, "1 < -1");
        l_ok &= quicky_test::check_expected(l_1 < l_127, true, "1 < 127");
        l_ok &= quicky_test::check_expected(l_1 < l_128, true, "1 < 128");
        l_ok &= quicky_test::check_expected(l_1 < l_m128, false, "1 < -128");
        l_ok &= quicky_test::check_expected(l_1 < l_256, true, "1 < 256");
        l_ok &= quicky_test::check_expected(l_1 < l_m256, false, "1 < -256");
        l_ok &= quicky_test::check_expected(l_1 < l_257, true, "1 < 257");

        l_ok &= quicky_test::check_expected(l_m1 < l_0, true, "-1 < 0");
        l_ok &= quicky_test::check_expected(l_m1 < l_1, true, "-1 < 1");
        l_ok &= quicky_test::check_expected(l_m1 < l_m1, false, "-1 < -1");
        l_ok &= quicky_test::check_expected(l_m1 < l_127, true, "-1 < 127");
        l_ok &= quicky_test::check_expected(l_m1 < l_128, true, "-1 < 128");
        l_ok &= quicky_test::check_expected(l_m1 < l_m128, false, "-1 < -128");
        l_ok &= quicky_test::check_expected(l_m1 < l_256, true, "-1 < 256");
        l_ok &= quicky_test::check_expected(l_m1 < l_m256, false, "-1 < -256");
        l_ok &= quicky_test::check_expected(l_m1 < l_257, true, "-1 < 257");

        l_ok &= quicky_test::check_expected(l_127 < l_0, false, "127 < 0");
        l_ok &= quicky_test::check_expected(l_127 < l_1, false, "127 < 1");
        l_ok &= quicky_test::check_expected(l_127 < l_m1, false, "127 < -1");
        l_ok &= quicky_test::check_expected(l_127 < l_127, false, "127 < 127");
        l_ok &= quicky_test::check_expected(l_127 < l_128, true, "127 < 128");
        l_ok &= quicky_test::check_expected(l_127 < l_m128, false, "127 < -128");
        l_ok &= quicky_test::check_expected(l_127 < l_256, true, "127 < 256");
        l_ok &= quicky_test::check_expected(l_127 < l_m256, false, "127 < -256");
        l_ok &= quicky_test::check_expected(l_127 < l_257, true, "127 < 257");

        l_ok &= quicky_test::check_expected(l_128 < l_0, false, "128 < 0");
        l_ok &= quicky_test::check_expected(l_128 < l_1, false, "128 < 1");
        l_ok &= quicky_test::check_expected(l_128 < l_m1, false, "128 < -1");
        l_ok &= quicky_test::check_expected(l_128 < l_127, false, "128 < 127");
        l_ok &= quicky_test::check_expected(l_128 < l_128, false, "128 < 128");
        l_ok &= quicky_test::check_expected(l_128 < l_m128, false, "128 < -128");
        l_ok &= quicky_test::check_expected(l_128 < l_256, true, "128 < 256");
        l_ok &= quicky_test::check_expected(l_128 < l_m256, false, "128 < -256");
        l_ok &= quicky_test::check_expected(l_128 < l_257, true, "128 < 257");

        l_ok &= quicky_test::check_expected(l_m128 < l_0, true, "-128 < 0");
        l_ok &= quicky_test::check_expected(l_m128 < l_1, true, "-128 < 1");
        l_ok &= quicky_test::check_expected(l_m128 < l_m1, true, "-128 < -1");
        l_ok &= quicky_test::check_expected(l_m128 < l_127, true, "-128 < 127");
        l_ok &= quicky_test::check_expected(l_m128 < l_128, true, "-128 < 128");
        l_ok &= quicky_test::check_expected(l_m128 < l_m128, false, "-128 < -128");
        l_ok &= quicky_test::check_expected(l_m128 < l_256, true, "-128 < 256");
        l_ok &= quicky_test::check_expected(l_m128 < l_m256, false, "-128 < -256");
        l_ok &= quicky_test::check_expected(l_m128 < l_257, true, "-128 < 257");

        l_ok &= quicky_test::check_expected(l_256 < l_0, false, "256 < 0");
        l_ok &= quicky_test::check_expected(l_256 < l_1, false, "256 < 1");
        l_ok &= quicky_test::check_expected(l_256 < l_m1, false, "256 < -1");
        l_ok &= quicky_test::check_expected(l_256 < l_127, false, "256 < 127");
        l_ok &= quicky_test::check_expected(l_256 < l_128, false, "256 < 128");
        l_ok &= quicky_test::check_expected(l_256 < l_m128, false, "256 < -128");
        l_ok &= quicky_test::check_expected(l_256 < l_256, false, "256 < 256");
        l_ok &= quicky_test::check_expected(l_256 < l_m256, false, "256 < -256");
        l_ok &= quicky_test::check_expected(l_256 < l_257, true, "256 < 257");

        l_ok &= quicky_test::check_expected(l_m256 < l_0, true, "-256 < 0");
        l_ok &= quicky_test::check_expected(l_m256 < l_1, true, "-256 < 1");
        l_ok &= quicky_test::check_expected(l_m256 < l_m1, true, "-256 < -1");
        l_ok &= quicky_test::check_expected(l_m256 < l_127, true, "-256 < 127");
        l_ok &= quicky_test::check_expected(l_m256 < l_128, true, "-256 < 128");
        l_ok &= quicky_test::check_expected(l_m256 < l_m128, true, "-256 < -128");
        l_ok &= quicky_test::check_expected(l_m256 < l_256, true, "-256 < 256");
        l_ok &= quicky_test::check_expected(l_m256 < l_m256, false, "-256 < -256");
        l_ok &= quicky_test::check_expected(l_m256 < l_257, true, "-256 < 257");

        l_ok &= quicky_test::check_expected(l_257 < l_0, false, "257 < 0");
        l_ok &= quicky_test::check_expected(l_257 < l_1, false, "257 < 1");
        l_ok &= quicky_test::check_expected(l_257 < l_m1, false, "257 < -1");
        l_ok &= quicky_test::check_expected(l_257 < l_127, false, "257 < 127");
        l_ok &= quicky_test::check_expected(l_257 < l_128, false, "257 < 128");
        l_ok &= quicky_test::check_expected(l_257 < l_m128, false, "257 < -128");
        l_ok &= quicky_test::check_expected(l_257 < l_256, false, "257 < 256");
        l_ok &= quicky_test::check_expected(l_257 < l_m256, false, "257 < -256");
        l_ok &= quicky_test::check_expected(l_257 < l_257, false, "257 < 257");

        l_ok &= quicky_test::check_expected(l_0 > l_0, false, "0 > 0");
        l_ok &= quicky_test::check_expected(l_0 > l_1, false, "0 > 1");
        l_ok &= quicky_test::check_expected(l_0 > l_m1, true, "0 > -1");
        l_ok &= quicky_test::check_expected(l_0 > l_127, false, "0 > 127");
        l_ok &= quicky_test::check_expected(l_0 > l_128, false, "0 > 128");
        l_ok &= quicky_test::check_expected(l_0 > l_m128, true, "0 > -128");
        l_ok &= quicky_test::check_expected(l_0 > l_256, false, "0 > 256");
        l_ok &= quicky_test::check_expected(l_0 > l_m256, true, "0 > -256");
        l_ok &= quicky_test::check_expected(l_0 > l_257, false, "0 > 257");

        l_ok &= quicky_test::check_expected(l_1 > l_0, true, "1 > 0");
        l_ok &= quicky_test::check_expected(l_1 > l_1, false, "1 > 1");
        l_ok &= quicky_test::check_expected(l_1 > l_m1, true, "1 > -1");
        l_ok &= quicky_test::check_expected(l_1 > l_127, false, "1 > 127");
        l_ok &= quicky_test::check_expected(l_1 > l_128, false, "1 > 128");
        l_ok &= quicky_test::check_expected(l_1 > l_m128, true, "1 > -128");
        l_ok &= quicky_test::check_expected(l_1 > l_256, false, "1 > 256");
        l_ok &= quicky_test::check_expected(l_1 > l_m256, true, "1 > -256");
        l_ok &= quicky_test::check_expected(l_1 > l_257, false, "1 > 257");

        l_ok &= quicky_test::check_expected(l_m1 > l_0, false, "-1 > 0");
        l_ok &= quicky_test::check_expected(l_m1 > l_1, false, "-1 > 1");
        l_ok &= quicky_test::check_expected(l_m1 > l_m1, false, "-1 > -1");
        l_ok &= quicky_test::check_expected(l_m1 > l_127, false, "-1 > 127");
        l_ok &= quicky_test::check_expected(l_m1 > l_128, false, "-1 > 128");
        l_ok &= quicky_test::check_expected(l_m1 > l_m128, true, "-1 > -128");
        l_ok &= quicky_test::check_expected(l_m1 > l_256, false, "-1 > 256");
        l_ok &= quicky_test::check_expected(l_m1 > l_m256, true, "-1 > -256");
        l_ok &= quicky_test::check_expected(l_m1 > l_257, false, "-1 > 257");

        l_ok &= quicky_test::check_expected(l_127 > l_0, true, "127 > 0");
        l_ok &= quicky_test::check_expected(l_127 > l_1, true, "127 > 1");
        l_ok &= quicky_test::check_expected(l_127 > l_m1, true, "127 > -1");
        l_ok &= quicky_test::check_expected(l_127 > l_127, false, "127 > 127");
        l_ok &= quicky_test::check_expected(l_127 > l_128, false, "127 > 128");
        l_ok &= quicky_test::check_expected(l_127 > l_m128, true, "127 > -128");
        l_ok &= quicky_test::check_expected(l_127 > l_256, false, "127 > 256");
        l_ok &= quicky_test::check_expected(l_127 > l_m256, true, "127 > -256");
        l_ok &= quicky_test::check_expected(l_127 > l_257, false, "127 > 257");

        l_ok &= quicky_test::check_expected(l_128 > l_0, true, "128 > 0");
        l_ok &= quicky_test::check_expected(l_128 > l_1, true, "128 > 1");
        l_ok &= quicky_test::check_expected(l_128 > l_m1, true, "128 > -1");
        l_ok &= quicky_test::check_expected(l_128 > l_127, true, "128 > 127");
        l_ok &= quicky_test::check_expected(l_128 > l_128, false, "128 > 128");
        l_ok &= quicky_test::check_expected(l_128 > l_m128, true, "128 > -128");
        l_ok &= quicky_test::check_expected(l_128 > l_256, false, "128 > 256");
        l_ok &= quicky_test::check_expected(l_128 > l_m256, true, "128 > -256");
        l_ok &= quicky_test::check_expected(l_128 > l_257, false, "128 > 257");

        l_ok &= quicky_test::check_expected(l_m128 > l_0, false, "-128 > 0");
        l_ok &= quicky_test::check_expected(l_m128 > l_1, false, "-128 > 1");
        l_ok &= quicky_test::check_expected(l_m128 > l_m1, false, "-128 > -1");
        l_ok &= quicky_test::check_expected(l_m128 > l_127, false, "-128 > 127");
        l_ok &= quicky_test::check_expected(l_m128 > l_128, false, "-128 > 128");
        l_ok &= quicky_test::check_expected(l_m128 > l_m128, false, "-128 > -128");
        l_ok &= quicky_test::check_expected(l_m128 > l_256, false, "-128 > 256");
        l_ok &= quicky_test::check_expected(l_m128 > l_m256, true, "-128 > -256");
        l_ok &= quicky_test::check_expected(l_m128 > l_257, false, "-128 > 257");

        l_ok &= quicky_test::check_expected(l_256 > l_0, true, "256 > 0");
        l_ok &= quicky_test::check_expected(l_256 > l_1, true, "256 > 1");
        l_ok &= quicky_test::check_expected(l_256 > l_m1, true, "256 > -1");
        l_ok &= quicky_test::check_expected(l_256 > l_127, true, "256 > 127");
        l_ok &= quicky_test::check_expected(l_256 > l_128, true, "256 > 128");
        l_ok &= quicky_test::check_expected(l_256 > l_m128, true, "256 > -128");
        l_ok &= quicky_test::check_expected(l_256 > l_256, false, "256 > 256");
        l_ok &= quicky_test::check_expected(l_256 > l_m256, true, "256 > -256");
        l_ok &= quicky_test::check_expected(l_256 > l_257, false, "256 > 257");

        l_ok &= quicky_test::check_expected(l_m256 > l_0, false, "-256 > 0");
        l_ok &= quicky_test::check_expected(l_m256 > l_1, false, "-256 > 1");
        l_ok &= quicky_test::check_expected(l_m256 > l_m1, false, "-256 > -1");
        l_ok &= quicky_test::check_expected(l_m256 > l_127, false, "-256 > 127");
        l_ok &= quicky_test::check_expected(l_m256 > l_128, false, "-256 > 128");
        l_ok &= quicky_test::check_expected(l_m256 > l_m128, false, "-256 > -128");
        l_ok &= quicky_test::check_expected(l_m256 > l_256, false, "-256 > 256");
        l_ok &= quicky_test::check_expected(l_m256 > l_m256, false, "-256 > -256");
        l_ok &= quicky_test::check_expected(l_m256 > l_257, false, "-256 > 257");

        l_ok &= quicky_test::check_expected(l_257 > l_0, true, "257 > 0");
        l_ok &= quicky_test::check_expected(l_257 > l_1, true, "257 > 1");
        l_ok &= quicky_test::check_expected(l_257 > l_m1, true, "257 > -1");
        l_ok &= quicky_test::check_expected(l_257 > l_127, true, "257 > 127");
        l_ok &= quicky_test::check_expected(l_257 > l_128, true, "257 > 128");
        l_ok &= quicky_test::check_expected(l_257 > l_m128, true, "257 > -128");
        l_ok &= quicky_test::check_expected(l_257 > l_256, true, "257 > 256");
        l_ok &= quicky_test::check_expected(l_257 > l_m256, true, "257 > -256");
        l_ok &= quicky_test::check_expected(l_257 > l_257, false, "257 > 257");

        l_ok &= quicky_test::check_expected(l_0 + l_0, l_0, "0 + 0 == 0");
        l_ok &= quicky_test::check_expected(l_0 + l_1, l_1, "0 + 1 == 1");
        l_ok &= quicky_test::check_expected(l_0 + l_m1, l_m1, "0 + -1 == -1");
        l_ok &= quicky_test::check_expected(l_0 + l_127, l_127, "0 + 127 == 127");
        l_ok &= quicky_test::check_expected(l_0 + l_128, l_128, "0 + 128 == 128");
        l_ok &= quicky_test::check_expected(l_0 + l_m128, l_m128, "0 + -128 == -128");
        l_ok &= quicky_test::check_expected(l_0 + l_256, l_256, "0 + 256 == 256");
        l_ok &= quicky_test::check_expected(l_0 + l_m256, l_m256, "0 + -256 == -256");
        l_ok &= quicky_test::check_expected(l_0 + l_257, l_257, "0 + 257 == 257");

        l_ok &= quicky_test::check_expected(l_1 + l_0, l_1, "1 + 0 == 1");
        l_ok &= quicky_test::check_expected(l_1 + l_1, ext_int<int8_t>(2,{}), "1 + 1 == 2");
        l_ok &= quicky_test::check_expected(l_1 + l_m1, l_0, "1 + -1 == 0");
        l_ok &= quicky_test::check_expected(l_1 + l_127, l_128, "1 + 127 == 128");
        l_ok &= quicky_test::check_expected(l_1 + l_128, ext_int<int8_t>(0,{129}), "1 + 128 == 129");
        l_ok &= quicky_test::check_expected(l_1 + l_m128, ext_int<int8_t>(-127,{}), "1 + -128 == -127");
        l_ok &= quicky_test::check_expected(l_1 + l_256, l_257, "1 + 256 == 257");
        l_ok &= quicky_test::check_expected(l_1 + l_m256, ext_int<int8_t>(-1,{1}), "1 + -256 == -255");
        l_ok &= quicky_test::check_expected(l_1 + l_257, ext_int<int8_t>(1, {2}), "1 + 257 == 258");

        l_ok &= quicky_test::check_expected(l_m1 + l_0, ext_int<int8_t>(-1,{}), "-1 + 0 == -1");
        l_ok &= quicky_test::check_expected(l_m1 + l_1, l_0, "-1 + 1 == 0");
        l_ok &= quicky_test::check_expected(l_m1 + l_m1, ext_int<int8_t>(-2,{}), "-1 + -1 == -2");
        l_ok &= quicky_test::check_expected(l_m1 + l_127, ext_int<int8_t>(126,{}), "-1 + 127 == 126");
        l_ok &= quicky_test::check_expected(l_m1 + l_128, l_127, "-1 + 128 == 127");
        l_ok &= quicky_test::check_expected(l_m1 + l_m128, ext_int<int8_t>(-1,{127}), "-1 + -128 == -129");
        l_ok &= quicky_test::check_expected(l_m1 + l_256, ext_int<int8_t>(0,{255}), "-1 + 256 == 255");
        l_ok &= quicky_test::check_expected(l_m1 + l_m256, ext_int<int8_t>(-2,{255}), "-1 + -256 == -257");
        l_ok &= quicky_test::check_expected(l_m1 + l_257, l_256, "-1 + 257 == 256");

        return l_ok;
    }

}

#endif // QUICKY_UTILS_SELF_TEST
// EOF
