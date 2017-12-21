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

#ifndef QUICKY_UTILS_SAFE_INT_H
#define QUICKY_UTILS_SAFE_INT_H

#include "safe_uint_exception.h"
#include <limits>
#include <iostream>

namespace  quicky_utils
{
    template <typename T>
    class safe_int;

    template <typename T>
    class safe_uint;

    template <typename T>
    std::ostream &
    operator<<(std::ostream & p_stream,
               const safe_int<T> & p_safe_int
              );


    /**
 * Class redefining basic built-in int types but raising exception in case of overflow/underflow etc
 * @tparam T build-in types made safe
 */
    template <typename T>
    class safe_int
    {
      public:
        safe_int(void);
        safe_int(const T & p_value);

        safe_int(const safe_uint<typename std::make_unsigned<T>::type> & p_value);

        explicit operator bool() const;

        bool
        operator!=(const safe_int & p_op) const;

        bool
        operator<(const safe_int & p_op) const;

        bool
        operator>(const safe_int & p_op) const;

        bool
        operator<=(const safe_int & p_op) const;

        bool
        operator>=(const safe_int & p_op) const;

        safe_int
        operator+(const safe_int & p_op);

        safe_int
        operator-(const safe_int & p_op);

        safe_int
        operator*(const safe_int & p_op);

        safe_int
        operator/(const safe_int & p_op);

        safe_int
        operator%(const safe_int & p_op);

        safe_int
        operator-(void)const;

        safe_int
        operator+(void)const;

        const T & get_value(void)const;

        friend std::ostream &
        operator<< <>(std::ostream & os,
                   const safe_int<T> & safe_int
                  );


