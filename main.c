#include "arvB.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
Guilherme dos Santos Wisniewski - 813319
Luiz Otávio Teixeira Mello - 811967
Matheus Yuiti Moriy Miata - 802097
*/
int main(void) {
  srand(10);
  ArvB *raiz = arvB_cria();

  for (int i = 0; i < 20; i++) {
    int x = rand() % 101;
    printf("Posição %d Numero Inserido %d\n", i + 1, x);
    arvB_insere(raiz, x);
    arvB_imprime(*raiz);
    printf("Quantidade de Chaves: %d\n", arvB_qtd_chaves(raiz));
    printf("Quantidade de Nos: %d\n", arvB_qtd_nos(raiz));
    printf("\n");
  }

  
  arvB_remove(raiz, 6);
  arvB_remove(raiz, 28);
  arvB_remove(raiz, 29);
  arvB_remove(raiz, 35);
  arvB_remove(raiz, 59);
  arvB_remove(raiz, 60);
  arvB_remove(raiz, 17);

  arvB_imprime(*raiz);
  printf("Quantidade de Chaves: %d\n", arvB_qtd_chaves(raiz));
  printf("Quantidade de Nos: %d\n", arvB_qtd_nos(raiz));
  printf("\n");
  printf("Esta eh a altura da arvore: %d\n", arvB_altura(raiz));
  arvB_destroi(raiz);

  return 0;
}