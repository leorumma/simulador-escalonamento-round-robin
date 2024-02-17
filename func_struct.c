#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include “header.h”

Process* cria_processo() //Função que retorna um ponteiro pra processo
{
    Process *newer;
    if((newer = (Process *) malloc(sizeof(Process)))==NULL)
    {
        puts("Sem espaco livre");
        exit(1);
    }
    return newer;
}

ProcessIO* cria_processoIO() //Função que retorna um ponteiro pra processoIO
{
    ProcessIO *newer;
    if((newer = (ProcessIO *) malloc(sizeof(ProcessIO)))==NULL)
    {
        puts("Sem espaco livre");
        exit(1);
    }
    return newer;
}

void troca_de_fila(fila **antiga,fila*nova) //Uma função que faz a troca de filas pode ser muito útil
{
    void troca (fila **sai,fila *entra)
    fila *copy;
    copy = entra;
    while (entra->prox!= NULL)
    {
        entra = entra-> prox;
    }
    entra->prox = cria();
    entra->prox->processo = (*sai)->processo;
    *sai = (*sai)->prox;
}
// TODO: adicionar função (builder) que instancia o struct do processo

// TODO: adicionar função (builder) que instancia o struct do IO

// TODO: adicionar função que lida com preempção

// TODO: adicionar função que lida com retorno de IO

// TODO: adicionar orquestrator do escalonamento (filas podem ser definidas aqui)


int main() {
    /* definir número de processos e instanciá-los aqui */

    return 0;
}

