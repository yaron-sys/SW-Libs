#ifndef BARR_HPP_
#define BARR_HPP_

#include <climits>      // CHAR_BIT
#include <string>       // std::string
#include <algorithm>    // for_each, fill, copy
#include <stdexcept>    // std::out of range

#include "util.hpp"     // nullptr

namespace ilrd{

typedef unsigned long WordT;
enum { BARR_WORD_SIZE = sizeof(WordT) * CHAR_BIT };

template <size_t N>
class BitArray;

template <size_t N>
const BitArray<N> operator^(BitArray<N> lhs,const BitArray<N>& rhs);

template <size_t N>
const BitArray<N> operator&(BitArray<N> lhs,const BitArray<N>& rhs);

template <size_t N>
const BitArray<N> operator|(BitArray<N> lhs,const BitArray<N>& rhs);

template <size_t N>
const BitArray<N> operator>>(BitArray<N> barr,size_t shift_num);

template <size_t N>
const BitArray<N> operator<<(BitArray<N> barr,size_t shift_num);

template <size_t N>
class BitArray{
    
    class BitProxy;

public:
    BitArray();
    //~BitArray() = defult;
    BitArray(const BitArray& other);
    BitArray& operator=(const BitArray& other);
    
    BitArray& operator&=(const BitArray& other);
    BitArray& operator^=(const BitArray& other);
    BitArray& operator|=(const BitArray& other);
    BitArray& operator>>=(size_t shift_num);
    BitArray& operator<<=(size_t shift_num);
    const BitArray operator~() const;
  
    BitProxy operator[](size_t idx);
    bool operator[](size_t idx) const;
    
    void set_bit(size_t idx, bool state); //safe, throws std::out of range
    bool get_bit(size_t idx) const;       //safe, throws std::out of range
    void flip_bit(size_t idx); 
        
    size_t count_on() const;
    const std::string to_string() const; 
    
private:
    static inline size_t calc_len(size_t len)
    {
        return (len / BARR_WORD_SIZE) + !!(len % BARR_WORD_SIZE);
    }
    size_t m_len;
    WordT m_barr[(N / BARR_WORD_SIZE) + !!(N % BARR_WORD_SIZE)];
    
    class BitProxy{
    public:  
        BitProxy(BitArray* barr, size_t idx); 
        //~BitProxy() = defult;
        bool operator=(const BitProxy& other); //b[6] = b[5]
        bool operator=(bool bit);              //b[1] = true
        operator bool() const;                 //b[6] == b[5]
    private:
        BitArray& m_bit_array;
        size_t m_idx; 
    };
};

/*******************************************************************************
                                    BitProxy
*******************************************************************************/
template <size_t N>
inline BitArray<N>::BitProxy::BitProxy(BitArray* barr, size_t idx): m_bit_array(*barr), m_idx(idx)
{}

template <size_t N>
inline bool BitArray<N>::BitProxy::operator=(const BitProxy& other)
{
    return m_bit_array[m_idx] = other.m_bit_array.get_bit(other.m_idx);
}

template <size_t N>
inline bool BitArray<N>::BitProxy::operator=(bool bit)
{
    m_bit_array.set_bit(m_idx, bit);
    return bit;
}

template <size_t N>
inline BitArray<N>::BitProxy::operator bool() const
{
    return m_bit_array.get_bit(m_idx);
}

/*******************************************************************************
                                    BitArray
*******************************************************************************/
template <size_t N>
BitArray<N>::BitArray(): m_len(N)
{
    std::fill(m_barr, m_barr + calc_len(N), 0);
}

template <size_t N>
BitArray<N>::BitArray(const BitArray& other): m_len(N)
{
    std::copy(other.m_barr, other.m_barr + calc_len(N), m_barr);
}

template <size_t N>
BitArray<N>& BitArray<N>::operator=(const BitArray& other)
{
    m_len = other.m_len;
    std::copy(other.m_barr, other.m_barr + calc_len(N), m_barr);
    return *this;
}

template <size_t N>
inline void BitArray<N>::set_bit(size_t idx, bool state)
{
	WordT mask = 1;
    size_t real_idx = idx / BARR_WORD_SIZE;
    
    if(m_len <= idx){
        throw std::out_of_range("index is out of range");
    }
    
    if(state){
		m_barr[real_idx] |= (mask << idx % BARR_WORD_SIZE);
	}
	
	else{
		m_barr[real_idx] &= ~(mask << idx % BARR_WORD_SIZE);
	}   
}

template <size_t N>
inline bool BitArray<N>::get_bit(size_t idx) const
{
    if(m_len <= idx){
        throw std::out_of_range("index is out of range");
    }
    
    WordT mask = 1;
    return !!(m_barr[idx / BARR_WORD_SIZE] & 
             (mask << (idx % BARR_WORD_SIZE)));
}

template <size_t N>
inline typename BitArray<N>::BitProxy BitArray<N>::operator[](size_t idx)
{
    return BitProxy(this, idx);
}

template <size_t N>
inline bool BitArray<N>::operator[](size_t idx)const
{
    return get_bit(idx);
}

struct ToString
{
    ToString(std::string& str): m_str(str){}
    std::string& m_str;
    
