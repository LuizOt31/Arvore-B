#include "arvB.h"
#include <stdio.h>
#include <stdlib.h>

struct NO {
  int qtd;
  int ehFolha;
  int chaves[ORDEM - 1];
  ArvB filhos[ORDEM];
};

ArvB criaNO() {
  // Aloca memoria para um novo no
  ArvB novo = (ArvB)malloc(sizeof(struct NO));

  // Inicializa a estrutura do no
  if (novo != NULL) {
    novo->qtd = 0;
    novo->ehFolha = 1;
    for (int i = 0; i < ORDEM; i++) {
      novo->filhos[i] = NULL;
    }
  }

  return novo;
}

ArvB* arvB_cria() {
  // Aloca memoria para um ponteiro para a raiz da arvore
  ArvB* raiz = (ArvB*)malloc(sizeof(struct NO));

  // Cria um no para a raiz
  if (raiz != NULL) {
    *raiz = criaNO();
  }

  return raiz;
}

void destroiNO(ArvB no) {
  if (no == NULL) {
    return;
  }
  // Libera a memoria dos niveis mais baixos
  if (!no->ehFolha) {
    for (int i = 0; i < no->qtd + 1; i++) {
      destroiNO(no->filhos[i]);
    }
  }

  // Libera o no atual
  free(no);
}

void arvB_destroi(ArvB *raiz)
{
  if (raiz != NULL && *raiz != NULL)
  {
    destroiNO(*raiz);
    *raiz = NULL;
  }
}

void split1to2(ArvB pai, int indice){
  ArvB filho = pai->filhos[indice];
  ArvB* novo_filho = arvB_cria();
  (*novo_filho)->ehFolha = filho->ehFolha;
  (*novo_filho)->qtd = (ORDEM / 2) - 1;

  //Adiciona as chaves da segunda metade no filho a esquerda
  for (int i = 0; i < ORDEM / 2 - 1; i++) {
    (*novo_filho)->chaves[i] = filho->chaves[i + ORDEM / 2];
  }
  // copia os filhos correspondentes caso nao seja folha
  if (!filho->ehFolha) {
    for (int i = 0; i < ORDEM / 2; i++) {
      (*novo_filho)->filhos[i] = filho->filhos[i + ORDEM / 2];
    }
  }

  //Atualiza a quantidade do filho que foi splitado
  filho->qtd = ORDEM / 2 - 1;

  //Ajusta os filhos do pai para adicionar o novo filho
  for (int i = pai->qtd; i >= indice + 1; i--) {
    pai->filhos[i + 1] = pai->filhos[i];
  }

  //Adiciona o novo filho na posicao correta no pai
  pai->filhos[indice + 1] = (*novo_filho);

  //Ajusta as chaves existentes para inserir a chave do meio
  for (int i = pai->qtd - 1; i >= indice; i--) {
    pai->chaves[i + 1] = pai->chaves[i];
  }
  
  // Insere a chave do meio do filho original no pai e atuaçiza a quantidade do pai
  pai->chaves[indice] = filho->chaves[ORDEM / 2 - 1];
  pai->qtd++;
}

void insereNaoCheio(ArvB raiz, int valor) {
  int i = raiz->qtd - 1;

  // Se eh folha
  if (raiz->ehFolha) {
    // Busca a posicao correta e move as chaves para inserir o novo valor 
    while (i >= 0 && raiz->chaves[i] > valor) {
      raiz->chaves[i + 1] = raiz->chaves[i];
      i--;
    }
    //Insere a chave na posicao correta e atualiza a quantidade de chaves
    raiz->chaves[i + 1] = valor;
    raiz->qtd++;
  } 
  //Se nao
  else {
    // Busca o filho correta
    while (i >= 0 && raiz->chaves[i] > valor) {
      i--;
    }
    i++;
    //Se o filho esta cheio, faz o split nele
    if (raiz->filhos[i]->qtd == ORDEM - 1) {
      split1to2(raiz, i);
      //Se a chave atual eh menor, incrementa o contador para inserir no filho correto
      if (raiz->chaves[i] < valor)
        i++;
    }
    insereNaoCheio(raiz->filhos[i], valor);
  }
}

