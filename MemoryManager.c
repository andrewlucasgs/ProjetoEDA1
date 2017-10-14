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
#include<string.h>


#define MEMORY_SIZE 100 // tamanho da memória
#define P 1
#define H 0

int closeThreads = 1; // Variável para execução de loop de exibicao da tabela
int idGeneretor = 1; // Gerador de id, incrementado a cada processo criado

typedef struct MemoryList{
  int type; // P -> processo; H -> buraco;
  int id;
  char label;
  int size;
  int startAt; // endereço de memória
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
void initializeProcess(Memory *memory, int id, char label, int size, int duration, int initTime, int mode); // Cria novo processo e o aloca na memoria, se possivel;
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
void compactMemory(Memory *memory, MemorySpace *process); // compacta a memoria;
void mergeHole(Memory *memory, MemorySpace *p); // Mescla os buracos vizinhos na memoria;
void garbageCollector(Memory *memory); // verifica os processos que já se encerram;
void logRegister(MemorySpace *p, int mode); // Registra log
void showLog(void); // Ler e mostra na tela o log

int main() {
  int op=-1;
  Memory *memory = malloc(sizeof(Memory)); // Cabecalho da memoria;
  setlocale(LC_ALL,"Portuguese"); // Permite utilizacao de caracteres especiais;
  initialize(memory); // Inicia memoria;
  for(;;){
    system("clear");
    printf("\n");
    printf("|                 GERENCIADOR DE MEMÓRIA                 |\n" );
    printf("|________________________________________________________|\n");
    printf("|                                                        |\n");
    printf("|   >>> 1 - Criar novo processo                          |\n"); // Cria novo processo, e insere ele na memoria;
    printf("|   >>> 2 - Mostrar processos em execução                |\n"); // Mostra todos processos em execucao;
    printf("|   >>> 3 - Encerrar processo em execução                |\n"); // Forca encerramento de processo, antes do tempo estabelecido;
    printf("|   >>> 4 - Exibir registros do sistema                  |\n"); // Mostra o log do Sitema;
    printf("|   >>> 5 - Sobre o sitema                               |\n"); // Exibe dados do desenvolvimento do sistema
    printf("|   >>> 0 - Fechar                                       |\n"); // inicia processo de encerramento do sistema
    printf("|________________________________________________________|\n");
    printf("\n|  >> Escolha uma opção:");
    printf("\n|  >> ");

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
      case 4: system("clear");
              showLog();
              break;
      case 5: system("clear");
              printf("\n");
              printf("|                 GERENCIADOR DE MEMÓRIA                 |\n" );
              printf("|________________________________________________________|\n");
              printf("|                                                        |\n");
              printf("|   TRABALHO P/ DISCIPLINA DE ESTRUTURA DE DADOS E       |\n");
              printf("|                                       ALGORITMOS       |\n");
              printf("|                                                        |\n");
              printf("|   ANDREW LUCAS GUEDES DE SOUZA - 16/0023921            |\n");
              printf("|   NIVALDO PEREIRA LOPO JUNIOR  - 12/0039460            |\n");
              printf("|   MAX HENRIQUE BARBOSA         - 16/0047013            |\n");
              printf("|________________________________________________________|\n");
              printf("\n|  >> Aperte Enter para voltar.");
              getchar();
              getchar();
              break;
      default: break;
    }
  }

  return 0;
}

void initialize(Memory *memory){
  char resp;
  MemorySpace *hole  = (MemorySpace*) malloc(sizeof(MemorySpace));
  hole->type = H;           //
  hole->startAt = 0;        // Criando espaco vazio do tamanho da memória
  hole->size = MEMORY_SIZE; //
  hole->next = hole;        //
  hole->prev = hole;        // Aponta para ele mesmo
  memory->first = hole;     //
  memory->last = hole;      // Como e unico, ele e o primeiro e o ultimo
  logRegister(NULL, 0);     // Registra inicializacao no log
  freeSpaceCounter(memory); // Verifica espaco vazio e atribui a free_space
  FILE *fp  = fopen ("swap", "r");  // Verifica se há arquivo de swap
  if (fp != NULL) {
     printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
     printf("|________________________________________________________|\n");
     printf("|                                                        |\n");
     printf("|   Deseja recuperar os processos salvos em disco?[s/n]  |\n");
     printf("|________________________________________________________|\n");
     scanf(" %c", &resp);
     if(resp == 's' || resp == 'S'){
       readSwap(memory, fp); // Recupera processos salvos
       fclose(fp);
       return;
     }
     remove("swap"); // deleta arquivo de swap;
     fclose(fp);
     return;

  }
}

