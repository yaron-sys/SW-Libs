#include <stdio.h>   /* printf */
#include <stdlib.h>  /* malloc , free */

#define TYPE int
#include "max_func.h"
#undef TYPE

/*******************************************************************************
                                Declerations
*******************************************************************************/
typedef struct Animal Animal; 
typedef struct Centipede Centipede; 
typedef struct Squirl Squirl; 
typedef struct FlyingSquirl FlyingSquirl; 
typedef struct Legend Legend; 

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

/* FlyingSquirl struct */
void FlyingSquirl_Ctor(FlyingSquirl *fly_squirl);
void FlyingSquirl_Cctor(FlyingSquirl* dest, const FlyingSquirl* src);
void FlyingSquirl_Dtor(FlyingSquirl *fly_squirl);
void FlyingSquirl_Display(FlyingSquirl *fly_squirl);

/* Legendary Animal struct */
void Legend_Ctor(Legend *legend);
void Legend_Cctor(Legend* dest, const Legend* src);
void Legend_Dtor(Legend *legend);
void Legend_Display(Legend *legend);
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
    Animal_Cctor(&dest->animal, &src->animal);
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
    Animal_Cctor(&dest->animal, &src->animal);
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
                                Flying Squirl
*******************************************************************************/
typedef struct FlyingSquirl_vtbl_t
{
    void (*FlyingSquirl_Dtor_ptr)(FlyingSquirl *fly_squirl);
    void (*FlyingSquirl_Display_ptr)(FlyingSquirl *fly_squirl);    
} FlyingSquirl_vtable_t;

const static FlyingSquirl_vtable_t FlyingSquirl_vbtl = {&FlyingSquirl_Dtor,&FlyingSquirl_Display};

struct FlyingSquirl
{
    Squirl squirl;
};

void FlyingSquirl_Ctor(FlyingSquirl *fly_squirl)
{
	Squirl_Ctor(&fly_squirl->squirl);
    fly_squirl->squirl.animal.vtable = (Animal_vtable_t*)&FlyingSquirl_vbtl;
    printf("FlyingSquirl::Ctor()\n");
}

void FlyingSquirl_Cctor(FlyingSquirl* dest, const FlyingSquirl* src) 
{
    Squirl_Cctor(&dest->squirl, &src->squirl);
    dest->squirl.animal.vtable = (Animal_vtable_t*)&FlyingSquirl_vbtl;
    printf("FlyingSquirl::CCtor()\n");
}

void FlyingSquirl_Dtor(FlyingSquirl *fly_squirl)
{
    printf("FlyingSquirl::dtor()\n");
    fly_squirl->squirl.animal.vtable = (Animal_vtable_t*)&Squirl_vbtl;
    fly_squirl->squirl.animal.vtable->Dtor_ptr((Animal*)&fly_squirl->squirl);
}

void FlyingSquirl_Display(FlyingSquirl *fly_squirl)
{
    printf("FlyingSquirl::Display() ID: %d\n", Animal_GetID(&fly_squirl->squirl.animal));
}
/*******************************************************************************
                                Legendary Animal
*******************************************************************************/
typedef struct Legend_vtbl_t
{
    void (*Legend_Dtor_ptr)(Legend *legend);
    void (*Legend_Display_ptr)(Legend *legend);    
} Legend_vtbl_t;

const static Legend_vtbl_t Legend_vbtl = {&Legend_Dtor,&Legend_Display};

struct Legend
{
    Animal animal;
    Animal *m_a1;
    Animal *m_a2;
    Centipede m_c;
    Squirl m_sq;
};
            
void Legend_Ctor(Legend *legend)
{
	Animal_Ctor(&legend->animal);
    legend->animal.vtable = (Animal_vtable_t*)&Legend_vbtl;
    
    legend->m_a1 = (Animal*)malloc(sizeof(Centipede));
    Centipede_Ctor((Centipede*)legend->m_a1);
    
    legend->m_a2 = (Animal*)malloc(sizeof(Squirl));
    Squirl_Ctor((Squirl*)legend->m_a2);
    
    Centipede_Ctor(&legend->m_c);
    Squirl_Ctor(&legend->m_sq);
}

void Legend_Cctor(Legend* dest, const Legend* src)
{
    Animal_Cctor(&dest->animal, &src->animal); 
    dest->animal.vtable = (Animal_vtable_t*)&Legend_vbtl;
    
    dest->m_a1 = (Animal*)malloc(sizeof(Centipede));
    Centipede_Cctor((Centipede*)dest->m_a1, (Centipede*)src->m_a1);
    
    dest->m_a2 = (Animal*)malloc(sizeof(Squirl));
    Squirl_Cctor((Squirl*)dest->m_a2, (Squirl*)src->m_a2);
    
    Centipede_Cctor(&dest->m_c, &src->m_c);
    Squirl_Cctor(&dest->m_sq, &src->m_sq);
}

