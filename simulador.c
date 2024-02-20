#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0
int QUANTUM  = 0;
int quantidadeProcessos = 0;
int exibicaoHistorico = 0;
#define DISCO_IO_DURATION 4
#define FITA_IO_DURATION 6
#define IMPRESSORA_IO_DURATION 10
#define TEMPO_ENTRADA_MAX 5
#define TOTAL_CPU_MIN 1
#define TOTAL_CPU_MAX 10
#define TOTAL_OPERACOES_ENTRADA_SAIDA_MAX 2
#define TIPO_ENTRADA_SAIDA_NUM 3
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
    char pid[10];
    char ppid[5];
    TipoEntradaSaida tipo;
    int totalDuracao;
    int inicio;
    int totalExecucao;
    int fim;
    Process* processoPai;
    struct ProcessIO* proximo;
} ProcessIO;


typedef struct Process {
    char pid[5];
    int prioridade;
    StatusProcesso status;
    int tempoEntrada;
    int totalCPUNecessario;
    int tempoConclusao;
    int tempoCPUUtilizado;
    int totalOperacoesEntradaSaida;
    int tempQuantum;
    float turnaroundTime;
    ProcessIO* operacoesEntradasSaidas;
    ProcessIO* operacaoEntradaSaidaAtual;
} Process;

typedef struct FILA {
    Process* processo;
    struct FILA *prox;
} Fila;

typedef struct FILA_IO {
    ProcessIO* operacaoIO;
    struct FILA_IO *prox;
} FilaIO;

Process* criarProcesso(int id);

int getTotalDuracaoIO(TipoEntradaSaida tipo);

void removerProcessIO(Process* p, ProcessIO* io);

void processarRoundRobin(int quantidadeProcessos, int *tempoExecucao, Process **processos, Fila **filaAltaPrioridade, Fila **filaBaixaPrioridade, Fila **filaBloqueado, FilaIO **filaImpressora, FilaIO **filaDisco, FilaIO **filaFita, Fila **filaProcessosConcluidos);

Process** gerarProcessos(int quantidadeProcessos);

Fila* inicializarFila();

FilaIO* inicializarFilaIO();

const char* TipoEntradaSaidaParaString(TipoEntradaSaida tipo);

void printConsoleIncializacaoProcessos(Process** processos, int quantidadeProcessos);

void adicionarNaFila(Fila** fila, Process* processo);

void adicionarNovosProcessos(int quantidadeProcessos, Process **processos, int tempo, Fila **filaAltaPrioridade);

void processarFilaCPU(int tempo, int *quantidadeProcessosFinalizados, Process **processoSendoExecutado, Fila **filaBaixaPrioridade, Fila **filaAltaPrioridade, Fila **filaBloqueado, FilaIO **filaImpressora, FilaIO **filaDisco, FilaIO **filaFita, Fila **filaProcessosConcluidos);

bool isEmpty(Fila* fila);

bool isIoEmpty(FilaIO* fila);

Process* getProcessoFromFila(Fila** fila);

ProcessIO* getIOFromFila(FilaIO** fila);

void imprimirProcessosConcluidos(int quantidadeProcessos, int tempoExecucao, Fila **filaProcessosConcluidos);

void imprimirFila(Fila** fila, char* nomeFila);

void imprimirIO(FilaIO ** fila, char* nomeFila);

ProcessIO* getIOFromProcesso(Process **processoSendoExecutado);

void adicionarIOFila(FilaIO** fila, ProcessIO* operacaoIO);

void executarIO(ProcessIO **ioAtual, FilaIO **filaIO, Fila **filaAltaPrioridade, Fila **filaBaixaPrioridade, Fila **filaBloqueado);

Process* getProcessByPidAndFila(char* pid, Fila** fila);

