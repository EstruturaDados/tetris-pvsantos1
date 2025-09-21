#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h> // Para usar memcpy

// --- Constantes Globais ---
#define TAMANHO_FILA 5
#define CAPACIDADE_PILHA 3

// --- Estrutura de Dados ---
struct Peca {
    char nome;
    int id;
};

struct EstadoJogo {
    struct Peca fila[TAMANHO_FILA];
    struct Peca pilha[CAPACIDADE_PILHA];
    int front;
    int rear;
    int topo;
    int proximoId;
};

struct EstadoJogo estadoAtual;
struct EstadoJogo estadoAnterior;
bool podeDesfazer = false;

// --- Protótipos ---
void inicializarJogo();
struct Peca gerarPeca();
void visualizarFila();
void visualizarPilha();
void salvarEstado();
void jogarPeca();
void reservarPeca();
void usarPecaReservada();
void trocarPeca();
void desfazerJogada();
void inverterFilaComPilha();
void limparTela();

// --- Função Principal ---
int main() {
    srand(time(NULL));
    inicializarJogo();
    int opcao = -1;
    char bufferEntrada[10];

    do {
        limparTela();
        printf("============================================\n");
        printf("     TETRIS STACK - Integracao Total      \n");
        printf("                 NIVEL MESTRE               \n");
        printf("============================================\n\n");
        
        visualizarFila();
        printf("\n");
        visualizarPilha();

        printf("\n--- MENU DE ACOES ESTRATEGICAS ---\n");
        printf("1. Jogar Peca da Fila\n");
        printf("2. Reservar Peca\n");
        printf("3. Usar Peca Reservada\n");
        printf("4. Trocar (Frente da Fila <-> Topo da Pilha)\n");
        printf("5. Desfazer Ultima Jogada %s\n", podeDesfazer ? "(Disponivel)" : "(Indisponivel)");
        printf("6. Inverter Fila com Pilha\n");
        printf("0. Sair do Jogo\n\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin)) {
            sscanf(bufferEntrada, "%d", &opcao);
        }
        if (opcao >= 1 && opcao <= 4 || opcao == 6) {
            salvarEstado();
        }
        switch (opcao) {
            case 1: jogarPeca(); break;
            case 2: reservarPeca(); break;
            case 3: usarPecaReservada(); break;
            case 4: trocarPeca(); break;
            case 5: desfazerJogada(); break;
            case 6: inverterFilaComPilha(); break;
            case 0: printf("\nSaindo do jogo...\n"); continue;
            default: printf("\nOpcao invalida!\n"); break;
        }

        printf("\nPressione Enter para continuar...");
        getchar();

    } while (opcao != 0);
    return 0;
}

// --- Funções de Inicialização e Geração ---
struct Peca gerarPeca() {
    struct Peca novaPeca;
    char nomesPossiveis[] = {'I', 'O', 'T', 'L'};
    novaPeca.nome = nomesPossiveis[rand() % 4];
    novaPeca.id = estadoAtual.proximoId++;
    return novaPeca;
}

void inicializarJogo() {
    estadoAtual.front = 0;
    estadoAtual.rear = TAMANHO_FILA - 1;
    estadoAtual.topo = -1;
    estadoAtual.proximoId = 1;
    for (int i = 0; i < TAMANHO_FILA; i++) {
        estadoAtual.fila[i] = gerarPeca();
    }
}

// --- Funções de Visualização ---
void visualizarFila() {
    printf("--> Fila de Proximas Pecas\n\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        int idx = (estadoAtual.front + i) % TAMANHO_FILA;
        printf("  Pos %d: [ Peca: %c | ID: %d ]", i + 1, estadoAtual.fila[idx].nome, estadoAtual.fila[idx].id);
        if (idx == estadoAtual.front) printf(" <-- FRENTE");
        printf("\n");
    }
}

void visualizarPilha() {
    printf("--> Reserva de Pecas (Capacidade: %d)\n\n", CAPACIDADE_PILHA);
    if (estadoAtual.topo == -1) {
        printf("  [ VAZIO ]\n");
        return;
    }
    for (int i = 0; i <= estadoAtual.topo; i++) {
        printf("  Nivel %d: [ Peca: %c | ID: %d ]", i + 1, estadoAtual.pilha[i].nome, estadoAtual.pilha[i].id);
        if (i == estadoAtual.topo) printf(" <-- TOPO");
        printf("\n");
    }
}