int arvB_insere(ArvB *raiz, int valor){
  //Se o valor ja existe na arvore, nao insere
  if(arvB_busca(raiz, valor))
      return 0;
  
  ArvB* raizA = raiz;
  //Se a raiz esta cheia, splita e insere no filho correto
  if ((*raizA)->qtd == (ORDEM - 1)){
    ArvB nova_raiz = criaNO();
    nova_raiz->ehFolha = 0;
    nova_raiz->filhos[0] = *raizA;
    split1to2(nova_raiz, 0);
    insereNaoCheio(nova_raiz, valor);
    *raiz = nova_raiz;
  }
    //Se nao, insere na raiz nao cheia
  else{
    insereNaoCheio(*raizA, valor);
  }
  return 1;
}

void concatena(ArvB pai, int indice){
  ArvB filhoEsq = pai->filhos[indice];
  ArvB filhoDir = pai->filhos[indice + 1];

  // Move a chave do pai para o filho a esquerda
  filhoEsq->chaves[ORDEM / 2 - 1] = pai->chaves[indice];

  // Move as chaves do irmao a direita para o irmao a esquerda
  for (int i = 0; i < filhoDir->qtd; i++){
      filhoEsq->chaves[i + ORDEM / 2] = filhoDir->chaves[i];
  }

  // Move os filhos do irmao a direita para o irmao a esquerda
  if (!filhoDir->ehFolha)
  {
    for (int i = 0; i <= filhoDir->qtd; i++)
    {
      filhoEsq->filhos[i + ORDEM / 2] = filhoDir->filhos[i];
    }
  }

  for (int i = indice + 1; i < pai->qtd; i++){
    pai->chaves[i - 1] = pai->chaves[i];
  }

  for (int i = indice + 2; i <= pai->qtd; i++){
    pai->filhos[i - 1] = pai->filhos[i];
  }

  filhoEsq->qtd += filhoDir->qtd + 1;
  
  pai->qtd--;

  free(filhoDir);
}

// Função auxiliar para remover uma chave de um nó folha
void remover_chave_folha(ArvB no, int indice){
  for (int i = indice; i < no->qtd - 1; i++)
  {
      no->chaves[i] = no->chaves[i + 1];
  }
  no->qtd--;
}

// Função auxiliar para trocar uma chave por seu predecessor em um nó não folha
int trocar_por_predecessor(ArvB no, int indice)
{
    ArvB no_atual = no->filhos[indice];
    while (!no_atual->ehFolha){
        no_atual = no_atual->filhos[no_atual->qtd];
    }
    int predecessor = no_atual->chaves[no_atual->qtd - 1];
    no->chaves[indice] = predecessor;
    return predecessor;
}

int trocar_por_sucessor(ArvB no, int indice){
    ArvB no_atual = no->filhos[indice + 1];
    while (!no_atual->ehFolha){
        no_atual = no_atual->filhos[0];
    }
    int sucessor = no_atual->chaves[0];
    no->chaves[indice] = sucessor;
    return sucessor;
}

