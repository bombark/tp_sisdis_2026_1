# TRABALHO PRÁTICO 1: Dois Algoritmos para Eleição Distribuída de Líder em Anel 

Nome: Felipe Gustavo Bombardelli

Compilação do projeto. Isso gerará três executáveis, algorimo1, algoritmo2 e trabalho_pratico

```bash
make
```


# 0. Trabalho Prático

## 0.1. Tarefa 1

Fazer cada um dos processos testar o seguinte no anel. Implemente o teste com a função status() do SMPL e imprimir (printf) o resultado de cada teste executado. Por exemplo: “O processo i testou o processo j correto no tempo tal.”

```c
int main() {
    [...]
    case TEST: {
        const int proximo_processo = (id_processo + 1) % N;
        info(id_processo, "testando processo %d", proximo_processo);
        if (status(processo[proximo_processo].id) != 0 ) {
            info(id_processo, "detectou que o processo %d falhou", proximo_processo);
        }
        schedule(TEST, 30.0, id_processo);
    } break;
    [...]
}
```

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=5 processos.
           Tempo Total de Simulação = 300
===============================================================
[30.0, Proc. 0]: testando processo 1
[30.0, Proc. 1]: testando processo 2
[30.0, Proc. 2]: testando processo 3
[30.0, Proc. 3]: testando processo 4
[30.0, Proc. 4]: testando processo 0
[60.0, Proc. 0]: testando processo 1
[60.0, Proc. 1]: testando processo 2
[60.0, Proc. 2]: testando processo 3
[60.0, Proc. 3]: testando processo 4
[60.0, Proc. 4]: testando processo 0
[90.0, Proc. 0]: testando processo 1
[90.0, Proc. 1]: testando processo 2
[90.0, Proc. 2]: testando processo 3
[90.0, Proc. 3]: testando processo 4
[90.0, Proc. 4]: testando processo 0
[120.0, Proc. 0]: testando processo 1
[120.0, Proc. 1]: testando processo 2
[120.0, Proc. 2]: testando processo 3
[120.0, Proc. 3]: testando processo 4
[120.0, Proc. 4]: testando processo 0
[130.0, Proc. 1]: Socooorro!!! Estou falhando
[150.0, Proc. 0]: testando processo 1
[150.0, Proc. 0]: detectou que o processo 1 falhou
[150.0, Proc. 1]: testando processo 2
[150.0, Proc. 2]: testando processo 3
[150.0, Proc. 3]: testando processo 4
[150.0, Proc. 4]: testando processo 0
[180.0, Proc. 0]: testando processo 1
[180.0, Proc. 0]: detectou que o processo 1 falhou
```


## 0.2. Tarefa 2

Cada processo correto executa testes até achar outro processo correto. Lembre-se de tratar o caso em que todos os demais processos estão falhos. Imprimir os testes e resultados.

```c
int main() {
    [...]
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
            if (status(processo[proximo_processo].id) != 0 ) {
                info(id_processo, "processo %d detectou que o processo %d falhou", id_processo, proximo_processo);
            } else { 
                info(id_processo, "processo %d testou o processo %d", id_processo, proximo_processo);
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
    [...]
}
```


```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=5 processos.
           Tempo Total de Simulação = 200
===============================================================
[30.0, Proc. 0]: processo 0 testou o processo 1
[30.0, Proc. 1]: processo 1 testou o processo 2
[30.0, Proc. 2]: processo 2 testou o processo 3
[30.0, Proc. 3]: processo 3 testou o processo 4
[30.0, Proc. 4]: processo 4 testou o processo 0
[50.0, Proc. 1]: Socooorro!!! Estou falhando
[60.0, Proc. 0]: processo 0 detectou que o processo 1 falhou
[60.0, Proc. 0]: processo 0 testou o processo 2
[60.0, Proc. 2]: processo 2 testou o processo 3
[60.0, Proc. 3]: processo 3 testou o processo 4
[60.0, Proc. 4]: processo 4 testou o processo 0
[90.0, Proc. 2]: Socooorro!!! Estou falhando
[90.0, Proc. 0]: processo 0 detectou que o processo 1 falhou
[90.0, Proc. 0]: processo 0 detectou que o processo 2 falhou
[90.0, Proc. 0]: processo 0 testou o processo 3
[90.0, Proc. 3]: processo 3 testou o processo 4
[90.0, Proc. 4]: processo 4 testou o processo 0
[110.0, Proc. 3]: Socooorro!!! Estou falhando
[110.0, Proc. 4]: Socooorro!!! Estou falhando
[120.0, Proc. 0]: processo 0 detectou que o processo 1 falhou
[120.0, Proc. 0]: processo 0 detectou que o processo 2 falhou
[120.0, Proc. 0]: processo 0 detectou que o processo 3 falhou
[120.0, Proc. 0]: processo 0 detectou que o processo 4 falhou
[120.0, Proc. 0]: sobrou apenas eu
```

## 0.3. Tarefa 3

Cada processo mantém localmente o vetor State[N]. A entrada do vetor State[j] indica o estado do processo j. O estado de cada processo pode ser: -1 (unknown), 0 (correto) ou 1 (falho). Inicialize (para todos os processos) o State[N] com -1 (indicando estado “unknown”) para todos os demais processos e 0 para o próprio processo. Nesta tarefa ao executar um teste em um processo j, o testador atualiza a entrada correspondente no vetor State[j]. Em cada intervalo de testes, mostre o vetor State[N].

```c
typedef struct{ 
   int id;            // identificador de facility do SMPL
   int state[100];
} TipoProcesso;

[...]

int main() {
    [...]

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
                processo[id_processo].state[proximo_processo] = 0;
                info(id_processo, "processo %d testou o processo %d", id_processo, proximo_processo);
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

    [...]
}

```

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=5 processos.
           Tempo Total de Simulação = 200
===============================================================
[30.0, Proc. 0, state  0  0 -1 -1 -1 ]: processo 0 testou o processo 1
[30.0, Proc. 1, state -1  0  0 -1 -1 ]: processo 1 testou o processo 2
[30.0, Proc. 2, state -1 -1  0  0 -1 ]: processo 2 testou o processo 3
[30.0, Proc. 3, state -1 -1 -1  0  0 ]: processo 3 testou o processo 4
[30.0, Proc. 4, state  0 -1 -1 -1  0 ]: processo 4 testou o processo 0
[50.0, Proc. 1, state -1  0  0 -1 -1 ]: Socooorro!!! Estou falhando
[60.0, Proc. 0, state  0  1 -1 -1 -1 ]: processo 0 detectou que o processo 1 falhou
[60.0, Proc. 0, state  0  1  0 -1 -1 ]: processo 0 testou o processo 2
[60.0, Proc. 2, state -1 -1  0  0 -1 ]: processo 2 testou o processo 3
[60.0, Proc. 3, state -1 -1 -1  0  0 ]: processo 3 testou o processo 4
[60.0, Proc. 4, state  0 -1 -1 -1  0 ]: processo 4 testou o processo 0
[90.0, Proc. 2, state -1 -1  0  0 -1 ]: Socooorro!!! Estou falhando
[90.0, Proc. 0, state  0  1  0 -1 -1 ]: processo 0 detectou que o processo 1 falhou
[90.0, Proc. 0, state  0  1  1 -1 -1 ]: processo 0 detectou que o processo 2 falhou
[90.0, Proc. 0, state  0  1  1  0 -1 ]: processo 0 testou o processo 3
[90.0, Proc. 3, state -1 -1 -1  0  0 ]: processo 3 testou o processo 4
[90.0, Proc. 4, state  0 -1 -1 -1  0 ]: processo 4 testou o processo 0
[110.0, Proc. 3, state -1 -1 -1  0  0 ]: Socooorro!!! Estou falhando
[110.0, Proc. 4, state  0 -1 -1 -1  0 ]: Socooorro!!! Estou falhando
[120.0, Proc. 0, state  0  1  1  0 -1 ]: processo 0 detectou que o processo 1 falhou
[120.0, Proc. 0, state  0  1  1  0 -1 ]: processo 0 detectou que o processo 2 falhou
[120.0, Proc. 0, state  0  1  1  1 -1 ]: processo 0 detectou que o processo 3 falhou
[120.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 4 falhou
[120.0, Proc. 0, state  0  1  1  1  1 ]: sobrou apenas eu
[150.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 1 falhou
[150.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 2 falhou
[150.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 3 falhou
[150.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 4 falhou
[150.0, Proc. 0, state  0  1  1  1  1 ]: sobrou apenas eu
[180.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 1 falhou
[180.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 2 falhou
[180.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 3 falhou
[180.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 4 falhou
[180.0, Proc. 0, state  0  1  1  1  1 ]: sobrou apenas eu
[210.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 1 falhou
[210.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 2 falhou
[210.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 3 falhou
[210.0, Proc. 0, state  0  1  1  1  1 ]: processo 0 detectou que o processo 4 falhou
[210.0, Proc. 0, state  0  1  1  1  1 ]: sobrou apenas eu
```



## 0.4. Tarefa 4

Quando um processo correto testa outro processo correto obtém as informações do estado dos demais processos do sistema, processos do sistema exceto aqueles que testou nesta rodada, além do próprio testador. 

```c
void copia_vetor_state(int id_destino, int id_fonte) {
    int* state_dest = &processo[id_destino].state[0];
    const int* state_orig = &processo[id_fonte].state[0];
    for (int i=0; i<N; i++) {
        state_dest[i] = state_orig[i];
    }
}

[...]

int main() {
    [...]
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
    [...]
}

```

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=8 processos.
           Tempo Total de Simulação = 200
===============================================================
[30.0, Proc. 0, state  0  0 -1 -1 -1 -1 -1 -1 ]: processo 0 testou o processo 1
[30.0, Proc. 1, state  0  0  0 -1 -1 -1 -1 -1 ]: processo 1 testou o processo 2
[30.0, Proc. 2, state  0  0  0  0 -1 -1 -1 -1 ]: processo 2 testou o processo 3
[30.0, Proc. 3, state  0  0  0  0  0 -1 -1 -1 ]: processo 3 testou o processo 4
[30.0, Proc. 4, state  0  0  0  0  0  0 -1 -1 ]: processo 4 testou o processo 5
[30.0, Proc. 5, state  0  0  0  0  0  0  0 -1 ]: processo 5 testou o processo 6
[30.0, Proc. 6, state  0  0  0  0  0  0  0  0 ]: processo 6 testou o processo 7
[30.0, Proc. 7, state  0  0  0  0  0  0  0  0 ]: processo 7 testou o processo 0
[50.0, Proc. 1, state  0  0  0 -1 -1 -1 -1 -1 ]: Socooorro!!! Estou falhando
[60.0, Proc. 0, state  0  1  0  0  0  0  0  0 ]: processo 0 detectou que o processo 1 falhou
[60.0, Proc. 0, state  0  1  0  0  0  0  0  0 ]: processo 0 testou o processo 2
[60.0, Proc. 2, state  0  1  0  0  0  0  0  0 ]: processo 2 testou o processo 3
[60.0, Proc. 3, state  0  1  0  0  0  0  0  0 ]: processo 3 testou o processo 4
[60.0, Proc. 4, state  0  1  0  0  0  0  0  0 ]: processo 4 testou o processo 5
[60.0, Proc. 5, state  0  1  0  0  0  0  0  0 ]: processo 5 testou o processo 6
[60.0, Proc. 6, state  0  1  0  0  0  0  0  0 ]: processo 6 testou o processo 7
[60.0, Proc. 7, state  0  1  0  0  0  0  0  0 ]: processo 7 testou o processo 0
[90.0, Proc. 0, state  0  1  0  0  0  0  0  0 ]: processo 0 detectou que o processo 1 falhou
[90.0, Proc. 0, state  0  1  0  0  0  0  0  0 ]: processo 0 testou o processo 2
[90.0, Proc. 2, state  0  1  0  0  0  0  0  0 ]: processo 2 testou o processo 3
[90.0, Proc. 3, state  0  1  0  0  0  0  0  0 ]: processo 3 testou o processo 4
[90.0, Proc. 4, state  0  1  0  0  0  0  0  0 ]: processo 4 testou o processo 5
[90.0, Proc. 5, state  0  1  0  0  0  0  0  0 ]: processo 5 testou o processo 6
[90.0, Proc. 6, state  0  1  0  0  0  0  0  0 ]: processo 6 testou o processo 7
[90.0, Proc. 7, state  0  1  0  0  0  0  0  0 ]: processo 7 testou o processo 0
[110.0, Proc. 3, state  0  1  0  0  0  0  0  0 ]: Socooorro!!! Estou falhando
[110.0, Proc. 4, state  0  1  0  0  0  0  0  0 ]: Socooorro!!! Estou falhando
[120.0, Proc. 0, state  0  1  0  0  0  0  0  0 ]: processo 0 detectou que o processo 1 falhou
[120.0, Proc. 0, state  0  1  0  0  0  0  0  0 ]: processo 0 testou o processo 2
[120.0, Proc. 2, state  0  1  0  1  0  0  0  0 ]: processo 2 detectou que o processo 3 falhou
[120.0, Proc. 2, state  0  1  0  1  1  0  0  0 ]: processo 2 detectou que o processo 4 falhou
[120.0, Proc. 2, state  0  1  0  1  1  0  0  0 ]: processo 2 testou o processo 5
[120.0, Proc. 5, state  0  1  0  1  1  0  0  0 ]: processo 5 testou o processo 6
[120.0, Proc. 6, state  0  1  0  1  1  0  0  0 ]: processo 6 testou o processo 7
[120.0, Proc. 7, state  0  1  0  1  1  0  0  0 ]: processo 7 testou o processo 0
[150.0, Proc. 0, state  0  1  0  1  1  0  0  0 ]: processo 0 detectou que o processo 1 falhou
[150.0, Proc. 0, state  0  1  0  1  1  0  0  0 ]: processo 0 testou o processo 2
[150.0, Proc. 2, state  0  1  0  1  1  0  0  0 ]: processo 2 detectou que o processo 3 falhou
[150.0, Proc. 2, state  0  1  0  1  1  0  0  0 ]: processo 2 detectou que o processo 4 falhou
[150.0, Proc. 2, state  0  1  0  1  1  0  0  0 ]: processo 2 testou o processo 5
[150.0, Proc. 5, state  0  1  0  1  1  0  0  0 ]: processo 5 testou o processo 6
```



# 1. ALGORITMO 1 
 
## 1.0. Enunciado

Implemente o algoritmo de eleição de líder com candidatos baseado em Chang-Roberts no anel (VRing). Neste algoritmo inicialmente são definidos quais processos (com identificador 0..N-1) são candidatos a líder. Todo processo mantém uma variável local Lider na qual armazenda o id do processo que considera líder. Se o processo é candidato, já assinala seu próprio id para Lider na inicialização. Um não candidato assinala o valor -1 na inicialização. Um processo que é candidato manda uma mensagem para o seguinte informando seu id e que é candidato. Vamos usar como critério para a eleição o maior identificador. Assim, se um processo recebe uma mensagem de um candidato com id maior que aquele indicado pela variável Lider, modifica o valor e repassa para frente no anel a mensagem recebida. Quando um processo recebe de volta sua própria mensagem informando que é candidato, sabe que é o líder e todos o processos do anel já receberam sua mensagem. O processo pode então descartar a mensagem e executar procedimento de lider. Mostre no log execuções do algoritmo para 1 único candidato, vários candidatos selecionados aleatoriamente e todos os N processos inicialmente candidatos. Destaque para cada execução quem é o líder eleito. Conte quantas mensagens foram necessárias em cada caso e também quanto tempo (do SMPL) foi necessário para executar a eleição.


Se o processo é candidato, já assinala seu próprio id para Lider na inicialização. Um não candidato assinala o valor -1 na inicialização.
Um processo que é candidato manda uma mensagem para o seguinte informando seu id e que é candidato.
Vamos usar como critério para a eleição o maior identificador.
sabe que é o líder e todos o processos do anel já receberam sua mensagem. O processo pode então descartar a mensagem e executar procedimento de lider.

## 1.1. Algoritmo

### Quando INICIO_ELEICAO:
- 1a. Processos candidatos enviam sua candidatura
- 1b. Processos não candidatos não fazem nada

### Quando RECEBE_CANDIDATURA:
1. Calcula o id do processo anterior do anel e recebe a candidatura do lider
2. Caso o id do candidato for maior que o atual, então atualiza o lider e envia a nova candidatura
3. Caso um processo recebe de volta sua própria mensagem informando que é candidato, então ele é o lider


## 1.2. Código

```c
[...]

int main() {
    [...]

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
        info(id_processo, "recebe do processo %d a candidatura de %d", processo_que_enviou, candidato_recebido);

        // Caso o id candidato for maior que o atual, então atualiza o lider e envia a nova candidatura
        if ( candidato_recebido > processo[id_processo].lider) {
            const int envia_para = (id_processo+1) % N;
            processo[id_processo].lider = candidato_recebido;
            schedule(RECEBE_CANDIDATURA, +10.0, envia_para);
            info(id_processo, "envia candidatura de %d para %d", candidato_recebido, envia_para);
        }

        // Caso um processo recebe de volta sua própria mensagem informando que é candidato, então ele é o lider
        if ( candidato_recebido == id_processo ) {
            info(id_processo, "Fui eleito lider");
        }
    } break;

    [...]
}
```



## 1.3. Caso de Execução do Algoritmo 1 para 1 único candidato

```bash
./algoritmo1 10 b
```

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=10 processos.
           Tempo Total de Simulação = 300
===============================================================
[10.0, Proc. 0, Lider  0]: estou iniciando a eleição e sou candidato
[11.0, Proc. 1, Lider -1]: estou iniciando a eleição
[12.0, Proc. 2, Lider -1]: estou iniciando a eleição
[13.0, Proc. 3, Lider -1]: estou iniciando a eleição
[14.0, Proc. 4, Lider -1]: estou iniciando a eleição
[15.0, Proc. 5, Lider -1]: estou iniciando a eleição
[15.0, Proc. 1, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 2
[16.0, Proc. 6, Lider -1]: estou iniciando a eleição
[17.0, Proc. 7, Lider -1]: estou iniciando a eleição
[18.0, Proc. 8, Lider -1]: estou iniciando a eleição
[19.0, Proc. 9, Lider -1]: estou iniciando a eleição
[25.0, Proc. 2, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 3
[35.0, Proc. 3, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 4
[45.0, Proc. 4, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 5
[55.0, Proc. 5, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 6
[65.0, Proc. 6, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 7
[75.0, Proc. 7, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 8
[85.0, Proc. 8, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 9
[95.0, Proc. 9, Lider  0]: recebe candidatura 0, atualiza lider e reenvia candidatura para 0
[105.0, Proc. 0, Lider  0]: Fui eleito lider
[150.0, Proc. 0, Lider  0]: estou testando
[150.0, Proc. 1, Lider  0]: estou testando
[150.0, Proc. 2, Lider  0]: estou testando
[150.0, Proc. 3, Lider  0]: estou testando
[150.0, Proc. 4, Lider  0]: estou testando
[150.0, Proc. 5, Lider  0]: estou testando
[150.0, Proc. 6, Lider  0]: estou testando
[150.0, Proc. 7, Lider  0]: estou testando
[150.0, Proc. 8, Lider  0]: estou testando
[150.0, Proc. 9, Lider  0]: estou testando
[180.0, Proc. 0, Lider  0]: estou testando

```

