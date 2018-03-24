#include "AVL.h"

link NEW(Item item, link l, link r) //cria um novo no na arvore
{
	link x = (link)malloc(sizeof(struct STnode));
	x->item = item;
	x->l = l;
	x->r = r;
	x->height=1;
	return x;
}

void STinit(link*head) //inicia a raiz da arvore
{
	*head = NULL;
}

int height(link h) //devolve a altura da arvore
{
	if (h == NULL) 
		return 0;
	return h->height;
}

void update_height(link h) //atualiza a altura dos nos
{
	int height_left = height(h->l);
	int height_right = height(h->r);
	h->height = height_left > height_right ? height_left + 1 : height_right + 1;
}

link rotL(link h) //funcao responsavel por fazer a rotacao para a esquerda
{
	link x = h->r;
	h->r = x->l;
	x->l = h;
	update_height(h); //vai atualizando a altura dos nos envolvidos
	update_height(x);
	return x;
}

link rotR(link h) //rotacao para a direita
{
	link x = h->l;
	h->l = x->r;
	x->r = h;
	update_height(h);
	update_height(x);
	return x;
}

link rotLR(link h) //rotacao dupla esquerda-direita
{
	if (h==NULL) return h;
	h->l=rotL(h->l);
	return rotR(h);
}

link rotRL(link h) //rotacao dupla direita-esquerda
{
	if (h==NULL) return h;
	h->r=rotR(h->r);
	return rotL(h);
}

int Balance(link h) { //balance-factor da arvore
	if(h == NULL) return 0;
	return height(h->l)-height(h->r);
}

link max(link h) { //funcao que percorre a arvore pela direita ate encontrar o ultimo no desta sub-arvore
	if (h==NULL || h->r==NULL) return h;
	else return max(h->r);
}

link AVLbalance(link h) { //funcao responsavel por manter a arvore equilibraada
	int balanceFactor;
	if (h==NULL) return h;
	balanceFactor= Balance(h);
	if(balanceFactor>1) {
		if (Balance(h->l)>=0) h=rotR(h);
		else
			h=rotLR(h);
	}
	else if(balanceFactor<-1){
		if (Balance(h->r)<=0) h = rotL(h);
		else
			h = rotRL(h);
	}
	else
		update_height(h);
	return h;
}

link insertR(link h, Item item) //insere um elemento ordenadamente
{
	if (h == NULL)
	return NEW(item, NULL, NULL);
	if (less(key(item), key(h->item)))
		h->l = insertR(h->l, item);
	else
		h->r = insertR(h->r, item);
	h=AVLbalance(h);
	return h;
}

void STinsert(link*head, Item item)
{
	*head = insertR(*head, item);
}


Item search(link h, key v) //procura um elemento ordenadamente pelas chaves
{
	if (h == NULL)
		return NULLitem;
	if (eq(v, key(h->item)))
		return h->item;
	if (less(v, key(h->item)))
		return search(h->l, v);
	else
		return search(h->r, v);
}

Item STsearch(link head, key v)
{
	return search(head, v);
}

void sort(link h, void (*visit)(Item)) //imprime todos os elementos por ordem
{
	if (h == NULL)
		return;
	sort(h->l, visit);
	visit(h->item);
	sort(h->r, visit);
}


void STsort(link head, void (*visit)(Item))
{
	sort(head, visit);
}


link deleteR(link h, key k, int *a) { //elimina um elemento
	if (h==NULL) return h;
	else if (less(k, key(h->item))) h->l=deleteR(h->l,k,a);
	else if (less(key(h->item), k)) h->r=deleteR(h->r,k,a) ;
	else{
		if (h->l !=NULL && h->r !=NULL){ /*caso  3*/
			link aux=max(h->l);
			{Item x; x=h->item; h->item=aux->item; aux->item=x;}
			h->l= deleteR(h->l, key(aux->item),a);
		}
		else {
			link aux=h;
			if (h->l == NULL && h->r == NULL) h=NULL; /*caso 1 e 2*/
			else if (h->l==NULL) h=h->r;
			else h=h->l;
			freeItem(aux->item);
			free(aux);
			*a = 1; //devolve 1 se o elemento dado foi eliminado
		}
	}
	h=AVLbalance(h);
	return h;
}

int STdelete(link*head, key k){
	/* funcao que devolve 1 se o elemento de uma dada chave foi eliminado, e 0 caso contrario */
	int a = 0;
	*head = deleteR(*head, k, &a); //elimina o elemento da arvore
	return a;

}

link freeR(link h) //liberta todos os elementos da arvore
{
	int a = 0;
	if (h==NULL)
		return h;
	h->l=freeR(h->l);
	h->r=freeR(h->r);
	return deleteR(h,key(h->item),&a);
}


void STfree(link*head)
{
	*head=freeR(*head);
}

Item STprocura_max(link h, Item* max) //procura o item max de toda a arvore
{
	if (h == NULL)
		return *max;
	STprocura_max(h->l, max);
	*max = compare_max(h->item, *max); //vai comparando os sucessivos elementos
	return STprocura_max(h->r, max);
}