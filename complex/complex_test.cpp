#include <iostream>

#include "complex.hpp"


int main()
{
	using namespace std;
	using namespace ilrd;

	Complex comp1(5,6);
    Complex comp2(5,7);
    
    if(comp1!=comp2)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }
    
    comp1 = comp2;
    if(comp1==comp2)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }
	
    Complex comp3(5.2, 3.0);
    Complex comp4 = comp3 + comp2;
    if(comp4.get_real() == 10.2 && comp4.get_imag() == 10.0)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }
    
    comp4 += comp2;
    if(comp4.get_real() == 15.2 && comp4.get_imag() == 17.0)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }
    
    comp1-=comp1;
    if(comp1.get_real() == 0.0 && comp1.get_imag() == 0.0)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }
    
    comp3 = comp1 - comp2;
    if(comp3.get_real() == -5.0 && comp3.get_imag() == -7.0)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }
    
    Complex num1(7.5, 4.3);
    Complex num2(8.7, -6.5);
    
    Complex num3 = num1 / num2;
    cout << "\n" << num3 << endl;
    cout << "Result should be: (0.316263, 0.730541)\n";
    
    num3 /= num3;
    if(num3.get_real() == 1.0 && num3.get_imag() == 0.0)
    {
        cout << "success\n";
    }
    else
    {
        cout << "fail\n";
    }

    Complex num4 = num1 * num2;
    cout  << "\n" << num4 << endl;
    cout << "Result should be: (93.2,-11.34)\n";
    
    num1 *= num2;
    cout << "\n" << num4 << endl;
    cout << "Result should be: (93.2,-11.34)\n";
    
    Complex num5 = num1 * num1;
    cout << "\n"  << num5 << endl;
    cout << "Result should be: (8557.64,-2113.78)\n";
    
   
    Complex try1,try2,try3;
    cout << "\nenter 3 complex numbers: ";
    cin >> try1 >> try2 >> try3;
    
    cout << "you entered the following complex: " << try1 << try2 << try3 << endl;
    
    return 0;
}

