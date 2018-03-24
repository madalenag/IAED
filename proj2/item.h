#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>


typedef struct produto{
	unsigned long chave; int num;
}*Item;

typedef unsigned long key;
#define key(a) (a->chave)
#define eq(a,b) (a==b)
#define less(a,b) (a<b)
#define NULLitem (NULL)

Item newItem (unsigned long chave, int num);
void freeItem(Item a);
void showItem(Item a);
Item compare_max(Item a, Item max);

#endif