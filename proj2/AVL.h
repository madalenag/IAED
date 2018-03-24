#ifndef _AVL_
#define _AVL_

#include <stdlib.h>
#include <stdio.h>
#include "item.h"

typedef struct STnode* link;
struct STnode { Item item; link l, r; int height;};

void STinit(link*); //funcao que inicializa a arvore
Item STsearch(link,key); //procura por um elemento atraves de uma chave
void STinsert(link*,Item); //insere um elemento na arvore
int STdelete(link*,key); //elimina um elemento
void STsort(link,void (*visit)(Item)); //imprime a arvore atraves de uma determinada funcao (visit)
void STfree(link*); //liberta a arvore
Item STprocura_max(link h, Item* max); //procura o elemento com uma certa caraterista maxima
#endif

