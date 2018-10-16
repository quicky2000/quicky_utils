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

#ifndef QUICKY_UTILS_TEST_FRACT_H
#define QUICKY_UTILS_TEST_FRACT_H

#include "fract.h"
#include <iostream>
#include "quicky_test.h"

namespace quicky_utils
{
    template <unsigned int NB>
    void
    display_NB()
    {
        std::cout << NB << std::endl;
    }

    template <typename FRACT_INTERNAL_TYPE>
    bool
    test_fract()
    {
        bool l_ok = true;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "- PGCD" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den l_pos_num = 3;
        typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_num l_neg_num = -3;
        typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den l_pos_den = 6;
        typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_num l_neg_den = -6;

        typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef l_result;

	    l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(l_pos_num, l_pos_den), 3, "PGCD( 3, 6)");
	    l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(l_neg_num, l_pos_den), 3, "PGCD(-3, 6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(l_pos_num, l_neg_den), 3, "PGCD( 3,-6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD(l_neg_num, l_neg_den), 3, "PGCD(-3,-6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<uint32_t>::t_coef_den>(quicky_utils::fract<uint32_t>::PGCD(1071, 1029),21, "PGCD(1071,1029)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<uint32_t>::t_coef_den>(quicky_utils::fract<uint32_t>::PGCD(1029, 1071),21, "PGCD(1029,1071)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<uint32_t>::t_coef_den>(quicky_utils::fract<uint32_t>::PGCD(60, 168),12, "PGCD(60,168)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<uint32_t>::t_coef_den>(quicky_utils::fract<uint32_t>::PGCD(168, 60),12, "PGCD(168,60)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PGCD((typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den)6
                                                                                                                                                         ,(typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den)9
                                                                                                                                                         )
                                                                                                          ,3
                                                                                                          ,"PGCD(6,9)"
                                                                                                          );

        std::cout << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "- PPCM" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_pos_num, l_pos_den), 6, "PPCM( 3, 6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_neg_num, l_pos_den), 6, "PPCM(-3, 6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_pos_num, l_neg_den), 6, "PPCM( 3,-6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM(l_neg_num, l_neg_den), 6, "PPCM(-3,-6)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<uint32_t>::t_coef_den>(quicky_utils::fract<uint32_t>::PPCM(60, 168), 840, "PPCM(60,168)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<uint32_t>::t_coef_den>(quicky_utils::fract<uint32_t>::PPCM(168, 60), 840, "PPCM(168,60)");
        l_ok &= quicky_test::check_expected<typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den>(quicky_utils::fract<FRACT_INTERNAL_TYPE>::PPCM((typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den)6
                                                                                                                                                         ,(typename quicky_utils::fract<FRACT_INTERNAL_TYPE>::t_coef_den)9
                                                                                                                                                         )
                                                                                                          , 18
                                                                                                          , "PPCM(6,9)"
                                                                                                          );

        std::cout << "---------------------------------------" << std::endl;

        l_ok &= quicky_test::check_expected(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3, 1) * quicky_utils::fract<FRACT_INTERNAL_TYPE>(7, 1), quicky_utils::fract<FRACT_INTERNAL_TYPE>(-21), "-3/1 * 7/1 == -21");
        l_ok &= quicky_test::check_expected(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-21, 1) / quicky_utils::fract<FRACT_INTERNAL_TYPE>(6, 1), quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7, 2), "-21/1 / 6/1 == -7/2");
        l_ok &= quicky_test::check_expected(quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4, 1) - quicky_utils::fract<FRACT_INTERNAL_TYPE>(-21, 6), quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1, 2), "-4/1 - (-21/6) == -1/2");

        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_nb1(1, 2);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_nb2(2);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_nb3(10, 2);
        l_ok &= quicky_test::check_expected(l_nb3, quicky_utils::fract<FRACT_INTERNAL_TYPE>(5), "10/2 == 5");
        std::cout << l_nb1 << std::endl;
        std::cout << l_nb2 << std::endl;
        std::cout << l_nb3 << std::endl;

        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_a(1, 3);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_b(1, 4);

        l_ok &= quicky_test::check_expected(l_a + l_b, quicky_utils::fract<FRACT_INTERNAL_TYPE>(7, 12),std::to_string(l_a) + " + " + std::to_string(l_b));
        l_ok &= quicky_test::check_expected(l_a - l_b, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1, 12),std::to_string(l_a) + " - " + std::to_string(l_b));
        l_ok &= quicky_test::check_expected(l_a * l_b, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1, 12),std::to_string(l_a) + " * " + std::to_string(l_b));
        l_ok &= quicky_test::check_expected(l_a / l_b, quicky_utils::fract<FRACT_INTERNAL_TYPE>(4, 3),std::to_string(l_a) + " / " + std::to_string(l_b));
        l_ok &= quicky_test::check_expected(1 + l_a, quicky_utils::fract<FRACT_INTERNAL_TYPE>(4, 3), "1 + " + std::to_string(l_a));

        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_x(1, 2);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_x2(l_x);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_y(2, -3);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_y2(l_y);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_z(-3, 5);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_z2(l_z);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_x_bis(-1, -2);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_x_bis2(l_x_bis);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_y_bis(-2, 3);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_y_bis2(l_y_bis);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_z_bis(3, -5);
        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_z_bis2(l_z_bis);

        // Check equality independantly of - position ( num or den)
        l_ok &= quicky_test::check_expected(l_x == l_x_bis, true, std::to_string(l_x) + " == " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y == l_y_bis, true, std::to_string(l_y) + " == " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z == l_z_bis, true, std::to_string(l_z) + " == " + std::to_string(l_z_bis));
        l_ok &= quicky_test::check_expected(l_x != l_y, true, std::to_string(l_x) + " != " + std::to_string(l_y));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x + l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x) + " + " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x + l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1, 6), std::to_string(l_x) + " + " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x + l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1, 10), std::to_string(l_x) + " + " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x + l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x) + " + " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_x + l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1, 6), std::to_string(l_x) + " + " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x + l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_x) + " + " + std::to_string(l_z_bis));


        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y + l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6), std::to_string(l_y) + " + " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y + l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y) + " + " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_y + l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_y) + " + " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y + l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6), std::to_string(l_y) + " + " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y + l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y) + " + " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_y + l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_y) + " + " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z + l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_z) + " + " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z + l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_z) + " + " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z + l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z) + " + " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_z + l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_z) + " + " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z + l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_z) + " + " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z + l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z) + " + " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x_bis + l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x_bis) + " + " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x_bis + l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6), std::to_string(l_x_bis) + " + " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x_bis + l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_x_bis) + " + " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x_bis + l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x_bis) + " + " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_x_bis + l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6), std::to_string(l_x_bis) + " + " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x_bis + l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_x_bis) + " + " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y_bis + l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6), std::to_string(l_y_bis) + " + " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y_bis + l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y_bis) + " + " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_y_bis + l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_y_bis) + " + " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y_bis + l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,6), std::to_string(l_y_bis) + " + " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y_bis + l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y_bis) + " + " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_y_bis + l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_y_bis) + " + " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z_bis + l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_z_bis) + " + " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z_bis + l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_z_bis) + " + " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z_bis + l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z_bis) + " + " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_z_bis + l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,10), std::to_string(l_z_bis) + " + " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z_bis + l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-19,15), std::to_string(l_z_bis) + " + " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z_bis + l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z_bis) + " + " + std::to_string(l_z_bis));

        //----------------------------------------
        // Operator -
        //----------------------------------------
        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x - l_x2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_x) + " - " + std::to_string(l_x2));
        l_ok &= quicky_test::check_expected(l_x - l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6), std::to_string(l_x) + " - " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x - l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10), std::to_string(l_x) + " - " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x - l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_x) + " - " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_x - l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6), std::to_string(l_x) + " - " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x - l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10), std::to_string(l_x) + " - " + std::to_string(l_z_bis));


        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y - l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6), std::to_string(l_y) + " - " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y - l_y2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_y) + " - " + std::to_string(l_y2));
        l_ok &= quicky_test::check_expected(l_y - l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15), std::to_string(l_y) + " - " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y - l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6), std::to_string(l_y) + " - " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y - l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_y) + " - " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_y - l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15), std::to_string(l_y) + " - " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z - l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10), std::to_string(l_z) + " - " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z - l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15), std::to_string(l_z) + " - " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z - l_z2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_z) + " - " + std::to_string(l_z2));
        l_ok &= quicky_test::check_expected(l_z - l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10), std::to_string(l_z) + " - " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z - l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15), std::to_string(l_z) + " - " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z - l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_z) + " - " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x_bis - l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_x_bis) + " - " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x_bis - l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6), std::to_string(l_x_bis) + " - " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x_bis - l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10), std::to_string(l_x_bis) + " - " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x_bis - l_x_bis2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_x_bis) + " - " + std::to_string(l_x_bis2));
        l_ok &= quicky_test::check_expected(l_x_bis - l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(7,6), std::to_string(l_x_bis) + " - " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x_bis - l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(11,10), std::to_string(l_x_bis) + " - " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y_bis - l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6), std::to_string(l_y_bis) + " - " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y_bis - l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_y_bis) + " - " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_y_bis - l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15), std::to_string(l_y_bis) + " - " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y_bis - l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-7,6), std::to_string(l_y_bis) + " - " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y_bis - l_y_bis2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_y_bis) + " - " + std::to_string(l_y_bis2));
        l_ok &= quicky_test::check_expected(l_y_bis - l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,15), std::to_string(l_y_bis) + " - " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z_bis - l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10), std::to_string(l_z_bis) + " - " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z_bis - l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15), std::to_string(l_z_bis) + " - " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z_bis - l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_z_bis) + " - " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_z_bis - l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-11,10), std::to_string(l_z_bis) + " - " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z_bis - l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,15), std::to_string(l_z_bis) + " - " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z_bis - l_z_bis2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(0), std::to_string(l_z_bis) + " - " + std::to_string(l_z_bis2));


        //----------------------------------------
        // Operator *
        //----------------------------------------
        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x * l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4), std::to_string(l_x) + " * " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x * l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6), std::to_string(l_x) + " * " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x * l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_x) + " * " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x * l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4), std::to_string(l_x) + " * " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_x * l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6), std::to_string(l_x) + " * " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x * l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_x) + " * " + std::to_string(l_z_bis));


        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y * l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6), std::to_string(l_y) + " * " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y * l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9), std::to_string(l_y) + " * " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_y * l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_y) + " * " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y * l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-2,6), std::to_string(l_y) + " * " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y * l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9), std::to_string(l_y) + " * " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_y * l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_y) + " * " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z * l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_z) + " * " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z * l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_z) + " * " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z * l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25), std::to_string(l_z) + " * " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_z * l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_z) + " * " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z * l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_z) + " * " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z * l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25), std::to_string(l_z) + " * " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x_bis * l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4), std::to_string(l_x_bis) + " * " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x_bis * l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3), std::to_string(l_x_bis) + " * " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x_bis * l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_x_bis) + " * " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x_bis * l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,4), std::to_string(l_x_bis) + " * " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_x_bis * l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3), std::to_string(l_x_bis) + " * " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x_bis * l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_x_bis) + " * " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y_bis * l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3), std::to_string(l_y_bis) + " * " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y_bis * l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9), std::to_string(l_y_bis) + " * " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_y_bis * l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_y_bis) + " * " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y_bis * l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1,3), std::to_string(l_y_bis) + " * " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y_bis * l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(4,9), std::to_string(l_y_bis) + " * " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_y_bis * l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_y_bis) + " * " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z_bis * l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_z_bis) + " * " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z_bis * l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_z_bis) + " * " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z_bis * l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25), std::to_string(l_z_bis) + " * " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_z_bis * l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,10), std::to_string(l_z_bis) + " * " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z_bis * l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5), std::to_string(l_z_bis) + " * " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z_bis * l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,25), std::to_string(l_z_bis) + " * " + std::to_string(l_z_bis));

        //----------------------------------------
        // Operator /
        //----------------------------------------
        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x / l_x2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x) + " / " + std::to_string(l_x2));
        l_ok &= quicky_test::check_expected(l_x / l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4), std::to_string(l_x) + " / " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x / l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6), std::to_string(l_x) + " / " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x / l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x) + " / " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_x / l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4), std::to_string(l_x) + " / " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x / l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6), std::to_string(l_x) + " / " + std::to_string(l_z_bis));


        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y / l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y) + " / " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y / l_y2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_y) + " / " + std::to_string(l_y2));
        l_ok &= quicky_test::check_expected(l_y / l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9), std::to_string(l_y) + " / " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y / l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y) + " / " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y / l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_y) + " / " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_y / l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9), std::to_string(l_y) + " / " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z / l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z) + " / " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z / l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10), std::to_string(l_z) + " / " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z / l_z2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_z) + " / " + std::to_string(l_z2));
        l_ok &= quicky_test::check_expected(l_z / l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z) + " / " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z / l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10), std::to_string(l_z) + " / " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z / l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_z) + " / " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_x_bis / l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x_bis) + " / " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x_bis / l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4), std::to_string(l_x_bis) + " / " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x_bis / l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6), std::to_string(l_x_bis) + " / " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_x_bis / l_x_bis2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_x_bis) + " / " + std::to_string(l_x_bis2));
        l_ok &= quicky_test::check_expected(l_x_bis / l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-3,4), std::to_string(l_x_bis) + " / " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_x_bis / l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-5,6), std::to_string(l_x_bis) + " / " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_y_bis / l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y_bis) + " / " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_y_bis / l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_y_bis) + " / " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_y_bis / l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9), std::to_string(l_y_bis) + " / " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_y_bis / l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-4,3), std::to_string(l_y_bis) + " / " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_y_bis / l_y_bis2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_y_bis) + " / " + std::to_string(l_y_bis2));
        l_ok &= quicky_test::check_expected(l_y_bis / l_z_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(10,9), std::to_string(l_y_bis) + " / " + std::to_string(l_z_bis));

        //------------------------------------------------------------------------
        l_ok &= quicky_test::check_expected(l_z_bis / l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z_bis) + " / " + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_z_bis / l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10), std::to_string(l_z_bis) + " / " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_z_bis / l_z, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_z_bis) + " / " + std::to_string(l_z));
        l_ok &= quicky_test::check_expected(l_z_bis / l_x_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-6,5), std::to_string(l_z_bis) + " / " + std::to_string(l_x_bis));
        l_ok &= quicky_test::check_expected(l_z_bis / l_y_bis, quicky_utils::fract<FRACT_INTERNAL_TYPE>(9,10), std::to_string(l_z_bis) + " / " + std::to_string(l_y_bis));
        l_ok &= quicky_test::check_expected(l_z_bis / l_z_bis2, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1), std::to_string(l_z_bis) + " / " + std::to_string(l_z_bis2));

        std::cout << "Operator ++" << std::endl;
        l_ok &= quicky_test::check_expected(l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1, 2), "l_x = 1/2");
        ++l_x;
        l_ok &= quicky_test::check_expected(l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(3, 2), "l_x = 3/2");
        l_x++;
        l_ok &= quicky_test::check_expected(l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(5, 2), "l_x = 5/2");
        std::cout << "Operator --" << std::endl;
        --l_x;
        l_ok &= quicky_test::check_expected(l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(3, 2), "l_x = 3/2");
        l_x--;
        l_ok &= quicky_test::check_expected(l_x, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1, 2), "l_x = 1/2");

        quicky_utils::fract<FRACT_INTERNAL_TYPE> l_zero(1,2);
        assert(l_zero);
        l_zero -= quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,2);
        assert(!l_zero);

        l_x = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,2);
        l_y = quicky_utils::fract<FRACT_INTERNAL_TYPE>(2,5);
        l_ok &= quicky_test::check_expected(l_x < l_y, false, std::to_string(l_x) + " < " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x > l_y, true, std::to_string(l_x) + " > " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x <= l_y, false, std::to_string(l_x) + " <= " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x >= l_y, true, std::to_string(l_x) + " >= " + std::to_string(l_y));
        l_y = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,5);
        l_ok &= quicky_test::check_expected(l_x != l_y, true, std::to_string(l_x) + " != " + std::to_string(l_y));
        l_ok &= quicky_test::check_expected(l_x == l_y, false, std::to_string(l_x) + " == " + std::to_string(l_y));


        l_y = -quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,5);
        l_ok &= quicky_test::check_expected(l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(-1, 5), "l_y = -1/5");
        l_y = +quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,5);
        l_ok &= quicky_test::check_expected(l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(1, 5), "l_y = 1/5");
        l_y += (quicky_utils::fract<FRACT_INTERNAL_TYPE>) 3;
        l_ok &= quicky_test::check_expected(l_y, quicky_utils::fract<FRACT_INTERNAL_TYPE>(16, 5), "l_y = 16/5");

        l_ok &= quicky_test::check_expected(l_x < 2, true, std::to_string(l_x) + " < 2");
        l_ok &= quicky_test::check_expected(l_x <= 2, true, std::to_string(l_x) + " <= 2");
        l_ok &= quicky_test::check_expected(l_x > 2, false, std::to_string(l_x) + " > 2");
        l_ok &= quicky_test::check_expected(l_x >= 2, false, std::to_string(l_x) + " >= 2");

        /* Test for GCC > 4.9.4 */
