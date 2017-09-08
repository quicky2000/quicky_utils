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
    friend bool operator==(const fract & p_op1,
			   const int & p_op2
			   );
    friend bool operator<(const fract & p_op1,
			  const int & p_op2
			  );
    friend bool operator<=(const fract & p_op1,
			   const int & p_op2
			   );
    friend bool operator>(const fract & p_op1,
			  const int & p_op2
			  );
    friend bool operator>=(const fract & p_op1,
			   const int & p_op2
			   );
    public:
    typedef uint32_t t_coef;
    typedef std::make_signed<t_coef>::type t_coef_num;
    typedef std::make_unsigned<t_coef>::type t_coef_den;

    inline constexpr fract(void) noexcept;

    inline constexpr fract(const t_coef_num & p_num);

    inline constexpr fract(const t_coef_den & p_den,std::nullptr_t);

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
    inline fract& operator=(const fract& p_other);
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
  constexpr fract::fract(void) noexcept:
    m_num(0),
    m_den(1)
  {
  }

  //----------------------------------------------------------------------------
  constexpr fract::fract(const t_coef_num & p_num):
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
  constexpr fract::fract(const t_coef_den & p_den,
			 std::nullptr_t
			 ):
    m_num(1),
    m_den(p_den)
  {
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
  fract& fract::operator=(const fract& p_other)
  {
    this->m_num = p_other.m_num;
    this->m_den = p_other.m_den;
    return *this;
  }

  //----------------------------------------------------------------------------
  fract::t_coef_den fract::PGCD(const t_coef_den & p_a,
				const t_coef_den & p_b
				)
  {
    assert(p_b);
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

  //----------------------------------------------------------------------------
  inline bool operator==(const fract & p_op1,
			 const int & p_op2
			 )
  {
    return p_op1 == fract(p_op2);
  }

  //----------------------------------------------------------------------------
  inline bool operator<(const fract & p_op1,
			const int & p_op2
			)
  {
    return p_op1 < fract(p_op2);
  }

  //----------------------------------------------------------------------------
  inline bool operator<=(const fract & p_op1,
			const int & p_op2
			)
  {
    return p_op1 <= fract(p_op2);
  }

  //----------------------------------------------------------------------------
  inline bool operator>(const fract & p_op1,
			const int & p_op2
			)
  {
    return p_op1 > fract(p_op2);
  }

  //----------------------------------------------------------------------------
  inline bool operator>=(const fract & p_op1,
			const int & p_op2
			)
  {
    return p_op1 >= fract(p_op2);
  }
}

#include <limits>

namespace std
{
  template<>
    class is_signed<quicky_utils::fract>
    {
    public:
      static const bool value = true;
    };

  template<>
    class numeric_limits<quicky_utils::fract>
    {
    public:
      static constexpr bool is_specialized = true;
      static constexpr quicky_utils::fract min() noexcept
	{
	  return quicky_utils::fract(std::numeric_limits<quicky_utils::fract::t_coef_num>::min());
	}
      static constexpr quicky_utils::fract max() noexcept
	{
	  return quicky_utils::fract(std::numeric_limits<quicky_utils::fract::t_coef_num>::max());
	}
      static constexpr quicky_utils::fract lowest() noexcept
	{
	  return quicky_utils::fract(std::numeric_limits<quicky_utils::fract::t_coef_num>::lowest());
	}
      static constexpr int digits = std::numeric_limits<quicky_utils::fract::t_coef_num>::digits;
      static constexpr int digits10 = std::numeric_limits<quicky_utils::fract::t_coef_num>::digits10;
      static constexpr int max_digits10 = std::numeric_limits<quicky_utils::fract::t_coef_num>::max_digits10;
      static constexpr bool is_signed = true;
      static constexpr bool is_integer = false;
      static constexpr bool is_exact = true;
      static constexpr int radix = 2;
      static constexpr quicky_utils::fract epsilon() noexcept
	{
	  return quicky_utils::fract(std::numeric_limits<quicky_utils::fract::t_coef_den>::max(),
				     nullptr
				     );
	}
      static constexpr quicky_utils::fract round_error() noexcept
	{
	  return quicky_utils::fract(std::numeric_limits<quicky_utils::fract::t_coef_den>::max(),
				     nullptr
				     );
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
      static constexpr bool infinity() noexcept { return 0; } // No sense
      static constexpr bool quiet_NaN() noexcept { return 0; } // No sense
      static constexpr bool signaling_NaN() noexcept { return 0; } // No sense
      static constexpr bool denorm_min() noexcept { return 0; } // No sense
      static constexpr bool is_iec559 = false;
      static constexpr bool is_bounded = true;
      static constexpr bool is_modulo = true;
      static constexpr bool traps = false;
      static constexpr bool tinyness_before = false;
      static constexpr float_round_style round_style = round_toward_zero;
    };
}

#endif // _FRACT_H_
// EOF
