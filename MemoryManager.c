/***********************************************************/
/*                   ESTRUTURA DE DADOS                    */
/*            Projeto Gerenciamento de memoria             */
/*                em sistemas operacionais                 */
/*                                                         */
/*                             Andrew Lucas Guedes de Souza*/
/*                                     Matrícula: 160023921*/
/*---------------------------------------------------------*/
/*                              Nivaldo Pereira Lopo Junior*/
/*                                     Matrícula: 120039460*/
/*---------------------------------------------------------*/
/*                                     Max Henrique Barbosa*/
/*                                     Matrícula: 160047013*/
/***********************************************************/

/*Importando bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<locale.h>
#include<unistd.h>
#include <pthread.h>


#define MEMORY_SIZE 10
#define P 1
#define H 0

int closeThreads = 1;
int idGeneretor = 1;

typedef struct MemoryList{
  int type; // P -> processo; H -> buraco;
  int id;
  char label;
  int size;
  int startAt;
  int initialTime;
  int duration;
  struct MemoryList *next;
  struct MemoryList *prev;
}MemorySpace;

typedef struct MemoryHeader{
  MemorySpace *first; // Aponta para o primeiro espaço na memoria;
  MemorySpace *last; // Aponta para o ultimo espaço na memoria;

  int free_space; // Indica quantidade de memoria livre;
}Memory;


/*DECLARACAO DE FUNCOES*/
void initialize(Memory *memory); // Inicializa Lista;
void shut(Memory *memory); // Chama encerramento do programa;
int swap(Memory *memory); // Grava dados da memoria em disco;
void readSwap(Memory *memory, FILE *fp); // Realiza leitura da memoria gravada em disco caso exista;
void initializeProcess(Memory *memory, int id, char label, int size, int duration); // Cria novo processo e o aloca na memoria, se possivel;
void newProcess(Memory *memory); // Recebe parametros de criacao de um processo;
int spaceVerify(Memory *memory, int processSize); // Verifica se ha espaco suficiente para alocar processo;
void *showMemory(); // Imprime estado atual da memoria na tela;
void *closeThread(void);
void callShowMemory(Memory *memory);
MemorySpace *getProcess(Memory *memory, int id); // Seleciona processo, buscando-o pela label;
void callShutProcess(Memory *memory); // Chama e força encerramento do processo, lendo a label do processo
void shutProcess(Memory *memory, int id); // Encerra processo
void freeSpaceCounter(Memory *memory); // Retorna a quantidade de espaco livre;
int findSpace(Memory *memory, int size); // Verifica se e necessario compactar/oraganizar a memoria
int compactMemory(Memory *memory); // compacta a memoria;
void mergeHole(Memory *memory, MemorySpace *p); // Mescla os buracos vizinhos na memoria;
void garbageCollector(Memory *memory); // verifica os processos que já se encerram;


int main() {
  int op=-1;
  Memory *memory = malloc(sizeof(Memory)); // Cabecalho da memoria;
  setlocale(LC_ALL,"Portuguese"); // Permite utilizacao de caracteres especiais;
  initialize(memory); // Inicia memoria;
  for(;;){
    system("clear");
    printf("\n");
    printf(" %c 1 - Criar novo processo. \n", 254); // Cria novo processo, e insere ele na memoria;
    printf(" %c 2 - Mostrar processos em execução. \n", 254); // Mostra todos processos em execucao;
    printf(" %c 3 - Encerrar processo em execução. \n", 254); // Forca encerramento de processo, antes do tempo estabelecido;
    printf(" %c 0 - Fechar \n", 175);
    scanf(" %d", &op);
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
              callShutProcess(memory);
              break;
      default: break;
    }
  }

  return 0;
}

