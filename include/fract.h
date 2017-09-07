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
#ifndef _FRACT_H_
#define _FRACT_H_

#include <iostream>
#include <cinttypes>
#include <cstdlib>
#include <type_traits>
#include <cassert>

namespace quicky_utils
{
  class fract
  {
    friend std::ostream & operator<<(std::ostream & p_stream,
				     const fract & p_fract
				     );
    friend fract operator+(const int & p_op1,
			   const fract & p_op2
			   );
    friend fract operator-(const int & p_op1,
			   const fract & p_op2
			   );
    friend fract operator*(const int & p_op1,
			   const fract & p_op2
			   );
    friend fract operator/(const int & p_op1,
			   const fract & p_op2
			   );
    friend bool operator==(const int & p_op1,
			   const fract & p_op2
			   );
    public:
    typedef uint32_t t_coef;
    typedef std::make_signed<t_coef>::type t_coef_num;
    typedef std::make_unsigned<t_coef>::type t_coef_den;

    inline fract(const t_coef_num & p_num);

    inline fract(const t_coef_num & p_num,
		 const t_coef_num & p_den
		 );

    inline fract operator+(const fract & p_op1
			   ) const;

    inline fract operator-(const fract & p_op1
			   ) const;

    inline fract operator*(const fract & p_op1
			   ) const;

    inline fract operator/(const fract & p_op1
			   ) const;

    inline bool operator==(const fract & p_op)const;

    inline bool operator!=(const fract & p_op)const;

    inline fract operator-(void)const;
    inline fract operator+(void)const;

   /**
       Pre increment
    */
    inline fract & operator++(void);

    /**
       Post increment
     */
    inline fract operator++(int);

    /**
       Pre decrement
    */
    inline fract & operator--(void);

    /**
       Post decrement
     */
    inline fract operator--(int);

    /**
       Conversion operator
    */
    inline operator bool(void)const;

    inline fract & operator+=(const fract & p_op1);
    inline fract & operator-=(const fract & p_op1);
    inline fract & operator*=(const fract & p_op1);
    inline fract & operator/=(const fract & p_op1);

    inline bool operator<(const fract & p_op)const;
    inline bool operator<=(const fract & p_op)const;
    inline bool operator>(const fract & p_op)const;
    inline bool operator>=(const fract & p_op)const;

    inline fract& operator=(fract&& p_other) noexcept;
    inline fract(const fract & p_op);

    /**
       Return PGCD computed yusing Euclide algorithm
       @param first number
       @param second number
       @return PGCD of first and second number
    */
    inline static t_coef_den PGCD(const t_coef_den & p_a,
				  const t_coef_den & p_b
				  );

    /**
       Return PPCM using PGCD/PPCM correspondancy
       @param first number
       @param second number
       @return PPCM of first and second number
    */
    inline static t_coef_den PPCM(const t_coef_den & p_a,
				  const t_coef_den & p_b
			      );
  private:
    t_coef_num m_num;
    t_coef_den m_den;
  };

  //----------------------------------------------------------------------------
  fract::fract(const t_coef_num & p_num):
    m_num(p_num),
    m_den(1)
  {
  }

  //----------------------------------------------------------------------------
  fract::fract(const t_coef_num & p_num,
	       const t_coef_num & p_den
	       )
  {
    assert(p_den);
    t_coef_den l_pgcd = PGCD(p_num,p_den);
    m_num = abs(p_num) / ((t_coef_num)l_pgcd);
    if(p_num * p_den < 0)
      {
	m_num = -m_num;
      }
    m_den = (t_coef_den)(abs(p_den)) / l_pgcd;
  }

  //----------------------------------------------------------------------------
  fract fract::operator+(const fract & p_op
			 ) const
  {
    t_coef_den l_ppcm = PPCM(this->m_den,
			 p_op.m_den
			 );
    t_coef_num l_num = (t_coef_num)(l_ppcm / this->m_den) * this->m_num + (t_coef_num)(l_ppcm / p_op.m_den) * p_op.m_num;
    return fract(l_num, l_ppcm);
  }

  //----------------------------------------------------------------------------
  fract fract::operator-(const fract & p_op
			 ) const
  {
    t_coef_den l_ppcm = PPCM(this->m_den,
			 p_op.m_den
			 );
    t_coef_num l_num = (t_coef_num)(l_ppcm / this->m_den) * this->m_num - (t_coef_num)(l_ppcm / p_op.m_den) * p_op.m_num;
    return fract(l_num, l_ppcm);
  }

  //----------------------------------------------------------------------------
  fract fract::operator*(const fract & p_op
			 ) const 
  {
    return fract(m_num * p_op.m_num,m_den * p_op.m_den);
  }

  //----------------------------------------------------------------------------
  fract fract::operator/(const fract & p_op
			 )const
  {
    return fract(m_num * (t_coef_num)p_op.m_den,((t_coef_num)(m_den)) * p_op.m_num);
  }

  //----------------------------------------------------------------------------
  bool fract::operator==(const fract & p_op)const
  {
    return m_num == p_op.m_num && m_den == p_op.m_den;
  }

  //----------------------------------------------------------------------------
  bool fract::operator!=(const fract & p_op)const
  {
    return m_num != p_op.m_num || m_den != p_op.m_den;
  }

  //----------------------------------------------------------------------------
  fract & fract::operator++(void)
  {
    *this = *this + fract(1);
    return *this;
  }

