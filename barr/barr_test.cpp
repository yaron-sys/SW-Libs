#include <iostream> // std
#include <bitset>

#include "barr.hpp"

int main()
{
    using namespace std;
    using namespace ilrd;
    
    //~ size_t i = 0;
    //~ i = ~i;
    //~ cout << i << " < i\n";
    //~ i <<= 100UL;
    //~ cout << i << " < i\n";
    
    BitArray<70> x1;
    
    x1.set_bit(5, 1);
    x1.set_bit(2, 1);
    x1.set_bit(69, 1);
    
    cout << "x1: " << endl;
    cout << x1.to_string() << endl;
    
    BitArray<70> x2(x1);
    cout << "x2 copy of c1: " << endl;
    cout << x2.to_string() << endl;

    x2.set_bit(20, 1);
    x2.set_bit(7, 1);
    x2.set_bit(0, 1);
    
    cout << "x2 advanced: " << endl;
    cout << x2.to_string() << endl; 
    
    /*x1 |= x2;
    cout << "x1 |= x2: " << endl;
    cout << x1.to_string() << endl;*/
    
    x1 &= x2;
    cout << "x1 &= x2: " << endl;
    cout << x1.to_string() << endl;
    
    BitArray<70> x3;
    x3 = x1 ^ x2;
    cout << "x3 = x1 ^ x2: " << endl;
    cout << x3.to_string() << endl;
    
    x3.flip_bit(2);
    cout << "x3 flip 2: " << endl;
    cout << x3.to_string() << endl;
    
    x3.flip_bit(0);
    cout << "x3 flip 0: " << endl;
    cout << x3.to_string() << endl;
    
    x3.flip_bit(69);
    cout << "x3 flip 69: " << endl;
    cout << x3.to_string() << endl;
    
    x2 = ~x3;
    cout << "x2 = ~x3: " << endl;
    cout << x2.to_string() << endl;

    cout << x2.to_string() << endl;
    size_t ctr = x2.count_on();
    cout << ctr << endl;

    ctr = x2.count_on();
    cout << ctr << endl;
    
    cout << x2.to_string() << endl;
    ctr = x2.count_on();
    cout << ctr << endl;
    
    BitArray<200> x4;
    x4[0] = 1;
    x4[71] = 1;
    x4[10] = 1;
    x4[12] = 1;
    
    bitset<200> bs;
    bs[0] = 1;
    bs[71] = 1;
    bs[10] = 1;
    bs[12] = 1;
    
    cout << "mine:\n";
    cout << x4.to_string() << endl;
    x4>>=70;
    cout << endl <<  x4.to_string() << endl;
    ctr = x4.count_on();
    cout << ctr << endl;
    
    cout << "\bs:\n";
    cout << bs.to_string() << endl;
    bs>>=70;
    cout << endl <<  bs.to_string() << endl;
    ctr = bs.count();
    cout << ctr << endl;
    
    //~ BitArray<200> x6;
    //~ x6 = x4 << 2;
    //~ cout << endl <<  x6.to_string() << endl;
    //~ ctr = x6.count_on();
    //~ cout << ctr << endl;
    
    return 0;
}

/*
    BitArray<70> x1;
    
    try{
        x1.set_bit(5, 1);
    }
    catch(out_of_range){
        cout << "bla\n";
    }
    x1.set_bit(2, 1);
    cout << "x1.get_bit(10): " << x1.get_bit(10) << endl;
    cout << x1.to_string() << endl;
    
    BitArray<70> x2(x1);
    cout << "x2 copy of c1: " << endl;
    cout << x2.to_string() << endl;

    x1[3] = 1;
    x1.set_bit(15,1);
    cout << "x1: " << endl;
    cout << x1.to_string() << endl;

    x1[3] = x2[3];
    cout << x1.to_string() << endl;
    
    if(x1[3]==x2[3])
        cout << "true" << endl;  
*/
