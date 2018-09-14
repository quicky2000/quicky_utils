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
#ifndef QUICKY_UTILS_EXT_INT_H
#define QUICKY_UTILS_EXT_INT_H

#include "safe_int.h"
#include "quicky_exception.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <type_traits>
#include <sstream>
#include <cassert>

namespace quicky_utils
{
    template <typename T>
    class ext_int;

    template <typename T>
    class ext_uint;

    template <typename T>
    std::ostream &
    operator<<(std::ostream & p_stream,
               const ext_int<T> & p_ext_uint
              );

    /**
     * Class defining uint type that will extend as necessary to avoid overflows
     * @tparam T build-in types made safe
     */
    template <typename T>
    class ext_int
    {
      public:

        typedef T base_type;
        typedef typename std::make_unsigned<T>::type ubase_type;

        /**
         * Empty constructor
         */
        ext_int();

        /**
         * Move constructor
         */
        ext_int(const ext_int && p_value)noexcept;

        /**
         * Copy constructor
         */
        ext_int(const ext_int & p_value);

        /**
         * Initializer list constructor
         * @param p_root root value
         * @param p_init_list list of values to fill vector
         */
        ext_int(const T & p_root,
                const std::initializer_list<ubase_type> & p_init_list
               );

        /**
         * Constructor from uint32_t
         * @param p_value value to convert to ext_int
         */
        explicit ext_int(const uint32_t & p_value);

        /**
         * Constructor from int32_t
         * @param p_value value to convert to ext_int
         */
        explicit ext_int(const int32_t & p_value);

        /**
         * Constructor from uint64_t
         * @param p_value value to convert to ext_int
         */
        explicit ext_int(const uint64_t & p_value);

        /**
         * Constructor from int64_t
         * @param p_value value to convert to ext_int
         */
        explicit ext_int(const int64_t & p_value);

        /**
         * Constructor from ext_uint
         * @param p_value
         */
        explicit ext_int(const ext_uint<ubase_type > & p_value);

        /**
         * Display content of ext_int in hexadecimal
         * @param os output stream
         * @param p_ext_int ext_int to display
         */
        friend std::ostream &
        operator<< <>(std::ostream & os,
                      const ext_int<T> & p_ext_int
                     );

        /**
         * Move assignment operator
         * @param p_op value to move
         * @return assigned object
         */
        ext_int<T> & operator=(ext_int<T> && p_op);

        /**
         * Assignment operator
         * @param p_op value to move
         * @return assigned object
         */
        ext_int<T> & operator=(const ext_int<T> & p_op);

        bool
        operator==(const ext_int & p_op) const;

        bool
        operator!=(const ext_int & p_op) const;

        bool
        operator<(const ext_int & p_op) const;

        bool
        operator>(const ext_int & p_op) const;

        bool
        operator<=(const ext_int & p_op) const;

        bool
        operator>=(const ext_int & p_op) const;

        ext_int<T>
        operator+(const ext_int & p_op) const;

        ext_int<T>
        operator-(const ext_int & p_op) const;

        ext_int<T>
        operator*(const ext_int & p_op) const;

        ext_int<T>
        operator/(const ext_int & p_op) const;

        ext_int<T>
        operator%(const ext_int & p_op) const;

        ext_int<T> operator~(void) const;

        ext_int<T>
        operator-(void)const;

        ext_int<T>
        operator+(void)const;

        /**
         * string cast operator
         * @return string representation
         */
        operator std::string() const;

        /**
         * Root accessor
         * @return root
         */
        T get_root() const;

        /**
         * Extension accesssor
         * @return extension
         */
        const std::vector<ubase_type> & get_extension() const;

      private:
        /**
        * Perform addition and manage input output overflow
        * @param p_op1 first operand
        * @param p_op2 second operand
        * @param p_overflow input/output overflow
        * @return sum of both operands
        */
        static
        ubase_type propagate_add(const ubase_type & p_op1,
                                 const ubase_type & p_op2,
                                 bool & p_overflow
                                );

