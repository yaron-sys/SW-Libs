#include <iostream> // std

#include "ref_string.hpp"

int main()
{
    using namespace std;
    using namespace ilrd;

    RefString ref1("hello");
    cout << "ref1 before:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "create ref2 with cctor\n";
    RefString ref2(ref1);
    cout << ref2.mycount() << endl;
    cout << ref2.length() << endl;
    cout << ref2.c_str() << endl;
    
    cout << "ref1 after:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "-------------------------" << endl;
    RefString ref3("cockie");
    ref1 = ref3;
    
    cout << "ref3:\n";
    cout << ref3.mycount() << endl;
    cout << ref3.length() << endl;
    cout << ref3.c_str() << endl;
    
    cout << "ref1:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "ref2:\n";
    cout << ref2.mycount() << endl;
    cout << ref2.length() << endl;
    cout << ref2.c_str() << endl;
    
    cout << "-------------------------" << endl;
    ref2 = ref3;
    cout << "ref3:\n";
    cout << ref3.mycount() << endl;
    cout << ref3.length() << endl;
    cout << ref3.c_str() << endl;
    
    cout << "ref1:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "ref2:\n";
    cout << ref2.mycount() << endl;
    cout << ref2.length() << endl;
    cout << ref2.c_str() << endl;
    
    const char ret = ref2[0];
    cout << "ref2[1]: " << ret << endl;
    
    ref3[0] = 'g';
    cout << "\nref3:\n";
    cout << ref3.mycount() << endl;
    cout << ref3.length() << endl;
    cout << ref3.c_str() << endl;
    
    
    cout << "\nref1:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "\nref2:\n";
    cout << ref2.mycount() << endl;
    cout << ref2.length() << endl;
    cout << ref2.c_str() << endl;
    
    ref3[2] = 'a';
    cout << "\nref3:\n";
    cout << ref3.mycount() << endl;
    cout << ref3.length() << endl;
    cout << ref3.c_str() << endl;
    
    
    cout << "\nref1:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "\nref2:\n";
    cout << ref2.mycount() << endl;
    cout << ref2.length() << endl;
    cout << ref2.c_str() << endl;
    
    ref2[1] = 'b';
    cout << "\nref3:\n";
    cout << ref3.mycount() << endl;
    cout << ref3.length() << endl;
    cout << ref3.c_str() << endl;
    
    
    cout << "\nref1:\n";
    cout << ref1.mycount() << endl;
    cout << ref1.length() << endl;
    cout << ref1.c_str() << endl;
    
    cout << "\nref2:\n";
    cout << ref2.mycount() << endl;
    cout << ref2.length() << endl;
    cout << ref2.c_str() << endl;
    
    return 0;
}