void imprimirHistoricoFilasExecucao(int tempo,  Process **processoSendoExecutado, ProcessIO **impressoraIOAtual, ProcessIO **fitaIoAtual, ProcessIO **discoIOAtual, Fila **filaBaixaPrioridade, Fila **filaAltaPrioridade, Fila **filaBloqueado, FilaIO **filaImpressora, FilaIO **filaDisco, FilaIO **filaFita, Fila **filaProcessosConcluidos);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if(argc < 3) {
        printf("Uso: %s <quantidadeProcessos> <quantum> \n", argv[0]);
        return 1;
    }
    quantidadeProcessos = atoi(argv[1]);
    QUANTUM = atoi(argv[2]);
    if (argc == 4) {
        exibicaoHistorico = atoi(argv[3]);
    }
    int tempoExecucao = 0;
    Process** processos = gerarProcessos(quantidadeProcessos);
    Fila *filaBaixaPrioridade = inicializarFila();
    Fila *filaAltaPrioridade = inicializarFila();
    Fila *filaBloqueado = inicializarFila();
    FilaIO *filaDisco = inicializarFilaIO();
    FilaIO *filaImpressora = inicializarFilaIO();
    FilaIO *filaFita = inicializarFilaIO();
    Fila *filaProcessosConcluidos = inicializarFila();
    printConsoleIncializacaoProcessos(processos, quantidadeProcessos);
    processarRoundRobin(quantidadeProcessos, &tempoExecucao, processos, &filaAltaPrioridade, &filaBaixaPrioridade, &filaBloqueado, &filaImpressora, &filaDisco, &filaFita, &filaProcessosConcluidos);
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

FilaIO* inicializarFilaIO() {
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
        sprintf(io->pid, "IO%s%d", p->pid, i);
        strcpy(io->ppid, p->pid);
        io->tipo = (TipoEntradaSaida) (rand() % TIPO_ENTRADA_SAIDA_NUM);
        io->totalDuracao = getTotalDuracaoIO(io->tipo);
        io->inicio = rand() % (TEMPO_CHAMADA_IO + 2 + io->tipo);
        io->totalExecucao = 0;
        io->fim = 0;
        io->processoPai = p;
        io->proximo = NULL;
        if (ultimoIO == NULL) {
            p->operacoesEntradasSaidas = io;
        } else {
            ultimoIO->proximo = io;
        }
        ultimoIO = io;
    }
    p->operacaoEntradaSaidaAtual = p->operacoesEntradasSaidas;
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

bool isInterrupcaoIO(Process **processoSendoExecutado);

void processarRoundRobin(int quantidadeProcessos, int *tempoExecucao, Process **processos, Fila **filaAltaPrioridade, Fila **filaBaixaPrioridade, Fila **filaBloqueado, FilaIO **filaImpressora, FilaIO **filaDisco, FilaIO **filaFita, Fila **filaProcessosConcluidos) {
    int tempo = 0;
    int quantidadeProcessosFinalizados = 0;
    Process* processoSendoExecutado = NULL;
    ProcessIO* impressoraIOAtual = NULL;
    ProcessIO* fitaIoAtual = NULL;
    ProcessIO* discoIOAtual = NULL;
    while (quantidadeProcessosFinalizados < quantidadeProcessos) {
        adicionarNovosProcessos(quantidadeProcessos, processos, tempo, filaAltaPrioridade);
        processarFilaCPU(tempo, &quantidadeProcessosFinalizados, &processoSendoExecutado, filaBaixaPrioridade, filaAltaPrioridade, filaBloqueado, filaImpressora, filaDisco, filaFita, filaProcessosConcluidos);
        executarIO(&impressoraIOAtual, filaImpressora, filaAltaPrioridade, filaBaixaPrioridade, filaBloqueado);
        executarIO(&fitaIoAtual, filaFita, filaAltaPrioridade, filaBaixaPrioridade, filaBloqueado);
        executarIO(&discoIOAtual, filaDisco, filaAltaPrioridade, filaBaixaPrioridade, filaBloqueado);
        imprimirHistoricoFilasExecucao(tempo, &processoSendoExecutado, &impressoraIOAtual, &fitaIoAtual, &discoIOAtual, filaBaixaPrioridade, filaAltaPrioridade, filaBloqueado, filaImpressora, filaDisco, filaFita, filaProcessosConcluidos);
        tempo+= 1;
    }
    (*tempoExecucao) = tempo;
}

