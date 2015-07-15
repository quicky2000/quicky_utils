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

namespace quicky_utils
{
  class quicky_bitfield
  {
    friend std::ostream & operator<<(std::ostream & p_stream,const quicky_bitfield & p_bitfield);
  public:
    /**
       Constructor of bitfield
       @param p_size : bitfield size in bits
     */
    inline quicky_bitfield(const unsigned int & p_size, bool p_reset_value=false);
    inline quicky_bitfield(const quicky_bitfield & p_bitfield);
    inline ~quicky_bitfield(void);
    inline void set(const unsigned int & p_data,
                    const unsigned int & p_size,
                    const unsigned int & p_offset);
    inline void get(unsigned int & p_data,
                    const unsigned int & p_size,
                    const unsigned int & p_offset)const;
    inline void reset(bool p_reset_value=false);
    inline void dump_in(std::ostream & p_stream)const;
    inline void read_from(std::istream & p_stream);
    inline const size_t size(void)const;
    inline const size_t bitsize(void)const;
    inline void apply_and(const quicky_bitfield & p_operand1,
			  const quicky_bitfield & p_operand2);
    inline int ffs(void)const;
    inline quicky_utils::quicky_bitfield & operator=(const quicky_bitfield & p_bitfield);
  private:
    const unsigned int m_size;
    typedef uint32_t t_array_unit;
    const unsigned int m_array_size;
    t_array_unit * m_array;
  };

  //----------------------------------------------------------------------------
  inline std::ostream & operator<<(std::ostream & p_stream,const quicky_bitfield & p_bitfield)
    {
      p_stream << "0x";
      for(unsigned int l_index = 0 ; l_index < p_bitfield.m_array_size ; ++l_index)
        {
          p_stream << std::hex << std::setfill('0') << std::setw(2*sizeof(quicky_bitfield::t_array_unit)) << p_bitfield.m_array[l_index] ;
        }
      return p_stream;
    }

