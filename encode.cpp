#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TAD.h"

Floresta *CriaFloresta(int frequencia, int simbolo)
{
    Floresta *novaCaixa;
    novaCaixa = (Floresta *)malloc(sizeof(Floresta));
    novaCaixa->arbusto = (Tree *)malloc(sizeof(Tree));
    novaCaixa->arbusto->simbolo = simbolo;
    novaCaixa->arbusto->frequencia = frequencia;
    novaCaixa->arbusto->dir = NULL;
    novaCaixa->arbusto->esq = NULL;
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

        while (aux != NULL && aux->arbusto->frequencia <= frequencia)
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
	Floresta *tr = NULL, *prElemento, *segElemento;
	Lista *aux;
    if (L != NULL)
    {
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
            tr = segElemento->prox;
            if (tr == NULL || novoGalho->arbusto->frequencia <= tr->arbusto->frequencia)
            {
                novoGalho->prox = tr;
                tr = novoGalho;
            }
            else
            {
                Floresta *aux = tr;
                Floresta *ant = NULL;

                while (aux != NULL && aux->arbusto->frequencia < novoGalho->arbusto->frequencia)
                {
                    ant = aux;
                    aux = aux->prox;
                }
                if (ant != NULL)
                {
                    novoGalho->prox = ant->prox;
                    ant->prox = novoGalho;
                }
            }
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

Lista *criaCaixa(int simbolo, char palavra[]) 
{
    Lista *novaCaixa;
    novaCaixa = (Lista *)malloc(sizeof(Lista));
    strcpy(novaCaixa->D.codigo, "0");
    novaCaixa->D.frequencia = 1;
    novaCaixa->D.simbolo = simbolo;
    novaCaixa->prox = NULL;
    strcpy(novaCaixa->D.palavra, palavra);
    return novaCaixa;
}
void inserePalavra(char palavra[], Lista **L) // Insere a palavra na lista encadeada
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

void colocaCodigoNaLista(int simbolo, Lista *lista, char codigoAtual[]) // Coloca o código na lista encadeada
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

void criaCodigos(Tree *T, char codigoAtual[], int index, Lista **lista) // Cria os códigos da árvore
{

    if (T != NULL)
    {

        if (T -> dir == NULL && T -> esq == NULL)
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
    FILE *arquivo = fopen("texto1.txt", "r");
    FILE *arquivo2 = fopen("texto2.txt", "r");

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
    salvarCodigoCompactado(L, arquivo2);
    fclose(arquivo);
}

int main()
{
    lerArquivo();
    printf("Arquivo codificado com sucesso!\n");
}