        typedef T base_type;
      private:
        static_assert(std::is_signed<T>::value,"Ckeck base type is signed");
        T m_value;
    };

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>::operator bool() const
    {
        return m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>::safe_int(void):
            m_value(0)
    {}

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>::safe_int(const T & m_value):
            m_value(m_value)
    {}

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>::safe_int(const safe_uint<typename std::make_unsigned<T>::type> & p_value)
    {
        if(p_value.get_value() > std::numeric_limits<T>::max())
        {
            throw safe_uint_exception("Unsigned to signed constructor overflow",
                                      __LINE__,
                                      __FILE__
                                     );
        }
        m_value = (T)p_value.get_value();
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    safe_int<T>::operator!=(const safe_int & p_op) const
    {
        return m_value != p_op.m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    safe_int<T>::operator<(const safe_int & p_op) const
    {
        return m_value < p_op.m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    safe_int<T>::operator>(const safe_int & p_op) const
    {
        return m_value > p_op.m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    safe_int<T>::operator<=(const safe_int & p_op) const
    {
        return m_value <= p_op.m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    safe_int<T>::operator>=(const safe_int & p_op) const
    {
        return m_value >= p_op.m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator+(const safe_int & p_op)
    {
        T l_sum = m_value + p_op.m_value;
        if(m_value && p_op.m_value)
        {
            unsigned int l_pos1 = m_value >= 0;
            unsigned int l_pos2 = p_op.m_value >= 0;
            unsigned int l_pos_result = l_sum >= 0;
            unsigned int l_status = (l_pos1 << 2) | (l_pos2 << 1) | l_pos_result;
            bool l_exception = false;
            switch (l_status)
            {
                case 0: // (-) + (-) => (-) OK
                case 7: // (+) + (+) => (+) OK
                    break;
                case 2: // (-) + (+) => (-) It depends
                    l_exception = m_value > -p_op.m_value;
                    break;
                case 4: // (+) + (-) => (-) It depends
                    l_exception = -m_value < p_op.m_value;
                    break;
                    // Abs(min) > Abs(max)
                case 3: // (-) + (+) => (+) It depends
                    l_exception = m_value < -p_op.m_value;
                    break;
                case 5: // (+) + (-) => (+) It depends
                    l_exception = -m_value > p_op.m_value;
                    break;

                case 1: // (-) + (-) => (+) NOK
                case 6: // (+) + (+) => (-) NOK
                    l_exception = true;
                    break;
            }
            if (l_exception)
            {
                throw safe_uint_exception("Addition overflow",
                                          __LINE__,
                                          __FILE__
                                         );
            }
        }
        return safe_int(l_sum);
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator-(const safe_int & p_op)
    {
        T l_result = m_value - p_op.m_value;
        unsigned int l_pos1 = m_value >= 0;
        unsigned int l_pos2 = p_op.m_value >= 0;
        unsigned int l_pos_result = l_result >= 0;
        unsigned int l_status = (l_pos1 << 2) | (l_pos2 << 1) | l_pos_result;
        bool l_exception = false;
        switch (l_status)
        {
            case 2: // (-) - (+) => (-) OK
            case 5: // (+) - (-) => (+) OK
                break;

            case 0: // (-) - (-) => (-) It depends
            case 6: // (+) - (+) => (-) It depends
                l_exception = m_value > p_op.m_value;
                break;
            case 1: // (-) - (-) => (+) It depends
            case 7: // (+) - (+) => (+) It depends
                l_exception = m_value < p_op.m_value;
                break;
            case 4: // (+) - (-) => (-) NOK
            case 3: // (-) - (+) => (+) NOK
                l_exception = true;
            break;
        }
        if (l_exception)
        {
            throw safe_uint_exception("Substraction underflow",
                                      __LINE__,
                                      __FILE__
                                     );
        }
        return safe_int(l_result);
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator*(const safe_int & p_op)
    {
        T l_result = m_value * p_op.m_value;
        if(m_value && p_op.m_value)
        {
            unsigned int l_pos1 = m_value >= 0;
            unsigned int l_pos2 = p_op.m_value >= 0;
            bool l_pos_result = l_result >= 0;
            bool l_exception = false;
            // (-) * (-) => (-)
            // (-) * (+) => (+)
            // (+) * (-) => (+)
            // (+) * (+) => (-)
            if ((l_pos1 ^ l_pos2) == l_pos_result)
            {
                l_exception = true;
            }
            else
            {
                switch ((l_pos1 << 1) + l_pos2)
                {
                    case 0:
                        l_exception = (std::numeric_limits<T>::max() / -m_value) < -p_op.m_value;
                        break;
                    case 1:
                        l_exception = (std::numeric_limits<T>::min() / m_value) < p_op.m_value;
                        break;
                    case 2:
                        l_exception = (std::numeric_limits<T>::min() / -m_value) < -p_op.m_value;
                        break;
                    case 3:
                        l_exception = (std::numeric_limits<T>::max() / m_value) < p_op.m_value;
                        break;
                }
            }
            if(l_exception)
            {
                throw safe_uint_exception("Multiplication overflow",
                                          __LINE__,
                                          __FILE__
                                         );
            }
        }
        return safe_int(l_result);
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator/(const safe_int & p_op)
    {
        T l_result = m_value / p_op.m_value;
        if(m_value)
        {
            unsigned int l_pos1 = m_value >= 0;
            unsigned int l_pos2 = p_op.m_value >= 0;
            bool l_pos_result = l_result >= 0;
            if (l_result && (l_pos1 ^ l_pos2) == l_pos_result)
            {
                throw safe_uint_exception("Division overflow",
                                          __LINE__,
                                          __FILE__
                                         );
            }
        }
        return safe_int(l_result);
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator%(const safe_int & p_op)
    {
        T l_result = m_value % p_op.m_value;
        return safe_int(l_result);
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator-(void)const
    {
        if (std::numeric_limits<T>::min() == m_value)
        {
            throw quicky_exception::quicky_logic_exception("safe_int unitary - overflow",
                                                           __LINE__,
                                                           __FILE__
                                                          );
        }
        return safe_int<T>(-m_value);
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    safe_int<T>
    safe_int<T>::operator+(void)const
    {

    }

    //-----------------------------------------------------------------------------
    template <typename T>
    const T &
    safe_int<T>::get_value(void) const
    {
        return m_value;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    std::ostream &
    operator<<(std::ostream & p_stream,
               const safe_int<T> & p_safe_int
              )
    {
        p_stream << p_safe_int.m_value;
        return p_stream;
    }

}

namespace std
{
    template <typename T>
    quicky_utils::safe_int<T>
    abs(const quicky_utils::safe_int<T> & p_safe_int)
    {
        if (std::numeric_limits<T>::min() == p_safe_int.get_value())
        {
            throw quicky_exception::quicky_logic_exception("safe_int abs overflow",
                                                           __LINE__,
                                                           __FILE__
                                                          );
        }
        return quicky_utils::safe_int<T>(std::abs(p_safe_int.get_value()));
    }

    template <>
    template <typename T>
    class is_signed<quicky_utils::safe_int<T> >
    {
      public:
        static const bool value = true;
    };

    template <>
    template <typename T>
    class make_signed<quicky_utils::safe_int<T> >
    {
      public:
        typedef quicky_utils::safe_int<typename std::make_signed<T>::type> type;
    };

    template <>
    template <typename T>
    class make_unsigned<quicky_utils::safe_int<T> >
    {
      public:
        typedef quicky_utils::safe_uint<typename std::make_unsigned<T>::type> type;
    };

    template <>
    template <typename T>
    class numeric_limits<quicky_utils::safe_int<T> >
    {
      public:
        static constexpr bool is_specialized = true;

        static constexpr quicky_utils::safe_int<T>
        min() noexcept
        {
            return quicky_utils::safe_int<T>(std::numeric_limits<T>::min());
        }

        static constexpr quicky_utils::safe_int<T>
        max() noexcept
        {
            return quicky_utils::safe_int<T>(std::numeric_limits<T>::max());
        }

        static constexpr quicky_utils::safe_int<T>
        lowest() noexcept
        {
            return quicky_utils::safe_int<T>(std::numeric_limits<T>::lowest());
        }

        static constexpr int digits = std::numeric_limits<T>::digits;
        static constexpr int digits10 = std::numeric_limits<T>::digits10;
        static constexpr int max_digits10 = std::numeric_limits<T>::max_digits10;
        static constexpr bool is_signed = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact = true;
        static constexpr int radix = 2;

        static constexpr quicky_utils::safe_int<T>
        epsilon() noexcept
        {
            return std::numeric_limits<T>::epsilon();
        }

        static constexpr quicky_utils::safe_int<T>
        round_error() noexcept
        {
            return std::numeric_limits<T>::round_error();
        }

        static constexpr int min_exponent = 0; // No sense
        static constexpr int min_exponent10 = 0; // No sense
        static constexpr int max_exponent = 0; // No sense
        static constexpr int max_exponent10 = 0; // No sense
        static constexpr bool has_infinity = false;
        static constexpr bool has_quiet_NaN = false;
        static constexpr bool has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm = denorm_absent;
        static constexpr bool has_denorm_loss = false;

        static constexpr bool
        infinity() noexcept
        { return 0; } // No sense
        static constexpr bool
        quiet_NaN() noexcept
        { return 0; } // No sense
        static constexpr bool
        signaling_NaN() noexcept
        { return 0; } // No sense
        static constexpr bool
        denorm_min() noexcept
        { return 0; } // No sense
        static constexpr bool is_iec559 = false;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = false;
        static constexpr bool traps = true;
        static constexpr bool tinyness_before = false;
        static constexpr float_round_style round_style = round_toward_zero;
    };
}

template <typename T>
quicky_utils::safe_int<T> abs(const quicky_utils::safe_int<T> & p_safe_int)
{
    return std::abs(p_safe_int);
}
#endif //QUICKY_UTILS_SAFE_INT_H
