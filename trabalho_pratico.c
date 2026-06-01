/**
 *  @author Felipe Gustavo Bombardelli
 *  @date   31/maio/2026
 *
 *  @author Elias P. Duarte Jr.
 *  @date   11/abril/2024
 *
 *  @brief
 *     Implementação do VRing
 */ 
// ======================================================================================
//  Header
// ======================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#include "smpl.h"

// Tempo maximo de simulação
#define TEMPO_MAXIMO     200

// Eventos da simulação
#define TEST                1
#define FAULT               2
#define RECOVERY            3
#define INICIO_ELEICAO      4
#define RECEBE_CANDIDATURA  5

// Vamos definir o descritor do processo

typedef struct{ 
   int id;            // identificador de facility do SMPL
   int state[100];
} TipoProcesso;

TipoProcesso *processo;
int N=0;

// ======================================================================================
//  Funções
// ======================================================================================

#define info(id_processo, fmt, ...) printf("[%4.1f, Proc.%2d, state %s]: " fmt "\n", time(), \
    id_processo, mostra_estado(id_processo, N), ##__VA_ARGS__ )

const char* mostra_estado(int id_processo, int tamanho) {
    static char buffer[1024];
    int cursor = 0;
    for (int i=0; i<tamanho; i++) {
        cursor += sprintf(&buffer[cursor], "%2d ", processo[id_processo].state[i]);
    }
    return buffer;
}

void copia_vetor_state(int id_destino, int id_fonte) {
    int* state_dest = &processo[id_destino].state[0];
    const int* state_orig = &processo[id_fonte].state[0];
    for (int i=0; i<N; i++) {
        state_dest[i] = state_orig[i];
    }
}

// ======================================================================================
//  Main
// ======================================================================================

int main (int argc, char *argv[]) {
    // 0. Tratamento dos argumentos
    static char fa_name[16];
    if (argc != 2) {
        puts("Uso correto: tempo <número de processos>");
        exit(1);
    }
    N = atoi(argv[1]); 

    // 1. Inicialização do SMPL
    smpl(0, "Um Exemplo de Simulação");
    reset();
    stream(3);

    // 2. Inicialização dos processos
    processo = (TipoProcesso *) malloc(sizeof(TipoProcesso)*N);
    for (int i=0; i<N; i++) {
        memset(fa_name, '\0', 5);
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name,1);
        for (int j=0; j<N; j++) {
            processo[i].state[j] = -1;
        }
        processo[i].state[i] = 0;
    }

    // 3. vamos fazer o escalonamento inicial de eventos
    for (int i=0; i<N; i++) {
        schedule(TEST, 30.0, i);
        // schedule(INICIO_ELEICAO, 40.0+i, i);
    }
    schedule(FAULT, 50.0, 1);
    // schedule(FAULT, 90.0, 2);
    schedule(FAULT, 110.0, 3);
    schedule(FAULT, 110.0, 4);
   
    // 4. Cabeçalho
    puts("===============================================================");
    puts("           Sistemas Distribuí­dos Prof. Elias");
    puts("          LOG do Trabalho Prático 0, Tarefa 0");
    puts("      Digitar, compilar e executar o programa tempo.c");
    printf("   Este programa foi executado para: N=%d processos.\n", N); 
    printf("           Tempo Total de Simulação = %d\n", TEMPO_MAXIMO);
    puts("===============================================================");

    // 5. Loop principal
    while(time() < TEMPO_MAXIMO) {
        int event, id_processo;
        cause(&event, &id_processo);
        switch(event) {               
            case TEST: {
                // Processo falho, não faz nenhum teste
                if (status(processo[id_processo].id) != 0 ) {
                    continue;
                }

                // Testa até achar o próximo processo correto
                int i;
                int proximo_processo = id_processo;
                for (i=0; i<N-1; i++) {
                    proximo_processo = (proximo_processo + 1) % N;

                    // Testou um processo falho
                    if (status(processo[proximo_processo].id) != 0 ) {
                        processo[id_processo].state[proximo_processo] = 1;
                        info(id_processo, "processo %d detectou que o processo %d falhou", id_processo, proximo_processo);

                    // Achou um processo correto
                    } else {
                        // Atualiza o vetor state
                        processo[id_processo].state[proximo_processo] = 0;
                        info(id_processo, "processo %d testou o processo %d", id_processo, proximo_processo);

                        // Copia o vetor state do processo atual para o proximo processo correto
                        copia_vetor_state(proximo_processo, id_processo);
                        break;
                    }
                }

                // Sobrou apenas 1 processo correto
                if ( i == N-1 ) {
                    info(id_processo, "sobrou apenas eu");
                }

                // Agenda o proximo teste
                schedule(TEST, 30.0, id_processo);
            } break;

            case FAULT: {
                int r = request(processo[id_processo].id, id_processo, 0);
                info(id_processo, "Socooorro!!! Estou falhando");
            } break;

            case RECOVERY: {
                release(processo[id_processo].id, id_processo);
                info(id_processo, "Viva!!! Acabo de recuperar\n");
                schedule(TEST, 1.0, id_processo);
            } break;
        } // switch
    } // while
} // main
        

