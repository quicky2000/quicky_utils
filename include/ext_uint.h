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
#ifndef QUICKY_UTILS_EXT_UINT_H
#define QUICKY_UTILS_EXT_UINT_H

#include "safe_uint.h"
#include "quicky_exception.h"
#include <vector>
#include <cstdio>
#include <cassert>
#include <iomanip>

namespace quicky_utils
{

    template <typename T>
    class ext_uint;

    template <typename T>
    std::ostream &
    operator<<(std::ostream & p_stream,
               const ext_uint<T> & p_infinite_uint
              );

    /**
     * Class defining uint type that will extend as necessary to avoid overflows
     * @tparam T build-in types made safe
     */
    template <typename T>
    class ext_uint
    {
      public:
        /**
         * Empty constructor
         */
        ext_uint();

        /**
         * Move constructor
         */
        ext_uint(const ext_uint && p_value)noexcept;

        /**
         * Copy constructor
         */
        ext_uint(const ext_uint & p_value);

        /**
         * Initializer list constructor
         * @param p_init_list list of values to fill vector
         */
        ext_uint(const std::initializer_list<T> & p_init_list);

        explicit operator bool() const;

        /**
         * Accessor returning number of words composing type
         * @return number of word composing type
         */
        size_t get_nb_words()const;

        /**
         * Accessor returning number of bytes composing type
         * @return number of bytes composing type
         */
        size_t get_nb_bytes() const;

        /**
         * Return word located at p_index
         * @param p_index location of word to return
         * @return word located at place defined by paramter
         */
        const T & get_word(const unsigned int & p_index) const;

        /**
         * Display content of infinite_uint in hexadecimal
         * @param os output stream
         * @param p_infinite_uint infinite_uint to display
         */
        friend std::ostream &
        operator<< <>(std::ostream & os,
                   const ext_uint<T> & p_infinite_uint
                  );

        /**
         * Move assignment operator
         * @param p_op value to move
         * @return assigned object
         */
        ext_uint<T> & operator=(ext_uint<T> && p_op);

        bool
        operator==(const ext_uint & p_op) const;

        bool
        operator!=(const ext_uint & p_op) const;

        bool
        operator<(const ext_uint & p_op) const;

        bool
        operator>(const ext_uint & p_op) const;

        bool
        operator<=(const ext_uint & p_op) const;

        bool
        operator>=(const ext_uint & p_op) const;

        typedef T base_type;

      private:

        ext_uint(const std::vector<T> & p_vec);

        static_assert(std::is_unsigned<T>::value,"Ckeck base type is unsigned");

        /**
         * Extensions
         */
        std::vector<T> m_ext;

    };

