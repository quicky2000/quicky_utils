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
#ifndef TYPE_STRING_H
#define TYPE_STRING_H

#include "quicky_exception.h"
#include <string>
#include <cinttypes>

namespace quicky_utils
{
    /**
     * Class providing string representation of type name
     * A specialisation should be done for each type
     * @tparam T type to provide name as string
     */
    template <typename T>
    class type_string
    {
      public:
        type_string() = delete;
        type_string(const type_string & ) = delete;
        type_string(const type_string && ) = delete;

        inline static constexpr const std::string & name();
      private:
        static const std::string m_name;
    };

    template <typename T>
    constexpr const std::string & type_string<T>::name()
    {
        return m_name;
    }

    template <typename T>
    const std::string type_string<T>::m_name = "Unknown type";

#define specialise_type_string(type_name)                         \
    template <>                                                   \
    class type_string<type_name>                                  \
    {                                                             \
      public:                                                     \
        type_string() = delete;                                   \
        type_string(const type_string & ) = delete;               \
        type_string(const type_string && ) = delete;              \
                                                                  \
        inline static constexpr const std::string & name();       \
      private:                                                    \
        static const std::string m_name;                          \
    };                                                            \
                                                                  \
    constexpr const std::string & type_string<type_name>::name()  \
    {                                                             \
        return m_name;                                            \
    }                                                             \
                                                                  \
    const std::string type_string<type_name>::m_name = #type_name;

    specialise_type_string(uint8_t)
    specialise_type_string(uint16_t)
    specialise_type_string(uint32_t)
    specialise_type_string(uint64_t)

    specialise_type_string(int8_t)
    specialise_type_string(int16_t)
    specialise_type_string(int32_t)
    specialise_type_string(int64_t)

    specialise_type_string(char)
    specialise_type_string(float)
    specialise_type_string(double)
    specialise_type_string(std::string)

}
#endif //TYPE_STRING_H
// EOF

