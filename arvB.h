#ifndef ARVB_H
#define ARVB_H

#define ORDEM 8 // número máximo de filhos

typedef struct NO *ArvB;

ArvB *arvB_cria();
void arvB_destroi(ArvB *raiz);
int arvB_insere(ArvB *raiz, int valor);
int arvB_remove(ArvB *raiz, int valor);
int arvB_busca(ArvB *raiz, int valor);
int arvB_qtd_nos(ArvB *raiz);
int arvB_qtd_chaves(ArvB *raiz);

ArvB criaNO();
void destroiNO(ArvB no);
void split1to2(ArvB pai, int indice);
void insereNaoCheio(ArvB no, int valor);
void redistribuiEsq(ArvB no, int indice);
void redistribuiDir(ArvB no, int indice);
void concatena(ArvB pai, int indice);
int removeMax(ArvB *no);
int arvB_altura(ArvB *raiz);
void arvB_imprimeR(ArvB no);
void arvB_imprime(ArvB raiz);
int compare(const void *a, const void *b);

#endif