void initialize(Memory *memory){
  char resp;
  MemorySpace *hole  = (MemorySpace*) malloc(sizeof(MemorySpace));
  hole->type = H;
  hole->size = MEMORY_SIZE;
  hole->startAt = 0;
  hole->next = hole;
  hole->prev = hole;
  memory->first = hole;
  memory->last = hole;
  freeSpaceCounter(memory);
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
  int duration, size, id;
  printf("Digite um rótulo para o processo (1 caracter): \n");
  scanf(" %c", &label);
  printf("Tamanho (kbit): \n");
  scanf(" %d", &size);
  printf("Duração (s): \n");
  scanf(" %d", &duration);
  id = idGeneretor;
  idGeneretor++;
  initializeProcess(memory, id, label, size, duration);
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
  garbageCollector(memory);
  p = memory->first;
  fp = fopen ("swap.txt", "w");
  if (fp == NULL) {
     printf ("Houve um erro ao gravar a memória em disco.\n");
     fclose (fp);
     return 1;
  }
  do{
    if(p->type == P){
      fprintf(fp, "%d %c %d %d\n", p->id, p->label, p->size, p->duration);
    }
    p = p->next;
  }while(p != memory->first);
  printf ("Swap realizado com sucesso!.\n");
  fclose (fp);
  return 0;
}

void readSwap(Memory *memory, FILE *fp){
  int size, duration, id;
  char label;
  if (fp == NULL) {
     printf ("Houve um erro ao ler a memória em disco.\n");
     return;
  }
  garbageCollector(memory);
  while( (fscanf(fp,"%d %c %d %d\n", &id, &label, &size, &duration))!=EOF )
    initializeProcess(memory, id, label, size, duration);
  printf ("Dados restaurados com sucesso!.\n");
  remove("swap.txt");
  fclose (fp);
  freeSpaceCounter(memory);
  return;
}