void processarFilaCPU(int tempo, int *quantidadeProcessosFinalizados, Process **processoSendoExecutado, Fila **filaBaixaPrioridade, Fila **filaAltaPrioridade, Fila **filaBloqueado, FilaIO **filaImpressora, FilaIO **filaDisco, FilaIO **filaFita, Fila **filaProcessosConcluidos) {
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
        else if (isInterrupcaoIO(processoSendoExecutado)) {
            (*processoSendoExecutado)->tempQuantum = 0;
            (*processoSendoExecutado)->status = BLOQUEADO;
            adicionarNaFila(filaBloqueado, *processoSendoExecutado);
            ProcessIO* operacaoIO = getIOFromProcesso(processoSendoExecutado);
            (*processoSendoExecutado)->operacaoEntradaSaidaAtual = operacaoIO;
            if (IMPRESSORA == operacaoIO->tipo) {
                adicionarIOFila(filaImpressora, operacaoIO);
            }
            if (DISCO == operacaoIO->tipo) {
                adicionarIOFila(filaDisco, operacaoIO);
            }
            if (FITA == operacaoIO->tipo) {
                adicionarIOFila(filaFita, operacaoIO);
            }
            *processoSendoExecutado = NULL;
        }
        else if ((*processoSendoExecutado)->tempQuantum == QUANTUM) {
            (*processoSendoExecutado)->tempQuantum = 0;
            (*processoSendoExecutado)->status = PRONTO;
            adicionarNaFila(filaBaixaPrioridade, *processoSendoExecutado);
            *processoSendoExecutado = NULL;
        }
        else {
            (*processoSendoExecutado)->status = EXECUTANDO;
            (*processoSendoExecutado)->tempoCPUUtilizado += 1;
            (*processoSendoExecutado)->tempQuantum += 1;
        }
    }
    if (*processoSendoExecutado == NULL) {
        if (!isEmpty(*filaAltaPrioridade)) {
            *processoSendoExecutado = getProcessoFromFila(filaAltaPrioridade);
            return;
        }
        if (!isEmpty(*filaBaixaPrioridade)) {
            *processoSendoExecutado = getProcessoFromFila(filaBaixaPrioridade);
            return;
        }
    }
}

