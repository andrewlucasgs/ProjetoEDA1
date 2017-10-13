/***********************************************************/
/*                   ESTRUTURA DE DADOS                    */
/*            Projeto Gerenciamento de memoria             */
/*                em sistemas operacionais                 */
/*                                                         */
/*                             Andrew Lucas Guedes de Souza*/
/*                                     Matrícula: 160023921*/
/*                              Nivaldo Pereira Lopo Junior*/
/*                                     Matrícula: 120039460*/
/*                                     Max Henrique Barbosa*/
/*                                     Matrícula: 160047013*/
/***********************************************************/

/*Importando bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<locale.h>
#include<unistd.h>

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

/*DECLARACAO DE FUNCOES*/
void initialize(Memory *memory); // Inicializa Lista;
void shut(Memory *memory); // Chama encerramento do programa;
int swap(Memory *memory); // Grava dados da memoria em disco;
void readSwap(Memory *memory, FILE *fp); // Realiza leitura da memoria gravada em disco caso exista;
void initializeProcess(Memory *memory, char label, int size, int duration); // Cria novo processo e o aloca na memoria, se possivel;
void newProcess(Memory *memory); // Recebe parametros de criacao de um processo;
int spaceVerify(Memory *memory, int processSize); // Verifica se ha espaco suficiente para alocar processo;
void showMemory(Memory *memory); // Imprime estado atual da memoria na tela;
//void getProcess(Memory *memory, char label); // Seleciona processo, buscando-o pela label;
//void shutProcess(Memory *memory, char label); // Encerra processo
//int freeSpaceCounter(Memory *memory); // Retorna a quantidade de espaco livre;
int findSpace(Memory *memory, int size);
int compactMemory(Memory *memory);

