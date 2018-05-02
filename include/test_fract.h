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

template<unsigned int NB>
void display_NB()
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

#endif //QUICKY_UTILS_TEST_FRACT_H
// EOF