#if GCC_VERSION >= 40904
        display_NB<quicky_utils::fract<uint32_t>::PGCD(20,15,nullptr)>();
        display_NB<quicky_utils::fract<uint32_t>::PPCM(20,15,nullptr)>();
#endif // GCC_VERSION

        l_ok &= quicky_test::check_expected(1 != l_x, true, std::string("1 != ") + std::to_string(l_x));
        l_ok &= quicky_test::check_expected(l_x != 1, true, std::to_string(l_x) + std::string(" != 1"));

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "- to_float and to double" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        l_a = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,2);
        l_b = quicky_utils::fract<FRACT_INTERNAL_TYPE>(1,3);
        l_ok &= quicky_test::check_expected(l_a.to_float(), 1.0f / 2.0f, std::to_string(l_a) + " to float");
        l_ok &= quicky_test::check_expected(l_b.to_float(), 1.0f / 3.0f, std::to_string(l_b) + " to float");
        l_ok &= quicky_test::check_expected(l_a.to_double(), 1.0 / 2.0, std::to_string(l_a) + " to double");
        l_ok &= quicky_test::check_expected(l_b.to_double(), 1.0 / 3.0, std::to_string(l_b) + " to double");

        return l_ok;
    }
}
#endif //QUICKY_UTILS_TEST_FRACT_H
// EOF
