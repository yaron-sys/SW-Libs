

#include <iostream>
using namespace std;
	
class Animal
{
public:
	Animal()
	: m_id(++s_count)
	{ 
		cout << "Animal::Ctor()"  << m_id << endl; 
	}

	virtual ~Animal(){ --s_count; cout << "Animal::Dtor()" << m_id  << endl; }

	Animal(const Animal &other) 
	: m_id(++s_count)
	{ 
		cout << "Animal::CCtor() "  << m_id << endl; 
	}

	virtual void Display() 
	{ 
		cout << "Animal::Display() : " << m_id << endl; 
	}

	static void PrintCount() { cout << "s_count: " << s_count << endl;}
protected:
	int GetID() { return m_id; }

private:
	static int s_count;
	int m_id;
	Animal &operator=(const Animal &); // disabled
};

int Animal::s_count = 0;


class Centipede: public Animal
{
public:
	Centipede()
	: m_numLegs(46)
	{ 
		cout << "Centipede::Ctor()" << endl; 
	}
	Centipede(const Centipede &other )
	: Animal(other),
	  m_numLegs(other.m_numLegs)
	{ 
		cout << "Centipede::CCtor()" << endl; 
	}
	~Centipede() { cout << "Centipede::dtor()" << endl; }
	void Display() { cout << "Centipede::Display() ID:"<<GetID()<<"  num legs:" << m_numLegs << endl; }
	virtual void BuyShues() { cout << "Centipede::BuyShues() ID:"<<GetID() << endl; }

private:
	int m_numLegs;
};

class Squirl: public Animal
{
public:
	Squirl() { cout << "Squirl::Ctor()" << endl; }
	Squirl(const Squirl &other ): Animal(other) { cout << "Squirl::CCtor()" << endl; }
	~Squirl() { cout << "Squirl::dtor()" << endl; }
	void Display() { cout << "Squirl::Display() ID:"<<GetID()<< endl; }

private:
};

template<class T>
inline T maxFunc(const T &t1, const T &t2)
{
	return ((t1 > t2)? t1: t2);
}

class FlyingSquirl : public Squirl
{
public:
	FlyingSquirl() { cout << "FlyingSquirl::Ctor()" << endl; }
	FlyingSquirl(const FlyingSquirl &other): Squirl(other) { cout << "FlyingSquirl::CCtor()" << endl; }
	~FlyingSquirl(){ cout << "FlyingSquirl::dtor()" << endl; }
	void Display() { cout << "FlyingSquirl::Display() ID:"<<GetID()<<endl;}
private:
};


class LegendaryAnimal: public Animal
{
public:
  LegendaryAnimal() : m_a1(new Centipede()), m_a2(new Squirl())
  {}
   
   // Add to fix bug (deep copy)
   LegendaryAnimal(const LegendaryAnimal &other) : Animal(other), 
                    m_a1(new Centipede(other.m_c)), m_a2(new Squirl(other.m_sq)),
                    m_c(Centipede(other.m_c)), m_sq(Squirl(other.m_sq)) {}
  
  ~LegendaryAnimal() 
  { 
	delete m_a1; 
	delete m_a2;
  }

  void Display()
  {
	m_a1->Display();
	m_a2->Display();
	m_c.Display();
	m_sq.Display();
  }

private:
  Animal *m_a1;
  Animal *m_a2;
  Centipede m_c;
  Squirl m_sq;
};

void Foo(Animal &a) { a.Display();}
void Foo(){ Animal::PrintCount(); }

Animal Foo(int i)
{
	Centipede ret;
	
	cout<<"Foo(int i)"<<endl;
	ret.Display();
	
	return ret;
}

void FiFi(Squirl s) { s.Display(); }

int main(int argc, char **argv, char **envp)
{
	Centipede c;
	Foo(c);
	Foo(3).Display();


	Animal *array[] = 
	{
		new Centipede(),
		new Squirl(),
		new Centipede()
	};

	for(int i = 0; i < 3; ++i) array[i]->Display();
	for(int i = 0; i < 3; ++i) delete array[i];

	Animal arr2[] =
	{
		Centipede(),
		Squirl(),
		Animal()
	};

	for(int i = 0; i < 3; ++i) 
	{
		arr2[i].Display();
	}
	 
	Animal::PrintCount();
	Centipede c2;
	c2.PrintCount();
	
	Centipede arr3[4];
	Squirl *arr4 = new Squirl[4];
	delete [] arr4;

	std::cout<<maxFunc<int>(1,2)<<std::endl;
   
    
	FlyingSquirl fs;
	FiFi(fs);


	LegendaryAnimal *la1 = new LegendaryAnimal();
	LegendaryAnimal *la2 = new LegendaryAnimal(*la1); //enter default cctor
	la1->Display();
	la2->Display();
	
    delete la1;
    // this crashes. fix the bug! - new CCtor should be added for deep copy
	la2->Display(); 
	delete la2;
    
	return 0;
} 
