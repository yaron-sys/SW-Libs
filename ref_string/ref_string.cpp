#include <cstring>  // memcpy, strlen

#include "ref_string.hpp"

namespace ilrd{

/*******************************************************************************
                                RefString 
*******************************************************************************/
RefString::RefString(const char* str)
{ 
    size_t len = strlen(str);
    m_string = reinterpret_cast<Data*>(new char[sizeof(Data) + len + 1]);
    m_string->m_counter = 1;
    memcpy(m_string->m_arr,str, len + 1);
}

RefString::~RefString()
{
    --m_string->m_counter;
    if(m_string->m_counter == 0){
        delete[] m_string;
    }
}

RefString::RefString(const RefString& other): m_string(other.m_string)
{
    ++m_string->m_counter;
}

RefString& RefString::operator=(const RefString& other)
{
    if(this != &other){
        --m_string->m_counter;
        if(m_string->m_counter == 0)
            delete[] m_string;

        m_string = other.m_string;
        ++m_string->m_counter;
    }
    return *this;
}

RefString::CharProxy RefString::operator[](size_t index)
{
    return CharProxy(*this, index);
}

const char& RefString::operator[](size_t index) const
{
    return m_string->m_arr[index];
}

size_t RefString::length() const
{    
    return strlen(m_string->m_arr);
}

const char* RefString::c_str() const
{
    return m_string->m_arr;
}

/*******************************************************************************
                                CharProxy 
*******************************************************************************/
RefString::CharProxy::CharProxy(RefString& ref_str, size_t index): 
                                m_str(ref_str), m_index(index){}

RefString::CharProxy::~CharProxy(){}

RefString::CharProxy& RefString::CharProxy::operator=(char other)
{
    if(m_str.m_string->m_counter > 1){
        --m_str.m_string->m_counter;
        Data* new_data = reinterpret_cast<Data*>(new char[sizeof(Data) + m_str.length() + 1]);
        strcpy(new_data->m_arr, m_str.m_string->m_arr);
        new_data->m_counter = 1;
        m_str.m_string = new_data;
    }
    m_str.m_string->m_arr[m_index] = other;
    return *this;
}

RefString::CharProxy::CharProxy(const CharProxy& other):
                                m_str(other.m_str), m_index(other.m_index){}

RefString::CharProxy& RefString::CharProxy::operator=(CharProxy other)
{
     return *this = other.m_str.m_string->m_arr[other.m_index];
}

RefString::CharProxy::operator char() const
{
    return m_str.m_string->m_arr[m_index];
}

} // namespace ilrd
