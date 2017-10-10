/***********************************************************/
/*                   ESTRUTURA DE DADOS                    */
/*            Projeto Gerenciamento de memoria             */
/*                em sistemas operacionais                 */
/*                                                         */
/*                             Andrew Lucas Guedes de Souza*/
/*                                     Matrícula: 160023921*/
/*                              Nivaldo Pereira Lopo Junior*/
/*                                     Matrícula: 120039460*/
/***********************************************************/

/*Importando bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<locale.h>

#define MEMORY_SIZE 10
#define P 1
#define H 0

typedef struct MemoryList{
  int type; // P -> processo; H -> buraco;
  char label;
  int size;
  int duration;
  struct MemoryList *next;
  struct MemoryList *prev;
}MemorySpace;

typedef struct MemoryHeader{
  MemorySpace *first; // Aponta para o primeiro processo;
  MemorySpace *last; // Aponta para o ultimo processo;
  int free_space; // Indica quantidade de memoria livre;
}Memory;


/*Definindo Variaveis globais*/
int MEMORY_PART[MEMORY_SIZE]; //vetor de memoria, onde P sao espacos ocupados e H sao espacos livres
Memory *memory; // Cabecalho da memoria;


/*DECLARACAO DE FUNCOES*/
Memory* initialize(void); // Inicializa Lista;
void shut(void); // Chama encerramento do programa;
int swap(void); // Grava dados da memoria em disco;
//char* list2String(int i) // Converte lista para string p/ gravacao em disco;
//int readSwap(void); // Realiza leitura da memoria gravada em disco caso exista;
//void newProcess(void); // Cria novo processo e o aloca na memoria, se possivel;
int spaceVerify(int processSize); // Verifica se ha espaco suficiente para alocar processo;
//void showMemory(); // Imprime estado atual da memoria na tela;
//void getProcess(char label); // Seleciona processo, buscando-o pela label;
//void shutProcess(char label); // Encerra processo
//int freeSpaceCounter(); // Retorna a quantidade de espaco livre;

int main() {
  int op=-1;
  setlocale(LC_ALL,"Portuguese"); // Permite utilizacao de caracteres especiais;
  for(;;){
    system("clear");
    printf("\n");
    printf("1 - Criar novo processo. \n"); // Cria novo processo, e insere ele na memoria;
    printf("2 - Mostrar processos em execução. \n"); // Mostra todos processos em execucao;
    printf("3 - Encerrar processo em execução. \n"); // Forca encerramento de processo, antes do tempo estabelecido;
    printf("4 - Congelar processos \n"); // Para o tempo de execucao dos processos;
    printf("0 - Fechar \n");
    scanf("%d", &op);
    switch (op) {
      case 0: system("clear");
              shut();
              break;
      case 1: system("clear");
              break;
      case 2: system("clear");
              break;
      case 3: system("clear");
              break;
      case 4: system("clear");
              break;
      case 5: system("clear");
              break;
    }
  }

  return 0;
}

Memory* initialize(void){
  int i;
  for (i=0;i<MEMORY_SIZE;i++){
    MEMORY_PART[i] = H;
  }
  return NULL;
}

void shut(void){
  int op=0;
  char resp;
  printf("Deseja salvar o estado atual da memória para continuar depois?[s/n]\n");
  scanf(" %c", &resp);
  if(resp == 's' || resp == 'S'){
    op = swap();
  }
  if (op == 1) {
    printf("Deseja continuar mesmo assim?[s/n]\n");
    scanf(" %c", &resp);
    if(resp == 'n' || resp == 'N'){
      return;
    }
  }
  exit(0);
}

int swap(void){
  FILE *fp;
  fp = fopen ("swap.txt", "w");
  if (fp == NULL) {
     printf ("Houve um erro ao gravar a memória em disco.\n");
     return 1;
  }

  printf ("Swap realizado com sucesso!.\n");
  fclose (fp);
  return 0;
}

int spaceVerify(int processSize){
  if(memory->free_space >= processSize)
    return 1;
  else
    return 0;
}
