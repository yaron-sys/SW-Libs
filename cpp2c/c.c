#include <stdio.h>

typedef struct Animal Animal; 
typedef struct Centipede Centipede; 
typedef struct Squirl Squirl; 

/* animal global count */
static int s_count = 0;

/* animal struct */
void Animal_Ctor(Animal *animal);
void Animal_Cctor(Animal* dest, const Animal* src);
void Animal_Dtor(Animal *animal);
void Animal_PrintCount();
void Animal_Display(Animal *animal);
int  Animal_GetID(Animal *animal);

/* Centipede struct */
void Centipede_Dtor(Centipede *centipede);
void Centipede_Display(Centipede *centipede);
void BuyShues(Centipede *centipede);

/* Squirl struct */
void Squirl_Ctor(Squirl *squirl);
void Squirl_Cctor(Squirl* dest, const Squirl* src);
void Squirl_Dtor(Squirl *squirl);
void Squirl_Display(Squirl *squirl);

/*******************************************************************************
                                    Animal
*******************************************************************************/
typedef struct Animal_vtbl_t
{
    void (*Dtor_ptr)(Animal *animal);
    void (*Display_ptr)(Animal *animal);
    
}Animal_vtable_t;

const static Animal_vtable_t Animal_vbtl = {&Animal_Dtor, &Animal_Display};

struct Animal
{
	const Animal_vtable_t *vtable;
    
    int m_id;
};

void Animal_Ctor(Animal *animal)
{
    animal->vtable = &Animal_vbtl;    
    animal->m_id = ++s_count;
    printf("Animal::Ctor() %d\n", animal->m_id);
}

void Animal_Cctor(Animal* dest, const Animal* src) 
{
    dest->vtable = &Animal_vbtl;
    dest->m_id = ++s_count;
    printf("Animal::CCtor() %d\n", dest->m_id);
}

void Animal_Dtor(Animal *animal)
{
    --s_count;
    printf("Animal::Dtor() %d\n", animal->m_id);
}

void Animal_Display(Animal *animal)
{
    printf("Animal::Display() : %d\n", animal->m_id);
}

void Animal_PrintCount(void)
{
    printf("s_count: %d\n", s_count);
}

int Animal_GetID(Animal *animal)
{ 
    return animal->m_id;
}

/*******************************************************************************
                                Centipede
*******************************************************************************/
typedef struct Centipede_vtbl_t
{
    void (*Centipede_Dtor_ptr)(Centipede *centipede);
    void (*Centipede_Display_ptr)(Centipede *centipede);
    void (*Centipede_BuyShues_ptr)(Centipede *centipede);
    
} Centipede_vtable_t;

const static Centipede_vtable_t Centipede_vbtl = {&Centipede_Dtor,&Centipede_Display,&BuyShues};

struct Centipede
{
    Animal animal;
    int m_numLegs;
};

void Centipede_Ctor(Centipede *centipede)
{
	Animal_Ctor(&centipede->animal);
    centipede->animal.vtable = (Animal_vtable_t*)&Centipede_vbtl;
    centipede->m_numLegs = 46;
    printf("Centipede::Ctor()\n");
}

void Centipede_Cctor(Centipede* dest, const Centipede* src) 
{
    Animal_Ctor(&dest->animal);
    dest->animal.vtable = (Animal_vtable_t*)&Centipede_vbtl;
    dest->m_numLegs = src->m_numLegs;
    printf("Centipede::CCtor()\n");
}

void Centipede_Dtor(Centipede *centipede)
{
    printf("Centipede::dtor()\n");
    
    centipede->animal.vtable = &Animal_vbtl;
    centipede->animal.vtable->Dtor_ptr(&centipede->animal);
}

void Centipede_Display(Centipede *centipede)
{
    printf("Centipede::Display() ID: %d num legs: %d\n", centipede->animal.m_id, 
            centipede->m_numLegs);
}