    void operator()(WordT& m_barr)
    {
        for (long shift = 0; shift < BARR_WORD_SIZE; ++shift){
            m_str.push_back((m_barr & (WordT)1) + '0');
            m_barr >>= (WordT)1;
        }
    }
};

template <size_t N>
const std::string BitArray<N>::to_string() const
{
	std::string str;
    ToString str_s(str);
    BitArray<N> copy(*this);
    
    std::for_each(copy.m_barr, copy.m_barr + calc_len(N), str_s);
    str.resize(N);
    std::reverse(str.begin(), str.end());
    return str;
}

template <size_t N>
inline void BitArray<N>::flip_bit(size_t idx)
{
    set_bit(idx, !get_bit(idx));
}


struct CountOn{
    CountOn(size_t& ctr): m_ctr(ctr) {}
    size_t& m_ctr;
    
    void operator()(WordT& m_barr)
    {
        while(m_barr){
            m_barr &= (m_barr - 1);
            ++m_ctr;
        }
    }
};

template <size_t N>
size_t BitArray<N>::count_on() const
{
    size_t ctr = 0;
    BitArray<N> copy(*this);
    CountOn count_on(ctr);
    std::for_each(copy.m_barr, copy.m_barr + calc_len(N), count_on);
    return ctr;
}

/*******************************************************************************/

struct OrOper{
    OrOper(const WordT *other_barr): m_other_barr(other_barr){}
    const WordT *m_other_barr;
    
    void operator()(WordT& m_barr)
    {
        m_barr |= *m_other_barr;
        ++m_other_barr;
    }
};

template <size_t N>
BitArray<N>& BitArray<N>::operator|=(const BitArray<N>& other)
{
    std::for_each(m_barr, m_barr + calc_len(N), OrOper(other.m_barr));
    return *this;
}

struct AndOper{
    AndOper(const WordT *other_barr): m_other_barr(other_barr){}
    const WordT *m_other_barr;
    
    void operator()(WordT& m_barr)
    {
        m_barr &= *m_other_barr;
        ++m_other_barr;
    }
};

template <size_t N>
BitArray<N>& BitArray<N>::operator&=(const BitArray<N>& other)
{
    std::for_each(m_barr, m_barr + calc_len(N), AndOper(other.m_barr));
    return *this;
}

struct XorOper{
    XorOper(const WordT *other_barr): m_other_barr(other_barr){}
    const WordT *m_other_barr;
    
