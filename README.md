# Gerenciamento de memória em sistemas operacionais
#### Projeto para a disciplina Estruturas de Dados e Algoritmos
***
#### Alunos:
- Andrew Lucas Guedes de Souza - 16/0023921
- Nivaldo Pereira Lopo Junior - 12/0039460
---
## Relatório de Projeto
#### Índice
[1. Objetivo do projeto](#1-objetivo-do-projeto)  
[2. Requisitos do projeto](#2-requisitos-do-projeto)  
[3. Falhas do projeto](#3-falhas-do-projeto)  
[4. Melhorias do projeto](#4-melhorias-do-projeto)  

#### 1. Introdução do projeto
Um gerenciador de memória de sistemas operacionais, é utilizado para gerir a memória, manipulando espaços na memória, alguns com processos, outros livres. Os processos serão executados por um período de tempo definido pelo usuário, e inserção de um novo processo na memória, será feita utilizando o método *First-Fit*, que utiliza a primeira lacuna que encontrar com tamanho suficiente para alocar o processo, esse método é o que possui melhor performance em comparação com os outros métodos.


#### 1. Objetivo do projeto
<p> O projeto visa o desenvolvimento de um simulador de gerenciamento de memória em sistemas operacionais, utilizando a linguagem C. </p>  

#### 2. Requisitos do projeto
- [x] Os processos devem ter como parâmetros o seu tamanho (em Kbytes) e o tempo de execução (em segundos).

- [x] Os processos devem ser alocados na memória de acordo com o seu tamanho e devem ter um identificador (label).

- [x] O processo deve ser colocado na primeira região de memória (a contar do início para o final da memória) onde esse processo couber.

- [x] O programa deve tentar encontrar uma forma de reorganizar os processos na memória de modo a acomodar o novo processo. Se depois dessa tentativa não houver espaço disponível, deve-se imprimir a seguinte mensagem: "Não foi possível alocar memória para o processo: [label do processo]".

- [ ] O tempo de execução de um processo é aleatório (não previsível). Portanto, caberá ao aluno definir como ele vai simular esse tempo de execução e o término de um processo. Sugere-se ainda que o programa imprima o registro de entrada e saída de processos na memória.

- [x] O programa deve ser capaz de imprimir, a qualquer momento, quais posições de memória estão ocupadas (P) e quais estão livres (H). Imagine que a memória é um grande vetor onde cada posição armazena um Kbyte.

- [x] Além das opções para inclusão de processos na memória e impressão, o programa deve ter a opção de sair (deixar o programa).

- [x]  O programa deve oferecer a opção de gravar os dados em arquivo. E quando o programa for iniciado, deve ser possível ler as informações desse arquivo para continuar a execução da simulação da memória.

- [ ] A gerência de espaços de processo (P) e buracos (H) na memória deve ser feito por meio de uma lista circular duplamente encadeada, com cabeçalho. Nesse caso, o aluno deve desenhar o nó considerando pelo menos: tipo de nó (P ou H), posição de memória inicial, tamanho da área de memória livre ou ocupada.

#### 3. Metodologia
##### 3.1 Estruturas de dados
<div style="text-align: justify">
Para o desenvolvimento do simulador, foi utilizada uma lista circular duplamente duplamente encadeada com cabeçalho, para gerenciar a memória.<br>
O cabeçalho é composto por dois apontadores, um apontando para o primeiro elemento no primeiro endereço da memoria, seja um processo ou um buraco. E outro apontando para o útimo elemento da lista, E por fim um inteiro para registrar a soma de todos os espaçoes livres.<br><br>
Na lista, cada elemento possui:  
</div>

| Nome | Nome na estrutura | Tipo |
|:-:|:-:|:-:|
| tipo | type | P - processo ou H - buraco|
| id | id | int |
| rótulo| label | char |
| tamanho | size | int |
| Endereço| startAt | int |
| Tempo de início | initialTime| int |
| Duração| duration | int |
| Apontador próximo| next | struct MemoryList |
| Apontador anterior| prev | struct MemoryList|

<div style="text-align: justify">
O tipo de estrutura de dados mais recomendado para gerenciamento de memória é uma lista duplamente encadeada, porém para facilitar o processo de pecorrer a lista, tornamos ela circular.<br>
</div>


##### 3.2 Bibliotecas
<div style="text-align: justify">


</div>
| Biblioteca | Descrição | Utilidade |
|:-:|:-:|:-:|
| stdio | Possui definições de subrotinas relativas às operações de entrada/saída, como leitura de dados digitados no teclado e exibição de informações na tela do programa de computador| P - processo ou H - buraco|
| stdlib | id | int |
| time| label | char |
| locale | size | int |
| unistd| startAt | int |
| pthread| initialTime| int |
| string| duration | int |

##### 3.3 Funções
<div style="text-align: justify">
Para o desenvolvimento do simulador, foi utilizada uma lista circular duplamente duplamente encadeada com cabeçalho, para gerenciar a memória.<br>
O cabeçalho é composto por dois apontadores, um apontando para o primeiro elemento no primeiro endereço da memoria, seja um processo ou um buraco. E outro apontando para o útimo elemento da lista, E por fim um inteiro para registrar a soma de todos os espaçoes livres.<br>
Na lista, cada elemento possui um tipo (P - processo ou H - buraco),  
</div>

#### 4. Melhorias do projeto

## Relatórios individuais

#### Andrew Lucas Guedes de Souza 16/0023921
###### Atividades:
-
-
-
###### Estudos:
- Uso de thread para realizar a verificação e encerramento dos processos, que concluiram o tempo de execução.
- Utilização da bilioteca time.h e como ela funciona, para implementar a verificação do tempo de execução de um processo;
-
###### Dificuldades:
- Implementar um método eficiente de compactação da memória
-
-