void BuyShues(Centipede *centipede)
{ 
    printf("Centipede::BuyShues() ID: %d\n", Animal_GetID(&centipede->animal));
}
/*******************************************************************************
                                Squirl
*******************************************************************************/
typedef struct Squirl_vtbl_t
{
    void (*Squirl_Dtor_ptr)(Squirl *squirl);
    void (*Squirl_Display_ptr)(Squirl *squirl);    
} Squirl_vtable_t;

const static Squirl_vtable_t Squirl_vbtl = {&Squirl_Dtor,&Squirl_Display};

struct Squirl
{
    Animal animal;
};

void Squirl_Ctor(Squirl *squirl)
{
	Animal_Ctor(&squirl->animal);
    squirl->animal.vtable = (Animal_vtable_t*)&Squirl_vbtl;
    printf("Squirl::Ctor()\n");
}

void Squirl_Cctor(Squirl* dest, const Squirl* src) 
{
    Animal_Ctor(&dest->animal);
    dest->animal.vtable = (Animal_vtable_t*)&Squirl_vbtl;
    printf("Squirl::CCtor()\n");
}

void Squirl_Dtor(Squirl *squirl)
{
    printf("Squirl::dtor()\n");
    squirl->animal.vtable = &Animal_vbtl;
    squirl->animal.vtable->Dtor_ptr(&squirl->animal);
}

void Squirl_Display(Squirl *squirl)
{
    printf("Squirl::Display() ID: %d\n", Animal_GetID(&squirl->animal));
}

/*******************************************************************************
                                TEST / FUNCTIONS
*******************************************************************************/
/* sent parameter should be casted to ANIMAL first(in main) */
void Foo_Animal_Display(Animal *a)
{
    a->vtable->Display_ptr(a);
}

void Foo_Animal_Print()
{ 
    Animal_PrintCount();
}

Animal Foo_ret(int i)
{
	Centipede ret;
    Animal real_ret;
	Centipede_Ctor(&ret);
    
	printf("Foo(int i)\n");
    ret.animal.vtable->Display_ptr((Animal*)&ret);
	
    Animal_Cctor(&real_ret, (Animal*)&ret);
	Centipede_Dtor(&ret);
    
    return real_ret;
}

void FiFi(Squirl s)
{
    s.animal.vtable->Display_ptr((Animal*)&s);
}
/******************************************************************************
                                 MAIN
******************************************************************************/
int main()
{
    Animal ret;
    
    /*Centipede c;
    Centipede_Ctor(&c);
	Foo_Animal_Display((Animal *)&c);*/
    
    /*Animal_Ctor(&ret);*/
    ret = Foo_ret(3);
    ret.vtable->Display_ptr((Animal*)&ret);
    Animal_Dtor(&ret);
    
    /*Animal lion1;
    Centipede cent;
    Centipede cent_cpy;
    
    Centipede_Ctor(&cent);
    cent.animal.vtable->Display_ptr((Animal*)&cent);
    ((Centipede_vtable_t *)cent.animal.vtable)->Centipede_BuyShues_ptr(&cent);
    
    Centipede_Cctor(&cent_cpy, &cent);
    cent_cpy.animal.vtable->Display_ptr((Animal*)&cent_cpy);*/
    
    
    /*Animal_Ctor(&lion1);
    Centipede_Ctor(&cent1);*/
    
    /* Centipede_Ctor((Centipede*) &lion1);
    lion1.vtable->Animal_Display_ptr(&lion1); */
    
    /* Animal lion1;
    Animal lion2;
    Animal lion3; */
    /*Animal copy_lion;*/
    
    /*Animal_Ctor(&lion1);
    Animal_Ctor(&lion2);
    Animal_Ctor(&lion3); */
    
/*    lion1.Display_ptr(&lion1);
    lion1.PrintCount_ptr();

    printf("lion2 ID: %d\n", lion2.GetID_ptr(&lion2));
    
    Animal_vtable.x = 6;
    printf("%d: ", Animal_vtable.x);
    
    Animal_Cctor(&copy_lion, &lion2);
    printf("copy_lion ID: %d\n", copy_lion.GetID_ptr(&lion2));
    copy_lion.PrintCount_ptr(); */
    return (0);
}