        /**
        * Perform addition and manage input output overflow
        * @param p_op1 first operand
        * @param p_op2 second operand
        * @param p_overflow input/output overflow
        * @return sum of both operands
        */
        static
        T propagate_add(const T & p_op1,
                        const T & p_op2,
                        bool & p_overflow
                       );
        /**
         * Method checking ig object has the shortest possible representation
         * @return true if object don't have shortest possible representation
         */
        bool is_trimmable() const;

        /**
         * Method ensuring that object has the shortest possible representation
         */
        ext_int<T> & trim();

        static_assert(std::is_signed<T>::value,"Ckeck base type is signed");

        /**
         * Methods extracting necessary info to build ext_int from an integer
         * value ( int64_t etc )
         * @tparam INT_TYPE type of integer value to extract
         * @param p_value value to extract
         * @param p_vector vector to fill
         * @return root value
         */
        template <typename INT_TYPE>
        static
        T extract(const INT_TYPE & p_value,
                 std::vector<ubase_type> & p_vector
                 );
        /**
         *
         * @param p_root  root value
         * @param p_ext values
         * @param p_check_trim indicate if we check the trim requirement
         */
        ext_int(const T & p_root,
                const std::vector<ubase_type> & p_ext,
                bool p_check_trim=true
               );

        /**
         * Root
         */
        T m_root;

        /**
         * Extensions
         */
         std::vector<ubase_type> m_ext;