  //----------------------------------------------------------------------------
  quicky_utils::quicky_bitfield & quicky_bitfield::operator=(const quicky_bitfield & p_bitfield)
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
  int quicky_bitfield::ffs(void)const
  {
    unsigned int l_index = 0;
    int l_result = 0;
    do
      {
	unsigned int v = m_array[l_index];
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
        l_result = v ? MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27] : 0;
#else
	l_result = ::ffs(v);
#endif // USE_HARDWARE_FFS

        if(l_result)
          {
            return l_result + 8 * sizeof(t_array_unit) * l_index;
          }
        ++l_index;
      }
    while(l_index < m_array_size);
    return 0;
 }

  //----------------------------------------------------------------------------
  void quicky_bitfield::apply_and(const quicky_bitfield & p_operand1,
				  const quicky_bitfield & p_operand2)
  {
    assert(m_size == p_operand1.m_size);
    assert(m_size == p_operand2.m_size);
    for(unsigned int l_index = 0 ; l_index < m_array_size ; ++l_index)
      {
	m_array[l_index] = p_operand1.m_array[l_index] & p_operand2.m_array[l_index];
      }
  }

  //----------------------------------------------------------------------------
  const size_t quicky_bitfield::bitsize(void)const
    {
      return m_size;
    }

  //----------------------------------------------------------------------------
  const size_t quicky_bitfield::size(void)const
    {
      return m_array_size * sizeof(t_array_unit);
    }

  //----------------------------------------------------------------------------
  void quicky_bitfield::dump_in(std::ostream & p_stream)const
  {
    p_stream.write((char*)m_array,m_array_size * sizeof(t_array_unit));
  }

  //----------------------------------------------------------------------------
  void quicky_bitfield::read_from(std::istream & p_stream)
  {
    p_stream.read((char*)m_array,m_array_size * sizeof(t_array_unit));
  }

  //----------------------------------------------------------------------------
  quicky_bitfield::quicky_bitfield(const unsigned int & p_size,bool p_reset_value):
    m_size(p_size),
    m_array_size(p_size / (8 * sizeof(t_array_unit)) + (p_size % (8 * sizeof(t_array_unit)) ? 1 : 0)),
    m_array(new t_array_unit[m_array_size])
      {
        if(!m_array) throw quicky_exception::quicky_runtime_exception("Unable to allocate array memory",__LINE__,__FILE__);
        reset(p_reset_value);
      }
    //----------------------------------------------------------------------------
    quicky_bitfield::quicky_bitfield(const quicky_bitfield & p_bitfield):
      m_size(p_bitfield.m_size),
      m_array_size(p_bitfield.m_array_size),
      m_array(new t_array_unit[m_array_size])
        {
          if(!m_array) throw quicky_exception::quicky_runtime_exception("Unable to allocate array memory",__LINE__,__FILE__);
          memcpy(m_array,p_bitfield.m_array,sizeof(t_array_unit) * m_array_size);
        }
    
      //----------------------------------------------------------------------------
      void quicky_bitfield::reset(bool p_reset_value)
      {
        memset(m_array,p_reset_value ? 0xFF : 0,sizeof(t_array_unit) * m_array_size);
      }

      //----------------------------------------------------------------------------
      void quicky_bitfield::set(const unsigned int & p_data,
                                const unsigned int & p_size,
                                const unsigned int & p_offset)
      {
        assert(p_size < 8 * sizeof(unsigned int));
        assert(p_offset + p_size -1 < m_array_size * 8 * sizeof(t_array_unit));
        assert(p_size <= 8 * sizeof(t_array_unit));
        assert(p_data < ( (unsigned int)1 << p_size));

        unsigned int l_data = (( 1 << p_size) - 1) & p_data;

        unsigned int l_min_index = p_offset / (8 * sizeof(t_array_unit));
        unsigned int l_max_index = ( p_offset + p_size - 1) / ( 8 * sizeof(t_array_unit));
        if(l_min_index == l_max_index)
          {
            t_array_unit l_mask = l_data << p_offset;
            m_array[l_min_index] &= ~(((( 1 << p_size) - 1)) << p_offset);
            m_array[l_min_index] |= l_mask;
          }
        else
          {
            unsigned int l_min_mod = p_offset % (8 * sizeof(t_array_unit));
            unsigned int l_size = 8 * sizeof(t_array_unit) - l_min_mod;
            m_array[l_min_index] &= ~((( 1 << l_size) - 1) << l_min_mod);
            t_array_unit l_mask = ((( 1 << l_size) - 1) & l_data) << l_min_mod;
             m_array[l_min_index] |= l_mask;
       
            l_mask = p_data >> l_size;
            m_array[l_max_index] &= ~((( 1 << (p_size - l_size)) -1));
            m_array[l_max_index] |= l_mask;

          }
      }

      //----------------------------------------------------------------------------
      void quicky_bitfield::get(unsigned int & p_data,
                                const unsigned int & p_size,
                                const unsigned int & p_offset)const
      {
        assert(p_size < 8 * sizeof(unsigned int));
        assert(p_offset + p_size -1 < m_array_size * 8 * sizeof(t_array_unit));
        assert(p_size <= 8 * sizeof(t_array_unit));
        unsigned int l_min_index = p_offset / (8 * sizeof(t_array_unit));
        unsigned int l_max_index = ( p_offset + p_size - 1) / ( 8 * sizeof(t_array_unit));
        if(l_min_index == l_max_index)
          {
            t_array_unit l_data = m_array[l_min_index] >> p_offset;
            p_data = l_data & ((t_array_unit)(pow(2,p_size) - 1));
          }
        else
          {
            unsigned int l_min_mod = p_offset % (8 * sizeof(t_array_unit));
            unsigned int l_size = 8 * sizeof(t_array_unit) - l_min_mod;
            p_data = (m_array[l_min_index] >> l_min_mod ) & ((1 << l_size) - 1);
       
            t_array_unit l_data = m_array[l_max_index] & ((1 << (p_size - l_size)) - 1);
            p_data |= (l_data << l_size);
          }
      }

      //----------------------------------------------------------------------------
      quicky_bitfield::~quicky_bitfield(void)
        {
          delete[] m_array;
        }
}
#endif // QUICKY_BITFIELD
//EOF
