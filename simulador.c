#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0
#define MAX_PROCESSOS 10
#define QUANTUM 5
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

//struct ProcessIO {
//    int ppid[3];
//    TipoEntradaSaida tipo;
//    int totalDuracao;
//    int inicio;
//    int totalExecucao;
//    int fim;
//    Process* processoPai;
//};

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
    ProcessIO* operacoesEntradasSaidas; // Ponteiro para a primeira ProcessIO na lista
    ProcessIO* operacaoEntradaSaidaAtual; // Ponteiro para a ProcessIO atual na lista
} Process;

typedef struct FILA {
    Process processo;
    Process *prox;
} Fila;

Process* criarProcesso(int id);

ProcessIO* criarProcessoIO(Process* process);

void processarRoundRobin(int quantidadeProcessos, Process **pProcess, Fila **pFila, Fila **pFila1, Fila **pFila2, Fila **pFila3, Fila **pFila4);

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

void removerProcessIO(Process* p, ProcessIO* io);

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
    ProcessIO* ultimoIO = NULL;
    for (int i = 0; i < p->totalOperacoesEntradaSaida; i++) {
        ProcessIO* io = (ProcessIO*) malloc(sizeof(ProcessIO));
        if (io == NULL) {
            printf("Erro na alocação de memória para a operação de entrada/saída.\n");
            exit(1);
        }
        io->tipo = (TipoEntradaSaida) (rand() % TIPO_ENTRADA_SAIDA_NUM); // Substitua NUM_TIPOS pelo número de tipos possíveis
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


//ProcessIO* criarProcessoIO(Process* process) {
//    ProcessIO* pio = (ProcessIO*) malloc(sizeof(ProcessIO));
//    if (pio == NULL) {
//        printf("Erro na alocação de memória para o ProcessIO.\n");
//        exit(1);
//    }
//    pio->type = 0;
//    pio->activation_time = 0;
//    pio->duration = 0;
//    pio->priority = 0;
//    pio->process = process;
//    return pio;
//}

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
    Fila* f = (Fila*) malloc(sizeof(Fila));
    if (f == NULL) {
        printf("Erro na alocação de memória para a fila.\n");
        exit(1);
    }
    f->prox = NULL;
    return f;
}

void processarRoundRobin(int quantidadeProcessos, Process **pProcess, Fila **pFila, Fila **pFila1, Fila **pFila2, Fila **pFila3, Fila **pFila4) {
    int tempo = 0;
    int quantidadeProcessosFinalizados = 0;

    //todo: condição de parada para o for
    while (quantidadeProcessosFinalizados < quantidadeProcessos) {
        //todo: adicionar na lista de prontos novos processos que chegaram;
        //todo: executarCPU
        //todo: executarIOImpressora
        //todo: executarIODisco
        //todo: executarIOFita
        tempo+= 1;
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

int main() {
    srand(time(NULL));
    //incialização do programa: quantidade de processos,
    int quantidadeProcessos = 5;
    Process** processos = gerarProcessos(quantidadeProcessos);
    for (int i = 0; i < quantidadeProcessos; ++i) {
        Process* processo = processos[i];
        printf("Pid do processo %s\n", processo->pid);
    }
    Fila *filaBaixaPrioridade = inicializarFila();
    Fila *filaAltaPrioridade = inicializarFila();
    Fila *filaDisco = inicializarFila();
    Fila *filaImpressora = inicializarFila();
    Fila *filaFita = inicializarFila();
//    processarRoundRobin(quantidadeProcessos,
//                        processos,
//                        &filaAltaPrioridade,
//                        &filaBaixaPrioridade,
//                        &filaImpressora,
//                        &filaDisco,
//                        &filaFita);

    printf("Os processos foram criados: ");
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
    return 0;
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