int main() {
  int op=-1;
  Memory *memory = malloc(sizeof(Memory)); // Cabecalho da memoria;
  setlocale(LC_ALL,"Portuguese"); // Permite utilizacao de caracteres especiais;
  initialize(memory); // Inicia memoria;
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
              shut(memory);
              break;
      case 1: system("clear");
              newProcess(memory);
              break;
      case 2: system("clear");
              showMemory(memory);
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

void initialize(Memory *memory){
  int i;
  char resp;
  MemorySpace *hole  = (MemorySpace*) malloc(sizeof(MemorySpace));
  hole->type = H;
  hole->size = MEMORY_SIZE;
  hole->startAt = 0;
  hole->next = hole;
  hole->prev = hole;
  memory->free_space = MEMORY_SIZE;
  memory->first = hole;
  for (i=0;i<MEMORY_SIZE;i++){
    MEMORY_PART[i] = H;
  }
  FILE *fp  = fopen ("swap.txt", "r");
  if (fp != NULL) {
     printf ("Deseja recuperar os processos salvos em disco?[s/n]\n");
     scanf(" %c", &resp);
     if(resp == 's' || resp == 'S'){
       readSwap(memory, fp);
       fclose(fp);
       return;
     }
     remove("swap.txt");

     fclose(fp);
     return;

  }
}

void newProcess(Memory *memory) {
  char label;
  int duration, size;
  printf("Digite um rótulo para o processo (1 caracter): \n");
  scanf(" %c", &label);
  printf("Tamanho (kbit): \n");
  scanf(" %d", &size);
  printf("Duração (s): \n");
  scanf(" %d", &duration);
  initializeProcess(memory, label, size, duration);
}

void shut(Memory *memory){
  int op=0;
  char resp;
  printf("Deseja salvar o estado atual da memória para continuar depois?[s/n]\n");
  scanf(" %c", &resp);
  if(resp == 's' || resp == 'S'){
    op = swap(memory);
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

int swap(Memory *memory){
  FILE *fp;
  MemorySpace *p;
  p = memory->first;
  fp = fopen ("swap.txt", "w");
  if (fp == NULL) {
     printf ("Houve um erro ao gravar a memória em disco.\n");
     fclose (fp);
     return 1;
  }
  do{
    if(p->type == P){
      fprintf(fp, "%c %d %d\n", p->label, p->size, p->duration);
    }
    p = p->next;
  }while(p != memory->first);
  printf ("Swap realizado com sucesso!.\n");
  fclose (fp);
  return 0;
}

void readSwap(Memory *memory, FILE *fp){
  int size, duration;
  char label;
  if (fp == NULL) {
     printf ("Houve um erro ao ler a memória em disco.\n");
     return;
  }
  while( (fscanf(fp,"%c %d %d\n", &label, &size, &duration))!=EOF )
    initializeProcess(memory, label, size, duration);
  printf ("Dados restaurados com sucesso!.\n");
  remove("swap.txt");
  fclose (fp);
  return;
}

void initializeProcess(Memory *memory, char label, int size, int duration){
  MemorySpace *process, *aux;
  aux = memory->first;
  int i, comp;
  process = (MemorySpace*) malloc(sizeof(MemorySpace));
  process->type = P;
  process->label = label;
  process->size = size;
  process->duration = duration;
  if(spaceVerify(memory, process->size)){
    i = findSpace(memory, size);
    if(i == -1){
      comp = compactMemory(memory);
    }
    do{
      if(aux->type == H || aux->size > size){
        aux->prev->next = process; //anterior espaco vazio aponta para novo processo
        process->prev = aux->prev->next; // o novo processo aponta para o anterior
        aux->size -= process->size; // Reduz o tamanho do buraco de acordo com o tamanho do procsesso
        process->next = aux; // novo processo aponta para o buraco;
        aux->prev = process; // buraco aponta para novo processo
        process->startAt = aux->startAt; // copia endereço de memoria do buraco para o proceso
        aux->startAt = aux->startAt + process->size + 1; // define novo endereco de memoria para o buraco;
        memory->free_space -= process->size;
        return;
      }else if(aux->type == H || aux->size == size){
        aux->prev->next = process; //anterior espaco vazio aponta para novo processo
        process->prev = aux->prev->next; // o novo processo aponta para o anterior
        process->next = aux->next; // novo processo aponta para o proximo  espaco após o buraco;
        aux->next->prev = process; // o espaco apoos o buraco aponta para novo processo
        process->startAt = aux->startAt;
        memory->free_space -= process->size;
        free(aux);
        return;
      }
      printf("flag\n" );
      aux = aux->next;
    }while(aux != memory->first);
  }else{
    printf("Não foi possível alocar memória para o processo: %c", process->label);
    system("clear");
    free(process);
    return;
  }
}

int compactMemory(Memory *memory){
  FILE *fp;
  MemorySpace *p;
  int size, duration;
  char label;
  p = memory->first;
  fp = fopen ("temp.txt", "w");
  if (fp == NULL) {
     printf ("Erro na compactação\n");
     fclose (fp);
     return 0;
  }
  do{
    if(p->type == P){
      fprintf(fp, "%c %d %d\n", p->label, p->size, p->duration);
    }
    p = p->next;
  }while(p != memory->first);
  fclose (fp);
  fp = fopen ("temp.txt", "r");
  while( (fscanf(fp,"%c %d %d\n", &label, &size, &duration))!=EOF )
    initializeProcess(memory, label, size, duration);
  fclose (fp);
  return 1;
}

int findSpace(Memory *memory, int size){
  MemorySpace *p;
  p = memory->first;
  do{
    if(p->type == H || p->size >= size){
      return p->startAt;
    }
    p = p->next;
  }while(p != memory->first);
  return -1;
}

int spaceVerify(Memory *memory, int processSize){
  if(memory->free_space >= processSize)
    return 1;
  else
    return 0;
}

void showMemory(Memory *memory){
  MemorySpace *p;
  p = memory->first;
  printf("Processo \t Tamanho \t Endereço\n");
  do{
    if(p->type == P)
      printf("%c \t %d \t %d\n", p->label, p->size, p->startAt);
    p = p->next;
  }while(p != memory->first);
  printf("Memória Livre: %d\n", memory->free_space);
  printf("Memória Ocupada: %d\n", MEMORY_SIZE - memory->free_space);
  printf("Memória Total: %d\n", MEMORY_SIZE);
  printf("Press [Enter] key to continue.");
  while(getchar()!='\n'); // option TWO to clean stdin
}
