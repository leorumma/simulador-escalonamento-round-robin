Segundo Trabalho Prático - Simulador de Escalonamento em C

header.h 
func_struct.c (código base)

1. Objetivo do Trabalho
Estimular a capacidade do aluno de trabalhar em equipe para organizar, projetar e
desenvolver soluções para problemas formulados que envolvam o estudo e o conhecimento
sobre gerenciamentos do sistema operacional.

2. Escopo do Trabalho
Desenvolver um simulador que implementa o algoritmo de escalonamento de processos,
usando a estratégia de seleção Round Robin (ou Circular) com Feedback. Esse simulador deve ser didático o suficiente para alunos no início do curso de Computação entenderem o que está sendo feito a cada iteração. Preparar um relatório contendo uma descrição sobre os objetivos do trabalho, as premissas consideradas no desenvolvimento do escalonador e a saída da execução do simulador. Os trabalhos devem ser feitos exclusivamente em C. Asavaliações sobre o funcionamento dos simuladores serão feitas em horário marcado.

4. Prazo de Entrega do Trabalho
Os materiais (código e relatório) devem ser postados no GDrive, pasta “Trabalho prático 2
2023-3” (link: Trabalho Prático 2 - 2023.3 ) até às 23:59 do dia 18/02/2024, com o título
“Trabalho 2 de ICP246 – 2023-3 - Grupo X”, onde X é o número do grupo, conforme descrito
na planilha “Grupos TP 2”. As apresentações serão realizadas no dia 20/02/2024, no horário
da aula, em ordem aleatória e previamente disponibilizada no mural do Classroom.

5. Penalidades
Caso o grupo atrase a entrega do trabalho seu grau final sofrerá um decréscimo na razão
de 0,5 pontos por dia de atraso.

6. Avaliação
Serão considerados os seguintes aspectos na avaliação do trabalho:
● Execução correta do simulador durante a apresentação - máximo de 10 minutos (3,5
pontos);
● Relatório contendo, minimamente, as premissas do trabalho, a saída do simulador e
as referências utilizadas (3,5 pontos);
● Entrega pontualmente efetuada no dia estipulado (1 ponto);
● Conhecimento utilizado no desenvolvimento do trabalho e distribuição das tarefas
entre os participantes do grupo (2 pontos).

7. Premissas a serem definidas pelo grupo para o desenvolvimento do
simulador
● Limite máximo de processos criados;
● O valor da fatia de tempo dada aos processos em execução;
● Tempos de serviço e de I/O aleatórios para cada processo criado;
● Tempos de duração de cada tipo de I/O (disco, fita magnética e impressora);
● Gerência de Processos
 Definição do PID de cada processo,
Informações do PCB (contexto de software – prioridade, PID, PPID, status);
Escalonador (pelo menos 3 filas, sendo uma fila de alta e uma de baixa prioridade para execução na CPU, e 1 fila de I/O, que pode ser implementada com filas diferentes para cada tipo de dispositivo);

● Tipos de I/O
 Disco – retorna para a fila de baixa prioridade;
Fita magnética - retorna para a fila de alta prioridade;
Impressora - retorna para a fila de alta prioridade;
● Ordem de entrada na fila de prontos
Processos novos - entram na fila de alta prioridade;
Processos que retornam de I/O – dependente do tipo de I/O solicitado;
Processos que sofreram preempção – retornam na fila de baixa prioridade.

OBS: As premissas estabelecidas pelo grupo devem estar explícitas no relatório.

8. Saída esperada do simulador
O simulador deve apresentar as informações dos processos criados no início, com o
tempo de CPU e instante e tipo da(s) E/S.

Além disso, a cada unidade de tempo, deve apresentar os eventos ocorridos
naquela unidade de tempo: se teve processo admitindo, se teve processo acabando E/S, se
teve processo preemptivo, etc, e colocar também o processo que está em execução e como
estão todas as filas do simulador.

Por fim, ao final da simulação deve-se calcular o turnaround de cada processo.