// --- FUNÇÕES DE ESTADO (Desfazer) ---
void salvarEstado() {
    memcpy(&estadoAnterior, &estadoAtual, sizeof(struct EstadoJogo));
    podeDesfazer = true;
    printf("\n(Ponto de restauracao criado...)\n");
}

void desfazerJogada() {
    if (!podeDesfazer) {
        printf("\nERRO: Nao ha acao para desfazer!\n");
        return;
    }
    memcpy(&estadoAtual, &estadoAnterior, sizeof(struct EstadoJogo));
    podeDesfazer = false;
    printf("\n-->> Ultima jogada foi desfeita! <<--\n");
}

// --- FUNÇÕES DE AÇÃO ---
void jogarPeca() {
    struct Peca pecaJogada = estadoAtual.fila[estadoAtual.front];
    printf("\n-->> Peca da FILA '%c' (ID: %d) foi jogada! <<--\n", pecaJogada.nome, pecaJogada.id);
    estadoAtual.fila[estadoAtual.front] = gerarPeca();
    estadoAtual.rear = estadoAtual.front;
    estadoAtual.front = (estadoAtual.front + 1) % TAMANHO_FILA;
}

void reservarPeca() {
    if (estadoAtual.topo >= CAPACIDADE_PILHA - 1) {
        printf("\nERRO: A reserva esta cheia!\n");
        podeDesfazer = false;
        return;
    }
    struct Peca pecaParaReserva = estadoAtual.fila[estadoAtual.front];
    estadoAtual.topo++;
    estadoAtual.pilha[estadoAtual.topo] = pecaParaReserva;
    printf("\n-->> Peca '%c' (ID: %d) movida para a Reserva! <<--\n", pecaParaReserva.nome, pecaParaReserva.id);
    
    estadoAtual.fila[estadoAtual.front] = gerarPeca();
    estadoAtual.rear = estadoAtual.front;
    estadoAtual.front = (estadoAtual.front + 1) % TAMANHO_FILA;
}

void usarPecaReservada() {
    if (estadoAtual.topo == -1) {
        printf("\nERRO: Nao ha pecas na reserva!\n");
        podeDesfazer = false;
        return;
    }
    struct Peca pecaUsada = estadoAtual.pilha[estadoAtual.topo];
    estadoAtual.topo--;
    printf("\n-->> Peca RESERVADA '%c' (ID: %d) foi usada! <<--\n", pecaUsada.nome, pecaUsada.id);
}

void trocarPeca() {
    if (estadoAtual.topo == -1) {
        printf("\nERRO: Impossivel trocar, a reserva esta vazia!\n");
        podeDesfazer = false;
        return;
    }
    struct Peca temp = estadoAtual.pilha[estadoAtual.topo];
    estadoAtual.pilha[estadoAtual.topo] = estadoAtual.fila[estadoAtual.front];
    estadoAtual.fila[estadoAtual.front] = temp;
    printf("\n-->> Peca '%c' (Fila) trocada com a peca '%c' (Reserva)! <<--\n", estadoAtual.pilha[estadoAtual.topo].nome, temp.nome);
}

void inverterFilaComPilha() {
    struct Peca tempFila[TAMANHO_FILA];
    int numItensPilha = estadoAtual.topo + 1;
    // 1. Salva o conteúdo lógico da fila atual
    for(int i=0; i < TAMANHO_FILA; i++){
        tempFila[i] = estadoAtual.fila[(estadoAtual.front + i) % TAMANHO_FILA];
    }
    // 2. A nova pilha recebe as 3 primeiras peças da antiga fila
    for(int i=0; i < CAPACIDADE_PILHA; i++){
        estadoAtual.pilha[i] = tempFila[i];
    }
    estadoAtual.topo = CAPACIDADE_PILHA - 1;
    // 3. A nova fila recebe as peças da antiga pilha
    for(int i=0; i < numItensPilha; i++){
        estadoAtual.fila[i] = estadoAnterior.pilha[i];
    }
    // 4. Preenche o resto da fila com peças novas
    for(int i=numItensPilha; i < TAMANHO_FILA; i++){
        estadoAtual.fila[i] = gerarPeca();
    }
    estadoAtual.front = 0;
    estadoAtual.rear = TAMANHO_FILA - 1;
    printf("\n-->> Fila e Pilha foram invertidas! <<--\n");
}
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}