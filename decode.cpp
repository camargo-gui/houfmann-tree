#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD.h"

Lista *carregarLista(char nomeArq[])
{
    FILE *arquivo = fopen(nomeArq, "rb");
    Lista *cabeca = NULL, *aux, *ant, *novo;

    if (arquivo == NULL)
    {
        printf("Por favor, gere o arquivo binario!\n");
    }
    else
    {

        novo = (Lista *)malloc(sizeof(Lista));
        while (fread(&(novo->D), sizeof(Dados), 1, arquivo) == 1)
        {

            if (cabeca == NULL)
            {
                cabeca = novo;
                novo->prox = NULL;
            }

            else
            {
                aux = cabeca;
                while (aux != NULL)
                {
                    ant = aux;
                    aux = aux->prox;
                }
                ant->prox = novo;
                novo->prox = aux;
            }
            novo = (Lista *)malloc(sizeof(Lista));
        }
    }
    return cabeca;
}

void buscaPalavra(Lista *L, int simbolo)
{
    while (L != NULL && simbolo != L->D.simbolo)
    {
        L = L->prox;
    }
    if (L != NULL)
    {
        printf("%s", L->D.palavra);
    }
    else
    {
        printf("*palavra-nao-encontrada*\n");
    }
}

void buscaSimboloNaArvore(Lista *L, Tree *T, char nomeArq[])
{

    char caractere;
    Tree *aux;
    FILE *ptr = fopen(nomeArq, "r");
    if (ptr != NULL)
    {
        int i = 0;

        aux = T;
        while (!feof(ptr))
        {

            caractere = fgetc(ptr);
            if (caractere == '0')
            {
                aux = aux->esq;
            }
            else if (caractere == '1')
            {
                aux = aux->dir;
            }
            else
            {
                if (!feof(ptr))
                {
                    printf("EXISTEM CARACTERES INVALIDOS!!!!\n");
                }
            }

            if (Folha(aux))
            {
                buscaPalavra(L, aux->simbolo);
                aux = T;
            }
        }
    }
}

void InsereNaArvore(Tree **T, int simbolo, char codigo[])
{
    Tree *aux;
    int i = 0;
    if (*T == NULL)
    {
        *T = (Tree *)malloc(sizeof(Tree));
        (*T)->esq = NULL;
        (*T)->dir = NULL;
        (*T)->simbolo = -1;
    }
    aux = *T;
    while (codigo[i] != '\0')
    {
        if (codigo[i] == '0')
        {
            if (aux->esq == NULL)
            {
                aux->esq = (Tree *)malloc(sizeof(Tree));
                aux->esq->esq = NULL;
                aux->esq->dir = NULL;
                aux->esq->simbolo = -1;
            }
            aux = aux->esq;
        }
        else if (codigo[i] == '1')
        {
            if (aux->dir == NULL)
            {
                aux->dir = (Tree *)malloc(sizeof(Tree));
                aux->dir->esq = NULL;
                aux->dir->dir = NULL;
                aux->dir->simbolo = -1;
            }
            aux = aux->dir;
        }
        i++;
    }
    aux->simbolo = simbolo;
}

Tree *CriaArvoreAPartirDosCodigos(Lista *tabela)
{
    Tree *arvore = NULL;
    Lista *aux = tabela;
    while (aux != NULL)
    {
        InsereNaArvore(&arvore, aux->D.simbolo, aux->D.codigo);
        aux = aux->prox;
    }
    return arvore;
}

int main()
{
    Lista *tabela;
    Tree *arvore;
    tabela = carregarLista("binario.dat");
    arvore = CriaArvoreAPartirDosCodigos(tabela);
    buscaSimboloNaArvore(tabela, arvore, "textoCodificado.txt");
}
