#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct dados
{
    char palavra[30];
    int frequencia;
    int codigo;
    int simbolo;
};

typedef struct dados Dados;

struct lista
{
    Dados D;
    struct lista *prox;
};

typedef struct lista Lista;

struct tree
{
    int frequencia;
    int simbolo;
    struct tree *dir, *esq;
};

typedef struct tree Tree;

struct florestaDeArvores
{
    Tree *arbusto;
    struct florestaDeArvores *prox;
};

typedef struct florestaDeArvores Floresta;

struct pilha
{
    Tree *T;
    struct pilha *prox;
};

typedef struct pilha Pilha;

void init(Pilha **P)
{
    *P = NULL;
}

// void push(Pilha **P, Tree *elem)
// {
//     Pilha *novaCaixa, *aux;
//     novaCaixa->T = elem;
//     novaCaixa->prox = *P;
//     *P = novaCaixa;
// }

// void pop(Pilha **P, Tree **elem)
// {
//     Pilha *aux;
//     *elem = (*P)->T;
//     *P = (*P)->prox;
// }

char isEmpty(Pilha *P)
{
    return P == NULL;
}

Lista *criaCaixa(int simbolo, char palavra[])
{
    Lista *novaCaixa;
    novaCaixa = (Lista *)malloc(sizeof(Lista));
    novaCaixa->D.codigo = 0;
    novaCaixa->D.frequencia = 1;
    novaCaixa->D.simbolo = simbolo;
    strcpy(novaCaixa->D.palavra, palavra);
    return novaCaixa;
}
void inserePalavra(char palavra[], Lista **L)
{
    Lista *ant, *aux;
    aux = *L;
    if (*L == NULL)
    {
        *L = criaCaixa(1, palavra);
    }
    else
    {
        while (aux != NULL && strcmp(aux->D.palavra, palavra) != 0)
        {
            ant = aux;
            aux = ant->prox;
        }
        if (aux != NULL)
        {
            aux->D.frequencia += 1;
        }
        else
        {
            ant->prox = criaCaixa(ant->D.simbolo + 1, palavra);
        }
    }
}

Floresta *CriaFloresta(int frequencia, int simbolo)
{
    Floresta *novaCaixa;
    novaCaixa = (Floresta *)malloc(sizeof(Floresta));
    novaCaixa->arbusto = (Tree *)malloc(sizeof(Tree));
    novaCaixa->arbusto->simbolo = simbolo;
    novaCaixa->arbusto->frequencia = frequencia;
    novaCaixa->prox = NULL;
    return novaCaixa;
}

void InsereNaFloresta(int frequencia, int simbolo, Floresta **flr)
{
    if (*flr == NULL || (*flr)->arbusto->frequencia >= frequencia)
    {
        Floresta *novaFloresta = CriaFloresta(frequencia, simbolo);
        novaFloresta->prox = *flr;
        *flr = novaFloresta;
    }
    else
    {
        Floresta *aux = *flr;
        Floresta *ant = NULL;

        while (aux != NULL && aux->arbusto->frequencia < frequencia)
        {
            ant = aux;
            aux = aux->prox;
        }

        Floresta *novaFloresta = CriaFloresta(frequencia, simbolo);
        novaFloresta->prox = ant->prox;
        ant->prox = novaFloresta;
    }
}

Tree *CriaArvore(Lista *L)
{
    if (L != NULL)
    {
        Floresta *tr, *prElemento, *segElemento;
        tr = NULL;
        Lista *aux;
        aux = L;
        while (aux != NULL)
        {
            InsereNaFloresta(aux->D.frequencia, aux->D.simbolo, &tr);
            aux = aux->prox;
        }
        prElemento = tr;
        segElemento = tr->prox;
        while (segElemento != NULL)
        {
            Floresta *novoGalho;
            novoGalho = CriaFloresta(prElemento->arbusto->frequencia + segElemento->arbusto->frequencia, 0);
            novoGalho->arbusto->esq = prElemento->arbusto;
            novoGalho->arbusto->dir = segElemento->arbusto;
            novoGalho->prox = segElemento->prox;
            tr = novoGalho;
            prElemento = tr;
            segElemento = tr->prox;
        }
        return tr->arbusto;
    }
    else
    {
        Floresta *tr = NULL;
        return tr->arbusto;
    }
}

char Folha(Tree *T)
{
    return T->dir == NULL && T->esq == NULL;
}

void colocaCodigoNaLista(int simbolo, Lista *lista, char codigoAtual[])
{
    Lista *aux;
    aux = lista;
    while (aux != NULL && aux->D.simbolo != simbolo)
    {
        aux = aux->prox;
    }

    if (aux != NULL)
    {
        aux->D.codigo = atoi(codigoAtual);
    }
}

void criaCodigos(Tree *T, char codigoAtual[], int index, Lista **lista)
{

    if (T != NULL)
    {

        if (Folha(T))
        {
            codigoAtual[index] = '\0';
            colocaCodigoNaLista(T->simbolo, *lista, codigoAtual);
        }
        else
        {
            codigoAtual[index] = '0';
            criaCodigos(T->esq, codigoAtual, index + 1, &(*lista));

            codigoAtual[index] = '1';
            criaCodigos(T->dir, codigoAtual, index + 1, &(*lista));
        }
    }
}

void limparString(char *str)
{
    str[0] = '\0';
}

void imprimeArvore(Tree *raiz, int nivel)
{
    if (raiz != NULL)
    {
        imprimeArvore(raiz->dir, nivel + 1);

        for (int i = 0; i < nivel; i++)
            printf("   ");

        printf("(%d,%d)\n", raiz->frequencia, raiz->simbolo);

        imprimeArvore(raiz->esq, nivel + 1);
    }
}
void lerArquivo()
{
    FILE *arquivo = fopen("texto.txt", "r");
    char caractere[2];
    char string[50];
    Lista *L;
    L = NULL;
    string[0] = '\0';

    if (arquivo != NULL)
    {
        while ((caractere[0] = fgetc(arquivo)) != EOF)
        {
            caractere[1] = '\0';
            if (caractere[0] != ' ' && caractere[0] != ',' && caractere[0] != '.')
            {
                strcat(string, caractere);
            }
            if (caractere[0] == ' ')
            {
                inserePalavra(string, &L);
                string[0] = ' ';
                string[1] = '\0';
                inserePalavra(string, &L);
                limparString(string);
            }
        }
        Tree *arvore;
        arvore = NULL;
        arvore = CriaArvore(L);
        imprimeArvore(arvore, 0);
        fclose(arquivo);
    }
}

int main()
{
    lerArquivo();
}