#ifndef COMPLEX_HPP_
#define COMPLEX_HPP_

#include <cstddef> // size_t
#include <iosfwd>  // istream, ostream

#if __cplusplus < 201103L
#define nullptr 0
#endif //__cplusplus < 201103L

namespace ilrd{

class Complex{
    friend std::istream& operator>>(std::istream& is, Complex& obj);
    friend std::ostream& operator<<(std::ostream& os, Complex& obj);

public:
    Complex(double real = 0, double imag = 0);

    inline bool operator==(const Complex& other) const
    {
        return m_real==other.m_real && m_imag==other.m_imag;
    }
    inline bool operator!=(const Complex& other) const
    {
        return m_real!=other.m_real || m_imag!=other.m_imag;
    }
    inline Complex operator+(const Complex& other) const
    {
        return Complex(m_real+other.m_real, m_imag+other.m_imag);
    }
    inline Complex operator-(const Complex& other) const
    {
        return Complex(m_real-other.m_real, m_imag-other.m_imag);
    }
    inline double get_real() const
    {
        return m_real;
    }
    inline double get_imag() const
    {
        return m_imag;
    }
    
    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex operator*(const Complex& other) const;
    Complex& operator*=(const Complex& other);
    Complex operator/(const Complex& other) const; //devision by 0 is undefined
    Complex& operator/=(const Complex& other); //devision by 0 is undefined

private:
    double m_real;
    double m_imag;
};

} // namespace ilrd

#endif // COMPLEX_HPP_
