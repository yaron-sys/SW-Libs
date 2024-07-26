#ifndef STRING_HPP_
#define STRING_HPP_

#include <cstddef> // size_t
#include <iosfwd>  // istream

#if __cplusplus < 201103L
#define nullptr 0
#endif

namespace ilrd{
	
class String{
public:
    String(const char* str = "");
   ~String();
	String(const String& other);
    
	String& operator=(String other);
    String& operator+=(const String& other);
	char& operator[](const size_t idx);
	bool operator==(const String& other) const;
	bool operator>(const String& other) const;
	bool operator<(const String& other) const;
	bool operator!=(const String& other) const;
	
    size_t length() const;
    char* c_str() const;
    
    friend std::istream& operator>>(std::istream& in, String& obj);

private:	
	char* m_str;
    size_t m_len;
    
    char* copy_str(const char* str);
};

} // namespace ilrd

#endif  // STRING_HPP_
