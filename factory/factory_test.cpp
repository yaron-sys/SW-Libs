#include <iostream> // std

#include "factory.hpp"

using namespace std;
using namespace ilrd;

class Animal{
public:    
    virtual void print() const = 0;
private:
    int m_id;
};

class Cat: public Animal{
public:    
    Cat(const int &id): m_id(id){}
    void print() const{
        cout << "i'm cat id " << m_id << endl;
    }
private:
    int m_id;
};

class Dog: public Animal{
public:    
    Dog(const int &id): m_id(id){}
    void print() const{
        cout << "i'm dog id " << m_id << endl;
    }
private:
    int m_id;
};

Animal* create1(const int& num){
    return new Cat(num);
}

Animal* create2(const int& num){
    return new Dog(num);
}

int main()
{
    Factory<Animal,string,int> f;
    
    try{
        f.add("cat", create1);
        std::unique_ptr<Animal> der1 = f.create("cat", 5);
        der1->print();
    }
    catch(const Factory<Animal,string,int>::FactoryException& e) {
        cout << e.what() << endl;
    }
    
    try{
        f.remove("cat");
    }
    catch(const Factory<Animal,string,int>::FactoryException& e) {
        cout << e.what() << endl;
    }
    
    try{
        f.add("cat", create1);
        std::unique_ptr<Animal> der1 = f.create("cat", 10);
        der1->print();
    }
    catch(const Factory<Animal,string,int>::FactoryException& e) {
        cout << e.what() << endl;
    }
    
    try{
        f.add("dog", create2);
        std::unique_ptr<Animal> der2 = f.create("dog", 15);
        der2->print();
    }
    catch(const Factory<Animal,string,int>::FactoryException& e) {
        cout << e.what() << endl;
    }

    try{
        f.remove("dog");
    }
    catch(const Factory<Animal,string,int>::FactoryException& e) {
        cout << e.what() << endl;
    }
    
    try{
        f.remove("cat");
    }
    catch(const Factory<Animal,string,int>::FactoryException& e) {
        cout << e.what() << endl;
    }

    return 0;
}