void initializeProcess(Memory *memory, int id, char label, int size, int duration){
  MemorySpace *process, *aux;
  aux = memory->first;
  int i;
  int startTime = time(NULL);
  garbageCollector(memory);
  process = (MemorySpace*) malloc(sizeof(MemorySpace));
  process->type = P;
  process->id = id;
  process->label = label;
  process->size = size;
  process->duration = duration;
  process->initialTime = startTime;
  process->next = process;
  process->prev = process;
  if(spaceVerify(memory, process->size)){ // verifica se há espaço livre na memoria, suficiente para alicar o novo processo;
    i = findSpace(memory, process->size); // Verifica se há necessidade de compactar/organizar memoria
    if(i == -1){
      compactMemory(memory); //organiza procesos na memoria para alocar o novo processo;
    }
    do{
      if(memory->first != NULL && aux->type == H){
        if(aux->size > process->size){
          if(aux->startAt == 0 ){ //Insercao no inicio
            memory->first->prev = process;
            process->next = memory->first;
            memory->last->next = process;
            process->prev = memory->last;
            memory->first = process;
            process->startAt = 0;
            process->next->size -= process->size;
            process->next->startAt = process->size;
          }else if(aux->startAt > 0 && aux->startAt + process->size < MEMORY_SIZE){ // Insercao no meio da memoria
            aux->prev->next = process;
            process->prev = aux->prev;
            aux->prev = process;
            process->next = aux;
            process->startAt = aux->startAt;
            aux->size -= process->size;
            aux->startAt = process->startAt + process->size;
          }else if(aux->startAt + process->size == MEMORY_SIZE){ //Insercao no final da memoria
            memory->first->prev = process;
            process->next = memory->first;
            memory->last->next = process;
            process->prev = memory->last;
            memory->first = process;
            process->startAt = 0;
            process->next->size -= process->size;
            process->next->startAt = process->size + 1;
          }
          return;
        } else if(aux->size == process->size){ //Insercao quando o espaço disponível for igual o tamanho do processo;
          aux->type = P;
          aux->label = label;
          aux->duration = duration;
          aux->initialTime = startTime;
          return;
          }
      }
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
  int size, duration, id  ;
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
      fprintf(fp, "%d %c %d %d\n",p->id, p->label, p->size, p->duration);
    }
    p = p->next;
  }while(p != memory->first);
  fclose (fp);
  fp = fopen ("temp.txt", "r");
  while( (fscanf(fp,"%d %c %d %d\n",&id, &label, &size, &duration))!=EOF )
    initializeProcess(memory, id, label, size, duration);
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

void garbageCollector(Memory *memory){
  int finalTime = time(NULL);
  MemorySpace *p;
  p = memory->first;
  do{
    if(p->duration <= difftime(finalTime, p->initialTime) && p->type == P){
      shutProcess(memory, p->id);
    }
    printf("garbageCollector\n" );
    p = p->next;
  }while(p != memory->first);
  freeSpaceCounter(memory);
}

MemorySpace *getProcess(Memory *memory, int id){
  MemorySpace *p;
  p = memory->first;
  do{
    if(p->type == P)
      if(p->id == id){
        return p;
      }
    printf("getProcess\n" );
    p = p->next;
  }while(p != memory->first);
  return NULL;
}

void mergeHole(Memory *memory, MemorySpace *p) {
  MemorySpace *aux;
  aux = p;
  while(p->prev->type == H && p != memory->first){ //Se o anterior for um buraco, o p passa a apontar para ele;
    p = p->prev;
    printf("mergeHole1\n" );
  }
  aux = p->next;
  if (aux->type == P) {
    return;
  }
  do{
    if(aux->type == H && aux->next != p){
      aux->next->prev = p;
      p->next = aux->next;
      p->size += aux->size;
      if(aux == memory->last) memory->last = p;
      printf("mergeHole2\n" );
      aux = p->next;
    }else if(aux->type == H && aux->next == p){
      p->next = p;
      p->prev = p;
      if(p->size != aux->size){
        p->size += aux->size;
      }
      if(aux == memory->last) memory->last = p;
      printf("mergeHole3\n" );
      aux = p->next;
    }else{
      system("clear");
      printf("Te acheu\n" );
      sleep(4);
      return;
    }
    printf("mergeHole4\n" );
  }while(aux->type != P && aux != p);
}

void callShutProcess(Memory *memory) {
  int id;
  printf("Digite o ID do processo que deseja encerrar:\n");
  scanf(" %d",&id);
  shutProcess(memory, id);
}

void shutProcess(Memory *memory, int id){
  MemorySpace *p;
  p = getProcess(memory, id);
  if(p != NULL){
    p->type = H;
    if(p->prev->type == H || p->next->type == H){
      mergeHole(memory, p);
      printf("shutProcess\n" );
    }
    freeSpaceCounter(memory);
    return;
  }
  printf("Processo não encontrado!\n");
  printf("\nAperte Enter para continuar.");
  getchar();
  getchar();
  return;
}

void freeSpaceCounter(Memory *memory){
  MemorySpace *p;
  p = memory->first;
  memory->free_space = 0;
  do{
    if(p->type == H){
    memory->free_space += p->size;
    }
    printf("freeSpaceCounter\n" );
  p = p->next;
  }while(p != memory->first);
}

void callShowMemory(Memory *memory){
  pthread_t t;
  if (pthread_create(&t, NULL, showMemory, (void *)&memory)) {
   perror("pthread_create1");
   exit(EXIT_FAILURE);
  }
  pthread_join(t,NULL);
  return;
}

void *closeThread(){
  getchar();
while(closeThreads){
  if(getchar() == '\n'){
    closeThreads = 0;
    return NULL;
  }else{
    continue;
  }
}
  return NULL;
}

void *showMemory(Memory *memory){
  MemorySpace *p;
  pthread_t t2;
  if (pthread_create(&t2, NULL, (void*)closeThread, NULL)) {
   perror("pthread_create1");
   exit(EXIT_FAILURE);
  }
  while(closeThreads){
    p = memory->first;
    garbageCollector(memory);

    printf("______________________________________\n");
    printf("| ID | Processo | Tamanho | Endereço |\n");
    printf("|____|__________|_________|__________|\n");

    do{
      if(p->type == P){
        printf("|%3d |%9c |%8d |%9d |\n",p->id, p->label, p->size, p->startAt);
        printf("|____|__________|_________|__________|\n");
      }
      if(p == p->next && p->type == H){
        printf("|                                    |\n");
        printf("|    NÃO HÁ PROCESSOS EM EXECUÇÃO    |\n" );
        printf("|                                    |\n");
        printf("|____________________________________|\n");
      }
      p = p->next;
    }while(p != memory->first);


    printf("Memória Livre:  %5d Kbits\n", memory->free_space);
    printf("Memória Ocupada:%5d Kbits\n", MEMORY_SIZE - memory->free_space);
    printf("Memória Total:  %5d Kbits\n", MEMORY_SIZE);
    printf("______________________________________\n");
    printf("\nAperte Enter para continuar.\n");
    sleep(1);
    system("clear");
  }
  pthread_join(t2,NULL);
  closeThreads = 1;
  return NULL;
}
