#include <iostream>
#include <cstring>

#include "string.hpp"

using namespace ilrd;

String::String(const char* str): m_str(copy_str(str)), m_len(strlen(str)){}

String::~String()
{
    delete[] m_str;
}

String::String(const String& other): m_str(copy_str(other.m_str)),
                                     m_len(other.m_len){}

String& String::operator=(String other)
{
    m_len = other.m_len;
    std::swap(m_str, other.m_str);
    return *this;
}

bool String::operator==(const String& other) const
{
    return strcmp(m_str, other.m_str) == 0;
}

bool String::operator!=(const String& other) const
{
    return strcmp(m_str, other.m_str) != 0;
}

bool String::operator>(const String& other) const
{
    return strcmp(m_str, other.m_str) > 0;
}

bool String::operator<(const String& other) const
{
    return strcmp(m_str, other.m_str) < 0;
}

String& String::operator+=(const String& other)
{
    m_len = length() + other.length();
    char* new_str = new char[m_len + 1];
    strcpy(new_str, m_str);
    new_str = strcat(new_str, other.m_str);
    delete[] m_str;
    m_str = new_str;
    return *this;
}

char& String::operator[](const size_t idx)
{    
    return m_str[idx];
}

std::istream& ilrd::operator>>(std::istream& in, String& obj)
{
    char buff[1000];
    in.getline(buff, 1000);
    obj = String(buff);
    return in;
}

char* String::copy_str(const char* str)
{
    char* buff = new char[strlen(str) + 1];
    strcpy(buff, str);
    return buff;
}

size_t String::length() const
{    
    return m_len;
}

char* String::c_str() const
{
    return m_str;
}
