#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Constantes Globais ---
#define TAMANHO_FILA 5

// --- Estrutura de Dados ---
struct Peca {
    char nome;
    int id;
};

// --- Variáveis Globais ---
struct Peca fila[TAMANHO_FILA];
int front = 0;
int rear = TAMANHO_FILA - 1;
int proximoId = 1;

// --- Protótipos das Funções ---
void inicializarFila();
struct Peca gerarPeca();
void jogarPeca();
void visualizarFila();
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
        printf("       TETRIS STACK - Pecas Futuras         \n");
        printf("               NIVEL NOVATO                 \n");
        printf("============================================\n\n");
        visualizarFila();
        printf("\n--- MENU DE ACOES ---\n");
        printf("1. Jogar Peca (Remove da frente, insere no fim)\n");
        printf("0. Sair do Jogo\n\n");
        printf("Escolha uma opcao: ");
        if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin)) {
            sscanf(bufferEntrada, "%d", &opcao);
        }
        switch (opcao) {
            case 1:
                jogarPeca();
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 0:
                printf("\nSaindo do jogo... Ate a proxima!\n");
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter para tentar novamente.\n");
                getchar();
                break;
        }
    } while (opcao != 0);
    return 0;
}

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
    front = 0;
    rear = TAMANHO_FILA - 1;
}

void visualizarFila() {
    printf("--> Fila de Proximas Pecas <--\n\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        int indiceAtual = (front + i) % TAMANHO_FILA;
        printf("  Pos %d: [ Peca: %c | ID: %d ]", i + 1, fila[indiceAtual].nome, fila[indiceAtual].id);
        if (indiceAtual == front) {
            printf(" <-- (FRENTE - Proxima a jogar)");
        }
        if (indiceAtual == rear) {
            printf(" <-- (FIM - Ultima a chegar)");
        }
        printf("\n");
    }
}

void jogarPeca() {
    struct Peca pecaJogada = fila[front];
    printf("\n-->> Peca '%c' (ID: %d) foi jogada! <<--\n", pecaJogada.nome, pecaJogada.id);
    struct Peca novaPeca = gerarPeca();
    printf("      Nova peca '%c' (ID: %d) entrou no final da fila.\n", novaPeca.nome, novaPeca.id);
    fila[front] = novaPeca;
    rear = front;
    front = (front + 1) % TAMANHO_FILA;
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}