void adicionarNovosProcessos(int quantidadeProcessos, Process **processos, int tempo, Fila **filaAltaPrioridade) {
    for (int i = 0; i < quantidadeProcessos; ++i) {
        Process* processo = processos[i];
        if (processo->tempoEntrada == tempo) {
            adicionarNaFila(filaAltaPrioridade, processo);
            processo->status = PRONTO;
            processo->prioridade = 0;
        }
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

bool isIoEmpty(FilaIO* fila) {
    return (fila == NULL);
}

Process* getProcessoFromFila(Fila** fila) {
    if (*fila == NULL) {
        return NULL;
    }
    Fila* primeiroNo = *fila;
    *fila = primeiroNo->prox;
    Process* processo = primeiroNo->processo;
    free(primeiroNo);
    return processo;
}


void imprimirProcessosConcluidos(int quantidadeProcessos, int tempoExecucao, Fila **filaProcessosConcluidos) {
    Fila* atual = *filaProcessosConcluidos;
    int totalCPUTempo = 0;
    int totalTurnAround = 0;
    printf("Processos concluídos:\n");
    while (atual != NULL) {
        Process* processo = atual->processo;
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

bool isInterrupcaoIO(Process **processoSendoExecutado) {
    ProcessIO* operacaoIO = (*processoSendoExecutado)->operacoesEntradasSaidas;
    while (operacaoIO != NULL) {
        if (operacaoIO->inicio == (*processoSendoExecutado)->tempoCPUUtilizado) {
            return true;
        }
        operacaoIO = operacaoIO->proximo;
    }
    return false;
}

void imprimirFila(Fila** fila, char* nomeFila) {
    Fila* atual = *fila;
    printf("Processos na fila %s: [", nomeFila);
    if (atual != NULL) {
        printf("%s", atual->processo->pid);
        atual = atual->prox;
    }
    while (atual != NULL) {
        printf(", %s", atual->processo->pid);
        atual = atual->prox;
    }
    printf("]\n");
}

void imprimirIO(FilaIO ** fila, char* nomeFila) {
    FilaIO* atual = *fila;
    printf("Processos na fila %s: [", nomeFila);
    if (atual != NULL) {
        printf("%s", atual->operacaoIO->pid);
        atual = atual->prox;
    }
    while (atual != NULL) {
        printf(", %s", atual->operacaoIO->pid);
        atual = atual->prox;
    }
    printf("]\n");
}

ProcessIO* getIOFromProcesso(Process **processoSendoExecutado) {
    ProcessIO* operacaoIO = (*processoSendoExecutado)->operacoesEntradasSaidas;
    ProcessIO* anteriorIO = NULL;
    while (operacaoIO != NULL) {
        if (operacaoIO->inicio == (*processoSendoExecutado)->tempoCPUUtilizado) {
            if (anteriorIO == NULL) {
                (*processoSendoExecutado)->operacoesEntradasSaidas = operacaoIO->proximo;
            } else {
                anteriorIO->proximo = operacaoIO->proximo;
            }
            operacaoIO->proximo = NULL;
            return operacaoIO;
        }
        anteriorIO = operacaoIO;
        operacaoIO = operacaoIO->proximo;
    }
    return NULL;
}

void adicionarNaFila(Fila** fila, Process* processo) {
    Fila* novoNo = (Fila*) malloc(sizeof(Fila));
    novoNo->processo = processo;
    novoNo->prox = NULL;
    if (*fila == NULL) {
        *fila = novoNo;
    } else {
        Fila* atual = *fila;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoNo;
    }
}

void adicionarIOFila(FilaIO** fila, ProcessIO* operacaoIO) {
    FilaIO* novoNo = (FilaIO*) malloc(sizeof(FilaIO));
    novoNo->operacaoIO = operacaoIO;
    novoNo->prox = NULL;
    if (*fila == NULL) {
        *fila = novoNo;
    } else {
        FilaIO* atual = *fila;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoNo;
    }
}

void executarIO(ProcessIO **ioAtual, FilaIO **filaIO, Fila **filaAltaPrioridade, Fila **filaBaixaPrioridade, Fila **filaBloqueado) {
    if (*ioAtual != NULL) {
        if ((*ioAtual)->totalDuracao == (*ioAtual)->totalExecucao) {
            Process* processo = getProcessByPidAndFila((*ioAtual)->ppid, filaBloqueado);
            if (IMPRESSORA == (*ioAtual)->tipo || FITA == (*ioAtual)->tipo) {
                adicionarNaFila(filaAltaPrioridade, processo);
            }
            if (DISCO == (*ioAtual)->tipo) {
                adicionarNaFila(filaBaixaPrioridade, processo);
            }
            *ioAtual = NULL;
        } else {
            ((*ioAtual)->totalExecucao++);
        }
    }
    if (*ioAtual == NULL) {
        if (filaIO != NULL) {
            *ioAtual = getIOFromFila(filaIO);
            return;
        }
    }
}

Process* getProcessByPidAndFila(char* pid, Fila** fila) {
    Fila* atual = *fila;
    Fila* anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->processo->pid, pid) == 0) {
            if (anterior == NULL) {
                *fila = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            Process* processo = atual->processo;
            free(atual);
            return processo;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return NULL;
}

ProcessIO* getIOFromFila(FilaIO** fila) {
    if (*fila == NULL) {
        return NULL;
    }
    FilaIO* primeiroNo = *fila;
    *fila = primeiroNo->prox;
    ProcessIO* operacaoIO = primeiroNo->operacaoIO;
    free(primeiroNo);
    return operacaoIO;
}

void imprimirHistoricoFilasExecucao(int tempo,  Process **processoSendoExecutado, ProcessIO **impressoraIOAtual, ProcessIO **fitaIoAtual, ProcessIO **discoIOAtual, Fila **filaBaixaPrioridade, Fila **filaAltaPrioridade, Fila **filaBloqueado, FilaIO **filaImpressora, FilaIO **filaDisco, FilaIO **filaFita, Fila **filaProcessosConcluidos) {
    if (exibicaoHistorico == 0) {
        return;
    }
    printf("Tempo de Execução: %d \n", tempo);
    printf("Executando: %s \n", (*processoSendoExecutado)->pid);
    printf("Executando Impressora: %s \n", (*impressoraIOAtual)->pid);
    printf("Executando Fita: %s \n", (*fitaIoAtual)->pid);
    printf("Executando Disco: %s \n", (*discoIOAtual)->pid);
    imprimirFila(filaAltaPrioridade, "Alta prioridade");
    imprimirFila(filaBaixaPrioridade, "baixa prioridade");
    imprimirFila(filaBloqueado, "bloqueados");
    imprimirIO(filaImpressora, "Impressora");
    imprimirIO(filaDisco, "Disco");
    imprimirIO(filaFita, "Fita");
    imprimirFila(filaProcessosConcluidos, "concluidos");
    printf("=========================== \n");
}