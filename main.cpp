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

// Tree * CriaNo(int frequencia, int simbolo){
//     Tree * novaCaixa;
//     novaCaixa = (Tree *)malloc(sizeof(Tree));
//     novaCaixa -> dir = NULL;
//     novaCaixa -> esq = NULL;
//     novaCaixa -> frequencia = frequencia;
//     novaCaixa -> simbolo = simbolo;
//     return novaCaixa;
// }

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

// void insereOrdenado(Lista * * L){
//     if(*L == NULL)
// }

Floresta *CriaFloresta(int frequencia, int simbolo)
{
    Floresta *novaCaixa;
    novaCaixa->arbusto->simbolo = simbolo;
    novaCaixa->arbusto->frequencia = frequencia;
    novaCaixa->prox = NULL;
    return novaCaixa;
}

void InsereNaFloresta(int frequencia, int simbolo, Floresta **flr)
{
    if (*flr == NULL)
    {
        *flr = CriaFloresta(frequencia, simbolo);
    }
    else
    {
        Floresta *aux, *ant;
        aux = *flr;
        while (aux != NULL && aux->arbusto->frequencia < frequencia)
        {
            ant = aux;
            aux = aux->prox;
        }
        Floresta *novaFloresta;
        novaFloresta = CriaFloresta(frequencia, simbolo);
        novaFloresta = ant->prox;
        ant->prox = novaFloresta;
    }
}

Tree *CriaArvore(Lista *L)
{
    if (L == NULL)
    {
        return;
    }
    else
    {
        Floresta *tr, *prElemento, *segElemento;
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
        }
        return tr->arbusto;
    }
}