void newProcess(Memory *memory) {
  char label;
  int duration, size, id;
  srand(time(NULL)); /*parameter for randamization of process duration */
  id = idGeneretor; // define id apartir do gerador de id
  idGeneretor++; // incrementa gerador de id
  duration = rand()%171+10;
  printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
  printf("|________________________________________________________|\n");
  printf("|                                                        |\n");
  printf("|   ID do processo: %4d                                 |\n", id);
  printf("|   O processo terá %3d segundos de duração.             |\n", duration);
  printf("|________________________________________________________|\n|\n");
  printf("|   Digite um rótulo para o processo (1 caracter): ");
  scanf(" %c", &label);
  printf("|   Tamanho (kbit): ");
  scanf(" %d", &size);
  // incializa processos com os dados recebidos do usuário
  initializeProcess(memory, id, label, size, duration, 0, 0);
}

void shut(Memory *memory){
  /*
  Encerra sistema, confirmando se o usuaria realmente deseja sair,
  e caso haja processos se ele deseja salvar
  */
  int op=0, process_check = 0;
  char resp, confirma;
  MemorySpace *p;
  p = memory->first;
  printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
  printf("|________________________________________________________|\n");
  printf("|                                                         \n");
  printf("|   Deseja realmente sair? [s/n]                          \n");
  printf("| ");
  scanf(" %c", &confirma);
  if(confirma == 'n' || confirma == 'N'){
    return;
  }
  do{ // pecorre a lista verificando se há processos em execucao
    if(p->type == P){
      process_check++;
    }
    p = p->next;
  }while(p != memory->first);
  if(process_check != 0){ // se houver processos em execucao
      printf("|    Deseja salvar o estado atual da memória para continuar depois?[s/n]\n");
      printf("| ");
      scanf(" %c", &resp);
      if(resp == 's' || resp == 'S'){
        op = swap(memory);  // realiza swap
      }
      if (op == 1) {// caso a gravacao dos processos em disco falhe
        printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
        printf("|________________________________________________________|\n");
        printf("|                                                        \n");
        printf("|    Houve um erro ao gravar a memória em disco.\n");
        printf("|    Deseja continuar mesmo assim?[s/n]\n");
        printf("| ");
        scanf(" %c", &resp);
        if(resp == 'n' || resp == 'N'){
          return;
        }
      }
  }/*fim-if-process_check*/
  if(confirma == 's' || confirma == 'S'){
      logRegister(NULL, 3); // registra encerramento do sistema
      exit(0);
  }
}

int swap(Memory *memory){
  FILE *fp;
  MemorySpace *p;
  garbageCollector(memory);
  p = memory->first;
  fp = fopen ("swap", "w");
  if (fp == NULL) {
     fclose (fp);
     return 1;
  }
  do{
    if(p->type == P){ // grava processos em arquivo swap
      fprintf(fp, "%d %c %d %ld\n", p->id, p->label, p->size, p->duration-(time(NULL) - p->initialTime));
    }
    p = p->next;
  }while(p != memory->first);
  fclose (fp);
  return 0;
}

void readSwap(Memory *memory, FILE *fp){
  int size, duration, id;
  char label;
  if (fp == NULL) {
     return;
  }
  garbageCollector(memory);
  while( (fscanf(fp,"%d %c %d %d\n", &id, &label, &size, &duration))!=EOF )
    initializeProcess(memory, id, label, size, duration, 0, 0); // inicializa processos apartir do swap
  remove("swap");
  fclose (fp);
  freeSpaceCounter(memory);
  return;
}

