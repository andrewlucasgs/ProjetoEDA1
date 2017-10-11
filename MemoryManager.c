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
  int startAt;
  int duration;
  struct MemoryList *next;
  struct MemoryList *prev;
}MemorySpace;

typedef struct MemoryHeader{
  MemorySpace *first; // Aponta para o primeiro processo;
  int free_space; // Indica quantidade de memoria livre;
}Memory;


/*Definindo Variaveis globais*/
int MEMORY_PART[MEMORY_SIZE]; //vetor de memoria, onde P sao espacos ocupados e H sao espacos livres
Memory *memory; // Cabecalho da memoria;


/*DECLARACAO DE FUNCOES*/
void initialize(void); // Inicializa Lista;
void shut(void); // Chama encerramento do programa;
int swap(void); // Grava dados da memoria em disco;
int readSwap(void); // Realiza leitura da memoria gravada em disco caso exista;
void initializeProcess(char label, int size, int duration); // Cria novo processo e o aloca na memoria, se possivel;
//void newProcess(void); // Recebe parametros de criacao de um processo;
int spaceVerify(int processSize); // Verifica se ha espaco suficiente para alocar processo;
//void showMemory(); // Imprime estado atual da memoria na tela;
//void getProcess(char label); // Seleciona processo, buscando-o pela label;
//void shutProcess(char label); // Encerra processo
//int freeSpaceCounter(); // Retorna a quantidade de espaco livre;
int findSpace(int size);

int main() {
  int op=-1;
  setlocale(LC_ALL,"Portuguese"); // Permite utilizacao de caracteres especiais;
  initialize(); // Inicia memoria;
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

void initialize(void){
  int i;
  MemorySpace *hole;
  hole = (MemorySpace*) malloc(sizeof(MemorySpace));
  hole->size = MEMORY_SIZE;
  memory->free_space = MEMORY_SIZE;
  memory->first = hole;
  for (i=0;i<MEMORY_SIZE;i++){
    MEMORY_PART[i] = H;
  }
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
  MemorySpace *p;
  p = memory->first;
  fp = fopen ("swap.txt", "w");
  if (fp == NULL) {
     printf ("Houve um erro ao gravar a memória em disco.\n");
     return 1;
  }
  for(p = memory->first; p != NULL;p = p->next){
    fprintf(fp, "%c %d %d\n", p->label, p->size, p->duration);
  }
  printf ("Swap realizado com sucesso!.\n");
  fclose (fp);
  return 0;
}

int readSwap(void){
  FILE *fp;
  int size, duration;
  char label;
  fp = fopen ("swap.txt", "r");
  if (fp == NULL) {
     printf ("Houve um erro ao ler a memória em disco.\n");
     return 1;
  }
  while( (fscanf(fp,"%c %d %d\n", &label, &size, &duration))!=EOF )
    initializeProcess(label, size, duration);
  printf ("Dados restaurados com sucesso!.\n");
  fclose (fp);
  return 0;
}

void initializeProcess(char label, int size, int duration){
  MemorySpace *process, *aux;
  int i;
  process = (MemorySpace*) malloc(sizeof(MemorySpace));
  process->type = P;
  process->label = label;
  process->size = size;
  process->duration = duration;
  if(spaceVerify(process->size)){
    i = findSpace(size);
    if(i == -1){
      // compactMemory();
    }
    for (aux = memory->first; aux != NULL; aux = aux->next){
      if(aux->type == H || aux->size >= size){

      }
    }

  }else{
    printf("Não foi possível alocar memória para o processo: %c", process->label);
    free(process);
  }
}

int findSpace(int size){
  MemorySpace *p;
  for (p = memory->first; p != NULL; p = p->next){
    if(p->type == H || p->size >= size){
      return p->startAt;
    }
  }
  return -1;
}

int spaceVerify(int processSize){
  if(memory->free_space >= processSize)
    return 1;
  else
    return 0;
}
