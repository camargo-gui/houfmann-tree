#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TAD.h"

Lista *criaCaixa(int simbolo, char palavra[])
{
    Lista *novaCaixa;
    novaCaixa = (Lista *)malloc(sizeof(Lista));
    strcpy(novaCaixa->D.codigo, "0");
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
        strcpy(aux->D.codigo, codigoAtual);
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

void salvarListaBinario(Lista *L, char *nomeArq)
{
    FILE *arquivo = fopen(nomeArq, "wb");

    if (arquivo != NULL)
    {

        Lista *aux = L;

        while (aux != NULL)
        {
            fwrite(&(aux->D), sizeof(Dados), 1, arquivo);
            aux = aux->prox;
        }
    }

    fclose(arquivo);
}

void procuraCodigo(Lista *L, char palavra[], char codigo[])
{
    while (L != NULL && strcmp(L->D.palavra, palavra) != 0)
    {
        L = L->prox;
    }
    if (L != NULL)
    {
        strcpy(codigo, L->D.codigo);
    }
    else
    {
        codigo[0] = '\0';
    }
}

void inserePalavraNoArquivo(Lista *L, char string[], char codigo[], FILE *novoArquivo)
{
    procuraCodigo(L, string, codigo);
    fprintf(novoArquivo, "%s", codigo);
    string[0] = ' ';
    string[1] = '\0';
    procuraCodigo(L, string, codigo);
    fprintf(novoArquivo, "%s", codigo);
    limparString(string);
    limparString(codigo);
}

void salvarCodigoCompactado(Lista *L, FILE *ptr)
{

    char caractere[2];
    char string[50];
    char codigo[30];
    caractere[1] = '\0';

    FILE *novoArquivo = fopen("textoCodificado.txt", "w");

    fseek(ptr, 0, SEEK_SET);

    if (ptr != NULL)
    {
        caractere[0] = fgetc(ptr);
        while (!feof(ptr))
        {
            if (caractere[0] != ' ')
            {
                strcat(string, caractere);
            }
            if (caractere[0] == ' ')
            {
                inserePalavraNoArquivo(L, string, codigo, novoArquivo);
            }
            caractere[0] = fgetc(ptr);
            while (caractere[0] == ',' || caractere[0] == '.')
            {
                caractere[0] = fgetc(ptr);
            }
            if (feof(ptr))
            {
                inserePalavraNoArquivo(L, string, codigo, novoArquivo);
            }
        }
    }
}

void inserePalavraNaLista(Lista **L, char string[])
{
    inserePalavra(string, &(*L));
    string[0] = ' ';
    string[1] = '\0';
    inserePalavra(string, &(*L));
    limparString(string);
}

void arquivoParaLista(FILE *arquivo, Lista **L)
{

    char caractere[2];
    char string[50];

    string[0] = '\0';
    caractere[1] = '\0';

    if (arquivo != NULL)
    {
        while (!feof(arquivo))
        {

            if (caractere[0] != ' ')
            {
                strcat(string, caractere);
            }
            if (caractere[0] == ' ')
            {
                inserePalavraNaLista(&(*L), string);
            }
            caractere[0] = fgetc(arquivo);
            while (caractere[0] == ',' || caractere[0] == '.')
            {
                caractere[0] = fgetc(arquivo);
            }
            if (feof(arquivo))
            {
                inserePalavraNaLista(&(*L), string);
            }
        }
    }
}

void lerArquivo()
{
    FILE *arquivo = fopen("texto.txt", "r");

    Lista *L;
    L = NULL;

    arquivoParaLista(arquivo, &L);

    Tree *arvore;
    arvore = NULL;
    arvore = CriaArvore(L);
    char codigos[30];
    strcpy(codigos, "0");
    criaCodigos(arvore, codigos, 0, &L);
    salvarListaBinario(L, "binario.dat");
    salvarCodigoCompactado(L, arquivo);
    fclose(arquivo);
}

int main()
{
    lerArquivo();
}