#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- Constantes Globais ---
#define TAMANHO_FILA 5
#define CAPACIDADE_PILHA 3

// --- Estrutura de Dados ---
struct Peca {
    char nome;
    int id;
};

// --- Variáveis Globais para a FILA ---
struct Peca fila[TAMANHO_FILA];
int front = 0;
int rear = TAMANHO_FILA - 1;
int proximoId = 1;

// --- Variáveis Globais para a PILHA ---
struct Peca pilha[CAPACIDADE_PILHA];
int topo = -1;

// --- Protótipos das Funções ---
void inicializarFila();
struct Peca gerarPeca();
void visualizarFila();
void visualizarPilha();
void jogarPeca();
void reservarPeca();
void usarPecaReservada();
void limparTela();

// --- Função Principal ---
int main() {
    srand(time(NULL));
    inicializarFila();
    int opcao = -1;
    char bufferEntrada[10];

    do {
        limparTela();
        printf("============================================\n");
        printf("       TETRIS STACK - Reserva de Pecas      \n");
        printf("              NIVEL AVENTUREIRO             \n");
        printf("============================================\n\n");
        
        visualizarFila();
        printf("\n");
        visualizarPilha();

        printf("\n--- MENU DE ACOES ---\n");
        printf("1. Jogar Peca da Fila\n");
        printf("2. Reservar Peca (da Fila para a Pilha)\n");
        printf("3. Usar Peca Reservada (da Pilha)\n");
        printf("0. Sair do Jogo\n\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin)) {
            sscanf(bufferEntrada, "%d", &opcao);
        }
        
        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\nSaindo do jogo... Ate a proxima!\n");
                continue; // Pula a pausa para sair direto
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
        printf("\nPressione Enter para continuar...");
        getchar();
    } while (opcao != 0);
    return 0;
}

// --- Funções da Fila --
struct Peca gerarPeca() {
    struct Peca novaPeca;
    char nomesPossiveis[] = {'I', 'O', 'T', 'L'};
    novaPeca.nome = nomesPossiveis[rand() % 4];
    novaPeca.id = proximoId++;
    return novaPeca;
}

void inicializarFila() {
    for (int i = 0; i < TAMANHO_FILA; i++) {
        fila[i] = gerarPeca();
    }
}

void visualizarFila() {
    printf("--> Fila de Proximas Pecas (Sempre com 5)\n\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        int indiceAtual = (front + i) % TAMANHO_FILA;
        printf("  Pos %d: [ Peca: %c | ID: %d ]", i + 1, fila[indiceAtual].nome, fila[indiceAtual].id);
        if (indiceAtual == front) printf(" <-- FRENTE");
        printf("\n");
    }
}

void jogarPeca() {
    struct Peca pecaJogada = fila[front];
    printf("\n-->> Peca da FILA '%c' (ID: %d) foi jogada! <<--\n", pecaJogada.nome, pecaJogada.id);
    struct Peca novaPeca = gerarPeca();
    printf("      Nova peca '%c' (ID: %d) entrou no final da fila.\n", novaPeca.nome, novaPeca.id);
    fila[front] = novaPeca;
    rear = front;
    front = (front + 1) % TAMANHO_FILA;
}

void reservarPeca() {
    // Verifica se a pilha está cheia (push)
    if (topo >= CAPACIDADE_PILHA - 1) {
        printf("\nERRO: A reserva de pecas esta cheia! Nao e possivel reservar.\n");
        return;
    }
    struct Peca pecaParaReserva = fila[front];
    // Push: Adiciona a peça no topo da pilha
    topo++;
    pilha[topo] = pecaParaReserva;
    printf("\n-->> Peca '%c' (ID: %d) movida da Fila para a Reserva! <<--\n", pecaParaReserva.nome, pecaParaReserva.id);
    struct Peca novaPeca = gerarPeca();
    printf("      Nova peca '%c' (ID: %d) entrou no final da fila.\n", novaPeca.nome, novaPeca.id);
    fila[front] = novaPeca;
    rear = front;
    front = (front + 1) % TAMANHO_FILA;
}

void usarPecaReservada() {
    if (topo == -1) {
        printf("\nERRO: Nao ha pecas na reserva para usar!\n");
        return;
    }
    struct Peca pecaUsada = pilha[topo];
    topo--;
    printf("\n-->> Peca RESERVADA '%c' (ID: %d) foi usada! <<--\n", pecaUsada.nome, pecaUsada.id);
}

void visualizarPilha() {
    printf("--> Reserva de Pecas (Capacidade: %d)\n\n", CAPACIDADE_PILHA);
    if (topo == -1) {
        printf("  [ VAZIO ]\n");
        return;
    }
    for (int i = 0; i <= topo; i++) {
        printf("  Nivel %d: [ Peca: %c | ID: %d ]", i + 1, pilha[i].nome, pilha[i].id);
        if (i == topo) {
            printf(" <-- TOPO");
        }
        printf("\n");
    }
}
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}