Mensagens enviadas: 9 mensagens


## 1.4. Caso de Execução do Algoritmo 1 para vários candidatos selecionados aleatoriamente

```bash
./algoritmo1 10 a
```

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=10 processos.
           Tempo Total de Simulação = 300
===============================================================
[10.0, Proc. 0, Lider -1]: estou iniciando a eleição
[11.0, Proc. 1, Lider -1]: estou iniciando a eleição
[12.0, Proc. 2, Lider -1]: estou iniciando a eleição
[13.0, Proc. 3, Lider  3]: estou iniciando a eleição e sou candidato
[14.0, Proc. 4, Lider  4]: estou iniciando a eleição e sou candidato
[15.0, Proc. 5, Lider -1]: estou iniciando a eleição
[16.0, Proc. 6, Lider  6]: estou iniciando a eleição e sou candidato
[17.0, Proc. 7, Lider  7]: estou iniciando a eleição e sou candidato
[18.0, Proc. 8, Lider -1]: estou iniciando a eleição
[18.0, Proc. 4, Lider  4]: recebe candidatura 3 do processo 3, descarta a candidatura
[19.0, Proc. 9, Lider -1]: estou iniciando a eleição
[19.0, Proc. 5, Lider  4]: recebe candidatura 4, atualiza lider e reenvia candidatura para 6
[21.0, Proc. 7, Lider  7]: recebe candidatura 6 do processo 6, descarta a candidatura
[22.0, Proc. 8, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 9
[29.0, Proc. 6, Lider  6]: recebe candidatura 4 do processo 5, descarta a candidatura
[32.0, Proc. 9, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 0
[42.0, Proc. 0, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 1
[52.0, Proc. 1, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 2
[62.0, Proc. 2, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 3
[72.0, Proc. 3, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 4
[82.0, Proc. 4, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 5
[92.0, Proc. 5, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 6
[102.0, Proc. 6, Lider  7]: recebe candidatura 7, atualiza lider e reenvia candidatura para 7
[112.0, Proc. 7, Lider  7]: Fui eleito lider
[150.0, Proc. 0, Lider  7]: estou testando
[150.0, Proc. 1, Lider  7]: estou testando
[150.0, Proc. 2, Lider  7]: estou testando
[150.0, Proc. 3, Lider  7]: estou testando
[150.0, Proc. 4, Lider  7]: estou testando
[150.0, Proc. 5, Lider  7]: estou testando
[150.0, Proc. 6, Lider  7]: estou testando
[150.0, Proc. 7, Lider  7]: estou testando
[150.0, Proc. 8, Lider  7]: estou testando
[150.0, Proc. 9, Lider  7]: estou testando
[180.0, Proc. 0, Lider  7]: estou testando
```

Mensagens enviadas: 13 mensagens


## 1.5. Caso de Execução do Algoritmo 1 para todos os N processos inicialmente candidatos

```bash
./algoritmo1 10 c
```

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=10 processos.
           Tempo Total de Simulação = 300
===============================================================
[10.0, Proc. 0, Lider  0]: estou iniciando a eleição e sou candidato
[11.0, Proc. 1, Lider  1]: estou iniciando a eleição e sou candidato
[12.0, Proc. 2, Lider  2]: estou iniciando a eleição e sou candidato
[13.0, Proc. 3, Lider  3]: estou iniciando a eleição e sou candidato
[14.0, Proc. 4, Lider  4]: estou iniciando a eleição e sou candidato
[15.0, Proc. 5, Lider  5]: estou iniciando a eleição e sou candidato
[15.0, Proc. 1, Lider  1]: recebe candidatura 0 do processo 0, descarta a candidatura
[16.0, Proc. 6, Lider  6]: estou iniciando a eleição e sou candidato
[16.0, Proc. 2, Lider  2]: recebe candidatura 1 do processo 1, descarta a candidatura
[17.0, Proc. 7, Lider  7]: estou iniciando a eleição e sou candidato
[17.0, Proc. 3, Lider  3]: recebe candidatura 2 do processo 2, descarta a candidatura
[18.0, Proc. 8, Lider  8]: estou iniciando a eleição e sou candidato
[18.0, Proc. 4, Lider  4]: recebe candidatura 3 do processo 3, descarta a candidatura
[19.0, Proc. 9, Lider  9]: estou iniciando a eleição e sou candidato
[19.0, Proc. 5, Lider  5]: recebe candidatura 4 do processo 4, descarta a candidatura
[20.0, Proc. 6, Lider  6]: recebe candidatura 5 do processo 5, descarta a candidatura
[21.0, Proc. 7, Lider  7]: recebe candidatura 6 do processo 6, descarta a candidatura
[22.0, Proc. 8, Lider  8]: recebe candidatura 7 do processo 7, descarta a candidatura
[23.0, Proc. 9, Lider  9]: recebe candidatura 8 do processo 8, descarta a candidatura
[24.0, Proc. 0, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 1
[34.0, Proc. 1, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 2
[44.0, Proc. 2, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 3
[54.0, Proc. 3, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 4
[64.0, Proc. 4, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 5
[74.0, Proc. 5, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 6
[84.0, Proc. 6, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 7
[94.0, Proc. 7, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 8
[104.0, Proc. 8, Lider  9]: recebe candidatura 9, atualiza lider e reenvia candidatura para 9
[114.0, Proc. 9, Lider  9]: Fui eleito lider
[150.0, Proc. 0, Lider  9]: estou testando
[150.0, Proc. 1, Lider  9]: estou testando
[150.0, Proc. 2, Lider  9]: estou testando
[150.0, Proc. 3, Lider  9]: estou testando
[150.0, Proc. 4, Lider  9]: estou testando
[150.0, Proc. 5, Lider  9]: estou testando
[150.0, Proc. 6, Lider  9]: estou testando
[150.0, Proc. 7, Lider  9]: estou testando
[150.0, Proc. 8, Lider  9]: estou testando
[150.0, Proc. 9, Lider  9]: estou testando
[180.0, Proc. 0, Lider  9]: estou testando
```

Mensagens enviadas: 18 mensagens


# 2. ALGORITMO 2

## 2.0. Enunciado

Implemente o algoritmo aleatorizado para eleição de líder (Randomized Leader Election Algorithm) baseado no anel (VRing). Neste algoritmo cada processo sorteia, em cada rodada um bit aleatório. O algoritmo funciona em rodadas. Na primeira rodada todos os N processos sorteiam um valor aleatório para o seu bit. Os processos com bit = 1 são candidatos, os demais processos (i.e. com seu bit = 0) não são candidatos. Cada processo deve enviar uma mensagem que roda todo o anel informando seu id e seu bit da rodada. Na rodada seguinte, apenas os processos com bit = 1 sorteiam novo valor aleatório para o bit. Esses processos então comunicam novamente no anel o valor dos seus bits. Novas rodadas vão acontecendo até que 1 processo se transforme em líder. Execute o algoritmo para diferentes valores de N. Destaque para cada execução quem é o líder eleito. Conte quantas mensagens foram necessárias em cada caso e também quanto tempo (do SMPL) foi necessário para executar a eleição. 

## 2.1. Algoritmo

### Quando INICIO_ELEICAO:
 - inicia as variaveis locais do processo
 - sorteia o numero, 0 para não candidato e 1 para candidato
 - agenda evento RECEBE_VALORES_SORTEADOS

### Quando RECEBE_VALORES_SORTEADOS:
  - 1. Inicializa variaveis necessarias
  - 2a. Caso a rodada recebida seja maior que a rodada corrente
    - salva o atual valor sorteado, limpa os valores sorteados para a nova rodada e recebe os dados do sorteio
    - sorteia um novo número, caso o processo seja candidato
  - 2b. Caso a rodada recebida seja igual à rodada corrente, apenas recebe os sorteio
  - 3. Verifica se o processo tem o sorteio de todos os processos
    - Conta quandos candidatos tem no vetor
    - Se tiver apenas um, então lider foi eleito
    - Senão inicia uma nova rodada e agenda evento RECEBE_VALORES_SORTEADOS

## 2.2. Código

```c
int main() {
    [...]

    case INICIO_ELEICAO: {
        // inicia as variaveis locais do processo 
        processo[id_processo].lider = -1;
        processo[id_processo].rodada = 0;
        for (int j=0; j<N; j++) {
            processo[id_processo].valores_sorteados[j] = -1;
            processo[id_processo].foi_alterado[j] = false;
        }

        // sorteia o numero, 0 para não candidato e 1 para candidato
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
            recebe_valores_sorteados(id_processo, processo_que_enviou, N);

            // Caso o processo seja ainda candidato, sorteia um novo numero
            if ( valor_atual == 1 ) {               
                const int valor_sorteado = randomic(0,1);
                processo[id_processo].valores_sorteados[id_processo] = valor_sorteado;
                processo[id_processo].foi_alterado[id_processo] = true;
                info(id_processo, "Recebido mensagem de %d com nova rodada e sorteie o numero %d", processo_que_enviou, valor_sorteado);

            // Caso o processo não seja candidato, apenas copia os dados recebidos
            } else {
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
                // Aumenta a rodada e set -1 em todos os candidatos
                info(id_processo, "tenho todos os sorteados, mas nao foi eleito");
                const int rodada = processo[id_processo].rodada;
                const int valor_sorteado_anterior = processo[id_processo].valores_sorteados[id_processo];
                processo[id_processo].rodada += 1;
                limpa_valores_sorteados_dos_candidatos(id_processo, N);

                // Se é candidato, então sorteio um novo numero
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

}
```



## 2.3. Caso de Execução do Algoritmo 2

```
===============================================================
           Sistemas Distribuí­dos Prof. Elias
          LOG do Trabalho Prático 0, Tarefa 0
      Digitar, compilar e executar o programa tempo.c
   Este programa foi executado para: N=10 processos.
           Tempo Total de Simulação = 300
===============================================================
[40.0, Proc. 0, Rodada 0, Sorteio:  1 -1 -1 -1 -1 -1 -1 -1 -1 -1  ]: Inicio da eleição
[41.0, Proc. 1, Rodada 0, Sorteio: -1  0 -1 -1 -1 -1 -1 -1 -1 -1  ]: Inicio da eleição
[42.0, Proc. 2, Rodada 0, Sorteio: -1 -1  0 -1 -1 -1 -1 -1 -1 -1  ]: Inicio da eleição
[43.0, Proc. 3, Rodada 0, Sorteio: -1 -1 -1  0 -1 -1 -1 -1 -1 -1  ]: Inicio da eleição
[44.0, Proc. 4, Rodada 0, Sorteio: -1 -1 -1 -1  1 -1 -1 -1 -1 -1  ]: Inicio da eleição
[45.0, Proc. 5, Rodada 0, Sorteio: -1 -1 -1 -1 -1  0 -1 -1 -1 -1  ]: Inicio da eleição
[46.0, Proc. 6, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1  1 -1 -1 -1  ]: Inicio da eleição
[47.0, Proc. 7, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1 -1  1 -1 -1  ]: Inicio da eleição
[48.0, Proc. 8, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1 -1 -1  1 -1  ]: Inicio da eleição
[49.0, Proc. 9, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1 -1 -1 -1  1  ]: Inicio da eleição


[60.0, Proc. 0, Rodada 0, Sorteio:  1 -1 -1 -1 -1 -1 -1 -1 -1 -1  ]: Recebido mensagem de 9
[61.0, Proc. 1, Rodada 0, Sorteio: -1  0 -1 -1 -1 -1 -1 -1 -1 -1  ]: Recebido mensagem de 0
[62.0, Proc. 2, Rodada 0, Sorteio: -1 -1  0 -1 -1 -1 -1 -1 -1 -1  ]: Recebido mensagem de 1
[63.0, Proc. 3, Rodada 0, Sorteio: -1 -1 -1  0 -1 -1 -1 -1 -1 -1  ]: Recebido mensagem de 2
[64.0, Proc. 4, Rodada 0, Sorteio: -1 -1 -1 -1  1 -1 -1 -1 -1 -1  ]: Recebido mensagem de 3
[65.0, Proc. 5, Rodada 0, Sorteio: -1 -1 -1 -1 -1  0 -1 -1 -1 -1  ]: Recebido mensagem de 4
[66.0, Proc. 6, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1  1 -1 -1 -1  ]: Recebido mensagem de 5
[67.0, Proc. 7, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1 -1  1 -1 -1  ]: Recebido mensagem de 6
[68.0, Proc. 8, Rodada 0, Sorteio: -1 -1 -1 -1 -1 -1 -1 -1  1 -1  ]: Recebido mensagem de 7
[68.0, Proc. 8, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1  1  1  ]: tenho todos os sorteados, mas nao foi eleito


[68.0, Proc. 8, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0 -1  ]: Nova rodada 0 -> 1
[69.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8 com nova rodada e sorteie o numero 0
[70.0, Proc. 1, Rodada 0, Sorteio:  1  0 -1 -1 -1 -1 -1 -1 -1  1  ]: Recebido mensagem de 0
[71.0, Proc. 2, Rodada 0, Sorteio:  1  0  0 -1 -1 -1 -1 -1 -1  1  ]: Recebido mensagem de 1
[72.0, Proc. 3, Rodada 0, Sorteio:  1  0  0  0 -1 -1 -1 -1 -1  1  ]: Recebido mensagem de 2
[73.0, Proc. 4, Rodada 0, Sorteio:  1  0  0  0  1 -1 -1 -1 -1  1  ]: Recebido mensagem de 3
[74.0, Proc. 5, Rodada 0, Sorteio:  1  0  0  0  1  0 -1 -1 -1  1  ]: Recebido mensagem de 4
[75.0, Proc. 6, Rodada 0, Sorteio:  1  0  0  0  1  0  1 -1 -1  1  ]: Recebido mensagem de 5
[76.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[78.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[79.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9 com nova rodada e sorteie o numero 0
[80.0, Proc. 2, Rodada 0, Sorteio:  1  0  0 -1 -1 -1 -1 -1 -1  1  ]: Recebido mensagem de 1
[81.0, Proc. 3, Rodada 0, Sorteio:  1  0  0  0 -1 -1 -1 -1 -1  1  ]: Recebido mensagem de 2
[82.0, Proc. 4, Rodada 0, Sorteio:  1  0  0  0  1 -1 -1 -1 -1  1  ]: Recebido mensagem de 3
[83.0, Proc. 5, Rodada 0, Sorteio:  1  0  0  0  1  0 -1 -1 -1  1  ]: Recebido mensagem de 4
[84.0, Proc. 6, Rodada 0, Sorteio:  1  0  0  0  1  0  1 -1 -1  1  ]: Recebido mensagem de 5
[85.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[87.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[88.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[89.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Processo 1 e recebeu mensagem de 0 com nova rodada e nao sou candidato
[90.0, Proc. 3, Rodada 0, Sorteio:  1  0  0  0 -1 -1 -1 -1 -1  1  ]: Recebido mensagem de 2
[91.0, Proc. 4, Rodada 0, Sorteio:  1  0  0  0  1 -1 -1 -1 -1  1  ]: Recebido mensagem de 3
[92.0, Proc. 5, Rodada 0, Sorteio:  1  0  0  0  1  0 -1 -1 -1  1  ]: Recebido mensagem de 4
[93.0, Proc. 6, Rodada 0, Sorteio:  1  0  0  0  1  0  1 -1 -1  1  ]: Recebido mensagem de 5
[94.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[96.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[97.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[98.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 0
[99.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Processo 2 e recebeu mensagem de 1 com nova rodada e nao sou candidato
[100.0, Proc. 4, Rodada 0, Sorteio:  1  0  0  0  1 -1 -1 -1 -1  1  ]: Recebido mensagem de 3
[101.0, Proc. 5, Rodada 0, Sorteio:  1  0  0  0  1  0 -1 -1 -1  1  ]: Recebido mensagem de 4
[102.0, Proc. 6, Rodada 0, Sorteio:  1  0  0  0  1  0  1 -1 -1  1  ]: Recebido mensagem de 5
[103.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[105.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[106.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[107.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 0
[108.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 1
[109.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Processo 3 e recebeu mensagem de 2 com nova rodada e nao sou candidato
[110.0, Proc. 5, Rodada 0, Sorteio:  1  0  0  0  1  0 -1 -1 -1  1  ]: Recebido mensagem de 4
[111.0, Proc. 6, Rodada 0, Sorteio:  1  0  0  0  1  0  1 -1 -1  1  ]: Recebido mensagem de 5
[112.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[114.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[115.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[116.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 0
[117.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 1
[118.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 2
[119.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Recebido mensagem de 3 com nova rodada e sorteie o numero 0
[120.0, Proc. 6, Rodada 0, Sorteio:  1  0  0  0  1  0  1 -1 -1  1  ]: Recebido mensagem de 5
[121.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[123.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[124.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[125.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 0
[126.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 1
[127.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 2
[128.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Recebido mensagem de 3
[129.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Processo 5 e recebeu mensagem de 4 com nova rodada e nao sou candidato
[130.0, Proc. 7, Rodada 0, Sorteio:  1  0  0  0  1  0  1  1 -1  1  ]: Recebido mensagem de 6
[132.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[133.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[134.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 0
[135.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 1
[136.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 2
[137.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Recebido mensagem de 3
[138.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Recebido mensagem de 4
[139.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0 -1  0  0  ]: Recebido mensagem de 5 com nova rodada e sorteie o numero 0
[141.0, Proc. 9, Rodada 1, Sorteio: -1  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 8
[142.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 9
[143.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 0
[144.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 1
[145.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0 -1  0 -1 -1  0  0  ]: Recebido mensagem de 2
[146.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Recebido mensagem de 3
[147.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0 -1 -1  0  0  ]: Recebido mensagem de 4
[148.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0 -1  0  0  ]: Recebido mensagem de 5


[149.0, Proc. 7, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 6 com nova rodada e sorteie o numero 1
[149.0, Proc. 7, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider

[...]

[233.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[234.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 0
[234.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[235.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 1
[235.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[236.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 2
[236.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[237.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 3
[237.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[238.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 4
[238.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[239.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0 -1  0  0  ]: Recebido mensagem de 5
[239.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[240.0, Proc. 8, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 7
[240.0, Proc. 8, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[241.0, Proc. 9, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 8
[241.0, Proc. 9, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[242.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 9
[242.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[243.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 0
[243.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[244.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 1
[244.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[245.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 2
[245.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[246.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 3
[246.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[247.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 4
[247.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[248.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 5
[248.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[249.0, Proc. 7, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 6
[249.0, Proc. 7, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[250.0, Proc. 9, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 8
[250.0, Proc. 9, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[251.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 9
[251.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[252.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 0
[252.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[253.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 1
[253.0, Proc. 2, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[254.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 2
[254.0, Proc. 3, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[255.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 3
[255.0, Proc. 4, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[256.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 4
[256.0, Proc. 5, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[257.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 5
[257.0, Proc. 6, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[258.0, Proc. 7, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 6
[258.0, Proc. 7, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[259.0, Proc. 8, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 7
[259.0, Proc. 8, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[260.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 9
[260.0, Proc. 0, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
[261.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Recebido mensagem de 0
[261.0, Proc. 1, Rodada 1, Sorteio:  0  0  0  0  0  0  0  1  0  0  ]: Eleito um lider
```

Outros Logs:
 - [Log 10 processos](https://github.com/bombark/tp_sisdis_2026_1/blob/main/log_algoritmo2_10.txt)
 - [Log 20 processos](https://github.com/bombark/tp_sisdis_2026_1/blob/main/log_algoritmo2_20.txt)


# Extra - Ideia para SMPL 2

Ideia principal: O mesmo código ser possível de simular usando SMPL e também de executar na prática
usando algum protocolo de comunicação.

A arquitetura do SMPL 2 seria constituido em dois arquivos. O primeiro arquivo lançaria os processos e 
agendaria os eventos. Segundo arquivo seria o código do processo em si. Segue a seguir um exemplo de lançador.

```c
// main_lançador.c
int main() {
    smpl2_launcher_init();

    // inicializa 10 processos
    smpl2_launcher_node("./processo", 10);  

    // agenda os eventos
    smpl2_launcher_schedule(TEST, 70, 0);
    smpl2_launcher_schedule(FAULT, 80, 1);
    smpl2_launcher_schedule(TEST, 90, 2);

    // espera os processos executarem
    smpl2_launcher_wait();

    // fim
    smpl2_launcher_close();
    return 0;
}
```

A seguir segue um exemplo de um código de um processo.

```c
int main(int argc, char** argv) {
    // Inicio
    smpl2_init(argc, argv);

    // Algumas funções para pegar o id do processo corrent
    const int id_processo = smpl2_id();
    const int N = smpl2_nprocess();

    // Loop principal
    while (1) {
        const int event = smpl2_cause();

        // Recebido o evento periodico para testar o nodo 
        if ( event == TEST ) {
            int proximo_processo = (id_processo + 1) % N;
            smpl2_sendto(proximo_processo, "%s", "test");

        // Recebido uma mensagem de teste
        } else if ( event == RECEIVE ) {
            smpl2_answer("%s", "OK");
        }
    }

    // Fim
    printf("FIM\n");
    return 0;
}
```

Infelizmente, não deu tempo para fazer algo funcional. Tentei inicialmente carregar o código do processo via biblioteca dinamica e diferentes threads para simular diferentes processos, mas compartilhando a mesma memória e utilizando diretamente as funções SMPL. Porém não foi uma boa abordagem.

Atualmente, cada processo é executado por fork, e existe um processo lançador que guarda os eventos e envia os eventos para os processos. Os processos no modo simulador fica preso até receber um evento do lançador. Ainda falta muita coisa para fazer :/
