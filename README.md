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

#### 1. Objetivo do projeto
<p> O projeto visa o desenvolvimento de um simulador de gerenciamento de memória em sistemas operacionais, utilizando a linguagem C. </p>  

#### 2. Requisitos do projeto
- [ ] Os processos devem ter como parâmetros o seu tamanho (em Kbytes) e o tempo de execução (em segundos).

- [ ] Os processos devem ser alocados na memória de acordo com o seu tamanho e devem ter um identificador (label).

- [ ] O processo deve ser colocado na primeira região de memória (a contar do início para o final da memória) onde esse processo couber.

- [ ] O programa deve tentar encontrar uma forma de reorganizar os processos na memória de modo a acomodar o novo processo. Se depois dessa tentativa não houver espaço disponível, deve-se imprimir a seguinte mensagem: "Não foi possível alocar memória para o processo: [label do processo]".

- [ ] O tempo de execução de um processo é aleatório (não previsível). Portanto, caberá ao aluno definir como ele vai simular esse tempo de execução e o término de um processo. Sugere-se ainda que o programa imprima o registro de entrada e saída de processos na memória.

- [ ] O programa deve ser capaz de imprimir, a qualquer momento, quais posições de memória estão ocupadas (P) e quais estão livres (H). Imagine que a memória é um grande vetor onde cada posição armazena um Kbyte.

- [ ] Além das opções para inclusão de processos na memória e impressão, o programa deve ter a opção de sair (deixar o programa).

- [ ]  O programa deve oferecer a opção de gravar os dados em arquivo. E quando o programa for iniciado, deve ser possível ler as informações desse arquivo para continuar a execução da simulação da memória.

- [ ] A gerência de espaços de processo (P) e buracos (H) na memória deve ser feito por meio de uma lista circular duplamente encadeada, com cabeçalho. Nesse caso, o aluno deve desenhar o nó considerando pelo menos: tipo de nó (P ou H), posição de memória inicial, tamanho da área de memória livre ou ocupada.

#### 3. Falhas do projeto

#### 4. Melhorias do projeto
