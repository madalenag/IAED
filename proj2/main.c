#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

static link armazem; //variavel onde vao estar os produtos
static Item max; //variavel que corresponde ao item com maior stock
static int count; //contador de produtos

void adiciona();
void lista();
void maximo();
void elimina();
void sai();

int main()
{
	max = NULL; //max inicialmente e nulo
	STinit(&armazem);
	char command;

	while (1) {
		switch (command = getchar()) {
			case 'a':
				adiciona();
				getchar();
				break;
			case 'l':
				lista();
				getchar();
				break;
			case 'm':
				maximo();
				getchar();
				break;
			case 'r':
				elimina();
				getchar();
				break;
			case 'x':
				sai();
				return 0;
			default:
				printf("O comando nao e valido\n");
				getchar();
		}
	}
	return -1;
}


void adiciona() {
	Item produto;
	int quant;
	unsigned long chave;

	scanf("%lx", &chave);
	getchar();
	scanf("%d", &quant);
	produto = STsearch(armazem, chave); //procura o produto no armazem

	if (produto == NULLitem) { /* se o produto nao existir */
		if (quant > 0) /*se a quantidade a inserir for maior do que 0 */
			produto = newItem(chave,quant); //adiciona a nova quantidade
		else 
			produto = newItem(chave,0); // caso contrario a quantidade fica a 0
		STinsert(&armazem, produto); //insere o produto no armazem
		max = compare_max(produto, max); //ve se o novo produto e o novo max
		count ++; //incrementa o numero de produtos
	}
	else { /*caso o produto ja exista no armazem*/
		if (quant >= 0) {
			produto->num += quant; //adiciona a quantidade
			max = compare_max(produto, max); //ve se o max nao foi alterado
		}
		else {
			int aux = produto->num;
			if ((aux += quant) > 0) //verifica se a quant do produto e pelo menos 0
				produto->num += quant; //se for, tira a quantidade
			else
				produto->num = 0; //caso contrario, quant = 0
			if (produto == max) /*se estivermos a retirar quant do max */
				STprocura_max(armazem, &max); //procura o max da arvore
		}
	}
}
	

void lista() //devolve os produtos do armazem listados alfabeticamente pelas suas chaves
{
	STsort(armazem,showItem);
}


void elimina() //elimina o produto correspondente a uma dada chave
{
	unsigned long chave;
	scanf("%lx", &chave);

	if (max != NULL && chave == max->chave) { /* caso se remova o max*/
		STdelete(&armazem, chave);
		max = NULL;
		STprocura_max(armazem, &max); //procura-se o novo max
		count --; // decrementa-se o numero de produtos
	}
	else if (STdelete(&armazem, chave)) /*se um produto do armazem for removido*/
			count--;//decrementa o numero de produtos
}


void maximo() //devolve o produto com quantidade maxima no armazem
{
	if (max != NULL) /*se existir max*/
		showItem(max); //imprime-o
}



void sai() //sai do armazem e devolve o numero de elementos
{
	printf("%d\n",count);
	STfree(&armazem); //liberta todos os produtos do armazem
}