    //-------------------------------------------------------------------------
    template <typename T>
    ext_uint<T>::ext_uint():
            m_ext({0})
    {
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    ext_uint<T>::ext_uint(const ext_uint<T> && p_value) noexcept:
            m_ext(std::move(p_value.m_ext))
    {

    }

    //-----------------------------------------------------------------------------
    template <typename T>
    ext_uint<T>::ext_uint(const ext_uint<T> & p_value):
            m_ext(p_value.m_ext)
    {

    }

    //-----------------------------------------------------------------------------
    template <typename T>
    ext_uint<T>::ext_uint(const std::initializer_list<T> & p_init_list):
            m_ext(p_init_list)
    {
        assert(p_init_list.size());
        if(!m_ext.back() && m_ext.size() > 1)
        {
            throw quicky_exception::quicky_logic_exception("Upper word of infinite int should be non-zero", __LINE__, __FILE__);
        }
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    ext_uint<T>::ext_uint(const std::vector<T> & p_vec):
            m_ext(p_vec)
    {

    }

    //-----------------------------------------------------------------------------
    template <typename T>
    ext_uint<T>::operator bool() const
    {
        assert(m_ext.size());
        return m_ext.back();
    }

    //-------------------------------------------------------------------------
    template <typename T>
    size_t
    ext_uint<T>::get_nb_words() const
    {
        return m_ext.size();
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    size_t
    ext_uint<T>::get_nb_bytes() const
    {
        return m_ext.size() * sizeof(T);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    const T &
    ext_uint<T>::get_word(const unsigned int & p_index) const
    {
        assert(p_index < m_ext.size());
        return m_ext[p_index];
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    std::ostream &
    operator<<(std::ostream & p_stream,
               const ext_uint<T> & p_infinite_uint
              )
    {
        p_stream << "0x";
        assert(p_infinite_uint.m_ext.size());
        char l_fill = p_stream.fill();
        std::streamsize l_width = p_stream.width();
        std::ios::fmtflags l_flags = p_stream.flags();

        for(typename std::vector<T>::const_reverse_iterator l_iter = p_infinite_uint.m_ext.rbegin();
            l_iter != p_infinite_uint.m_ext.rend();
            ++l_iter
            )
        {
            p_stream << std::hex << std::setw(2*sizeof(T)) << std::setfill('0') << (uint64_t)(*l_iter);
        }

        p_stream << std::setfill(l_fill) << std::setw(l_width) << std::setiosflags(l_flags);
        return p_stream;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_uint<T> &
    ext_uint<T>::operator=(ext_uint<T> && p_op)
    {
        m_ext = std::move(p_op.m_ext);
        return *this;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    ext_uint<T>::operator==(const ext_uint & p_op) const
    {
        if(m_ext.size() != p_op.m_ext.size())
        {
            return false;
        }
        else
        {
            for(unsigned int l_index = 0;
                l_index < m_ext.size();
                ++l_index
               )
            {
                if(m_ext[l_index] != p_op.m_ext[l_index]) return false;
            }
        }
        return true;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    ext_uint<T>::operator!=(const ext_uint & p_op) const
    {
        if(m_ext.size() != p_op.m_ext.size())
        {
            return true;
        }
        else
        {
            for(unsigned int l_index = 0;
                l_index < m_ext.size();
                ++l_index
                    )
            {
                if(m_ext[l_index] != p_op.m_ext[l_index]) return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    ext_uint<T>::operator<(const ext_uint & p_op) const
    {
        if (m_ext.size() != p_op.m_ext.size())
        {
            return m_ext.size() < p_op.m_ext.size();
        }
        assert(m_ext.size());
        size_t l_index = m_ext.size() - 1;
        while (l_index < m_ext.size())
        {
            if (m_ext[l_index] != p_op.m_ext[l_index])
            {
                return m_ext[l_index] < p_op.m_ext[l_index];
            }
            --l_index;
        }
        return false;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    ext_uint<T>::operator>(const ext_uint & p_op) const
    {
        if (m_ext.size() != p_op.m_ext.size())
        {
            return m_ext.size() > p_op.m_ext.size();
        }
        assert(m_ext.size());
        size_t l_index = m_ext.size() - 1;
        while (l_index < m_ext.size())
        {
            if (m_ext[l_index] != p_op.m_ext[l_index])
            {
                return m_ext[l_index] > p_op.m_ext[l_index];
            }
            --l_index;
        }
        return false;
     }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    ext_uint<T>::operator<=(const ext_uint & p_op) const
    {
        if (m_ext.size() != p_op.m_ext.size())
        {
            return m_ext.size() < p_op.m_ext.size();
        }
        assert(m_ext.size());
        size_t l_index = m_ext.size() - 1;
        while (l_index < m_ext.size())
        {
            if (m_ext[l_index] != p_op.m_ext[l_index])
            {
                return m_ext[l_index] <= p_op.m_ext[l_index];
            }
            --l_index;
        }
        return true;
    }

    //-----------------------------------------------------------------------------
    template <typename T>
    bool
    ext_uint<T>::operator>=(const ext_uint & p_op) const
    {
        if (m_ext.size() != p_op.m_ext.size())
        {
            return m_ext.size() > p_op.m_ext.size();
        }
        assert(m_ext.size());
        size_t l_index = m_ext.size() - 1;
        while (l_index < m_ext.size())
        {
            if (m_ext[l_index] != p_op.m_ext[l_index])
            {
                return m_ext[l_index] >= p_op.m_ext[l_index];
            }
            --l_index;
        }
        return true;
    }

}
#endif //QUICKY_UTILS_EXT_UINT_H
// EOF
