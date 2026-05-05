#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define ARQUIVO "historico.txt"

typedef struct {
    int hp;
    int ataque;
    int pocao;
} Personagem;

typedef struct {
    int venceu;
    int turnos;
} Sessao;

// ================= LIMPAR TELA =================
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ================= INIMIGO =================
Personagem gerarInimigo() {
    Personagem inimigo;
    inimigo.hp = rand() % 30 + 20;
    inimigo.ataque = rand() % 8 + 3;
    inimigo.pocao = 0;
    return inimigo;
}

// ================= SALVAR =================
void salvarHistorico(Sessao s) {
    FILE *f = fopen(ARQUIVO, "a");

    if (!f) {
        printf("Erro ao salvar historico!\n");
        return;
    }

    fprintf(f, "%d %d\n", s.venceu, s.turnos);
    fclose(f);
}

// ================= ANALISAR =================
void analisarHistorico() {
    limparTela();

    FILE *f = fopen(ARQUIVO, "r");

    if (!f) {
        printf("\nNenhum historico encontrado.\n");
        return;
    }

    int venceu, turnos;
    int total = 0, vitorias = 0, somaTurnos = 0;

    while (fscanf(f, "%d %d", &venceu, &turnos) != EOF) {
        total++;
        somaTurnos += turnos;
        if (venceu) vitorias++;
    }

    fclose(f);

    if (total == 0) {
        printf("\nSem dados ainda.\n");
        return;
    }

    printf("=== ANALISE ===\n");
    printf("Partidas jogadas: %d\n", total);
    printf("Vitorias: %d\n", vitorias);
    printf("Derrotas: %d\n", total - vitorias);
    printf("Taxa de vitoria: %.2f%%\n", (vitorias * 100.0) / total);
    printf("Media de turnos: %.2f\n", (float)somaTurnos / total);
}

// ================= BATALHA =================
Sessao batalha() {
    Personagem player = {100, 10, 3};
    Personagem inimigo = gerarInimigo();

    Sessao s;
    s.turnos = 0;

    int escolha;

    limparTela();
    printf("Um inimigo apareceu!\n");
    printf("Dica: Derrote o inimigo usando ataques e pocoes.\n");
    printf("Pressione ENTER para comecar...");
    getchar(); getchar();

    while (player.hp > 0 && inimigo.hp > 0) {
        limparTela();

        s.turnos++;

        printf("=== BATALHA ===\n");
        printf("Seu HP: %d | Inimigo: %d\n", player.hp, inimigo.hp);

        // ===== DICAS =====
        printf("\n--- DICAS ---\n");

        if (player.hp < 30 && player.pocao > 0)
            printf("Seu HP esta baixo! Use uma pocao.\n");

        if (player.pocao == 0 && player.hp < 40)
            printf("Voce esta sem pocoes! Tome cuidado.\n");

        if (inimigo.hp < 15)
            printf("O inimigo esta fraco! Ataque agora.\n");

        if (player.hp > 70)
            printf("Voce esta forte! Continue atacando.\n");

        printf("-------------\n");

        printf("\n1. Atacar\n2. Usar pocao (%d)\n3. Fugir\n", player.pocao);
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: {
                int dano = player.ataque + rand() % 5;
                inimigo.hp -= dano;
                printf("Voce causou %d de dano!\n", dano);
                break;
            }

            case 2:
                if (player.pocao > 0) {
                    player.hp += 15;
                    player.pocao--;
                    printf("Voce recuperou vida!\n");
                } else {
                    printf("Sem pocoes!\n");
                }
                break;

            case 3:
                printf("Voce fugiu!\n");
                s.venceu = 0;
                return s;

            default:
                printf("Opcao invalida!\n");
        }

        if (inimigo.hp > 0) {
            int dano = inimigo.ataque + rand() % 5;
            player.hp -= dano;
            printf("Inimigo causou %d de dano!\n", dano);
        }

        printf("\nPressione ENTER para continuar...");
        getchar(); getchar();
    }

    limparTela();

    if (player.hp > 0) {
        printf("Vitoria!\n");
        s.venceu = 1;
    } else {
        printf("Derrota...\n");
        s.venceu = 0;
    }

    printf("\nPressione ENTER para voltar ao menu...");
    getchar(); getchar();

    return s;
}

// ================= JOGO =================
void jogar() {
    Sessao s = batalha();
    salvarHistorico(s);
}

// ================= MAIN =================
int main() {
    setlocale(LC_ALL, "");

    srand(time(NULL));

    int opcao;

    do {
        limparTela();

        printf("=== DARK TERMINAL ===\n");
        printf("1. Jogar\n");
        printf("2. Analisar historico\n");
        printf("3. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar();
                break;

            case 2:
                analisarHistorico();
                printf("\nPressione ENTER para voltar...");
                getchar(); getchar();
                break;

            case 3:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                getchar(); getchar();
        }

    } while (opcao != 3);

    return 0;
}