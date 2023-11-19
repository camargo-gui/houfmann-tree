struct dados
{
    char palavra[30];
    int frequencia;
    char codigo[30];
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

char Folha(Tree *T)
{
    return T->dir == NULL && T->esq == NULL;
}

void imprimeLista(Lista *L)
{
    while (L != NULL)
    {
        printf("%s %d %s\n", L->D.palavra, L->D.frequencia, L->D.codigo);
        L = L->prox;
    }
}

void imprimeArvore(Tree *raiz, int nivel)
{
    if (raiz != NULL)
    {
        imprimeArvore(raiz->dir, nivel + 1);
        if (Folha(raiz))
        {
            printf("(%d,%d)\n", raiz->frequencia, raiz->simbolo);
        }
        imprimeArvore(raiz->esq, nivel + 1);
    }
}