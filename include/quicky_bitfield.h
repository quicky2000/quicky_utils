/*    This file is part of edge matching puzzlequicky_utils
      The aim of this software is to find some utilities
      Copyright (C) 2014  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#ifndef QUICKY_BITFIELD
#define QUICKY_BITFIELD

#include "quicky_exception.h"
#include <cstring>
#include <cassert>
#include <limits>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <type_traits>
#include <cinttypes>
#include "common.h"

#ifdef __MINGW32__ // seems to be defined by both mingw-32 nd mingw-64
#include <_mingw.h> // private MinGW header
# ifndef __MINGW64_VERSION_MAJOR // best way to check for mingw-w64 vs mingw-32
#define ffs(x) __builtin_ffs(x)
# endif // __MINGW64_VERSION_MAJOR
#endif // __MINGW32__

namespace quicky_utils
{
    template <class T>
    class quicky_bitfield;

    template <class T>
    std::ostream & operator<<(std::ostream & p_stream,const quicky_bitfield<T> & p_bitfield);

    template <class T>
    class quicky_bitfield
    {
        friend std::ostream & operator<< <>(std::ostream & p_stream,const quicky_bitfield<T> & p_bitfield);

      public:
        /**
         Constructor of bitfield
         @param p_size : bitfield size in bits
        */
        inline
        quicky_bitfield();

        inline explicit
        quicky_bitfield(const unsigned int & p_size
                       ,bool p_reset_value = false
                       );

        inline
        quicky_bitfield(const quicky_bitfield & p_bitfield);

        inline
        ~quicky_bitfield();

        inline
        void set(const unsigned int & p_data
                ,const unsigned int & p_size
                ,const unsigned int & p_offset
                );

        inline
        void get(unsigned int & p_data
                ,const unsigned int & p_size
                ,const unsigned int & p_offset
                ) const;

        inline
        void reset(bool p_reset_value = false);

        inline
        void dump_in(std::ostream & p_stream) const;

        inline
        void read_from(std::istream & p_stream);

        [[nodiscard]]
        inline
        size_t size() const;

        [[nodiscard]]
        inline
        size_t bitsize() const;

        inline
        void apply_and(const quicky_bitfield & p_operand1
                      ,const quicky_bitfield & p_operand2
                      );

        inline
        void apply_and(const quicky_bitfield & p_operand1
                      ,const quicky_bitfield & p_operand2
                      ,unsigned int p_limit_bit
                      );

        /**
         * Apply bitwise AND on 2 bitfields and store result in this in a multi
         * thread way and only on part defined by first containing limit bit
         * index until the end of bitfield
         * @param p_operand1 first operand
         * @param p_operand2 second operand
         * @param p_limit_bit first non null bit
         * @param p_thread_id thread id
         * @param p_thread_nb thread number
         */
        [[maybe_unused]]
        inline
        void apply_and(const quicky_bitfield & p_operand1
                      ,const quicky_bitfield & p_operand2
                      ,unsigned int p_limit_bit
                      ,unsigned int p_thread_id
                      ,unsigned int p_thread_nb
                      );

        inline
        void apply_or(const quicky_bitfield & p_operand1
                     ,const quicky_bitfield & p_operand2
                     );

        /**
         * Return index of first bit set
         * @return 0 if no bit set, index of first bit set ( first bit has index 1 )
         */
        [[nodiscard]]
        inline
        int ffs() const;

        /**
         * Return index of first bit set from p_start_index
         * @param p_start_index bit index from which search start
         * @return 0 if no bit set, index of first bit set ( first bit has index 1 )
         */
        [[nodiscard]]
        inline
        int ffs(unsigned int p_start_index) const;

        /**
         * Method checking if bitwise AND between two bitfields will result
         * in a bitfield with some non null bits
         * Method exit at first non null word
         * @param p_operand1 operand with which bitwise AND is performed
         * @return true if some results bits are 1 or false if no result bits are at zero
         */
        [[nodiscard]]
        inline
        bool and_not_null(const quicky_bitfield & p_operand1) const;

        /**
         * Method checking if bitwise AND between two bitfields will result
         * in a bitfield with some non null bits
         * Method exit at first non null word
         * Start by the end
         * @param p_operand1 operand with which bitwise AND is performed
         * @return true if some results bits are 1 or false if no result bits are at zero
         */
        [[nodiscard]]
        inline
        bool r_and_not_null(const quicky_bitfield & p_operand1) const;

        /**
         * Method checking if bitwise AND between two bitfields will result
         * in a bitfield with some non null bits
         * Method exit at first non null word
         * Start by the end and stop when reaching limit index
         * @param p_operand1 operand with which bitwise AND is performed
         * @param p_limit_bit firt non null bit
         * @return true if some results bits are 1 or false if no result bits are at zero
         */
        [[nodiscard]]
        inline
        bool r_and_not_null(const quicky_bitfield & p_operand1
                           ,unsigned int p_limit_bit
                           ) const;

        inline
        quicky_utils::quicky_bitfield<T> & operator=(const quicky_bitfield<T> & p_bitfield);

        inline
        bool operator==(const quicky_bitfield<T> & p_operand) const;
      private:

        [[nodiscard]]
        inline
        unsigned int compute_limit_index(unsigned int p_limit_bit) const;

        static_assert(std::is_unsigned<T>::value, "Check base type is unsigned");
        const unsigned int m_size;
        typedef T t_array_unit;
        const unsigned int m_array_size;
        t_array_unit * m_array;
    };

    //----------------------------------------------------------------------------
    template <class T>
    inline std::ostream & operator<<(std::ostream & p_stream,const quicky_bitfield<T> & p_bitfield)
    {
        p_stream << "0x";
        for(unsigned int l_index = 0 ; l_index < p_bitfield.m_array_size ; ++l_index)
        {
            p_stream << std::hex << std::setfill('0') << std::setw(2*sizeof(typename quicky_bitfield<T>::t_array_unit)) << p_bitfield.m_array[l_index];
        }
        return p_stream;
    }

    //----------------------------------------------------------------------------
    template <class T>
    quicky_utils::quicky_bitfield<T> & quicky_bitfield<T>::operator=(const quicky_bitfield<T> & p_bitfield)
    {
        assert(m_size == p_bitfield.m_size);
#ifdef USE_MEMCPY
        memcpy(m_array, p_bitfield.m_array,sizeof(t_array_unit) * m_array_size);
#else
        for(unsigned int l_index = 0 ; l_index < m_array_size ; ++l_index)
        {
	        m_array[l_index] = p_bitfield.m_array[l_index];
        }
#endif
        return *this;
    }

    //----------------------------------------------------------------------------
    template <class T>
    int quicky_bitfield<T>::ffs() const
    {
        unsigned int l_index = 0;
        int l_result;
        while(l_index < m_array_size)
        {
            t_array_unit v = m_array[l_index];
            l_result = ::ffs(v);
            if(l_result)
            {
                return l_result + 8 * sizeof(t_array_unit) * l_index;
            }
            ++l_index;
        }
        return 0;
    }

    //----------------------------------------------------------------------------
    template <>
    inline int quicky_bitfield<uint32_t>::ffs() const
    {
        unsigned int l_index = 0;
        int l_result;
        while(l_index < m_array_size)
        {
            t_array_unit v = m_array[l_index];
#ifndef USE_HARDWARE_FFS
            static const unsigned char MultiplyDeBruijnBitPosition[32] =
                    {
                            1,  // 0,
                            2,  // 1,
                            29, //28,
                            3,  // 2,
                            30, //29,
                            15, //14,
                            25, //24,
                            4,  // 3,
                            31, //30,
                            23, //22,
                            21, //20,
                            16, //15,
                            26, //25,
                            18, //17,
                            5,  // 4,
                            9,  // 8,
                            32, //31,
                            28, //27,
                            14, //13,
                            24, //23,
                            22, //21,
                            20, //19,
                            17, //16,
                            8,  // 7,
                            27, //26,
                            13, //12,
                            19, //18,
                            7,  // 6,
                            12, //11,
                            6,  // 5,
                            11, //10,
                            10, // 9
                    };
            l_result = v ? MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27u] : 0;
#else
            l_result = ::ffs(v);
#endif // USE_HARDWARE_FFS

            if(l_result)
            {
                return l_result + static_cast<int>(8 * sizeof(t_array_unit) * l_index);
            }
            ++l_index;
        }
        return 0;
    }

    //----------------------------------------------------------------------------
    template <>
    inline int quicky_bitfield<uint64_t>::ffs() const
    {
        unsigned int l_index = 0;
        int l_result;
        while(l_index < m_array_size)
        {
            t_array_unit v = m_array[l_index];
#ifndef USE_HARDWARE_FFS
            static const unsigned char MultiplyDeBruijnBitPosition[64] =
                    {
                            1,
                            2,
                            3,
                            8,
                            4,
                            14,
                            9,
                            20,
                            5,
                            26,
                            15,
                            29,
                            10,
                            35,
                            21,
                            41,
                            6,
                            18,
                            27,
                            39,
                            16,
                            47,
                            30,
                            49,
                            11,
                            32,
                            36,
                            55,
                            22,
                            51,
                            42,
                            58,
                            64,
                            7,
                            13,
                            19,
                            25,
                            28,
                            34,
                            40,
                            17,
                            38,
                            46,
                            48,
                            31,
                            54,
                            50,
                            57,
                            63,
                            12,
                            24,
                            33,
                            37,
                            45,
                            53,
                            56,
                            62,
                            23,
                            44,
                            52,
                            61,
                            43,
                            60,
                            59
                    };
            l_result = v ? MultiplyDeBruijnBitPosition[((uint64_t)((v & -v) * 0x0218a392cd3d5dbfUL)) >> 58u] : 0;
#else
            l_result = ::ffs(v);
#endif // USE_HARDWARE_FFS

            if(l_result)
            {
                return l_result + static_cast<int>(8 * sizeof(t_array_unit) * l_index);
            }
            ++l_index;
        }
        return 0;
    }

    //----------------------------------------------------------------------------
    template <class T>
    int quicky_bitfield<T>::ffs(unsigned int p_start_index) const
    {
        unsigned int l_index = compute_limit_index(p_start_index);
        int l_result;
        while(l_index < m_array_size)
        {
            t_array_unit v = m_array[l_index];
            l_result = ::ffs(v);
            if(l_result)
            {
                return l_result + 8 * sizeof(t_array_unit) * l_index;
            }
            ++l_index;
        }
        return 0;
    }

    //----------------------------------------------------------------------------
    template <>
    inline int quicky_bitfield<uint32_t>::ffs(unsigned int p_start_index) const
    {
        unsigned int l_index = compute_limit_index(p_start_index);
        int l_result;
        while(l_index < m_array_size)
        {
            t_array_unit v = m_array[l_index];
#ifndef USE_HARDWARE_FFS
            static const unsigned char MultiplyDeBruijnBitPosition[32] =
                    {
                            1,  // 0,
                            2,  // 1,
                            29, //28,
                            3,  // 2,
                            30, //29,
                            15, //14,
                            25, //24,
                            4,  // 3,
                            31, //30,
                            23, //22,
                            21, //20,
                            16, //15,
                            26, //25,
                            18, //17,
                            5,  // 4,
                            9,  // 8,
                            32, //31,
                            28, //27,
                            14, //13,
                            24, //23,
                            22, //21,
                            20, //19,
                            17, //16,
                            8,  // 7,
                            27, //26,
                            13, //12,
                            19, //18,
                            7,  // 6,
                            12, //11,
                            6,  // 5,
                            11, //10,
                            10, // 9
                    };
            l_result = v ? MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27u] : 0;
#else
            l_result = ::ffs(v);
#endif // USE_HARDWARE_FFS

            if(l_result)
            {
                return l_result + static_cast<int>(8 * sizeof(t_array_unit) * l_index);
            }
            ++l_index;
        }
        return 0;
    }

    //----------------------------------------------------------------------------
    template <>
    inline int quicky_bitfield<uint64_t>::ffs(unsigned int p_start_index) const
    {
        unsigned int l_index = compute_limit_index(p_start_index);
        int l_result;
        while(l_index < m_array_size)
        {
            t_array_unit v = m_array[l_index];
#ifndef USE_HARDWARE_FFS
            static const unsigned char MultiplyDeBruijnBitPosition[64] =
                    {
                            1,
                            2,
                            3,
                            8,
                            4,
                            14,
                            9,
                            20,
                            5,
                            26,
                            15,
                            29,
                            10,
                            35,
                            21,
                            41,
                            6,
                            18,
                            27,
                            39,
                            16,
                            47,
                            30,
                            49,
                            11,
                            32,
                            36,
                            55,
                            22,
                            51,
                            42,
                            58,
                            64,
                            7,
                            13,
                            19,
                            25,
                            28,
                            34,
                            40,
                            17,
                            38,
                            46,
                            48,
                            31,
                            54,
                            50,
                            57,
                            63,
                            12,
                            24,
                            33,
                            37,
                            45,
                            53,
                            56,
                            62,
                            23,
                            44,
                            52,
                            61,
                            43,
                            60,
                            59
                    };
            l_result = v ? MultiplyDeBruijnBitPosition[((uint64_t)((v & -v) * 0x0218a392cd3d5dbfUL)) >> 58u] : 0;
#else
            l_result = ::ffs(v);
#endif // USE_HARDWARE_FFS

            if(l_result)
            {
                return l_result + static_cast<int>(8 * sizeof(t_array_unit) * l_index);
            }
            ++l_index;
        }
        return 0;
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::apply_and(const quicky_bitfield & p_operand1
                                      ,const quicky_bitfield & p_operand2
                                      )
    {
        assert(m_size == p_operand1.m_size);
        assert(m_size == p_operand2.m_size);
        for(unsigned int l_index = 0 ; l_index < m_array_size ; ++l_index)
        {
            m_array[l_index] = p_operand1.m_array[l_index] & p_operand2.m_array[l_index];
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::apply_and(const quicky_bitfield & p_operand1
                                      ,const quicky_bitfield & p_operand2
                                      ,unsigned int p_limit_bit
                                      )
    {
        assert(m_size == p_operand1.m_size);
        assert(m_size == p_operand2.m_size);
        auto l_limit_index = compute_limit_index(p_limit_bit);
        for(unsigned int l_index = l_limit_index ; l_index < m_array_size ; ++l_index)
        {
            m_array[l_index] = p_operand1.m_array[l_index] & p_operand2.m_array[l_index];
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    [[maybe_unused]]
    void
    quicky_bitfield<T>::apply_and(const quicky_bitfield & p_operand1,
                                  const quicky_bitfield & p_operand2,
                                  unsigned int p_limit_bit,
                                  unsigned int p_thread_id,
                                  unsigned int p_thread_nb
                                 )
    {
        assert(m_size == p_operand1.m_size);
        assert(m_size == p_operand2.m_size);
#ifndef SIMPLE
        // Ensure pthread nb is a power of 2
        assert(!((p_thread_nb - 1) & p_thread_nb));

        auto l_limit_index = compute_limit_index(p_limit_bit);

        unsigned int l_effective_size = (m_array_size - l_limit_index);
        unsigned int l_trunk_size = l_effective_size / p_thread_nb;
        // std::cout << "Computed trunk size : " << l_trunk_size << std::endl;
        bool l_truncated = (( l_effective_size - 1) & l_effective_size);
        if(l_truncated)
        {
            // std::cout << "TRUNCATED!" << std::endl;
            ++l_trunk_size;
        }
        unsigned int l_index = l_limit_index + p_thread_id * l_trunk_size;
        unsigned int l_end_index = p_thread_nb - 1 != p_thread_id ?  l_index + l_trunk_size: m_array_size;
        for(; l_index < l_end_index ; ++l_index)
#else // SIMPLE
        for(unsigned int l_index = l_limit_index + p_thread_id; l_index < m_array_size ; l_index += p_thread_nb)
#endif // SIMPLE
        {
            m_array[l_index] = p_operand1.m_array[l_index] & p_operand2.m_array[l_index];
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    size_t quicky_bitfield<T>::bitsize() const
    {
        return m_size;
    }

    //----------------------------------------------------------------------------
    template <class T>
    size_t quicky_bitfield<T>::size() const
    {
        return m_array_size * sizeof(t_array_unit);
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::dump_in(std::ostream & p_stream) const
    {
        p_stream.write((char*)m_array,m_array_size * sizeof(t_array_unit));
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::read_from(std::istream & p_stream)
    {
        p_stream.read((char*)m_array,m_array_size * sizeof(t_array_unit));
    }

    //----------------------------------------------------------------------------
    template <class T>
    quicky_bitfield<T>::quicky_bitfield()
    :m_size(0)
    ,m_array_size(0)
    ,m_array(nullptr)
    {
    }

    //----------------------------------------------------------------------------
    template <class T>
    quicky_bitfield<T>::quicky_bitfield(const unsigned int & p_size,bool p_reset_value)
    :m_size(p_size)
    ,m_array_size(p_size / (8 * sizeof(t_array_unit)) + (p_size % (8 * sizeof(t_array_unit)) ? 1 : 0))
    ,m_array(new t_array_unit[m_array_size])
    {
        reset(p_reset_value);
    }

    //----------------------------------------------------------------------------
    template <class T>
    quicky_bitfield<T>::quicky_bitfield(const quicky_bitfield & p_bitfield)
    :m_size(p_bitfield.m_size)
    ,m_array_size(p_bitfield.m_array_size)
    ,m_array(new t_array_unit[m_array_size])
    {
        memcpy(m_array,p_bitfield.m_array,sizeof(t_array_unit) * m_array_size);
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::reset(bool p_reset_value)
    {
        memset(m_array,p_reset_value ? 0xFF : 0,sizeof(t_array_unit) * m_array_size);
        if(p_reset_value)
        {
            unsigned int l_remaining_bits = m_size % (8 * sizeof(t_array_unit));
            if (l_remaining_bits)
            {
                t_array_unit l_mask = (((t_array_unit) 1) << l_remaining_bits) - 1;
                t_array_unit l_result = m_array[m_array_size - 1] & l_mask;
                m_array[m_array_size - 1] = l_result;
            }
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::set(const unsigned int & p_data
                                ,const unsigned int & p_size
                                ,const unsigned int & p_offset
                                )
    {
        assert(p_size + p_offset <= m_size);
        assert(p_size < 8 * sizeof(unsigned int));
        assert(p_offset + p_size - 1 < m_array_size * 8 * sizeof(t_array_unit));
        assert(p_size <= 8 * sizeof(t_array_unit));
        assert(p_data < ( (unsigned int)1 << p_size));

        t_array_unit l_data = (( ((t_array_unit)1) << p_size) - 1) & p_data;

        unsigned int l_min_index = p_offset / (8 * sizeof(t_array_unit));
        unsigned int l_max_index = ( p_offset + p_size - 1) / ( 8 * sizeof(t_array_unit));
        if(l_min_index == l_max_index)
        {
            t_array_unit l_mask = l_data << p_offset;
            m_array[l_min_index] &= ~(((( ((t_array_unit)1) << p_size) - 1)) << p_offset);
            m_array[l_min_index] |= l_mask;
        }
        else
        {
            unsigned int l_min_mod = p_offset % (8 * sizeof(t_array_unit));
            unsigned int l_size = 8 * sizeof(t_array_unit) - l_min_mod;
            m_array[l_min_index] &= ~((( ((t_array_unit)1) << l_size) - 1) << l_min_mod);
            t_array_unit l_mask = ((( ((t_array_unit)1) << l_size) - 1) & l_data) << l_min_mod;
            m_array[l_min_index] |= l_mask;

            l_mask = p_data >> l_size;
            m_array[l_max_index] &= ~((( ((t_array_unit)1) << (p_size - l_size)) -1));
            m_array[l_max_index] |= l_mask;
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    void quicky_bitfield<T>::get(unsigned int & p_data
                                ,const unsigned int & p_size
                                ,const unsigned int & p_offset
                                ) const
    {
        assert(p_size + p_offset <= m_size);
        assert(p_size < 8 * sizeof(unsigned int));
        assert(p_offset + p_size -1 < m_array_size * 8 * sizeof(t_array_unit));
        assert(p_size <= 8 * sizeof(t_array_unit));
        unsigned int l_min_index = p_offset / (8 * sizeof(t_array_unit));
        unsigned int l_max_index = ( p_offset + p_size - 1) / ( 8 * sizeof(t_array_unit));
        if(l_min_index == l_max_index)
        {
            t_array_unit l_data = m_array[l_min_index] >> p_offset;
            p_data = l_data & (( ((t_array_unit)1) << p_size) - 1);
        }
        else
        {
            unsigned int l_min_mod = p_offset % (8 * sizeof(t_array_unit));
            unsigned int l_size = 8 * sizeof(t_array_unit) - l_min_mod;
            p_data = (m_array[l_min_index] >> l_min_mod ) & ((((t_array_unit)1) << l_size) - 1);

            t_array_unit l_data = m_array[l_max_index] & ((((t_array_unit)1) << (p_size - l_size)) - 1);
            p_data |= (l_data << l_size);
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    quicky_bitfield<T>::~quicky_bitfield()
    {
        delete[] m_array;
    }

    //----------------------------------------------------------------------------
    template <class T>
    void
    quicky_bitfield<T>::apply_or(const quicky_bitfield & p_operand1
                                ,const quicky_bitfield & p_operand2
                                )
    {
        assert(m_size == p_operand1.m_size);
        assert(m_size == p_operand2.m_size);
        for(unsigned int l_index = 0 ; l_index < m_array_size ; ++l_index)
        {
            m_array[l_index] = p_operand1.m_array[l_index] | p_operand2.m_array[l_index];
        }
    }

    //----------------------------------------------------------------------------
    template <class T>
    bool
    quicky_bitfield<T>::and_not_null(const quicky_bitfield & p_operand1) const
    {
        assert(m_size == p_operand1.m_size);
        for(unsigned int l_index = 0 ; l_index < m_array_size ; ++l_index)
        {
            if(m_array[l_index] & p_operand1.m_array[l_index])
            {
                return true;
            }
        }
        return false;
    }

    //----------------------------------------------------------------------------
    template <class T>
    bool
    quicky_bitfield<T>::r_and_not_null(const quicky_bitfield & p_operand1) const
    {
        assert(m_size == p_operand1.m_size);
        for(unsigned int l_index = m_array_size - 1 ; l_index < m_array_size ; --l_index)
        {
            if(m_array[l_index] & p_operand1.m_array[l_index])
            {
                return true;
            }
        }
        return false;
    }

    //----------------------------------------------------------------------------
    template <class T>
    bool
    quicky_bitfield<T>::r_and_not_null(const quicky_bitfield & p_operand1
                                      ,unsigned int p_limit_bit
                                      ) const
    {
        assert(m_size == p_operand1.m_size);
        auto l_limit_index = (int) compute_limit_index(p_limit_bit);
        for(int l_index = (int)(m_array_size - 1) ; l_index >= l_limit_index ; --l_index)
        {
            if(m_array[l_index] & p_operand1.m_array[l_index])
            {
                return true;
            }
        }
        return false;
    }

    //----------------------------------------------------------------------------
    template <class T>
    unsigned int
    quicky_bitfield<T>::compute_limit_index(unsigned int p_limit_bit) const
    {
        assert(p_limit_bit < m_size);
        unsigned int l_limit_index = p_limit_bit / (8 * sizeof(t_array_unit));
        return l_limit_index;
    }

    //----------------------------------------------------------------------------
    template <class T>
    bool
    quicky_bitfield<T>::operator==(const quicky_bitfield<T> & p_operand) const
    {
        return m_size == p_operand.m_size && !memcmp(m_array, p_operand.m_array, m_array_size);
    }

#ifdef QUICKY_UTILS_SELF_TEST
    bool test_quicky_bitfield();
#endif // QUICKY_UTILS_SELF_TEST

}
#endif // QUICKY_BITFIELD
//EOF