void Legend_Dtor(Legend *legend)
{
    legend->m_a1->vtable->Dtor_ptr(legend->m_a1);
    free(legend->m_a1);
    
    legend->m_a2->vtable->Dtor_ptr(legend->m_a2);
    free(legend->m_a2);

    Squirl_Dtor(&legend->m_sq);
    Centipede_Dtor(&legend->m_c);
    
    legend->animal.vtable = &Animal_vbtl;
    legend->animal.vtable->Dtor_ptr(&legend->animal);
}

void Legend_Display(Legend *legend)
{    
    legend->m_a1->vtable->Display_ptr(legend->m_a1);
    legend->m_a2->vtable->Display_ptr(legend->m_a2);
    legend->m_c.animal.vtable->Display_ptr((Animal*)&legend->m_c);
    legend->m_sq.animal.vtable->Display_ptr((Animal*)&legend->m_sq);
}

/*******************************************************************************
                                TEST / FUNCTIONS
*******************************************************************************/
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
    /* initialize fields */
    int i;
    Centipede c;
    Animal *array[3];
    Animal arr2[3];
    Centipede arr3[4];
    Squirl *arr4;
    Animal ret;
    Centipede c2;
    Centipede c_tmp;
    Squirl s_tmp;
    FlyingSquirl fs;
    Squirl cpy_squirl;
    Legend *la1;
    Legend *la2;
    
    Centipede_Ctor(&c);
	Foo_Animal_Display((Animal *)&c);
    
    ret = Foo_ret(3);
    ret.vtable->Display_ptr((Animal*)&ret);
    Animal_Dtor(&ret);
    
    array[0] = (Animal*)malloc(sizeof(Centipede));
    array[1] = (Animal*)malloc(sizeof(Squirl));
    array[2] = (Animal*)malloc(sizeof(Centipede));
    
    Centipede_Ctor((Centipede*) array[0]);
    Squirl_Ctor   ((Squirl   *) array[1]);
    Centipede_Ctor((Centipede*) array[2]);
    
    for(i = 0; i < 3; ++i)
    {
        array[i]->vtable->Display_ptr((Animal*)array[i]);
    }
    
    /* free array of pointers to Animal */
    for(i = 0; i < 3; ++i)
    {
        array[i]->vtable->Dtor_ptr((Animal*)array[i]);
        free(array[i]);
    }
    
    Centipede_Ctor(&c_tmp);
    Animal_Cctor(&arr2[0],(Animal*)&c_tmp);
    Squirl_Ctor(&s_tmp);
    Animal_Cctor(&arr2[1],(Animal*)&s_tmp);
    Animal_Ctor(&arr2[2]);

    Squirl_Dtor(&s_tmp);
    Centipede_Dtor(&c_tmp);

	for(i = 0; i < 3; ++i) 
	{
		arr2[i].vtable->Display_ptr(&arr2[i]);
	}

	Animal_PrintCount();
    Centipede_Ctor(&c2);
	Animal_PrintCount();

	for(i = 0; i < 4; ++i) 
	{
		Centipede_Ctor(&arr3[i]);
	}

    arr4 = (Squirl*)malloc(4 * sizeof(Squirl));
	for(i = 0; i < 4; ++i) 
	{
		Squirl_Ctor(&arr4[i]);
	}
    for(i = 3; i >= 0; --i) 
	{
        Squirl_Dtor(&arr4[i]);
	}
    free(arr4);
    
    printf("%d\n", maxFunc(1,2));
    
    FlyingSquirl_Ctor(&fs);
    Squirl_Cctor(&cpy_squirl, (Squirl*)&fs);
	FiFi(cpy_squirl);
    Squirl_Dtor(&cpy_squirl);
    
    la1 = (Legend*)malloc(sizeof(Legend));
    Legend_Ctor(la1);
    
    la2 = (Legend*)malloc(sizeof(Legend));
    Legend_Cctor(la2, la1);
    
    la1->animal.vtable->Display_ptr((Animal*)la1); /* Legend_Display(la1); */
    la2->animal.vtable->Display_ptr((Animal*)la2); /* Legend_Display(la2); */
    
    la1->animal.vtable->Dtor_ptr((Animal*)la1);
    free(la1);
    la2->animal.vtable->Display_ptr((Animal*)la2);
    la2->animal.vtable->Dtor_ptr((Animal*)la2);
    free(la2);
    
    /* clean ups before exiting main scope - calling dtor's' */
    FlyingSquirl_Dtor(&fs);
    
    for(i = 3; i >= 0; --i) 
	{
        Centipede_Dtor(&arr3[i]);
	}
    
    Centipede_Dtor(&c2);
    
    Animal_Dtor(&arr2[2]);
    Animal_Dtor(&arr2[1]);
    Animal_Dtor(&arr2[0]);
    
    Centipede_Dtor(&c);
    
    return (0);
}