void empresta_do_antecessor(ArvB pai, int indice){
    ArvB filho = pai->filhos[indice];
    ArvB irmao = pai->filhos[indice - 1];

    for (int i = filho->qtd - 1; i >= 0; i--)    {
        filho->chaves[i + 1] = filho->chaves[i];
    }

    if (!filho->ehFolha){
        for (int i = filho->qtd; i >= 0; i--){
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }
}
  
void empresta_do_sucessor(ArvB pai, int indice)
{
    ArvB filho_atual = pai->filhos[indice];
    ArvB filho_sucessor = pai->filhos[indice + 1];

    filho_atual->chaves[filho_atual->qtd] = pai->chaves[indice];
    filho_atual->qtd++;

    pai->chaves[indice] = filho_sucessor->chaves[0];

    for (int i = 1; i < filho_sucessor->qtd; i++)
    {
        filho_sucessor->chaves[i - 1] = filho_sucessor->chaves[i];
    }

    if (!filho_sucessor->ehFolha)
    {
        for (int i = 1; i <= filho_sucessor->qtd; i++)
        {
            filho_sucessor->filhos[i - 1] = filho_sucessor->filhos[i];
        }
    }

    filho_sucessor->qtd--;
}


int arvB_remove(ArvB *no, int valor){
  int indice = 0;
  while (indice < (*no)->qtd && valor > (*no)->chaves[indice]){
      indice++;
  }

  if (indice < (*no)->qtd && (*no)->chaves[indice] == valor){
    if ((*no)->ehFolha){
        remover_chave_folha((*no), indice);
    }
    else{
      int chave_substituta;
      if ((*no)->filhos[indice]->qtd >= ORDEM / 2){
          chave_substituta = trocar_por_predecessor((*no), indice);
          arvB_remove(&(*no)->filhos[indice], chave_substituta);
      }
      else if ((*no)->filhos[indice + 1]->qtd >= ORDEM / 2){
        chave_substituta = trocar_por_sucessor((*no), indice + 1);
        arvB_remove(&(*no)->filhos[indice + 1], chave_substituta);
      }
      else{
        concatena((*no), indice);
        arvB_remove(&(*no)->filhos[indice], valor);
      }
    }
  }
  else{
      if ((*no)->ehFolha){
          return 0;
      }
      int filho = indice;
      if ((*no)->filhos[indice]->qtd == ORDEM / 2 - 1){
          if (indice > 0 && (*no)->filhos[indice - 1]->qtd >= ORDEM / 2){
              empresta_do_antecessor((*no), indice);
          }
          else if (indice < (*no)->qtd && (*no)->filhos[indice + 1]->qtd >= ORDEM / 2){
              empresta_do_sucessor((*no), indice);
          }
          else{
              if (indice < (*no)->qtd){
                  concatena((*no), indice);
              }
              else{
                  concatena((*no), indice - 1);
              }
          }
      }
      arvB_remove(&(*no)->filhos[filho], valor);
  }
  return 1;
}

int arvB_busca(ArvB *raiz, int valor) {
  if (*raiz == NULL)
    return 0;
  
  // Busca a posicao ou filho correto
  int i = 0;
  while (i < (*raiz)->qtd && valor > (*raiz)->chaves[i])
    i++;

  // Achou o valor
  if (i < (*raiz)->qtd && valor == (*raiz)->chaves[i])
    return 1;

  // Busca no filho
  if ((*raiz)->filhos[0] != NULL)
    return arvB_busca(&(*raiz)->filhos[i], valor);

  // Nao achou
  return 0;
}

int arvB_qtd_nos(ArvB *raiz) {
  if (*raiz == NULL)
    return 0;

  // Total inicializado como um para a raiz ser contabilizada
  int total = 1;
  // Realiza a contagem para os nos abaixo do no atual
  for (int i = 0; i < (*raiz)->qtd + 1; i++)
    total += arvB_qtd_nos(&(*raiz)->filhos[i]);

  return total;
}

int arvB_qtd_chaves(ArvB *raiz) {
  if (*raiz == NULL)
    return 0;
  // Total inicializado como a quantidade total do no atual
  int total = (*raiz)->qtd;
  // Realiza a contagem para os nos abaixo do no atual
  for (int i = 0; i < (*raiz)->qtd + 1; i++)
    total += arvB_qtd_chaves(&(*raiz)->filhos[i]);

  return total;
}

int arvB_altura(ArvB *raiz) {
  if (*raiz == NULL)
    return 0;

  // Se e folha, retorna 1
  if ((*raiz)->filhos[0] == NULL)
    return 1;

  // Se nao, retorna 1 + o resultado da altura para os niveis mais baixos
  return 1 + arvB_altura(&(*raiz)->filhos[0]);
}

// Função callee do arv_B_imprime
void arvB_imprimeR(ArvB no){
  if (no != NULL){
    for (int i = 0; i < no->qtd; i++){
      arvB_imprimeR(no->filhos[i]);
      printf("%d ", no->chaves[i]);
    }
    arvB_imprimeR(no->filhos[no->qtd]);
  }
}

// Impressão da arvore inteira
void arvB_imprime(ArvB raiz){
  arvB_imprimeR(raiz);
  printf("\n");
}

// Função unicamente para comparar dois elementos
int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }