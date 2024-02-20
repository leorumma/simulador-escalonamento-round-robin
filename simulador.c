#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0
#define MAX_PROCESSOS 10
int QUANTUM  = 0;
int quantidadeProcessos = 0;
#define DISCO_IO_DURATION 4
#define FITA_IO_DURATION 6
#define IMPRESSORA_IO_DURATION 10
#define TEMPO_ENTRADA_MAX 5
#define TOTAL_CPU_MIN 1
#define TOTAL_CPU_MAX 10
#define TOTAL_OPERACOES_ENTRADA_SAIDA_MAX 2
#define TIPO_ENTRADA_SAIDA_NUM 3
#define QUANTIDADE_MAXIMA_PROCESSOS 6
#define TEMPO_CHAMADA_IO 3

typedef struct Process Process;

typedef struct ProcessIO ProcessIO;

typedef enum {
    PRONTO,
    EXECUTANDO,
    BLOQUEADO,
    CONCLUIDO
} StatusProcesso;

typedef enum {
    DISCO,
    FITA,
    IMPRESSORA
} TipoEntradaSaida;

typedef enum {
    ALTA_PRIORIDADE,
    BAIXA_PRIORIDADE,
    IO
} FilaTipo;

typedef struct ProcessIO {
    int ppid[3];
    TipoEntradaSaida tipo;
    int totalDuracao;
    int inicio;
    int totalExecucao;
    int fim;
    struct Process* processoPai;
    struct ProcessIO* proximo; // Ponteiro para o próximo ProcessIO na lista
} ProcessIO;


typedef struct Process {
    char pid[3];
    int prioridade;
    StatusProcesso status;
    int tempoEntrada;
    int totalCPUNecessario;
    int tempoConclusao;
    int tempoCPUUtilizado;
    int totalOperacoesEntradaSaida;
    int tempQuantum;
    float turnaroundTime;
    ProcessIO* operacoesEntradasSaidas; // Ponteiro para a primeira ProcessIO na lista
    ProcessIO* operacaoEntradaSaidaAtual; // Ponteiro para a ProcessIO atual na lista
} Process;

typedef struct FILA {
    Process* processo;
    struct FILA *prox;
} Fila;


Process* criarProcesso(int id);

int getTotalDuracaoIO(TipoEntradaSaida tipo);

void removerProcessIO(Process* p, ProcessIO* io);

void processarRoundRobin(int quantidadeProcessos, int *tempoExecucao, Process **pProcess, Fila **pFila, Fila **pFila1, Fila **pFila2, Fila **pFila3, Fila **pFila4, Fila **filaProcessosConcluidos);


Process** gerarProcessos(int quantidadeProcessos);

Fila* inicializarFila();

const char* TipoEntradaSaidaParaString(TipoEntradaSaida tipo);

void printConsoleIncializacaoProcessos(Process** processos, int quantidadeProcessos);

void adicionarNaFila(Fila** fila, Process* processo);

void adicionarNovosProcessos(int quantidadeProcessos, Process **processos, int tempo, Fila **filaAltaPrioridade);

void processarFilaCPU(int tempo, int *quantidadeProcessosFinalizados, Process **processoSendoExecutado, Fila **filaBaixaPrioridade, Fila **filaAltaPrioridade, Fila **filaProcessosConcluidos);

bool isEmpty(Fila* fila);

Process* getProcessoFromFila(Fila** fila);

void imprimirProcessosConcluidos(int quantidadeProcessos, int tempoExecucao, Fila **filaProcessosConcluidos);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if(argc != 3) {
        printf("Uso: %s <quantidadeProcessos> <quantum>\n", argv[0]);
        return 1;
    }
    quantidadeProcessos = atoi(argv[1]);
    QUANTUM = atoi(argv[2]);
    int tempoExecucao = 0;
    Process** processos = gerarProcessos(quantidadeProcessos);
    Fila *filaBaixaPrioridade = inicializarFila();
    Fila *filaAltaPrioridade = inicializarFila();
    Fila *filaDisco = inicializarFila();
    Fila *filaImpressora = inicializarFila();
    Fila *filaFita = inicializarFila();
    Fila *filaProcessosConcluidos = inicializarFila();
    printConsoleIncializacaoProcessos(processos, quantidadeProcessos);
    processarRoundRobin(quantidadeProcessos, &tempoExecucao, processos,&filaAltaPrioridade,&filaBaixaPrioridade,&filaImpressora,&filaDisco,&filaFita, &filaProcessosConcluidos);
    imprimirProcessosConcluidos(quantidadeProcessos, tempoExecucao, &filaProcessosConcluidos);
    return 0;
}

Process** gerarProcessos(int quantidadeProcessos) {
    Process** processos = malloc(quantidadeProcessos * sizeof(Process*));
    if (processos == NULL) {
        printf("Erro na alocação de memória para os processos.\n");
        exit(1);
    }
    for (int i = 0; i < quantidadeProcessos; ++i) {
        processos[i] = criarProcesso(i);
    }
    return processos;
}

Fila* inicializarFila() {
    return NULL;
}

Process* criarProcesso(int id) {
    Process* p = (Process*) malloc(sizeof(Process));
    if (p == NULL) {
        printf("Erro na alocação de memória para o processo.\n");
        exit(1);
    }
    sprintf(p->pid, "P%d", id);
    p->prioridade = 0;
    p->status = PRONTO;
    p->tempoEntrada = rand() % (TEMPO_ENTRADA_MAX + 1); // de 0 ate 5
    p->totalCPUNecessario = rand() % (TOTAL_CPU_MAX - TOTAL_CPU_MIN + 1) + TOTAL_CPU_MIN; // de 1 ate 10
    p->tempoConclusao = 0;
    p->tempoCPUUtilizado = 0;
    p->totalOperacoesEntradaSaida = rand() % (TOTAL_OPERACOES_ENTRADA_SAIDA_MAX + 1); // de 1 ate 2
    p->operacoesEntradasSaidas = NULL;
    p->tempQuantum = 0;
    ProcessIO* ultimoIO = NULL;
    for (int i = 0; i < p->totalOperacoesEntradaSaida; i++) {
        ProcessIO* io = (ProcessIO*) malloc(sizeof(ProcessIO));
        if (io == NULL) {
            printf("Erro na alocação de memória para a operação de entrada/saída.\n");
            exit(1);
        }
        io->tipo = (TipoEntradaSaida) (rand() % TIPO_ENTRADA_SAIDA_NUM);
        io->totalDuracao = getTotalDuracaoIO(io->tipo);
        io->inicio = 0;
        io->totalExecucao = 0;
        io->fim = 0;
        io->processoPai = p;
        io->proximo = NULL;
        if (ultimoIO == NULL) {
            // Se esta é a primeira operação de entrada/saída, ela se torna a cabeça da lista
            p->operacoesEntradasSaidas = io;
        } else {
            // Caso contrário, ela é anexada ao final da lista
            ultimoIO->proximo = io;
        }
        ultimoIO = io;
    }
    p->operacaoEntradaSaidaAtual = p->operacoesEntradasSaidas; // A primeira operação de entrada/saída é a atual
    return p;
}

int getTotalDuracaoIO(TipoEntradaSaida tipo) {
    if (IMPRESSORA == tipo) {
        return IMPRESSORA_IO_DURATION;
    }
    if (FITA == tipo) {
        return FITA_IO_DURATION;
    }
    if (DISCO == tipo) {
        return DISCO_IO_DURATION;
    }
    return 0;
}

void printConsoleIncializacaoProcessos(Process** processos, int quantidadeProcessos) {
    printf("Os processos foram criados: \n");
    for (int i = 0; i < quantidadeProcessos; ++i) {
        Process* processo = processos[i];
        printf("Processo %s:\n", processo->pid);
        printf("  Prioridade: %d\n", processo->prioridade);
        printf("  Status: %d\n", processo->status);
        printf("  Tempo de entrada: %d\n", processo->tempoEntrada);
        printf("  Total de CPU necessário: %d\n", processo->totalCPUNecessario);
        printf("  Tempo de conclusão: %d\n", processo->tempoConclusao);
        printf("  Tempo de CPU utilizado: %d\n", processo->tempoCPUUtilizado);
        printf("  Total de operações de entrada/saída: %d\n", processo->totalOperacoesEntradaSaida);
        ProcessIO* io = processo->operacoesEntradasSaidas;
        while (io != NULL) {
            printf("  Operação de entrada/saída:\n");
            printf("    Tipo: %s\n", TipoEntradaSaidaParaString(io->tipo));
            printf("    Duração total: %d\n", io->totalDuracao);
            printf("    Início: %d\n", io->inicio);
            printf("    Execução total: %d\n", io->totalExecucao);
            printf("    Fim: %d\n", io->fim);
            io = io->proximo;
        }
    }
}

void removerProcessIO(Process* p, ProcessIO* io) {
    if (p->operacoesEntradasSaidas == io) {
        // Se o ProcessIO a ser removido é o primeiro na lista
        p->operacoesEntradasSaidas = io->proximo;
    } else {
        // Se o ProcessIO a ser removido está no meio ou no fim da lista
        ProcessIO* anterior = p->operacoesEntradasSaidas;
        while (anterior->proximo != io) {
            anterior = anterior->proximo;
        }
        anterior->proximo = io->proximo;
    }
    free(io); // Libera a memória alocada para o ProcessIO
}

void processarRoundRobin(int quantidadeProcessos, int *tempoExecucao, Process **processos, Fila **filaAltaPrioridade, Fila **filaBaixaPrioridade, Fila **filaImpressora, Fila **filaDisco, Fila **filaFita, Fila **filaProcessosConcluidos) {
    int tempo = 0;
    int quantidadeProcessosFinalizados = 0;
    Process* processoSendoExecutado = NULL;
    while (quantidadeProcessosFinalizados < quantidadeProcessos) {
        adicionarNovosProcessos(quantidadeProcessos, processos, tempo, filaAltaPrioridade);
        processarFilaCPU(tempo, &quantidadeProcessosFinalizados, &processoSendoExecutado, filaBaixaPrioridade, filaAltaPrioridade, filaProcessosConcluidos);
        //todo: executarIOImpressora
        //todo: executarIODisco
        //todo: executarIOFita
        tempo+= 1;
    }
    (*tempoExecucao) = tempo;
}

void processarFilaCPU(int tempo, int *quantidadeProcessosFinalizados, Process **processoSendoExecutado, Fila **filaBaixaPrioridade, Fila **filaAltaPrioridade, Fila **filaProcessosConcluidos) {
    if (*processoSendoExecutado != NULL) {
        if ((*processoSendoExecutado)->totalCPUNecessario == (*processoSendoExecutado)->tempoCPUUtilizado) {
            (*processoSendoExecutado)->status = CONCLUIDO;
            (*processoSendoExecutado)->tempQuantum = 0;
            (*processoSendoExecutado)->tempoConclusao = tempo;
            (*processoSendoExecutado)->turnaroundTime = (float) (*processoSendoExecutado)->tempoConclusao - (float) (*processoSendoExecutado)->tempoEntrada;
            adicionarNaFila(filaProcessosConcluidos, *processoSendoExecutado);
            (*quantidadeProcessosFinalizados)++;
            *processoSendoExecutado = NULL;
        }
        else if ((*processoSendoExecutado)->tempQuantum == QUANTUM) {
            (*processoSendoExecutado)->tempQuantum = 0;
            (*processoSendoExecutado)->status = PRONTO;
            adicionarNaFila(filaBaixaPrioridade, *processoSendoExecutado);
            *processoSendoExecutado = NULL;
        } else {
            (*processoSendoExecutado)->tempoCPUUtilizado += 1;
            (*processoSendoExecutado)->tempQuantum += 1;
        }
    }
    if (*processoSendoExecutado == NULL) {
        if (!isEmpty(*filaAltaPrioridade)) {
            *processoSendoExecutado = getProcessoFromFila(filaAltaPrioridade);
//            imprimirFila(filaAltaPrioridade, "de Alta prioridade");
            return;
        }
        if (!isEmpty(*filaBaixaPrioridade)) {
            *processoSendoExecutado = getProcessoFromFila(filaBaixaPrioridade);
//            imprimirFila(filaBaixaPrioridade, "de baixa prioridade");
            return;
        }
    }
}

void adicionarNovosProcessos(int quantidadeProcessos, Process **processos, int tempo, Fila **filaAltaPrioridade) {
    for (int i = 0; i < quantidadeProcessos; ++i) {
        Process* processo = processos[i];
        if (processo->tempoEntrada == tempo) {
            adicionarNaFila(filaAltaPrioridade, processo);
        }
    }
}


void adicionarNaFila(Fila** fila, Process* processo) {
    // Cria um novo nó para a fila
    Fila* novoNo = (Fila*) malloc(sizeof(Fila));
    processo->status = PRONTO;
    processo->prioridade = 0;
    novoNo->processo = processo; // Agora armazena o ponteiro diretamente, sem copiar o processo
    novoNo->prox = NULL;
    if (*fila == NULL) {
        // Se a fila estiver vazia, o novo nó é o primeiro nó
        *fila = novoNo;
    } else {
        // Caso contrário, adiciona o novo nó ao final da fila
        Fila* atual = *fila;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoNo;
    }
}

const char* TipoEntradaSaidaParaString(TipoEntradaSaida tipo) {
    switch (tipo) {
        case DISCO: return "DISCO";
        case FITA: return "FITA";
        case IMPRESSORA: return "IMPRESSORA";
        default: return "Tipo desconhecido";
    }
}

bool isEmpty(Fila* fila) {
    return (fila == NULL);
}

Process* getProcessoFromFila(Fila** fila) {
    if (*fila == NULL) {
        return NULL;
    }
    Fila* primeiroNo = *fila;
    *fila = primeiroNo->prox; // Atualiza a fila para apontar para o segundo nó
    Process* processo = primeiroNo->processo; // Agora é um ponteiro para Process
    free(primeiroNo); // Libera a memória do nó removido
    return processo;
}


void imprimirProcessosConcluidos(int quantidadeProcessos, int tempoExecucao, Fila **filaProcessosConcluidos) {
    Fila* atual = *filaProcessosConcluidos;
    int totalCPUTempo = 0;
    int totalTurnAround = 0;
    printf("Processos concluídos:\n");
    while (atual != NULL) {
        Process* processo = atual->processo; // Agora é um ponteiro para Process
        totalCPUTempo = processo->totalCPUNecessario + totalCPUTempo;
        totalTurnAround = processo->turnaroundTime + totalTurnAround;
        printf("PID: %s, Tempo de Entrada: %d, Tempo de Conclusão: %d, Total CPU Necessario: %d, Turnaround Time: %.2f \n",
               processo->pid,
               processo->tempoEntrada,
               processo->tempoConclusao,
               processo->totalCPUNecessario,
               processo->turnaroundTime);
        atual = atual->prox;
    }
    float turnaroundMedio = (float) totalTurnAround / (float) quantidadeProcessos;
    float throughput = (float) quantidadeProcessos / (float) tempoExecucao;
    float utilizacaoCPU = ((float ) totalCPUTempo / (float) tempoExecucao) * 100;
    printf("Tempo total de execução: %d \n", tempoExecucao);
    printf("Throughput: %.2f \n", throughput);
    printf("Utilização CPU: %.2f%% \n", utilizacaoCPU);
    printf("Turnaround Time Medio: %.2f \n", turnaroundMedio);

}

