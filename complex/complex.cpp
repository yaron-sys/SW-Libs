#include <iostream>
#include <cassert>

#include "complex.hpp"

using namespace ilrd;

Complex::Complex(double real, double imag): m_real(real), m_imag(imag){}

Complex& Complex::operator+=(const Complex& other)
{
    m_real += other.m_real;
    m_imag += other.m_imag;
    return *this;
}

Complex& Complex::operator-=(const Complex& other)
{
    m_real -= other.m_real;
    m_imag -= other.m_imag;
    return *this;
}

Complex Complex::operator*(const Complex& other) const
{
    return Complex((m_real*other.m_real) - (m_imag*other.m_imag),
                    (m_real*other.m_imag) + (m_imag*other.m_real));
}

Complex& Complex::operator*=(const Complex& other)
{
    double new_real = (m_real*other.m_real) - (m_imag*other.m_imag);
    double new_imag = (m_real*other.m_imag) + (m_imag*other.m_real);
    
    m_real = new_real;
    m_imag = new_imag;
    return *this;
}

Complex Complex::operator/(const Complex& other) const
{
    double denominator = other.m_real*other.m_real + other.m_imag*other.m_imag;
    
    assert(denominator != 0);
    
    double new_real = ((m_real*other.m_real) + (m_imag*other.m_imag)) / denominator;
    double new_imag = ((m_imag*other.m_real) - (m_real*other.m_imag)) / denominator;
    return Complex(new_real,new_imag);
}

Complex& Complex::operator/=(const Complex& other)
{
    double denominator = other.m_real*other.m_real + other.m_imag*other.m_imag;
   
    assert(denominator != 0);
    
    double new_real = ((m_real*other.m_real) + (m_imag*other.m_imag)) / denominator;
    double new_imag = ((m_imag*other.m_real) - (m_real*other.m_imag)) / denominator; 
    
    m_real = new_real;
    m_imag = new_imag;
    return *this;
}

std::ostream& ilrd::operator<<(std::ostream& os, Complex& obj)
{
    os << "(" << obj.m_real << "," << obj.m_imag << ")";
    return os;
}

std::istream& ilrd::operator>>(std::istream& is, Complex& obj)
{
   double new_real, new_imag;

   is.ignore(256, '(');     // ignor chars up to first '('
   is>>new_real;            // read real part
   
   is.ignore(256,',');      // chars up to next "," are ignored.
   is>>new_imag;            // read imaginary part
   
   is.ignore(256,')');      // ignore white spaces after it.

   if (is.good())
   {
       obj.m_real = new_real;
       obj.m_imag = new_imag;
   }

   return is;
}
