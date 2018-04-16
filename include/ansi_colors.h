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

#ifndef QUICKY_UTILS_ANSI_COLORS_H
#define QUICKY_UTILS_ANSI_COLORS_H

#include "quicky_exception.h"
#include <iostream>
#include <string>

namespace quicky_utils
{
    typedef enum class _ansi_color
    {
        black = 0,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        def = 9
    } ansi_color;

    class ansi_escape_code
    {
        inline friend
        std::ostream &
        operator<<(std::ostream & p_stream,
                   const ansi_escape_code & p_color
                  );
      protected:
        inline ansi_escape_code(const std::string & p_code);

        std::string m_code;
    };

    //-------------------------------------------------------------------------
    std::ostream &
    operator<<(std::ostream & p_stream,
               const ansi_escape_code & p_color
              )
    {
        p_stream << "\x1b[" << p_color.m_code << "m";
        return p_stream;
    }

    //-------------------------------------------------------------------------
    ansi_escape_code::ansi_escape_code(const std::string & p_code):
            m_code(p_code)
    {
    }

    /**
     * I/O manipulator to set characters color
     */
    class set_fcolor: public ansi_escape_code
    {
      public:
        /**
         * Constructor for 3 bits colors
         * @param p_code code of 8 bit color
         */
        inline set_fcolor(const ansi_color & p_code);

    };

    //-------------------------------------------------------------------------
    set_fcolor::set_fcolor(const ansi_color & p_code):
            ansi_escape_code(std::to_string(30 + (unsigned int)p_code))
    {

    }

    /**
     * I/O manipulator to set background color
     */
    class set_bcolor: public ansi_escape_code
    {
      public:
        /**
         * Constructor for 3 bits colors
         * @param p_code code of 8 bit color
         */
        inline set_bcolor(const ansi_color & p_code);

    };

    //-------------------------------------------------------------------------
    set_bcolor::set_bcolor(const ansi_color & p_code):
            ansi_escape_code(std::to_string(40 + (unsigned int)p_code))
    {

    }

}
#endif //QUICKY_UTILS_ANSI_COLORS_H
//EOF
