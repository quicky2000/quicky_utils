/*    This file is part of quicky_utils
      Copyright (C) 2019 Julien Thevenon ( julien_thevenon at yahoo.fr )

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

#include "quicky_bitfield.h"
#include "quicky_test.h"
#include <sstream>

namespace quicky_utils
{
    template <typename T>
    bool test()
    {
        bool l_ok = true;
        quicky_bitfield<T> l_bitfield1(48);
        l_ok &= quicky_test::check_expected(l_bitfield1.bitsize(), (size_t)48, "bitsize");
        l_ok &= quicky_test::check_expected(l_bitfield1.size(), sizeof(T) * (48 / (8 * sizeof(T)) + (0 != (48 % (8 * sizeof(T))))),"array size");
        unsigned int l_data = 0xDEAD;
        l_bitfield1.set(l_data, 16, 30);
        unsigned int l_data2;
        l_bitfield1.get(l_data2, 16, 30);
        l_ok &= quicky_test::check_expected(l_data2, l_data, "set/get");

        {
            quicky_bitfield<T> l_bitfield_string(64);
            unsigned int l_index = 0;
            std::string l_string("HELLO!  ");
            for (auto l_iter: l_string)
            {
                l_bitfield_string.set((unsigned int) l_iter, 8, 8 * l_index);
                ++l_index;
            }
            std::stringstream l_stream;
            l_bitfield_string.dump_in(l_stream);
            l_ok &= quicky_test::check_expected(l_stream.str(), l_string, "dump_in");

            quicky_bitfield<T> l_bitfield_string_bis(l_bitfield_string);
            std::stringstream l_stream_bis;
            l_bitfield_string_bis.dump_in(l_stream_bis);
            l_ok &= quicky_test::check_expected(l_stream.str(), l_stream_bis.str(), "copy constructor");

            std::string l_string2("ABCDEFGH");
            std::stringstream l_stream2;
            l_stream2 << l_string2;
            l_bitfield_string.read_from(l_stream2);
            for (l_index = 0; l_index < l_string2.size(); ++l_index)
            {
                l_bitfield_string.get(l_data2, 8, 8 * l_index);
                l_string[l_index] = (char) l_data2;
            }

            l_ok &= quicky_test::check_expected(l_string, l_string2, "read_from");
        }
        l_bitfield1.set(0xDEAD, 16 ,16);
        l_bitfield1.set(0xCAFE, 16 ,0);
        l_bitfield1.set(0xBEEF, 16 ,32);
        quicky_bitfield<T> l_bitfield_bis(48);
        l_bitfield_bis.set(0xFF00, 16, 16);
        l_bitfield_bis.set(0x0000, 16, 0);
        l_bitfield_bis.set(0xFF, 16, 32);
        l_bitfield1.apply_and(l_bitfield1, l_bitfield_bis);
        l_bitfield1.get(l_data, 16, 0);
        l_ok &= quicky_test::check_expected(l_data, 0x0000u, "apply_and");
        l_bitfield1.get(l_data, 16, 32);
        l_ok &= quicky_test::check_expected(l_data, 0xEFu, "apply_and");
        l_bitfield1.get(l_data, 16, 24);
        l_ok &= quicky_test::check_expected(l_data, 0xEFDEu, "apply_and");
        quicky_bitfield<T> l_bitfield2(32);
        l_ok &= quicky_test::check_expected(l_bitfield2.size(), 1 * sizeof(T),"array size");

        quicky_bitfield<T> l_bitfield3(64);
        l_ok &= quicky_test::check_expected(l_bitfield3.ffs(), 0, "ffs");
        for(unsigned int l_index = 0; l_index < 64; ++l_index)
        {
            l_bitfield3.set(1, 1, l_index);
            l_ok &= quicky_test::check_expected((unsigned int)l_bitfield3.ffs(), l_index + 1, "ffs");
            l_bitfield3.set(0, 1, l_index);
        }

        l_bitfield1.set(0x0, 16, 0);
        l_bitfield1.set(0x0, 16, 16);
        l_bitfield1.set(0x0, 16, 32);
        l_bitfield_bis.set(0x0, 16, 0);
        l_bitfield_bis.set(0x0, 16, 16);
        l_bitfield_bis.set(0x0, 16, 32);
        l_bitfield1.set(1, 1, 0);
        l_bitfield_bis.set(1, 1, 47);
        l_bitfield1.apply_or(l_bitfield1, l_bitfield_bis);
        l_bitfield1.get(l_data, 16, 0);
        l_ok &= quicky_test::check_expected(l_data, 1u, "apply_or");
        l_bitfield1.get(l_data, 16, 16);
        l_ok &= quicky_test::check_expected(l_data, 0u, "apply_or");
        l_bitfield1.get(l_data, 16, 32);
        l_ok &= quicky_test::check_expected(l_data, 0x8000u, "apply_or");
        l_ok &= quicky_test::check_expected(l_bitfield2.size(), 1 * sizeof(T),"array size");

        {
            quicky_bitfield<T> l_bitfield4(14, true);
            unsigned int l_bit_index = 0;
            unsigned int l_nb_bit = 0;
            while ((l_bit_index = (unsigned int)l_bitfield4.ffs()) != 0)
            {
                l_bitfield4.set(0, 1, l_bit_index - 1);
                ++l_nb_bit;
            }
            l_ok &= quicky_test::check_expected(l_nb_bit, (unsigned int)l_bitfield4.bitsize(), "Nb bits seen with FFS");
        }
        {
            quicky_bitfield<T> l_bitfield_a(72);
            quicky_bitfield<T> l_bitfield_b(72);
            for(unsigned int l_index1 = 0; l_index1 < 72; ++l_index1)
            {
                l_bitfield_a.set(1, 1, l_index1);
                for(unsigned int l_index2 = 0; l_index2 < 72; ++l_index2)
                {
                    l_bitfield_b.set(1, 1, l_index2);
                    l_ok &= quicky_test::check_expected(l_bitfield_a.and_not_null(l_bitfield_b), l_index1 == l_index2, "ANN not null");
                    l_ok &= quicky_test::check_expected(l_bitfield_b.and_not_null(l_bitfield_a), l_index1 == l_index2, "ANN not null");
                    l_bitfield_b.set(0, 1, l_index2);
                }
                l_bitfield_a.set(0, 1, l_index1);
            }
        }
        return l_ok;
    }

    bool test_quicky_bitfield()
    {
        bool l_ok = true;
        l_ok &= test<uint32_t>();
        l_ok &= test<uint64_t>();
        return l_ok;
    }
}

#endif // QUICKY_UTILS_SELF_TEST
// EOF
