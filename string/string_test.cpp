#include <iostream>

#include "string.hpp"
#include <stdio.h>

//TEST
int main(){
	
	using namespace std;
	using namespace ilrd;
    
    String s1("hello");
    String s2(s1);

    cout << "s1 length: " << s1.length() << endl;
    cout << "s1 string: " << s1.c_str() << endl;
    puts(s1.c_str());
    cout << "\n";
    
    cout << "s2 length: " << s2.length() << endl;
    cout << "s2 string: " << s2.c_str() << endl;
    puts(s2.c_str());
    cout << "\n";
    
    String s3;
    cout << "s3 length: " << s3.length() << endl;
    cout << "s3 string: " << s3.c_str();
    
    s3 = s2;
    cout << "s3 after assignment: " << s3.length() << endl;
    cout << "s3 string after assignment: " << s3.c_str() << endl;
    
    if(s3 == s2)
        cout << "success" << endl;
    else
        cout << "fail" << endl;

    if(s3 != s2)
        cout << "fail" << endl;
    else
        cout << "success" << endl;

    String s4 = "bla"; //without explicit
    if(s4 == s2)
        cout << "fail" << endl;
    else
        cout << "success" << endl;
   
    if(s4 != s2)
        cout << "success" << endl;
    else
        cout << "s3fail" << endl;
        
    if(s1 == "hello")
        cout << "success" << endl;
    else
        cout << "fail" << endl;
        
    if(s1 > s2)
        cout << "fail" << endl;
    else
        cout << "success" << endl;
	
    if(s1 > s4)
        cout << "success" << endl;
    else
        cout << "fail" << endl;
    
    if(s1 < s4)
        cout << "fail" << endl;
    else
        cout << "success" << endl;
    
    if(s4 < s1)
        cout << "success" << endl;
    else
        cout << "fail" << endl;  
        
    s4 += s1;
    puts(s4.c_str());
    cout << "s4 length: " << s4.length() << endl;
    cout << "s4 += s1 -> new s4: " << s4.c_str() << endl;  
    
    cout << "blahello - 'e': " << s4[4] << endl;
    s4[4] = 'C';
    cout << "blahello - 'C': " << s4[4] << endl;
    cout << "s4 new: " << s4.c_str() << endl; 
    
    if('C' == s4[4])
        cout << "success" << endl;
    else
        cout << "fail" << endl;  
    
    if('e' == s4[0])
        cout << "fail" << endl;
    else
        cout << "success" << endl;
   
    cout << "please type something: ";
    cin >> s4;
    cout << "you typed: " << s4.c_str() << endl; 
    cout << "s4 length after typing in: " << s4.length() << endl;
    
    String x,y;
    cout << "enter x and y: ";
    cin >> x >> y;
    cout << "x: " << x.c_str() << " and y: " << y.c_str() << endl;
    
    return (0);
}