void initializeProcess(Memory *memory, int id, char label, int size, int duration, int initTime, int mode){
  MemorySpace *process, *aux;
  int i;
  int startTime = time(NULL);
  garbageCollector(memory);
  process = (MemorySpace*) malloc(sizeof(MemorySpace));
  process->type = P;
  process->id = id;
  process->label = label;
  process->size = size;
  process->duration = duration;
  if(mode == 1) // caso o initializeProcess tenha sido chamado pelo compact
    process->initialTime = initTime;
  else // caso o initializeProcess tenha sido chamado pelo readSwap ou newProcess
    process->initialTime = startTime;
  process->next = process;
  process->prev = process;
  if(spaceVerify(memory, process->size)){ // verifica se há espaço livre na memoria, suficiente para alicar o novo processo;
    i = findSpace(memory, process->size); // Verifica se há necessidade de compactar/organizar memoria
    if(i == -1){
      compactMemory(memory, process); //organiza procesos na memoria para alocar o novo processo;
      return;
    }
    aux = memory->first;
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
          if(mode != 1)
            logRegister(process, 1); // registra inicializacao do processo no log
          return;
        } else if(aux->size == process->size){ //Insercao quando o espaço disponível for igual o tamanho do processo;
          aux->type = P; //transforma o buraco em Processo e passa as informações
          aux->label = label;
          aux->duration = duration;
          aux->initialTime = startTime;
          if(mode != 1)
            logRegister(aux, 1); // registra inicializacao do processo no log
          return;
          }
      }
      aux = aux->next;
    }while(aux != memory->first);
  }else{
    printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
    printf("|________________________________________________________|\n");
    printf("|                                                        |\n");
    printf("|   Não foi possível alocar memória para o processo: %c  |\n", process->label);
    printf("|________________________________________________________|\n");
    sleep(3);
    system("clear");
    idGeneretor--; // desconta o incremento do gerador de id
    free(process); //libera memoria alocada para o processo que não cabe na memoria
    return;
  }
}

void compactMemory(Memory *memory, MemorySpace *process ){
  FILE *fp;
  MemorySpace *p;
  int size, duration, id, initialTime  ;
  char label;

  p = memory->first;
  fp = fopen ("temp.txt", "w");
  if (fp == NULL) {
     printf ("Erro na compactação\n");
     sleep(2);
     exit(0);
     fclose (fp);
  }
  do{ //pecorre a lista copiando processos para o arquivo temporario
    if(p->type == P){
      fprintf(fp, "%d %c %d %d %d\n",p->id, p->label, p->size, p->duration,p->initialTime); // gurada processos em um arquivo temporario
    }
    p = p->next;
    free(p->prev); // libera espaço da lista
  }while(p != memory->first);
  fclose (fp);
  MemorySpace *hole  = (MemorySpace*) malloc(sizeof(MemorySpace)); // cria um espaco vazio na memoria;
  hole->type = H;           //
  hole->startAt = 0;        // Criando espaco vazio do tamanho da memória
  hole->size = MEMORY_SIZE; //
  hole->next = hole;        //
  hole->prev = hole;        // Aponta para ele mesmo
  hole->id = process->id;   //
  memory->first = hole;     //
  memory->last = hole;      // Como e unico, ele e o ultimo
  fp = fopen ("temp.txt", "r");
  while( (fscanf(fp,"%d %c %d %d %d\n",&id, &label, &size, &duration, &initialTime))!=EOF )
    initializeProcess(memory, id, label, size, duration, initialTime, 1); // inicializa processos do arquivo temporario, com mode 1
  fclose (fp);                                                            //para manter o tempo inicial e não registrar inicialização no log
  remove("temp.txt"); // remove arquivo temporario
  // inicializa processo que antes da compctação não caberia na memoria
  initializeProcess(memory, process->id, process->label, process->size, process->duration, 0, 0);
}

