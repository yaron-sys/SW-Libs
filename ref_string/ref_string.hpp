#ifndef REF_STRING_HPP_
#define REF_STRING_HPP_

#include <cstddef> // size_t

#include "macros.hpp" //__cplusplus < 201103L

namespace ilrd{

struct Data{
    long m_counter;
    char m_arr[1];
};

class RefString{
private:
    Data* m_string; 

    class CharProxy{
    public:
        CharProxy(RefString& ref_str, size_t index);
       ~CharProxy();
        CharProxy(const CharProxy& other);

        CharProxy& operator=(CharProxy other);
        CharProxy& operator=(char other);
        operator char() const;
 
    private:
        RefString& m_str;
        size_t m_index;
    };

public:
    RefString(const char* str = "");
   ~RefString();
    RefString(const RefString& other);
    
    RefString& operator=(const RefString& other);
    RefString::CharProxy operator[](size_t index);
    const char& operator[](size_t index) const;

    size_t length() const;
    const char* c_str() const;
};

} // namespace ilrd

#endif // REF_STRING_HPP_
