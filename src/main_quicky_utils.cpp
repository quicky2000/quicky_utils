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
#include "quicky_exception.h"
#include "fract.h"
#include <iostream>


//------------------------------------------------------------------------------
int main(int argc,char ** argv)
{
  try
    {
      quicky_utils::fract l_nb1(1,2);
      quicky_utils::fract l_nb2(2);
      quicky_utils::fract l_nb3(10,2);
      std::cout << l_nb1 << std::endl;
      std::cout << l_nb2 << std::endl;
      std::cout << l_nb3 << std::endl;

      quicky_utils::fract::t_coef l_result = quicky_utils::fract::PGCD(1071,1029);
      std::cout << l_result << std::endl;
      assert(21 == l_result);

      l_result = quicky_utils::fract::PGCD(1029,1071);
      std::cout << l_result << std::endl;
      assert(21 == l_result);

      l_result = quicky_utils::fract::PGCD(60,168);
      std::cout << l_result << std::endl;
      assert(12 == l_result);

      l_result = quicky_utils::fract::PGCD(168,60);
      std::cout << l_result << std::endl;
      assert(12 == l_result);

      l_result = quicky_utils::fract::PPCM(60,168);
      assert(840 == l_result);
      std::cout << l_result << std::endl;

      l_result = quicky_utils::fract::PPCM(168,60);
      assert(840 == l_result);
      std::cout << l_result << std::endl;

      std::cout << quicky_utils::fract::PPCM(6,9) << std::endl;
      std::cout << quicky_utils::fract::PGCD(6,9) << std::endl;

      quicky_utils::fract l_a(1,3);
      quicky_utils::fract l_b(1,4);
      quicky_utils::fract l_c = l_a + l_b;
      std::cout << l_a << " + " << l_b << " => " << l_c << std::endl;
      assert(quicky_utils::fract(7,12) == l_c);

      quicky_utils::fract l_d = l_a - l_b;
      std::cout << l_a << " - " << l_b << " => " << l_d << std::endl;
      assert(quicky_utils::fract(1,12) == l_d);

      quicky_utils::fract l_e = l_a * l_b;
      std::cout << l_a << " * " << l_b << " => " << l_e << std::endl;
      assert(quicky_utils::fract(1,12) == l_e);

      quicky_utils::fract l_f = l_a / l_b;
      std::cout << l_a << " / " << l_b << " => " << l_f << std::endl;
      assert(quicky_utils::fract(4,3) == l_f);

      std::cout << 1 << " + " << l_a << " => " << 1 + l_a << std::endl;
      assert(quicky_utils::fract(4,3) == 1 + l_a);

      quicky_utils::fract l_x(1,2);
      quicky_utils::fract l_y(2,-3);
      quicky_utils::fract l_z(-3,5);
      quicky_utils::fract l_x_bis(-1,-2);
      quicky_utils::fract l_y_bis(-2,3);
      quicky_utils::fract l_z_bis(3,-5);

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
      assert(quicky_utils::fract(-1,6) == l_x + l_y);

      std::cout << l_x << " + " << l_z << " => " << l_x + l_z << std::endl;
      assert(quicky_utils::fract(-1,10) == l_x + l_z);

      std::cout << l_x << " + " << l_x_bis << " => " << l_x + l_x_bis << std::endl;
      assert(1 == l_x + l_x_bis);

      std::cout << l_x << " + " << l_y_bis << " => " << l_x + l_y_bis << std::endl;
      assert(quicky_utils::fract(-1,6) == l_x + l_y_bis);

      std::cout << l_x << " + " << l_z_bis << " => " << l_x + l_z_bis << std::endl;
      assert(quicky_utils::fract(-1,10) == l_x + l_z_bis);


      //------------------------------------------------------------------------
      std::cout << l_y << " + " << l_x << " => " << l_y + l_x << std::endl;
      assert(quicky_utils::fract(-1,6) == l_y + l_x);

      std::cout << l_y << " + " << l_y << " => " << l_y + l_y << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y + l_y);

      std::cout << l_y << " + " << l_z << " => " << l_y + l_z << std::endl;
      assert(quicky_utils::fract(-19,15) == l_y + l_z);

      std::cout << l_y << " + " << l_x_bis << " => " << l_y + l_x_bis << std::endl;
      assert(quicky_utils::fract(-1,6) == l_y + l_x_bis);

      std::cout << l_y << " + " << l_y_bis << " => " << l_y + l_y_bis << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y + l_y_bis);

      std::cout << l_y << " + " << l_z_bis << " => " << l_y + l_z_bis << std::endl;
      assert(quicky_utils::fract(-19,15) == l_y + l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z << " + " << l_x << " => " << l_z + l_x << std::endl;
      assert(quicky_utils::fract(-1,10) == l_z + l_x);

      std::cout << l_z << " + " << l_y << " => " << l_z + l_y << std::endl;
      assert(quicky_utils::fract(-19,15) == l_z + l_y);

      std::cout << l_z << " + " << l_z << " => " << l_z + l_z << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z + l_z);

      std::cout << l_z << " + " << l_x_bis << " => " << l_z + l_x_bis << std::endl;
      assert(quicky_utils::fract(-1,10) == l_z + l_x_bis);

      std::cout << l_z << " + " << l_y_bis << " => " << l_z + l_y_bis << std::endl;
      assert(quicky_utils::fract(-19,15) == l_z + l_y_bis);

      std::cout << l_z << " + " << l_z_bis << " => " << l_z + l_z_bis << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z + l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_x_bis << " + " << l_x << " => " << l_x_bis + l_x << std::endl;
      assert(1 == l_x_bis + l_x);

      std::cout << l_x_bis << " + " << l_y << " => " << l_x_bis + l_y << std::endl;
      assert(quicky_utils::fract(-1,6) == l_x_bis + l_y);

      std::cout << l_x_bis << " + " << l_z << " => " << l_x_bis + l_z << std::endl;
      assert(quicky_utils::fract(-1,10) == l_x_bis + l_z);

      std::cout << l_x_bis << " + " << l_x_bis << " => " << l_x_bis + l_x_bis << std::endl;
      assert(1 == l_x_bis + l_x_bis);

      std::cout << l_x_bis << " + " << l_y_bis << " => " << l_x_bis + l_y_bis << std::endl;
      assert(quicky_utils::fract(-1,6) == l_x_bis + l_y_bis);

      std::cout << l_x_bis << " + " << l_z_bis << " => " << l_x_bis + l_z_bis << std::endl;
      assert(quicky_utils::fract(-1,10) == l_x_bis + l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_y_bis << " + " << l_x << " => " << l_y_bis + l_x << std::endl;
      assert(quicky_utils::fract(-1,6) == l_y_bis + l_x);

      std::cout << l_y_bis << " + " << l_y << " => " << l_y_bis + l_y << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y_bis + l_y);

      std::cout << l_y_bis << " + " << l_z << " => " << l_y_bis + l_z << std::endl;
      assert(quicky_utils::fract(-19,15) == l_y_bis + l_z);

      std::cout << l_y_bis << " + " << l_x_bis << " => " << l_y_bis + l_x_bis << std::endl;
      assert(quicky_utils::fract(-1,6) == l_y_bis + l_x_bis);

      std::cout << l_y_bis << " + " << l_y_bis << " => " << l_y_bis + l_y_bis << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y_bis + l_y_bis);

      std::cout << l_y_bis << " + " << l_z_bis << " => " << l_y_bis + l_z_bis << std::endl;
      assert(quicky_utils::fract(-19,15) == l_y_bis + l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z_bis << " + " << l_x << " => " << l_z_bis + l_x << std::endl;
      assert(quicky_utils::fract(-1,10) == l_z_bis + l_x);

      std::cout << l_z_bis << " + " << l_y << " => " << l_z_bis + l_y << std::endl;
      assert(quicky_utils::fract(-19,15) == l_z_bis + l_y);

      std::cout << l_z_bis << " + " << l_z << " => " << l_z_bis + l_z << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z_bis + l_z);

      std::cout << l_z_bis << " + " << l_x_bis << " => " << l_z_bis + l_x_bis << std::endl;
      assert(quicky_utils::fract(-1,10) == l_z_bis + l_x_bis);

      std::cout << l_z_bis << " + " << l_y_bis << " => " << l_z_bis + l_y_bis << std::endl;
      assert(quicky_utils::fract(-19,15) == l_z_bis + l_y_bis);

      std::cout << l_z_bis << " + " << l_z_bis << " => " << l_z_bis + l_z_bis << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z_bis + l_z_bis);

      //----------------------------------------
      // Operator -
      //----------------------------------------
      //------------------------------------------------------------------------
      std::cout << l_x << " - " << l_x << " => " << l_x - l_x << std::endl;
      assert(0 == l_x - l_x);

      std::cout << l_x << " - " << l_y << " => " << l_x - l_y << std::endl;
      assert(quicky_utils::fract(7,6) == l_x - l_y);

      std::cout << l_x << " - " << l_z << " => " << l_x - l_z << std::endl;
      assert(quicky_utils::fract(11,10) == l_x - l_z);

      std::cout << l_x << " - " << l_x_bis << " => " << l_x - l_x_bis << std::endl;
      assert(0 == l_x - l_x_bis);

      std::cout << l_x << " - " << l_y_bis << " => " << l_x - l_y_bis << std::endl;
      assert(quicky_utils::fract(7,6) == l_x - l_y_bis);

      std::cout << l_x << " - " << l_z_bis << " => " << l_x - l_z_bis << std::endl;
      assert(quicky_utils::fract(11,10) == l_x - l_z_bis);


      //------------------------------------------------------------------------
      std::cout << l_y << " - " << l_x << " => " << l_y - l_x << std::endl;
      assert(quicky_utils::fract(-7,6) == l_y - l_x);

      std::cout << l_y << " - " << l_y << " => " << l_y - l_y << std::endl;
      assert(0 == l_y - l_y);

      std::cout << l_y << " - " << l_z << " => " << l_y - l_z << std::endl;
      assert(quicky_utils::fract(-1,15) == l_y - l_z);

      std::cout << l_y << " - " << l_x_bis << " => " << l_y - l_x_bis << std::endl;
      assert(quicky_utils::fract(-7,6) == l_y - l_x_bis);

      std::cout << l_y << " - " << l_y_bis << " => " << l_y - l_y_bis << std::endl;
      assert(0 == l_y - l_y_bis);

      std::cout << l_y << " - " << l_z_bis << " => " << l_y - l_z_bis << std::endl;
      assert(quicky_utils::fract(-1,15) == l_y - l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z << " - " << l_x << " => " << l_z - l_x << std::endl;
      assert(quicky_utils::fract(-11,10) == l_z - l_x);

      std::cout << l_z << " - " << l_y << " => " << l_z - l_y << std::endl;
      assert(quicky_utils::fract(1,15) == l_z - l_y);

      std::cout << l_z << " - " << l_z << " => " << l_z - l_z << std::endl;
      assert(0 == l_z - l_z);

      std::cout << l_z << " - " << l_x_bis << " => " << l_z - l_x_bis << std::endl;
      assert(quicky_utils::fract(-11,10) == l_z - l_x_bis);

      std::cout << l_z << " - " << l_y_bis << " => " << l_z - l_y_bis << std::endl;
      assert(quicky_utils::fract(1,15) == l_z - l_y_bis);

      std::cout << l_z << " - " << l_z_bis << " => " << l_z - l_z_bis << std::endl;
      assert(0 == l_z - l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_x_bis << " - " << l_x << " => " << l_x_bis - l_x << std::endl;
      assert(0 == l_x_bis - l_x);

      std::cout << l_x_bis << " - " << l_y << " => " << l_x_bis - l_y << std::endl;
      assert(quicky_utils::fract(7,6) == l_x_bis - l_y);

      std::cout << l_x_bis << " - " << l_z << " => " << l_x_bis - l_z << std::endl;
      assert(quicky_utils::fract(11,10) == l_x_bis - l_z);

      std::cout << l_x_bis << " - " << l_x_bis << " => " << l_x_bis - l_x_bis << std::endl;
      assert(0 == l_x_bis - l_x_bis);

      std::cout << l_x_bis << " - " << l_y_bis << " => " << l_x_bis - l_y_bis << std::endl;
      assert(quicky_utils::fract(7,6) == l_x_bis - l_y_bis);

      std::cout << l_x_bis << " - " << l_z_bis << " => " << l_x_bis - l_z_bis << std::endl;
      assert(quicky_utils::fract(11,10) == l_x_bis - l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_y_bis << " - " << l_x << " => " << l_y_bis - l_x << std::endl;
      assert(quicky_utils::fract(-7,6) == l_y_bis - l_x);

      std::cout << l_y_bis << " - " << l_y << " => " << l_y_bis - l_y << std::endl;
      assert(0 == l_y_bis - l_y);

      std::cout << l_y_bis << " - " << l_z << " => " << l_y_bis - l_z << std::endl;
      assert(quicky_utils::fract(-1,15) == l_y_bis - l_z);

      std::cout << l_y_bis << " - " << l_x_bis << " => " << l_y_bis - l_x_bis << std::endl;
      assert(quicky_utils::fract(-7,6) == l_y_bis - l_x_bis);

      std::cout << l_y_bis << " - " << l_y_bis << " => " << l_y_bis - l_y_bis << std::endl;
      assert(0 == l_y_bis - l_y_bis);

      std::cout << l_y_bis << " - " << l_z_bis << " => " << l_y_bis - l_z_bis << std::endl;
      assert(quicky_utils::fract(-1,15) == l_y_bis - l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z_bis << " - " << l_x << " => " << l_z_bis - l_x << std::endl;
      assert(quicky_utils::fract(-11,10) == l_z_bis - l_x);

      std::cout << l_z_bis << " - " << l_y << " => " << l_z_bis - l_y << std::endl;
      assert(quicky_utils::fract(1,15) == l_z_bis - l_y);

      std::cout << l_z_bis << " - " << l_z << " => " << l_z_bis - l_z << std::endl;
      assert(0 == l_z_bis - l_z);

      std::cout << l_z_bis << " - " << l_x_bis << " => " << l_z_bis - l_x_bis << std::endl;
      assert(quicky_utils::fract(-11,10) == l_z_bis - l_x_bis);

      std::cout << l_z_bis << " - " << l_y_bis << " => " << l_z_bis - l_y_bis << std::endl;
      assert(quicky_utils::fract(1,15) == l_z_bis - l_y_bis);

      std::cout << l_z_bis << " - " << l_z_bis << " => " << l_z_bis - l_z_bis << std::endl;
      assert(0 == l_z_bis - l_z_bis);


      //----------------------------------------
      // Operator *
      //----------------------------------------
      //------------------------------------------------------------------------
      std::cout << l_x << " * " << l_x << " => " << l_x * l_x << std::endl;
      assert(quicky_utils::fract(1,4) == l_x * l_x);

      std::cout << l_x << " * " << l_y << " => " << l_x * l_y << std::endl;
      assert(quicky_utils::fract(-2,6) == l_x * l_y);

      std::cout << l_x << " * " << l_z << " => " << l_x * l_z << std::endl;
      assert(quicky_utils::fract(-3,10) == l_x * l_z);

      std::cout << l_x << " * " << l_x_bis << " => " << l_x * l_x_bis << std::endl;
      assert(quicky_utils::fract(1,4) == l_x * l_x_bis);

      std::cout << l_x << " * " << l_y_bis << " => " << l_x * l_y_bis << std::endl;
      assert(quicky_utils::fract(-2,6) == l_x * l_y_bis);

      std::cout << l_x << " * " << l_z_bis << " => " << l_x * l_z_bis << std::endl;
      assert(quicky_utils::fract(-3,10) == l_x * l_z_bis);


      //------------------------------------------------------------------------
      std::cout << l_y << " * " << l_x << " => " << l_y * l_x << std::endl;
      assert(quicky_utils::fract(-2,6) == l_y * l_x);

      std::cout << l_y << " * " << l_y << " => " << l_y * l_y << std::endl;
      assert(quicky_utils::fract(4,9) == l_y * l_y);

      std::cout << l_y << " * " << l_z << " => " << l_y * l_z << std::endl;
      assert(quicky_utils::fract(2,5) == l_y * l_z);

      std::cout << l_y << " * " << l_x_bis << " => " << l_y * l_x_bis << std::endl;
      assert(quicky_utils::fract(-2,6) == l_y * l_x_bis);

      std::cout << l_y << " * " << l_y_bis << " => " << l_y * l_y_bis << std::endl;
      assert(quicky_utils::fract(4,9) == l_y * l_y_bis);

      std::cout << l_y << " * " << l_z_bis << " => " << l_y * l_z_bis << std::endl;
      assert(quicky_utils::fract(2,5) == l_y * l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z << " * " << l_x << " => " << l_z * l_x << std::endl;
      assert(quicky_utils::fract(-3,10) == l_z * l_x);

      std::cout << l_z << " * " << l_y << " => " << l_z * l_y << std::endl;
      assert(quicky_utils::fract(2,5) == l_z * l_y);

      std::cout << l_z << " * " << l_z << " => " << l_z * l_z << std::endl;
      assert(quicky_utils::fract(9,25) == l_z * l_z);

      std::cout << l_z << " * " << l_x_bis << " => " << l_z * l_x_bis << std::endl;
      assert(quicky_utils::fract(-3,10) == l_z * l_x_bis);

      std::cout << l_z << " * " << l_y_bis << " => " << l_z * l_y_bis << std::endl;
      assert(quicky_utils::fract(2,5) == l_z * l_y_bis);

      std::cout << l_z << " * " << l_z_bis << " => " << l_z * l_z_bis << std::endl;
      assert(quicky_utils::fract(9,25) == l_z * l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_x_bis << " * " << l_x << " => " << l_x_bis * l_x << std::endl;
      assert(quicky_utils::fract(1,4) == l_x_bis * l_x);

      std::cout << l_x_bis << " * " << l_y << " => " << l_x_bis * l_y << std::endl;
      assert(quicky_utils::fract(-1,3) == l_x_bis * l_y);

      std::cout << l_x_bis << " * " << l_z << " => " << l_x_bis * l_z << std::endl;
      assert(quicky_utils::fract(-3,10) == l_x_bis * l_z);

      std::cout << l_x_bis << " * " << l_x_bis << " => " << l_x_bis * l_x_bis << std::endl;
      assert(quicky_utils::fract(1,4) == l_x_bis * l_x_bis);

      std::cout << l_x_bis << " * " << l_y_bis << " => " << l_x_bis * l_y_bis << std::endl;
      assert(quicky_utils::fract(-1,3) == l_x_bis * l_y_bis);

      std::cout << l_x_bis << " * " << l_z_bis << " => " << l_x_bis * l_z_bis << std::endl;
      assert(quicky_utils::fract(-3,10) == l_x_bis * l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_y_bis << " * " << l_x << " => " << l_y_bis * l_x << std::endl;
      assert(quicky_utils::fract(-1,3) == l_y_bis * l_x);

      std::cout << l_y_bis << " * " << l_y << " => " << l_y_bis * l_y << std::endl;
      assert(quicky_utils::fract(4,9) == l_y_bis * l_y);

      std::cout << l_y_bis << " * " << l_z << " => " << l_y_bis * l_z << std::endl;
      assert(quicky_utils::fract(2,5) == l_y_bis * l_z);

      std::cout << l_y_bis << " * " << l_x_bis << " => " << l_y_bis * l_x_bis << std::endl;
      assert(quicky_utils::fract(-1,3) == l_y_bis * l_x_bis);

      std::cout << l_y_bis << " * " << l_y_bis << " => " << l_y_bis * l_y_bis << std::endl;
      assert(quicky_utils::fract(4,9) == l_y_bis * l_y_bis);

      std::cout << l_y_bis << " * " << l_z_bis << " => " << l_y_bis * l_z_bis << std::endl;
      assert(quicky_utils::fract(2,5) == l_y_bis * l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z_bis << " * " << l_x << " => " << l_z_bis * l_x << std::endl;
      assert(quicky_utils::fract(-3,10) == l_z_bis * l_x);

      std::cout << l_z_bis << " * " << l_y << " => " << l_z_bis * l_y << std::endl;
      assert(quicky_utils::fract(2,5) == l_z_bis * l_y);

      std::cout << l_z_bis << " * " << l_z << " => " << l_z_bis * l_z << std::endl;
      assert(quicky_utils::fract(9,25) == l_z_bis * l_z);

      std::cout << l_z_bis << " * " << l_x_bis << " => " << l_z_bis * l_x_bis << std::endl;
      assert(quicky_utils::fract(-3,10) == l_z_bis * l_x_bis);

      std::cout << l_z_bis << " * " << l_y_bis << " => " << l_z_bis * l_y_bis << std::endl;
      assert(quicky_utils::fract(2,5) == l_z_bis * l_y_bis);

      std::cout << l_z_bis << " * " << l_z_bis << " => " << l_z_bis * l_z_bis << std::endl;
      assert(quicky_utils::fract(9,25) == l_z_bis * l_z_bis);

      //----------------------------------------
      // Operator /
      //----------------------------------------
      //------------------------------------------------------------------------
      std::cout << l_x << " / " << l_x << " => " << l_x / l_x << std::endl;
      assert(1 == l_x / l_x);

      std::cout << l_x << " / " << l_y << " => " << l_x / l_y << std::endl;
      assert(quicky_utils::fract(-3,4) == l_x / l_y);

      std::cout << l_x << " / " << l_z << " => " << l_x / l_z << std::endl;
      assert(quicky_utils::fract(-5,6) == l_x / l_z);

      std::cout << l_x << " / " << l_x_bis << " => " << l_x / l_x_bis << std::endl;
      assert(1 == l_x / l_x_bis);

      std::cout << l_x << " / " << l_y_bis << " => " << l_x / l_y_bis << std::endl;
      assert(quicky_utils::fract(-3,4) == l_x / l_y_bis);

      std::cout << l_x << " / " << l_z_bis << " => " << l_x / l_z_bis << std::endl;
      assert(quicky_utils::fract(-5,6) == l_x / l_z_bis);


      //------------------------------------------------------------------------
      std::cout << l_y << " / " << l_x << " => " << l_y / l_x << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y / l_x);

      std::cout << l_y << " / " << l_y << " => " << l_y / l_y << std::endl;
      assert(1 == l_y / l_y);

      std::cout << l_y << " / " << l_z << " => " << l_y / l_z << std::endl;
      assert(quicky_utils::fract(10,9) == l_y / l_z);

      std::cout << l_y << " / " << l_x_bis << " => " << l_y / l_x_bis << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y / l_x_bis);

      std::cout << l_y << " / " << l_y_bis << " => " << l_y / l_y_bis << std::endl;
      assert(1 == l_y / l_y_bis);

      std::cout << l_y << " / " << l_z_bis << " => " << l_y / l_z_bis << std::endl;
      assert(quicky_utils::fract(10,9) == l_y / l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z << " / " << l_x << " => " << l_z / l_x << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z / l_x);

      std::cout << l_z << " / " << l_y << " => " << l_z / l_y << std::endl;
      assert(quicky_utils::fract(9,10) == l_z / l_y);

      std::cout << l_z << " / " << l_z << " => " << l_z / l_z << std::endl;
      assert(1 == l_z / l_z);

      std::cout << l_z << " / " << l_x_bis << " => " << l_z / l_x_bis << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z / l_x_bis);

      std::cout << l_z << " / " << l_y_bis << " => " << l_z / l_y_bis << std::endl;
      assert(quicky_utils::fract(9,10) == l_z / l_y_bis);

      std::cout << l_z << " / " << l_z_bis << " => " << l_z / l_z_bis << std::endl;
      assert(1 == l_z / l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_x_bis << " / " << l_x << " => " << l_x_bis / l_x << std::endl;
      assert(1 == l_x_bis / l_x);

      std::cout << l_x_bis << " / " << l_y << " => " << l_x_bis / l_y << std::endl;
      assert(quicky_utils::fract(-3,4) == l_x_bis / l_y);

      std::cout << l_x_bis << " / " << l_z << " => " << l_x_bis / l_z << std::endl;
      assert(quicky_utils::fract(-5,6) == l_x_bis / l_z);

      std::cout << l_x_bis << " / " << l_x_bis << " => " << l_x_bis / l_x_bis << std::endl;
      assert(1 == l_x_bis / l_x_bis);

      std::cout << l_x_bis << " / " << l_y_bis << " => " << l_x_bis / l_y_bis << std::endl;
      assert(quicky_utils::fract(-3,4) == l_x_bis / l_y_bis);

      std::cout << l_x_bis << " / " << l_z_bis << " => " << l_x_bis / l_z_bis << std::endl;
      assert(quicky_utils::fract(-5,6) == l_x_bis / l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_y_bis << " / " << l_x << " => " << l_y_bis / l_x << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y_bis / l_x);

      std::cout << l_y_bis << " / " << l_y << " => " << l_y_bis / l_y << std::endl;
      assert(1 == l_y_bis / l_y);

      std::cout << l_y_bis << " / " << l_z << " => " << l_y_bis / l_z << std::endl;
      assert(quicky_utils::fract(10,9) == l_y_bis / l_z);

      std::cout << l_y_bis << " / " << l_x_bis << " => " << l_y_bis / l_x_bis << std::endl;
      assert(quicky_utils::fract(-4,3) == l_y_bis / l_x_bis);

      std::cout << l_y_bis << " / " << l_y_bis << " => " << l_y_bis / l_y_bis << std::endl;
      assert(1 == l_y_bis / l_y_bis);

      std::cout << l_y_bis << " / " << l_z_bis << " => " << l_y_bis / l_z_bis << std::endl;
      assert(quicky_utils::fract(10,9) == l_y_bis / l_z_bis);

      //------------------------------------------------------------------------
      std::cout << l_z_bis << " / " << l_x << " => " << l_z_bis / l_x << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z_bis / l_x);

      std::cout << l_z_bis << " / " << l_y << " => " << l_z_bis / l_y << std::endl;
      assert(quicky_utils::fract(9,10) == l_z_bis / l_y);

      std::cout << l_z_bis << " / " << l_z << " => " << l_z_bis / l_z << std::endl;
      assert(1 == l_z_bis / l_z);

      std::cout << l_z_bis << " / " << l_x_bis << " => " << l_z_bis / l_x_bis << std::endl;
      assert(quicky_utils::fract(-6,5) == l_z_bis / l_x_bis);

      std::cout << l_z_bis << " / " << l_y_bis << " => " << l_z_bis / l_y_bis << std::endl;
      assert(quicky_utils::fract(9,10) == l_z_bis / l_y_bis);

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
    }
  catch(quicky_exception::quicky_runtime_exception & e)
    {
      std::cout << "ERROR : " << e.what() << std::endl ;
      return(-1);
    }
  catch(quicky_exception::quicky_logic_exception & e)
    {
      std::cout << "ERROR : " << e.what() << std::endl ;
      return(-1);
    }
  return 0;
  
}
#endif // QUICKY_UTILS_SELF_TEST
//EOF