int findSpace(Memory *memory, int size){
  /*Verifica se há espaço livre CONTINUO na memoria para o novo processo
  */
  MemorySpace *p;
  p = memory->first;
  do{
    if(p->type == H && p->size >= size){  // verifica se o espaco livre cabe o processo
      return p->startAt; // caso sim, retorna o endereco
    }
    p = p->next;
  }while(p != memory->first);
  return -1; // caso nenhum buraco caiba o processo retorna -1
}

int spaceVerify(Memory *memory, int processSize){
  if(memory->free_space >= processSize) // Se houver espaco livre para o processo retorna 1, caso não 0
    return 1;
  else
    return 0;
}

void garbageCollector(Memory *memory){
  int currentTime = time(NULL); // define currentTime como o tempo atual
  MemorySpace *p;
  p = memory->first;
  do{ //pecorre a memoria
    if(p->type == P){
      if(p->duration + p->initialTime <= currentTime){ // verifica se o processo atingiu o tempo de execucao definido
        shutProcess(memory, p->id); // encerra o processo que atingiu o tempo de execucao definido
      }
    }
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
    p = p->next;
  }while(p != memory->first);
  return NULL;
}

void mergeHole(Memory *memory, MemorySpace *p) {
  /*Memory *memory -> Cabecalho;
    MemorySpace *p -> processo;
    A função a partir do processo p passado como parametro, caso o vizinho da esquerda seja
    um buraco o p aponta para o vizinho a esquerda, e então mescla p->next.
  */
  MemorySpace *aux;
  aux = p;
  while(p->prev->type == H && p != memory->first){ //Se o anterior for um buraco, o p passa a apontar para ele;
    p = p->prev;
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
      aux = p->next;
    }else if(aux->type == H && aux->next == p){
      p->next = p;
      p->prev = p;
      if(p->size != aux->size){
        p->size += aux->size;
      }
      if(aux == memory->last) memory->last = p;
      aux = p->next;
    }else{
      return;
    }
  }while(aux->type != P && aux != p);
}

void callShutProcess(Memory *memory) {
  int id;
  printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
  printf("|________________________________________________________|\n");
  printf("|                                                        |\n");
  printf("|    Digite o ID do processo que deseja encerrar:");
  scanf(" %d",&id);
  shutProcess(memory, id);
}

void shutProcess(Memory *memory, int id){
  MemorySpace *p;
  p = getProcess(memory, id);
  if(p != NULL){
    p->type = H;
    logRegister(p, 2); // registra no log o encerramento do processo
    if(p->prev->type == H || p->next->type == H){
      mergeHole(memory, p); // mescla buracos viznhos
    }
    freeSpaceCounter(memory); // conta espaço livre na memoria
    return;
  }
  printf("|                                                        |\n");
  printf("|   Processo não encontrado!                             |\n");
  printf("|________________________________________________________|\n");
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
    memory->free_space += p->size; // incremneta o espaco livre com o tamnhao de cada buraco na memoria
    }
  p = p->next;
  }while(p != memory->first);
}

void callShowMemory(Memory *memory){
  pthread_t t;
  if (pthread_create(&t, NULL, showMemory, (void *)&memory)) { // Cria thread para rodar a showMemory
   perror("pthread_create1");
   exit(EXIT_FAILURE);
  }
  pthread_join(t,NULL); // aguarda retorno da thread que roda a showMemory
  return;
}

void *closeThread(){
  getchar();
while(closeThreads){
  if(getchar() == '\n'){
    closeThreads = 0; // Torna falsa a condicao do loop que da showMemory
    return NULL;
  }else{
    continue; // caso a tecla nao seja enter, continua aguardando
  }
}
  return NULL;
}

