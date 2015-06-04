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
    inline quicky_bitfield(const unsigned int & p_size);
    inline quicky_bitfield(const quicky_bitfield & p_bitfield);
    inline ~quicky_bitfield(void);
    inline void set(const unsigned int & p_data,
                    const unsigned int & p_size,
                    const unsigned int & p_offset);
    inline void get(unsigned int & p_data,
                    const unsigned int & p_size,
                    const unsigned int & p_offset)const;
    inline void reset(void);
    inline void dump_in(std::ostream & p_stream)const;
    inline void read_from(std::istream & p_stream);
    inline const size_t size(void)const;
  private:
    const unsigned int m_size;
    typedef unsigned int t_array_unit;
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
  quicky_bitfield::quicky_bitfield(const unsigned int & p_size):
    m_size(p_size),
    m_array_size(p_size / (8 * sizeof(t_array_unit)) + (p_size % (8 * sizeof(t_array_unit)) ? 1 : 0)),
    m_array(new t_array_unit[m_array_size])
      {
        if(!m_array) throw quicky_exception::quicky_runtime_exception("Unable to allocate array memory",__LINE__,__FILE__);
        reset();
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
      void quicky_bitfield::reset(void)
      {
        memset(m_array,0,sizeof(t_array_unit) * m_array_size);
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
