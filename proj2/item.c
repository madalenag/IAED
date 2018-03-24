#include <stdio.h>
#include <stdlib.h>
#include "item.h"

void freeItem(Item a) //liberta um item
{
	free(a);
}

void showItem(Item a) //mostra o item composto por uma chave e por uma quantidade(num)
{
	printf("%08lx %d\n",a->chave, a->num);
}

Item newItem (unsigned long chave, int num) //cria um novo item
{
	Item new = (Item)malloc(sizeof(struct produto)); //cria espaco
	new->chave = chave; //insere os parametros do novo item
	new->num = num;
	return new;
}

Item compare_max(Item a, Item max) //compara um item com o item maximo
{
	if (max == NULL) /*se o max nao existir, retorna o outro item */
		return a;
	/* se a quantidade do item a for maior do que a do max, ou tiverem a mesma
	quantidade mas a for menor alfabeticamente do que max */
	if (a->num > max->num || (a->num == max->num && a->chave < max->chave))
		return a; //o item a passa a ser o novo max
	return max;
}