        constexpr static const ubase_type m_upper_bit_mask = ((ubase_type)1) << (sizeof(ubase_type) * 8 - 1);

    };

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int():
            m_root(0)
    {
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const ext_int && p_value) noexcept:
    m_root(p_value.m_root),
    m_ext(std::move(p_value.m_ext))
    {

    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const ext_int & p_value):
    m_root(p_value.m_root),
    m_ext(p_value.m_ext)
    {

    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const T & p_root,
                        const std::initializer_list<ubase_type> & p_init_list
                       ):
            m_root(p_root),
            m_ext(p_init_list)
    {
        if(is_trimmable())
        {
            throw quicky_exception::quicky_logic_exception("NULL root whereas non full unsigned extension", __LINE__, __FILE__);
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const T & p_root,
                        const std::vector<ubase_type> & p_ext,
                        bool p_check_trim
                       ):
            m_root(p_root),
            m_ext(p_ext)
    {
        if(p_check_trim && is_trimmable())
        {
            throw quicky_exception::quicky_logic_exception("NULL root whereas non full unsigned extension", __LINE__, __FILE__);
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const int32_t & p_value)
    {
        m_root = extract<int32_t>(p_value, m_ext);
        if(is_trimmable())
        {
            trim();
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const uint32_t & p_value)
    {
        m_root = extract<uint32_t>(p_value, m_ext);
        if(is_trimmable())
        {
            trim();
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const int64_t & p_value)
    {
        m_root = extract<int64_t>(p_value, m_ext);
        if(is_trimmable())
        {
            trim();
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const uint64_t & p_value)
    {
        m_root = extract<uint64_t>(p_value, m_ext);
        if(is_trimmable())
        {
            trim();
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::ext_int(const ext_uint<ext_int::ubase_type> & p_value):
    m_root(0),
    m_ext(p_value.get_extension())
    {
        auto l_root_candidate = m_ext.back();
        if(!(l_root_candidate & m_upper_bit_mask))
        {
            m_ext.pop_back();
            m_root = l_root_candidate;
        }
    }

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::is_trimmable() const
    {
        return m_ext.size() && ((-1 == m_root && (m_ext.back() & m_upper_bit_mask)) || (0 == m_root && !(m_ext.back() & m_upper_bit_mask)));
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T> &
    ext_int<T>::trim()
    {
        while (is_trimmable())
        {
            m_root = m_ext.back();
            m_ext.pop_back();
        }
        return *this;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    std::ostream &
    operator<<(std::ostream & p_stream,
               const ext_int<T> & p_ext_int
              )
    {
        if(p_ext_int.m_root >= 0)
        {
            p_stream << "0x";
            char l_fill = p_stream.fill();
            std::streamsize l_width = p_stream.width();
            std::ios::fmtflags l_flags = p_stream.flags();

            p_stream << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << std::uppercase;
            if(p_ext_int.m_root || !p_ext_int.m_ext.size())
            {
                p_stream << (uint64_t) p_ext_int.m_root;
            }
            for (auto l_iter = p_ext_int.m_ext.crbegin();
                 l_iter != p_ext_int.m_ext.crend();
                 ++l_iter
                    )
            {
                p_stream << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << std::uppercase << (uint64_t) (*l_iter);
            }

            p_stream << std::setfill(l_fill) << std::setw(l_width) << std::setiosflags(l_flags);
        }
        else
        {
            p_stream << "-" << -p_ext_int;
        }
        return p_stream;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T> &
    ext_int<T>::operator=(ext_int<T> && p_op)
    {
        m_root = std::move(p_op.m_root);
        m_ext = std::move(p_op.m_ext);
        return *this;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T> &
    ext_int<T>::operator=(const ext_int<T> & p_op)
    {
        m_root = p_op.m_root;
        m_ext = p_op.m_ext;
        return *this;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::operator==(const ext_int & p_op) const
    {
        if(m_root != p_op.m_root)
        {
            return false;
        }
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

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::operator!=(const ext_int & p_op) const
    {
        return !(* this == p_op);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::operator<(const ext_int & p_op) const
    {
        // Basic integer comparison
        if (!m_ext.size() && !p_op.m_ext.size())
        {
            return m_root < p_op.m_root;
        }

        // Check signs
        bool l_root_pos = m_root >= 0;
        bool l_op_root_pos = p_op.m_root >= 0;
        if(l_root_pos != l_op_root_pos)
        {
            return m_root < p_op.m_root;
        }

        if (m_ext.size() != p_op.m_ext.size())
        {
            // For positive numbers absolute value comparison is good but this
            // is the opposite for negative numbers
            if(m_root >= 0)
            {
                return m_ext.size() < p_op.m_ext.size();
            }
            else
            {
                return m_ext.size() > p_op.m_ext.size();
            }
        }
        if(m_root != p_op.m_root)
        {
            return m_root < p_op.m_root;
        }
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

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::operator>(const ext_int & p_op) const
    {
        // Basic integer comparison
        if (!m_ext.size() && !p_op.m_ext.size())
        {
            return m_root > p_op.m_root;
        }

        // Check signs
        bool l_root_pos = m_root >= 0;
        bool l_op_root_pos = p_op.m_root >= 0;
        if(l_root_pos != l_op_root_pos)
        {
            return m_root > p_op.m_root;
        }

        if (m_ext.size() != p_op.m_ext.size())
        {
            // For positive numbers absolute value comparison is good but this
            // is the opposite for negative numbers
            if(m_root >= 0)
            {
                return m_ext.size() > p_op.m_ext.size();
            }
            else
            {
                return m_ext.size() < p_op.m_ext.size();
            }
        }
        if(m_root != p_op.m_root)
        {
            return m_root > p_op.m_root;
        }
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

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::operator<=(const ext_int & p_op) const
    {
        return ! (*this > p_op);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    bool
    ext_int<T>::operator>=(const ext_int & p_op) const
    {
        return ! (*this < p_op);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int <T>
    ext_int<T>::operator+(const ext_int & p_op) const
    {
        size_t l_max_size = m_ext.size() > p_op.m_ext.size() ? m_ext.size() : p_op.m_ext.size();
        size_t l_min_size = m_ext.size() < p_op.m_ext.size() ? m_ext.size() : p_op.m_ext.size();
        bool l_previous_overflow = false;
        std::vector<ubase_type> l_new_ext;
        T l_new_root;

        ubase_type l_sum;
        // Sum common extension part
        for(size_t l_index = 0;
            l_index < l_min_size;
            ++l_index
                )
        {
            l_sum = propagate_add(m_ext[l_index],p_op.m_ext[l_index], l_previous_overflow);
            l_new_ext.push_back(l_sum);
        }
        if(m_ext.size() != p_op.m_ext.size())
        {
            const ext_int<T> & l_shorter = m_ext.size() <= p_op.m_ext.size() ? *this : p_op;
            const ext_int<T> & l_longer = m_ext.size() > p_op.m_ext.size() ? *this : p_op;

            // Add shorter's root to longer.m_ext[l_min_size]
            ubase_type l_unsigned_root = l_shorter.m_root;
            l_sum = propagate_add(l_longer.m_ext[l_min_size],l_unsigned_root,l_previous_overflow);
            l_new_ext.push_back(l_sum);

            if(l_shorter.m_root >= 0)
            {
                for(size_t l_index = l_min_size + 1;
                    l_index < l_max_size;
                    ++l_index
                        )
                {
                    l_sum = propagate_add(l_longer.m_ext[l_index],0, l_previous_overflow);
                    l_new_ext.push_back(l_sum);
                }
            }
            else // Negative root
            {
                // Value used to complete number if case this and p_op don't have same size
                constexpr ubase_type l_fill_value = -1;
                for(size_t l_index = l_min_size + 1;
                    l_index < l_max_size;
                    ++l_index
                        )
                {
                    l_sum = propagate_add(l_longer.m_ext[l_index], l_fill_value,l_previous_overflow);
                    l_new_ext.push_back(l_sum);
                }
            }
            // Perform root operation
            const T l_fill_value = l_shorter.m_root >= 0 ? 0 : -1;
            l_sum = propagate_add(l_longer.m_root, l_fill_value, l_previous_overflow);
            if(l_previous_overflow)
            {
                l_new_ext.push_back(l_sum);
                l_new_root = l_fill_value;
            }
            else
            {
                l_new_root = l_sum;
            }
        }
        else // Extension has same size
        {
            l_sum = propagate_add(m_root, p_op.m_root,l_previous_overflow);
            if(l_previous_overflow)
            {
                l_new_ext.push_back(l_sum);
                l_new_root = m_root < 0 ? -1:0;
            }
            else
            {
                l_new_root = l_sum;
            }
        }
        return ext_int(l_new_root, l_new_ext, false).trim();
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>
    ext_int<T>::operator~(void) const
    {
        T l_root = ~m_root;
        std::vector<ubase_type> l_new_ext;
        for(auto l_iter:m_ext)
        {
           l_new_ext.push_back(~l_iter);
        }
        return ext_int<T>(l_root, l_new_ext, false);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>
    ext_int<T>::operator-(void) const
    {
        return (~*this + ext_int<T>(1,{}));
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>
    ext_int<T>::operator+(void) const
    {
        return *this;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    typename ext_int<T>::ubase_type
    ext_int<T>::propagate_add(const ext_int::ubase_type & p_op1,
                              const ext_int::ubase_type & p_op2,
                              bool & p_overflow
                             )
    {
        bool l_overflow;
        ubase_type l_sum = safe_uint<ubase_type >::check_add(p_op1, p_op2,l_overflow);
        if(p_overflow)
        {
            if (!l_overflow)
            {
                l_sum = safe_uint<ubase_type>::check_add(l_sum,
                                                         p_overflow,
                                                         l_overflow
                                                        );
            }
            else
            {
                l_sum += p_overflow;
            }
        }
        p_overflow = l_overflow;
        return l_sum;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    T
    ext_int<T>::propagate_add(const T & p_op1,
                              const T & p_op2,
                              bool & p_overflow
                             )
    {
        bool l_overflow;
        T l_sum = safe_int<T>::check_add(p_op1, p_op2,l_overflow);
        if(p_overflow)
        {
            if (!l_overflow)
            {
                l_sum = safe_int<T>::check_add(l_sum,
                                               p_overflow,
                                               l_overflow
                                              );
            }
            else
            {
                l_sum += p_overflow;
            }
        }
        p_overflow = l_overflow;
        return l_sum;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int <T>
    ext_int<T>::operator-(const ext_int & p_op) const
    {
        // Simple case where there are only 2 roots
        std::vector<ubase_type> l_new_ext;
        if(!m_ext.size() && !p_op.m_ext.size())
        {
            bool l_overflow = false;
            T l_sum = safe_int<T>::check_sub(m_root, p_op.m_root, l_overflow);
            if(!l_overflow)
            {
                return ext_int<T>(l_sum);
            }
            else
            {
                l_new_ext.push_back((ubase_type )l_sum);
                return ext_int<T>(m_root >= 0 ? 0 : -1, l_new_ext, false);
            }
        }

        T l_new_root;
        size_t l_max_size = m_ext.size() > p_op.m_ext.size() ? m_ext.size() : p_op.m_ext.size();
        size_t l_min_size = m_ext.size() < p_op.m_ext.size() ? m_ext.size() : p_op.m_ext.size();
        bool l_previous_overflow = true;

        bool l_this_shorter = m_ext.size() < p_op.m_ext.size();
        const ext_int<T> & l_shorter = m_ext.size() < p_op.m_ext.size() ? *this : p_op;
        const ext_int<T> & l_longer = m_ext.size() >= p_op.m_ext.size() ? *this : p_op;

        // Fill result with extended shorter operand
        // Manage short extension
        T l_fill_value = l_shorter.m_root >= 0 ? 0 : -1;
        ubase_type l_unsigned_root = l_shorter.m_root;
        if(l_this_shorter)
        {
            for(size_t l_index = 0; l_index < l_min_size; ++l_index)
            {
                l_new_ext.push_back(l_shorter.m_ext[l_index]);
            }
        }
        else
        {
            for(size_t l_index = 0; l_index < l_min_size; ++l_index)
            {
                l_new_ext.push_back(~l_shorter.m_ext[l_index]);
            }
            l_unsigned_root = ~l_unsigned_root;
            l_fill_value = ~l_fill_value;
        }
        if(m_ext.size() != p_op.m_ext.size())
        {
            // Manage root
            l_new_ext.push_back(l_unsigned_root);

            // Manage Fill values to reach max size
            for (size_t l_index = l_min_size + 1;
                 l_index < l_max_size;
                 ++l_index
                    )
            {
                l_new_ext.push_back(l_fill_value);
            }
            l_new_root = l_fill_value;
        }
        else
        {
            l_new_root = l_unsigned_root;
        }


        if(l_this_shorter)
        {
            for (size_t l_index = 0;
                 l_index < l_max_size;
                 ++l_index
                    )
            {
                l_new_ext[l_index] = propagate_add(~l_longer.m_ext[l_index],
                                                   l_new_ext[l_index],
                                                   l_previous_overflow
                                                  );
            }
            l_new_root = propagate_add(l_new_root, ~l_longer.m_root,l_previous_overflow);
        }
        else
        {
            for (size_t l_index = 0;
                 l_index < l_max_size;
                 ++l_index
                    )
            {
                l_new_ext[l_index] = propagate_add(l_longer.m_ext[l_index],
                                                   l_new_ext[l_index],
                                                   l_previous_overflow
                                                  );
            }
            l_new_root = propagate_add(l_new_root, l_longer.m_root,l_previous_overflow);
        }
        if(l_previous_overflow)
        {
            l_new_ext.push_back(l_new_root);
            l_new_root = m_root < 0 ? -1:0;
        }
        return ext_int(l_new_root, l_new_ext, false).trim();
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>::operator std::string() const
    {
        std::stringstream l_stream;
        l_stream << *this;
        return l_stream.str();
    }

    //-------------------------------------------------------------------------
    template <typename T>
    template <typename INT_TYPE>
    T
    ext_int<T>::extract(const INT_TYPE & p_value,
                        std::vector<ext_int::ubase_type> & p_vector
                       )
    {
        T l_root = 0;
        if(sizeof(INT_TYPE) >= sizeof(T))
        {
            typename std::make_unsigned<INT_TYPE>::type l_value = p_value;
            while(l_value)
            {
                ubase_type l_chunk = l_value & ((ubase_type) -1);
                l_value = l_value >> (8 * sizeof(ubase_type));
                if(l_value)
                {
                    p_vector.push_back(l_chunk);
                }
                else if(p_value > 0 && ((T)l_chunk) < 0)
                {
                    p_vector.push_back(l_chunk);
                }
                else
                {
                    l_root = l_chunk;
                }
            }
        }
        else
        {
            l_root = p_value;
        }
        return l_root;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    T
    ext_int<T>::get_root() const
    {
        return m_root;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    const std::vector<typename ext_int<T>::ubase_type> &
    ext_int<T>::get_extension() const
    {
        return m_ext;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>
    ext_int<T>::operator*(const ext_int & p_op) const
    {
        if(*this == ext_int<T>(0,{}) || p_op == ext_int<T>(0,{}))
        {
            return ext_int<T>(0,{});
        }
        unsigned int l_case = 2 * (this->m_root >= 0) + (p_op.m_root >= 0);
        switch(l_case)
        {
            case 0:
            {
                auto l_this_abs = ext_uint<ubase_type>(-*this);
                auto l_op_abs = ext_uint<ubase_type>(-p_op);
                return ext_int<T>(l_this_abs * l_op_abs);
            }
                break;
            case 1:
            {
                auto l_this_abs = ext_uint<ubase_type>(-*this);
                auto l_op_abs = ext_uint<ubase_type>(p_op);
                return -ext_int<T>(l_this_abs * l_op_abs);
            }
                break;
            case 2:
            {
                auto l_this_abs = ext_uint<ubase_type>(*this);
                auto l_op_abs = ext_uint<ubase_type>(-p_op);
                return -ext_int<T>(l_this_abs * l_op_abs);
            }
                break;
            case 3:
            {
                auto l_this_abs = ext_uint<ubase_type>(*this);
                auto l_op_abs = ext_uint<ubase_type>(p_op);
                return ext_int<T>(l_this_abs * l_op_abs);
            }
                break;
            default:
                // Should never occur
                assert(false);
        }
        return ext_int<T>(0,{});
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>
    ext_int<T>::operator/(const ext_int & p_op) const
    {
        if(!m_ext.size()  && !m_root)
        {
            return ext_int();
        }
        if(!p_op.m_ext.size())
        {
            if (1 == p_op.m_root)
            {
                return *this;
            }
            else if(-1 == p_op.m_root)
            {
                return -*this;
            }
            else if (!p_op.m_root)
            {
                throw quicky_exception::quicky_logic_exception("Illegal division by 0 ext_int",
                                                               __LINE__,
                                                               __FILE__
                                                              );
            }
            if(!m_ext.size())
            {
                return ext_int<T>({(T)(m_root / p_op.m_root)});
            }
        }
        unsigned int l_case = 2 * (this->m_root >= 0) + (p_op.m_root >= 0);
        switch(l_case)
        {
            case 0:
            {
                auto l_this_abs = ext_uint<ubase_type>(-*this);
                auto l_op_abs = ext_uint<ubase_type>(-p_op);
                if(l_this_abs < l_op_abs)
                {
                    return ext_int();
                }
                return ext_int<T>(l_this_abs / l_op_abs);
            }
                break;
            case 1:
            {
                auto l_this_abs = ext_uint<ubase_type>(-*this);
                auto l_op_abs = ext_uint<ubase_type>(p_op);
                if(l_this_abs < l_op_abs)
                {
                    return ext_int();
                }
                return -ext_int<T>(l_this_abs / l_op_abs);
            }
                break;
            case 2:
            {
                auto l_this_abs = ext_uint<ubase_type>(*this);
                auto l_op_abs = ext_uint<ubase_type>(-p_op);
                if(l_this_abs < l_op_abs)
                {
                    return ext_int();
                }
                return -ext_int<T>(l_this_abs / l_op_abs);
            }
                break;
            case 3:
            {
                auto l_this_abs = ext_uint<ubase_type>(*this);
                auto l_op_abs = ext_uint<ubase_type>(p_op);
                if(l_this_abs < l_op_abs)
                {
                    return ext_int();
                }
                return ext_int<T>(l_this_abs / l_op_abs);
            }
                break;
            default:
                // Should never occur
                assert(false);
        }
        return ext_int<T>(0,{});
    }

    //-------------------------------------------------------------------------
    template <typename T>
    ext_int<T>
    ext_int<T>::operator%(const ext_int & p_op) const
    {
        if(!m_ext.size() && m_root == 0)
        {
            return ext_int();
        }
        if(!p_op.m_ext.size())
        {
            if (1 == p_op.m_root || -1 == p_op.m_root)
            {
                return ext_int();
            }
            else if (!p_op.m_root)
            {
                throw quicky_exception::quicky_logic_exception("Illegal division by 0 ext_int",
                                                               __LINE__,
                                                               __FILE__
                                                              );
            }
            if(!m_ext.size())
            {
                return ext_int<T>(m_root % p_op.m_root,{});
            }
        }
        ext_uint<typename std::make_unsigned<T>::type> l_mult;
        unsigned int l_case = 2 * (this->m_root >= 0) + (p_op.m_root >= 0);
        switch(l_case)
        {
            case 0:
            {
                auto l_this_abs = ext_uint<ubase_type>(-*this);
                auto l_op_abs = ext_uint<ubase_type>(-p_op);
                if(l_this_abs < l_op_abs)
                {
                    return *this;
                }
                l_this_abs.div(l_op_abs, true, l_mult);
                return -ext_int<T>(l_this_abs - l_mult);
            }
                break;
            case 1:
            {
                auto l_this_abs = ext_uint<ubase_type>(-*this);
                auto l_op_abs = ext_uint<ubase_type>(p_op);
                if(l_this_abs < l_op_abs)
                {
                    return *this;
                }
                l_this_abs.div(l_op_abs, true, l_mult);
                return -ext_int<T>(l_this_abs - l_mult);
            }
                break;
            case 2:
            {
                auto l_this_abs = ext_uint<ubase_type>(*this);
                auto l_op_abs = ext_uint<ubase_type>(-p_op);
                if(l_this_abs < l_op_abs)
                {
                    return *this;
                }
                l_this_abs.div(l_op_abs, true, l_mult);
                return ext_int<T>(l_this_abs - l_mult);
            }
                break;
            case 3:
            {
                auto l_this_abs = ext_uint<ubase_type>(*this);
                auto l_op_abs = ext_uint<ubase_type>(p_op);
                if(l_this_abs < l_op_abs)
                {
                    return *this;
                }
                l_this_abs.div(l_op_abs, true, l_mult);
                return ext_int<T>(l_this_abs - l_mult);
            }
                break;
            default:
                // Should never occur
                assert(false);
        }
        return ext_int<T>(0,{});
    }

#ifdef QUICKY_UTILS_SELF_TEST
    /**
    * Method regrouping tests of ext_int class
    */
    bool
    test_ext_int();
#endif // QUICKY_UTILS_SELF_TEST

}

namespace std
{
    template <typename T>
    quicky_utils::ext_int<T>
    abs(const quicky_utils::ext_int<T> & p_ext_int)
    {
        if (p_ext_int.get_root() < 0)
        {
            return -p_ext_int;
        }
        return p_ext_int;
    }

}
#endif //QUICKY_UTILS_EXT_INT_H
// EOF
