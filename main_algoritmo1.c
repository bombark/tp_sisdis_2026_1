/**
 *  @author Felipe Gustavo Bombardelli
 *  @date   31/maio/2026
 *
 *  @author Elias P. Duarte Jr.
 *  @date   11/abril/2024
 * 
 *  @brief
 *     Implemente o algoritmo de eleição de líder com candidatos baseado em Chang-Roberts 
 *     no anel (VRing). Neste algoritmo inicialmente são definidos quais processos (com 
 *     identificador 0..N-1) são candidatos a líder. Todo processo mantém uma variável 
 *     local Lider na qual armazenda o id do processo que considera líder. Se o processo 
 *     é candidato, já assinala seu próprio id para Lider na inicialização. Um não 
 *     candidato assinala o valor -1 na inicialização. Um processo que é candidato 
 *     manda uma mensagem para o seguinte informando seu id e que é candidato. Vamos 
 *     usar como critério para a eleição o maior identificador. Assim, se um processo 
 *     recebe uma mensagem de um candidato com id maior que aquele indicado pela variável 
 *     Lider, modifica o valor e repassa para frente no anel a mensagem recebida. Quando 
 *     um processo recebe de volta sua própria mensagem informando que é candidato, sabe 
 *     que é o líder e todos o processos do anel já receberam sua mensagem. O processo 
 *     pode então descartar a mensagem e executar procedimento de lider. Mostre no log 
 *     execuções do algoritmo para 1 único candidato, vários candidatos selecionados 
 *     aleatoriamente e todos os N processos inicialmente candidatos. Destaque para cada 
 *     execução quem é o líder eleito. Conte quantas mensagens foram necessárias em cada 
 *     caso e também quanto tempo (do SMPL) foi necessário para executar a eleição.
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
#define TEMPO_MAXIMO     300

// Eventos da simulação
#define TEST                1
#define FAULT               2
#define RECOVERY            3
#define INICIO_ELEICAO      4
#define RECEBE_CANDIDATURA  5

// Vamos definir o descritor do processo

typedef struct{ 
   int id;            // identificador de facility do SMPL
   int lider;
   bool eh_candidato;
} TipoProcesso;

TipoProcesso *processo;

// ======================================================================================
//  Funções
// ======================================================================================

#define info(id_processo, fmt, ...) printf("[%4.1f, Proc.%2d, Lider %2d]: " fmt "\n", \
    time(), id_processo, processo[id_processo].lider, ##__VA_ARGS__ )

void mostra_uso_correto() {
    puts("Uso correto: tempo <número de processos> [abc]");
    puts(" - a: N processos candidatos");
    puts(" - b: 1 processo candidato");
    puts(" - c: Todos os processos candidatos");
}


// ======================================================================================
//  Main
// ======================================================================================

int main (int argc, char *argv[]) {
    // 0. Tratamento dos argumentos
    static char fa_name[16];
    char opcao;
    if (argc < 2) {
        mostra_uso_correto();
        exit(1);
    } else if ( argc == 2 ) {
        opcao = 'a';
    } else {
        opcao = argv[2][0];
        if ( opcao != 'a' && opcao != 'b' && opcao != 'c' ) {
            mostra_uso_correto();
            exit(1);
        }
    }
    const int N = atoi(argv[1]); 

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
        processo[i].lider = -1;
        processo[i].eh_candidato = false;
    }
   
    // 3a. Sorteio aleatorio dos candidatos
    if ( opcao == 'a' ) {
        for (int i=0; i<N; i++) {
            processo[i].eh_candidato = randomic(0,1);
        }

    // 3b. Apenas um processo é candidato
    } else if ( opcao == 'b' ) {
        processo[0].eh_candidato = true;

    // 3c. Todos os processos sao candidatos
    } else if ( opcao == 'c' ) {
        for (int i=0; i<N; i++) {
            processo[i].eh_candidato = true;
        }

    }

    // 4. vamos fazer o escalonamento inicial de eventos
    for (int i=0; i<N; i++) {
        schedule(TEST, 150.0, i);
        schedule(INICIO_ELEICAO, 10.0+i, i);
    }

    // 5. Cabeçalho
    puts("===============================================================");
    puts("           Sistemas Distribuí­dos Prof. Elias");
    puts("          LOG do Trabalho Prático 0, Tarefa 0");
    puts("      Digitar, compilar e executar o programa tempo.c");
    printf("   Este programa foi executado para: N=%d processos.\n", N); 
    printf("           Tempo Total de Simulação = %d\n", TEMPO_MAXIMO);
    puts("===============================================================");

    // 6. Loop principal
    while(time() < TEMPO_MAXIMO) {
        int event, id_processo;
        cause(&event, &id_processo);
        switch(event) {
            case INICIO_ELEICAO: {

                // Processos candidatos enviam sua candidatura
                if ( processo[id_processo].eh_candidato ) {
                    schedule(RECEBE_CANDIDATURA, 5.0, (id_processo+1)%N );
                    processo[id_processo].lider = id_processo;
                    info(id_processo, "estou iniciando a eleição e sou candidato");

                // Processos não candidatos não fazem nada
                } else {
                    processo[id_processo].lider = -1;
                    info(id_processo, "estou iniciando a eleição");
                }

            } break;

            case RECEBE_CANDIDATURA: {
                // Calcula o id do processo anterior do anel e recebe a candidatura do lider
                const int id_processo_menos_um = id_processo - 1;
                const int processo_que_enviou = ( id_processo_menos_um < 0 ) ? N-1 : id_processo_menos_um;
                const int candidato_recebido = processo[processo_que_enviou].lider;
                

                // Caso o id candidato for maior que o atual, então atualiza o lider e envia a nova candidatura
                if ( candidato_recebido > processo[id_processo].lider) {
                    const int envia_para = (id_processo+1) % N;
                    processo[id_processo].lider = candidato_recebido;
                    schedule(RECEBE_CANDIDATURA, +10.0, envia_para);
                    info(id_processo, "recebe candidatura %d, atualiza lider e reenvia candidatura para %d", candidato_recebido, envia_para);

                // Caso o candidato recebido tiver menor id que o atual, entao descarta
                } else if ( candidato_recebido < processo[id_processo].lider ) {
                    info(id_processo, "recebe candidatura %d do processo %d, descarta a candidatura", candidato_recebido, processo_que_enviou );
                }

                // Caso um processo recebe de volta sua própria mensagem informando que é candidato, então ele é o lider
                if ( candidato_recebido == id_processo ) {
                    info(id_processo, "Fui eleito lider");
                }
            } break;
                
            case TEST: {
                if (status(processo[id_processo].id) !=0) break; // se o processo está falho, não testa!
                info(id_processo, "estou testando");
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
        