void *showMemory(Memory *memory){
  MemorySpace *p;
  int i, eq;;
  pthread_t t2;
  if (pthread_create(&t2, NULL, (void*)closeThread, NULL)) { // cria trhead para ler entrada do usuario até ele teclar Enter
   perror("pthread_create1");
   exit(EXIT_FAILURE);
  }
  while(closeThreads){
    p = memory->first;
    garbageCollector(memory);
    printf("\n|                 GERENCIADOR DE MEMÓRIA                 |\n" );
    printf("|________________________________________________________|\n");
    printf("| ID | Processo | Tamanho | Endereço | Tempo de Execução |\n");
    printf("|____|__________|_________|__________|___________________|\n");

    do{
      if(p->type == P){
        printf("|%3d |%9c |%8d |%9d |%17lds |\n",p->id, p->label, p->size, p->startAt, time(NULL) - p->initialTime);
        printf("|____|__________|_________|__________|___________________|\n");
      }
      if(p == p->next && p->type == H){
        printf("|                                                        |\n");
        printf("|              NÃO HÁ PROCESSOS EM EXECUÇÃO              |\n" );
        printf("|                                                        |\n");
        printf("|________________________________________________________|\n");
      }

      p = p->next;
    }while(p != memory->first);
    p = memory->first;
    printf("| Uso da Memória: %3d%%                                   |\n|",(100*(MEMORY_SIZE - memory->free_space))/MEMORY_SIZE);

    do{ // pecorre a lista para exibir a posicao dos processo na memoria
      eq = (int) (56*p->size)/MEMORY_SIZE; // numero de caracteres a serem exibidos, fazendo relacao entre o tamanho do processo e o espaco disponivel para exibicao

      if(p->type == P){
        for(i=0;i<=eq;i++)
          printf("%c", p->label); // caso seja processo exibe a label do processo
      }
      else if(p->type == H){
        for(i=0;i<eq;i++)
          printf("-"); // caso seja buraco exibe '-'.
      }
        p = p->next;
    }while(p != memory->first);
    printf("|\n|________________________________________________________|\n");
    printf("|Memória Livre:  %5d Kbits                             |\n", memory->free_space);
    printf("|Memória Ocupada:%5d Kbits                             |\n", MEMORY_SIZE - memory->free_space);
    printf("|Memória Total:  %5d Kbits                             |\n", MEMORY_SIZE);

    printf("|________________________________________________________|\n");
    printf("|\n|  >> Aperte Enter para continuar.\n");
    sleep(1);
    system("clear");
  }
  pthread_join(t2,NULL); // aguarda retorno da closeThread
  closeThreads = 1; // torna a condicao do loop de exibicao verdadeira
  return NULL;
}

void logRegister(MemorySpace *p, int cod){
  /*
  cod 0: Sistema Iniciado
  cod 1: Processo Criado
  cod 2: Processo Encerrado
  cod 3: Sistema Encerrado
  */

  time_t nowTime=time(NULL);
  char *now=ctime(&nowTime); // converte tempo em string formatada
  now[strlen(now)-1]=0;        // remove \n
  FILE *fp;
  fp = fopen ("log.txt", "a");
  if (fp == NULL) {
     printf ("Houve um erro ao registrar log em disco.\n");
     fclose (fp);
     return;
  }
  switch (cod) {
    case 0: fprintf(fp, "%s -> Sistema Iniciado\n",now);
            break;
    case 1: fprintf(fp, "%s -> Processo Criado:    id:%4d, label:%2c, tamanho:%4d, duração: %4d\n",now, p->id, p->label, p->size, p->duration);
            break;
    case 2: fprintf(fp, "%s -> Processo Encerrado: id:%4d, label:%2c, tamanho:%4d, duração: %4d\n",now, p->id, p->label, p->size, p->duration);
            break;
    case 3: fprintf(fp, "%s -> Sistema Encerrado\n",now);
            break;
  }
  fclose (fp);
  return ;
}
void showLog(){
  FILE *fp;
  char line[100];
  fp = fopen ("log.txt", "r");
  if (fp == NULL) {
     printf ("Houve um erro ao ler log em disco.\n");
     fclose (fp);
     return;
  }
  while( fgets(line, 100, fp)!=NULL ) printf("| %s", line); // Ler uma linha por vez e mostra na tela;
  fclose (fp);
  printf("\n|  >> Aperte Enter para continuar.");
  getchar();
  getchar();
  return;
}
