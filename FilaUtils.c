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