  //----------------------------------------------------------------------------
  fract fract::operator++(int)
  {
    fract tmp(*this); 
    *this = *this + fract(1);
    return tmp;
  }

  //----------------------------------------------------------------------------
  fract & fract::operator--(void)
  {
    *this = *this - fract(1);
    return *this;
  }

  //----------------------------------------------------------------------------
  fract fract::operator--(int)
  {
    fract tmp(*this); 
    *this = *this - fract(1);
    return tmp;
  }

  //----------------------------------------------------------------------------
  fract fract::operator-(void)const
  {
    return fract(-m_num, m_den);
  }

  //----------------------------------------------------------------------------
  fract fract::operator+(void)const
  {
    return fract(m_num, m_den);
  }

  //----------------------------------------------------------------------------
  fract::operator bool(void)const
  {
    return m_num;
  }

  //----------------------------------------------------------------------------
  fract & fract::operator+=(const fract & p_op1)
  {
    *this = *this + p_op1;
    return * this;
  }

  //----------------------------------------------------------------------------
  fract & fract::operator-=(const fract & p_op1)
  {
    *this = *this - p_op1;
    return * this;
  }

  //----------------------------------------------------------------------------
  fract & fract::operator*=(const fract & p_op1)
  {
    *this = *this * p_op1;
    return * this;
  }

  //----------------------------------------------------------------------------
  fract & fract::operator/=(const fract & p_op1)
  {
    *this = *this / p_op1;
    return * this;
  }

  //----------------------------------------------------------------------------
  bool fract::operator<(const fract & p_op)const
  {
    t_coef_den l_ppcm = PPCM(this->m_den,
			     p_op.m_den
			     );
    return ((t_coef_num)(l_ppcm / this->m_den) * this->m_num) < ((t_coef_num)(l_ppcm / p_op.m_den) * p_op.m_num);
  }

  //----------------------------------------------------------------------------
  bool fract::operator<=(const fract & p_op)const
  {
    t_coef_den l_ppcm = PPCM(this->m_den,
			     p_op.m_den
			     );
    return ((t_coef_num)(l_ppcm / this->m_den) * this->m_num) <= ((t_coef_num)(l_ppcm / p_op.m_den) * p_op.m_num);
  }

  //----------------------------------------------------------------------------
  bool fract::operator>(const fract & p_op)const
  {
    t_coef_den l_ppcm = PPCM(this->m_den,
			     p_op.m_den
			     );
    return ((t_coef_num)(l_ppcm / this->m_den) * this->m_num) > ((t_coef_num)(l_ppcm / p_op.m_den) * p_op.m_num);
  }

  //----------------------------------------------------------------------------
  bool fract::operator>=(const fract & p_op)const
  {
    t_coef_den l_ppcm = PPCM(this->m_den,
			     p_op.m_den
			     );
    return ((t_coef_num)(l_ppcm / this->m_den) * this->m_num) >= ((t_coef_num)(l_ppcm / p_op.m_den) * p_op.m_num);
  }

  //----------------------------------------------------------------------------
  fract::fract(const fract & p_op):
    m_num(p_op.m_num),
    m_den(p_op.m_den)
  {
  }

  //----------------------------------------------------------------------------
  fract& fract::operator=(fract&& p_other) noexcept
  {
	// no-op on self-move-assignment
    if(this != &p_other)
      {
	std::swap(m_num,p_other.m_num);
	std::swap(m_den,p_other.m_den);
      }
    return *this;
  }

  //----------------------------------------------------------------------------
  fract::t_coef_den fract::PGCD(const t_coef_den & p_a,
				const t_coef_den & p_b
				)
  {
    t_coef_den l_a = abs(p_a);
    t_coef_den l_b = abs(p_b);
    t_coef_den l_r;
    while((l_r = (l_a % l_b)))
      {
	l_a = l_b;
	l_b = l_r;
      }
    return l_b;
  }

  //----------------------------------------------------------------------------
  fract::t_coef_den fract::PPCM(const t_coef_den & p_a,
				const t_coef_den & p_b
			    )
  {
    return p_a * p_b / PGCD(p_a,p_b);
  }

  //----------------------------------------------------------------------------
  inline std::ostream & operator<<(std::ostream & p_stream,
				   const fract & p_fract
				   )
  {
    p_stream << p_fract.m_num;
    if(1 != p_fract.m_den)
      {
	p_stream << "/" << p_fract.m_den;
      }
    return p_stream;
  }

  //----------------------------------------------------------------------------
  inline fract operator+(const int & p_op1,
			 const fract & p_op2
			 )
  {
    return fract(p_op1) + p_op2;
  }

  //----------------------------------------------------------------------------
  inline fract operator-(const int & p_op1,
			 const fract & p_op2
			 )
  {
    return fract(p_op1) - p_op2;
  }

  //----------------------------------------------------------------------------
  inline fract operator*(const int & p_op1,
			 const fract & p_op2
			 )
  {
    return fract(p_op1) * p_op2;
  }

  //----------------------------------------------------------------------------
  inline fract operator/(const int & p_op1,
			 const fract & p_op2
			 )
  {
    return fract(p_op1) / p_op2;
  }

  //----------------------------------------------------------------------------
  inline bool operator==(const int & p_op1,
			 const fract & p_op2
			 )
  {
      return fract(p_op1) == p_op2;
  }
}

namespace std
{
  template<>
    class is_signed<quicky_utils::fract>
    {
    public:
      static const bool value = true;
    };
}

#endif // _FRACT_H_
// EOF