    void operator()(WordT& m_barr)
    {
        m_barr ^= *m_other_barr;
        ++m_other_barr;
    }
};

template <size_t N>
BitArray<N>& BitArray<N>::operator^=(const BitArray<N>& other)
{
    std::for_each(m_barr, m_barr + calc_len(N), XorOper(other.m_barr));
    return *this;
}

template <size_t N>
inline const BitArray<N> operator^(BitArray<N> lhs,const BitArray<N>& rhs)
{
    return lhs ^= rhs;
}

template <size_t N>
inline const BitArray<N> operator&(BitArray<N> lhs,const BitArray<N>& rhs)
{
    return lhs &= rhs;
}

template <size_t N>
inline const BitArray<N> operator|(BitArray<N> lhs,const BitArray<N>& rhs)
{
    return lhs |= rhs;
}

inline void NotFunc(WordT& m_barr)
{
    m_barr = ~m_barr;
}

template <size_t N>
const BitArray<N> BitArray<N>::operator~() const
{
    BitArray<N> copy(*this);
    std::for_each(copy.m_barr, copy.m_barr + copy.calc_len(N), NotFunc);
    
    WordT mask = ~0;
    size_t shift_num = BARR_WORD_SIZE - (N % BARR_WORD_SIZE);
    if (shift_num != BARR_WORD_SIZE) {
        mask >>= shift_num;
        copy.m_barr[copy.calc_len(N) - 1] &= mask;
    }
    return copy;
}

/*******************************************************************************
                                ShiftNumLeft
*******************************************************************************/

struct ShiftNumLeft{
    ShiftNumLeft(size_t shift): m_shift(shift), m_prev(0){}
    size_t m_shift;
    WordT m_prev;
    
    void operator()(WordT& m_barr)
    {
        WordT m_next = m_barr >> (BARR_WORD_SIZE - m_shift);
        m_barr = (m_barr << m_shift) | m_prev;
        m_prev = m_next;
    }
};

template <size_t N>
BitArray<N>& BitArray<N>::operator<<=(size_t shift_num)
{
    if(shift_num >= N){
        std::fill(&m_barr[0], &m_barr[calc_len(N)], 0);
        return *this;
    }
    size_t real_idx = shift_num / BARR_WORD_SIZE;
    
    if(shift_num >= BARR_WORD_SIZE){
        std::copy(m_barr, m_barr+calc_len(N)-real_idx, m_barr+real_idx);
        
        //clean begin
        std::fill(m_barr, m_barr + real_idx, 0);
    }
    
    if(shift_num % BARR_WORD_SIZE != 0){
        std::for_each(m_barr + real_idx, &m_barr[calc_len(N)], ShiftNumLeft(shift_num));
    }
    
    //clean end
    WordT mask = ~0;
    shift_num = BARR_WORD_SIZE - (N % BARR_WORD_SIZE);
    if (shift_num != BARR_WORD_SIZE) {
        mask >>= shift_num;
        m_barr[calc_len(N) - 1] &= mask;
    }
    
    return *this;
}

template <size_t N>
const BitArray<N> operator<<(BitArray<N> barr,size_t shift_num)
{
    return barr <<= shift_num;
}

/*******************************************************************************
                                ShiftNumRight
*******************************************************************************/
struct ShiftNumRight{
    ShiftNumRight(size_t shift): m_shift(shift), m_prev(0){}
    size_t m_shift;
    WordT m_prev;
    
    void operator()(WordT& m_barr)
    {
        WordT m_next = m_barr << (BARR_WORD_SIZE - m_shift);
        m_barr = (m_barr >> m_shift) | m_prev;
        m_prev = m_next;
    }
};

template <size_t N>
BitArray<N>& BitArray<N>::operator>>=(size_t shift_num)
{
    std::reverse_iterator<WordT*> rbegin = std::reverse_iterator<WordT*>(m_barr+calc_len(N));
    std::reverse_iterator<WordT*> rend = std::reverse_iterator<WordT*>(m_barr);
    
    if(shift_num >= N){
        std::fill(rbegin, rend, 0);
        return *this;
    }
    size_t real_idx = shift_num / BARR_WORD_SIZE;
    
    if(real_idx > 0){
        std::copy(m_barr+real_idx, m_barr+calc_len(N), m_barr);
        std::fill(m_barr+calc_len(N)-real_idx, m_barr+calc_len(N), 0);
    }
    
    if(shift_num % BARR_WORD_SIZE != 0)
        std::for_each(rbegin, rend, ShiftNumRight(shift_num));

    return *this;
}

template <size_t N>
const BitArray<N> operator>>(BitArray<N> barr, size_t shift_num)
{
    return barr >>= shift_num;
}


} // namespace ilrd

#endif // BARR_HPP_
