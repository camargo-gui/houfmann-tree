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