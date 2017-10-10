/***********************************************************/
/*                   ESTRUTURA DE DADOS                    */
/*             Projeto Gerenciador de Memória              */
/*                                                         */
/*                                                         */
/*                             Andrew Lucas Guedes de Souza*/
/*                                    Matrícula: 160023921 */
/***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_SIZE 10

typedef struct {
  char label;
  int size;
  int duration;
  int adress;
}process;

typedef struct {
  char memory[MEMORY_SIZE];
  int ini;
  int tam;
}memory;


process createProcess(){
  process new;
  printf("Label (A-Z):\n");
  scanf("%c",&new.label);
  printf("Size (Kbytes):\n");
  scanf("%d",&new.size);
  printf("Duration (seconds):\n");
  scanf("%d",&new.duration);
  new.adress = 0;
  return new;
}

memory addProcess(process proc, memory memo){
  if (memo.tam + proc.size >= MEMORY_SIZE) {
    printf("Miss memory!");
    return 0;
  }
  proc.adress = (memo.ini + memo.tam) % MEMORY_SIZE;
  memo.memory[(memo.ini + memo.tam)%MEMORY_SIZE ] = proc;
  memo.tam++;
  return memo;
}

process getProcessPosition(char label, memory memo){
  process proc;
  int i;
  for(i=0;i<MEMORY_SIZE;i++){
    if(label == memo[i].label){
      return i;
    }
  }
}

memory removeProcess(char label, memory memo){
  process proc;
  if (memo.tam == 0) {
    printf("Nothing in memory!");
    return 0;
  }

  memo.memory[(memo.ini + memo.tam) % MEMORY_SIZE ] = proc;
  memo.tam++;
  return memo;
}

memory initMemory(){
  memory new;
  new.ini = 0;
  new.tam = 0;
  return new;
}

memory manager(){
  memory memory = initMemory();

}
int main() {

  return 0;
}
