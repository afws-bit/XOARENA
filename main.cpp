
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const int TAMANHO = 5;
int jogadorX, jogadorY;
int reliquiaX, reliquiaY;
bool vivo = true;
bool venceu = false;

// Matrizes: uma com a lógica do jogo (escondida) e uma para a tela do jogador
char mapaLogico[TAMANHO][TAMANHO];
char mapaTela[TAMANHO][TAMANHO];

void inicializarJogo() {
    srand(time(0));
    
    // Preenche os mapas com espaços vazios e a névoa ('#')
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            mapaLogico[i][j] = '.';
            mapaTela[i][j] = '#';
        }
    }

    // Posiciona o jogador
    jogadorX = 0;
    jogadorY = 0;
    mapaTela[jogadorX][jogadorY] = 'P';

    // Posiciona a relíquia (longe do [0][0])
    do {
        reliquiaX = rand() % TAMANHO;
        reliquiaY = rand() % TAMANHO;
    } while (reliquiaX == 0 && reliquiaY == 0);
    mapaLogico[reliquiaX][reliquiaY] = 'R';

    // Posiciona 3 armadilhas aleatórias
    int armadilhasColocadas = 0;
    while (armadilhasColocadas < 3) {
        int ax = rand() % TAMANHO;
        int ay = rand() % TAMANHO;
        if ((ax != 0 || ay != 0) && mapaLogico[ax][ay] == '.') {
            mapaLogico[ax][ay] = 'T'; // T de Trap (Armadilha)
            armadilhasColocadas++;
        }
    }
}

void desenharMapa() {
    cout << "\n=== CACADOR DE RELIQUIAS ===\n";
    cout << "Legenda: P = Voce | # = Nevoa | . = Caminho Livre\n";
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            cout << mapaTela[i][j] << " ";
        }
        cout << "\n";
    }

  
    int distanciaRadar = abs(jogadorX - reliquiaX) + abs(jogadorY - reliquiaY);
    cout << "RADAR: O tesouro esta a " << distanciaRadar << " passos de distancia!\n";
}

void moverJogador(char direcao) {
    int novoX = jogadorX;
    int novoY = jogadorY;

    if (direcao == 'w' || direcao == 'W') novoX--;
    else if (direcao == 's' || direcao == 'S') novoX++;
    else if (direcao == 'a' || direcao == 'A') novoY--;
    else if (direcao == 'd' || direcao == 'D') novoY++;
    else {
        cout << "Comando invalido!\n";
        return;
    }

    // Valida limites do mapa
    if (novoX < 0 || novoX >= TAMANHO || novoY < 0 || novoY >= TAMANHO) {
        cout << "Voce bateu na parede do templo!\n";
        return;
    }

    // Atualiza rastro
    mapaTela[jogadorX][jogadorY] = '.';
    
    // Atualiza posição
    jogadorX = novoX;
    jogadorY = novoY;
    mapaTela[jogadorX][jogadorY] = 'P';

    // Verifica colisões no mapa
    if (mapaLogico[jogadorX][jogadorY] == 'R') {
        venceu = true;
    } else if (mapaLogico[jogadorX][jogadorY] == 'T') {
        vivo = false;
    }
}

int main() {
    inicializarJogo();

    while (vivo && !venceu) {
        desenharMapa();
        cout << "\nMovimento (W=Cima, S=Baixo, A=Esquerda, D=Direita): ";
        char comando;
        cin >> comando;
        moverJogador(comando);
    }

    cout << "\n=== FIM DE JOGO ===\n";
    if (venceu) {
        cout << "PARABENS! Voce encontrou a Reliquia Sagrada!\n";
    } else {
        cout << "KABUM! Voce pisou em uma armadilha oculta e morreu.\n";
    }

    return 0;
}
