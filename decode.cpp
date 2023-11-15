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
}

void buscaSimboloNaArvore(Lista *L, Tree *T, char nomeArq[])
{

    char caractere;
    Tree *aux;
    FILE *ptr = fopen(nomeArq, "r");
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

int main()
{
    Lista *tabela;
    Tree *arvore;
    tabela = carregarLista("binario.dat");
    arvore = CriaArvore(tabela);
    buscaSimboloNaArvore(tabela, arvore, "textoCodificado.txt");
}
