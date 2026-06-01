/**
 *  @author Felipe Gustavo Bombardelli
 *  @date   31/maio/2026
 *
 *  @author Elias P. Duarte Jr.
 *  @date   11/abril/2024
 *
 *  @enunciado
 *    ALGORITMO 2: Implemente o algoritmo aleatorizado para eleição de líder (Randomized 
 *    Leader Election Algorithm) baseado no anel (VRing). Neste algoritmo cada processo 
 *    sorteia, em cada rodada um bit aleatório. O algoritmo funciona em rodadas. Na 
 *    primeira rodada todos os N processos sorteiam um valor aleatório para o seu bit. 
 *    Os processos com bit = 1 são candidatos, os demais processos (i.e. com seu bit = 0) 
 *    não são candidatos. Cada processo deve enviar uma mensagem que roda todo o anel 
 *    informando seu id e seu bit da rodada. Na rodada seguinte, apenas os processos 
 *    com bit = 1 sorteiam novo valor aleatório para o bit. Esses processos então 
 *    comunicam novamente no anel o valor dos seus bits. Novas rodadas vão acontecendo 
 *    até que 1 processo se transforme em líder. Execute o algoritmo para diferentes 
 *    valores de N. Destaque para cada execução quem é o líder eleito. Conte quantas 
 *    mensagens foram necessárias em cada caso e também quanto tempo (do SMPL) foi 
 *    necessário para executar a eleição. 
 */
// ======================================================================================
//  Header
// ======================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "smpl.h"

// Tempo maximo de simulação
#define TEMPO_MAXIMO     300

// Eventos da simulação
#define TEST                      1
#define FAULT                     2
#define RECOVERY                  3
#define INICIO_ELEICAO            4
#define RECEBE_VALORES_SORTEADOS  5
#define NOVA_RODADA               6

// Vamos definir o descritor do processo

typedef struct{ 
   int id;            // identificador de facility do SMPL
   int lider;
   int rodada;
   int valores_sorteados[100]; // maximo de 100 processos
   bool foi_alterado[100];
} TipoProcesso;

TipoProcesso *processo;
int N = 0;

// ======================================================================================
//  Funções
// ======================================================================================

#define info(id_processo, fmt, ...) printf("[%4.1f, Proc.%2d, Rodada %d, Sorteio: %s ]: " fmt "\n", \
    time(), id_processo, processo[id_processo].rodada, mostra_valores_sorteados(id_processo,N), ##__VA_ARGS__ )
   
void recebe_valores_sorteados(int id_destino, int id_fonte, int tamanho) {
    bool* foi_alterado = &processo[id_destino].foi_alterado[0];
    int* vetor_destino = &processo[id_destino].valores_sorteados[0];
    const int* vetor_fonte = &processo[id_fonte].valores_sorteados[0];
    for (int i=0; i<tamanho; i++) {
        if ( vetor_destino[i] < 0 ) {
            vetor_destino[i] = vetor_fonte[i];
            if ( vetor_fonte[i] >= 0 ) {
                foi_alterado[i] = true;
            }
        }
    }
}

void limpa_valores_sorteados_dos_candidatos(int id_processo, int tamanho) {
    int* valores = &processo[id_processo].valores_sorteados[0];
    for (int i=0; i<tamanho; i++) {
        if ( valores[i] == 1 ) {
            valores[i] = -1;
        }
    }
}

bool foram_todos_sorteados(int id_processo, int tamanho) {
    const int* valores_sorteados = &processo[id_processo].valores_sorteados[0];
    for (int i=0; i<tamanho; i++) {
        if ( valores_sorteados[i] < 0 ) {
            return false;
        }
    }
    return true;
}

