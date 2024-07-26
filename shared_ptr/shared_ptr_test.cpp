#include <iostream> // std

#include "shared_ptr.hpp"


int main()
{
    using namespace std;
    using namespace ilrd;

    class base{
    public:
        base(int mint=0 ,long mlong=0): m_int(mint), m_long(mlong){}
        int m_int;
        long m_long;
        void bla()
        {
            cout << "hello world\n";
        }
    };
    
    class derived: public base
    {
    };

    SharedPtr<derived> my_derived(new derived());
    SharedPtr<base> my_base(my_derived);
    
    cout << "my_base->m_int: " << my_base->m_int << endl;


    SharedPtr<int> p1(new int(10));
    SharedPtr<double> p2(new double(5.05));
    SharedPtr<base> p3(new base(17, 3000));
    
    cout << "*p1 = " << *p1 << endl;
    cout << "*p2 = " << *p2 << endl;
    
    cout << "base m_int: " << p3->m_int << endl;
    cout << "base m_long: " << p3->m_long << endl;
    p3->bla();
    
    
    SharedPtr<double> p2_cpy(p2);
    cout << "*p2_cpy = " << *p2_cpy << endl;
    
    SharedPtr<int> p1_assn;
    if(!p1_assn)
    {
        cout << "p1_assn is nullptr\n";
    }
    
    p1_assn = p1;
    if(p1_assn == p1){
        cout << "p1_assn==p1 (true)\n";
    }
    
    cout << "*p1_assn = " << *p1_assn << endl;
    *p1_assn = 12;
    cout << "*p1_assn = " << *p1_assn << endl;
    if(p1_assn)
    {
        cout << "p1_assn is not nullptr\n";
    }
    
    int* g = p1_assn.get();
    cout << "*g = " << *g << endl;
    
    return 0;
}


    //~ int* x = new int (5);
    //~ SharedPtr<int> p(x);
