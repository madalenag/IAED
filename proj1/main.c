#include <stdio.h>
#include <string.h>

#define MAXFRASE 140
#define MAXMSG 10000
#define MAXUSER 1000

typedef struct {
    int id, len;
    char frase[MAXFRASE+1];
} Mensagem;

typedef struct {
    int pos;
    int len;
} MaxFrase;


static Mensagem lista[MAXMSG]; //vetor de mensagens
static int count; //contador de mensagens

static MaxFrase max_frases[MAXMSG]; //vetor para as maiores frases
static int count_frase; //numero de frases com dimensao maxima ate ao momento

static int users[MAXUSER]; //histograma para cada user
static int mais_ativo = 1;
static int pos_ord[MAXMSG]; //vetor ordenado com posicoes


int le_linha(char buffer[]);
void atualizaMax();
void adiciona();
void imprime(int i, int n);
void lista_msg();
void lista_user();
void lista_longas();
void escreve_ativo();
int whitechar(char c);
int conta_occ(char s[], char p[]);
int less(Mensagem a, Mensagem b);
void conta_palavras();
void insertion();
void ordena();



int main()
{
    char command;

    while (1) {
        switch (command = getchar()) {
            case 'A':
                adiciona();
                break;
            case 'L':
                lista_msg();
                getchar();
                break;
            case 'U':
                lista_user();
                getchar();
                break;
            case 'O':
                lista_longas();
                getchar();
                break;
            case 'T':
                escreve_ativo();
                getchar();
                break;
            case 'S':
                ordena();
                getchar();
                break;
            case 'C':
                conta_palavras();
                getchar();
                break;
            case 'X':
                printf("%d\n", count);
                return 0;
            default:
                printf("O comando nao é valido\n");
                le_linha(NULL);
        }
    }
    return -1;
}

int le_linha(char buffer[]) //recebe o sitio onde vai guardar a linha e devolve a len dela
{
    int i = 0, c;
    c = getchar(); //le o espaço entre os argumentos

    while ( (c = getchar()) != EOF && c != '\n')
        if (i < MAXFRASE) //nao ultrapassa os 140 caracteres
            buffer[i++] = c;
    buffer[i] = '\0';

    return i;
}


void atualizaMax() 
//compara a len da nova frase com a len maxima previamente existente, atualizando a frase maxima
{
    if (lista[count].len > max_frases[0].len)
    //se a nova frase for maior, passa a ser a posicao 0 do vetor
        count_frase = 0;
    else if (lista[count].len < max_frases[0].len)
    //se for menor nao insere no vetor
        return;

    max_frases[count_frase].len = lista[count].len;
    max_frases[count_frase++].pos = count; //adiciona os dados
}


void adiciona() //le uma mensagem e adiciona-a ao forum de mensagens
{
    int u = -1;
    scanf("%d", &u);

    if (u < MAXUSER && u >= 0) { //verifica o numero de participantes
        lista[count].id = u;
        lista[count].len = le_linha(lista[count].frase);
        users[u]++; //incrementa as ocorrencias do user
        pos_ord[count] = count; //vai construindo o vetor de posicoes
       
        if (users[u] > mais_ativo) //atualiza o user mais ativo
            mais_ativo = users[u];
       
        atualizaMax();
       
        count ++; //incrementa o numero de mensagens
    }
}

void imprime(int i, int n) 
//se n=1 imprime a frase da mensagem correspondente, se n=2 imprime o id e a frase
{
    if (n == 1)
        printf("%s\n", lista[i].frase);
    else if (n == 2)
        printf("%d:%s\n", lista[i].id, lista[i].frase);
}


void lista_msg() //lista as mensagens existentes ate ao momento
{
    int i;
    printf("*TOTAL MESSAGES:%d\n", count);

    for (i = 0; i < count; i++) 
        imprime(i,2);
}

void lista_user() 
//dado um user, devolve todas as frases inseridas por esse user, pela ordem pelo qual foram inseridas
{
    int user, i;
    scanf("%d", &user);
   
    printf("*MESSAGES FROM USER:%d\n", user);

    for (i = 0; i < count; i++)
        if (lista[i].id == user)
            imprime(i,1);
}


void lista_longas() //lista as frases mais longas ate ao momento, pela ordem que foram inseridas
{
    int i;

    for (i = 0; i < count_frase; i++){
            printf("*LONGEST SENTENCE:");
            imprime(max_frases[i].pos,2);
    }
}


void escreve_ativo() 
/*imprime o user mais ativo ate ao momento, e quantas mensagens enviou.
Caso haja mais do que um user, lista-os por ordem crescente do seu id.*/
{
    int i;

    for (i = 0; i < MAXUSER; i++)
        if (users[i] == mais_ativo)
            printf("*MOST ACTIVE USER:%d:%d\n", i, users[i]);
}


int whitechar(char c) //recebe um caracter e devolve 1 caso ele seja branco e 0 caso contrario
{
    if (c == ';'|| c == ',' || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == '\0')
        return 1;
    return 0;

}

int conta_occ(char s[], char p[]) 
//recebe uma string e uma palavra, e devolve quantas vezes essa palavra apareceu limitada por caracteres brancos 
{
    int occ = 0, i, j;
    enum {FORA, DENTRO};
    int estado = FORA; //inicialmente estado = FORA
    
    for (i = 0, j = 0; s[i+j] != '\0'; j++) {
        if ( s[i+j] != p[j]) {
            estado = (whitechar(s[i+j])) ? FORA : DENTRO;
            /* estado = FORA quando encontramos um white char, caso
            contrario encontramos caracteres que nao pertecem a palavra que estamos a procurar, e estado = DENTRO*/
            j = -1; //para na proxima iteracao podermos voltar a comparar o inicio da palavra
            i++;
        }
        else if (p[j+1] == '\0' && whitechar(s[i+j+1]) && estado == FORA) {
                /* so incrementa occ quando esta limitada por whites (estado = FORA) e e 
                igual a palavra recebida (a palavra recebida foi percorrida na sua totalidade) */
                occ++;
                i += j;/*foi encontrada uma palavra, logo o i que percorre a string pode incrementar a len da palavra*/
                j = -1;
        }
    }
    return occ;
}

void conta_palavras() //devolve o numero de vezes que uma dada palavra ocorre em todas as frases do forum
{
    char p[MAXFRASE+1];
    scanf("%s", p);
    int i, num_occ = 0;

    for (i = 0; i < count; i++) //vai incrementando as ocorrencias
        num_occ += conta_occ(lista[i].frase, p);
    printf("*WORD %s:%d\n",p, num_occ);
}


int less(Mensagem a, Mensagem b) 
/*devolve 1 caso a frase da mensagem a seja alfabeticamente menor do que a b 
ou que a identificacao da a seja menor que a da b */
{
	int valor;
	valor = strcmp(a.frase, b.frase);

	if (valor < 0 || (valor == 0 && a.id < b.id))
		return 1;
	return 0;
}

void insertion()
//foi utilizado o algoritmo de ordenacao insertion sort para ordenar as mensagens do forum
{
    int i, j, aux;

    for (i = 1; i < count; i++)
    {
        aux = pos_ord[i];
        j = i-1;
        while (j >= 0 && less(lista[aux],lista[pos_ord[j]])) {
            pos_ord[j+1] = pos_ord[j];
            j--;
        }
        pos_ord[j+1] = aux;
    }
}


void ordena()//devolve as mensagens existentes no forum por ordem alfabetica
{
    int i;
    insertion();
    printf("*SORTED MESSAGES:%d\n", count);

    for (i = 0; i < count; i++)
        imprime(pos_ord[i],2);
}