const char* mostra_valores_sorteados(int id_processo, int tamanho) {
    static char buffer[1024];
    int cursor = 0;
    // cursor += sprintf(&buffer[cursor], "(%d) ", processo[id_processo].rodada);
    for (int i=0; i<tamanho; i++) {
        if ( processo[id_processo].foi_alterado[i] == true ) {
            cursor += sprintf(&buffer[cursor], "%2d ", processo[id_processo].valores_sorteados[i]);
            processo[id_processo].foi_alterado[i] = false;
        } else {
            cursor += sprintf(&buffer[cursor], "%2d ", processo[id_processo].valores_sorteados[i]);
        }
    }
    return buffer;
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
    stream(1);

    // 2. Inicialização dos processos
    processo = (TipoProcesso *) malloc(sizeof(TipoProcesso)*N);
    for (int i=0; i<N; i++) {
        memset(fa_name, '\0', 5);
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name,1);
    }

    // 3. vamos fazer o escalonamento inicial de eventos
    for (int i=0; i<N; i++) {
        schedule(INICIO_ELEICAO, 10.0+i, i);
    }

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
            case INICIO_ELEICAO: {
                // inicia as variaveis locais do processo 
                processo[id_processo].lider = -1;
                processo[id_processo].rodada = 0;
                for (int j=0; j<N; j++) {
                   processo[id_processo].valores_sorteados[j] = -1;
                   processo[id_processo].foi_alterado[j] = false;
                }

                // sorteia se é lider ou não
                const int valor_sorteado = randomic(0,1);
                processo[id_processo].valores_sorteados[id_processo] = valor_sorteado;
                processo[id_processo].foi_alterado[id_processo] = true;

                // log
                info(id_processo, "Inicio da eleição");

                // envia o valor sorteado para o proximo do anel
                schedule(RECEBE_VALORES_SORTEADOS, 20.0, (id_processo)%N );
            } break;

            case RECEBE_VALORES_SORTEADOS: {
                // 1. Inicializa variaveis necessarias
                const int id_processo_menos_um = id_processo - 1;
                const int processo_que_enviou = ( id_processo_menos_um < 0 ) ? N-1 : id_processo_menos_um;
                const int rodada_recebida = processo[processo_que_enviou].rodada;
                const int rodada_corrente = processo[id_processo].rodada;

                // 2a. Caso a rodada recebida seja maior que a corrente
                if ( rodada_recebida > rodada_corrente ) {
                    // Salva o atual valor e limpa os valores sorteados
                    const int valor_atual = processo[id_processo].valores_sorteados[id_processo];
                    processo[id_processo].rodada = processo[processo_que_enviou].rodada;
                    limpa_valores_sorteados_dos_candidatos(id_processo, N);

                    // Caso o processo seja ainda candidato, sorteia um novo numero
                    if ( valor_atual == 1 ) {
                        recebe_valores_sorteados(id_processo, processo_que_enviou, N);
                        const int valor_sorteado = randomic(0,1);
                        processo[id_processo].valores_sorteados[id_processo] = valor_sorteado;
                        processo[id_processo].foi_alterado[id_processo] = true;
                        info(id_processo, "Recebido mensagem de %d com nova rodada e sorteie o numero %d", processo_que_enviou, valor_sorteado);

                    // Caso o processo não seja candidato, apenas copia os dados recebidos
                    } else {
                        recebe_valores_sorteados(id_processo, processo_que_enviou, N);
                        info(id_processo, "Processo %d e recebeu mensagem de %d com nova rodada e nao sou candidato", id_processo, processo_que_enviou);
                    }
                    

                // 2b. Caso a rodada recebida seja igual aa rodada corrente, copia os dados
                } else if ( rodada_recebida == rodada_corrente ) {
                    info(id_processo, "Recebido mensagem de %d", processo_que_enviou);
                    recebe_valores_sorteados(id_processo, processo_que_enviou, N);
                }

                // 3a. Caso tenha o sorteio de todos os processos
                if ( foram_todos_sorteados(id_processo, N) ) {
                    
                    // Conta quandos 1 tem no valores sorteados
                    int contador = 0;
                    const int* valores_sorteados = &processo[id_processo].valores_sorteados[0];
                    for (int i=0; i<N; i++) {
                        if ( valores_sorteados[i] > 0 ) {
                            contador += 1;
                        }
                    }

                    // Se existe apenas 1, então foi eleito um lider
                    if ( contador == 1 ) {
                        info(id_processo, "Eleito um lider");
                        schedule(RECEBE_VALORES_SORTEADOS, 10.0, (id_processo+1)%N );

                    // Senão, inicia uma nova rodada
                    } else {
                        // Aumenta a rodada e seta -1 em todos os candidatos
                        info(id_processo, "tenho todos os sorteados, mas nao foi eleito");
                        const int rodada = processo[id_processo].rodada;
                        const int valor_sorteado_anterior = processo[id_processo].valores_sorteados[id_processo];
                        processo[id_processo].rodada += 1;
                        limpa_valores_sorteados_dos_candidatos(id_processo, N);

                        // Se é candidato, então sorteia um novo numero
                        if ( valor_sorteado_anterior == 1 ) {
                            const int valor_sorteado = randomic(0,1);
                            processo[id_processo].valores_sorteados[id_processo] = valor_sorteado;
                            processo[id_processo].foi_alterado[id_processo] = true;
                        }

                        // Envia mensagem para o proximo nodo
                        info(id_processo, "Nova rodada %d -> %d", rodada, rodada+1);
                        schedule(RECEBE_VALORES_SORTEADOS, 10.0, (id_processo+1)%N );
                    }

                // 3b. Envia mensagem para continua
                } else {
                    schedule(RECEBE_VALORES_SORTEADOS, 10.0, (id_processo+1)%N );
                }
            } break;

        } // switch
    } // while
} // main
        

