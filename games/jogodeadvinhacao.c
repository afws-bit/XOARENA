// =============================================================================
// GUESSING GAME - SPANE Engine SDK (Fixed - Game starts immediately)
// =============================================================================
// Compile: gcc -shared -fPIC -O3 -o jogodeadvinhacao.so jogodeadvinhacao.c
// =============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// SDK Constants
#define MAIN_WINDOW_WIDTH  1000
#define MAIN_WINDOW_HEIGHT 700
#define GAME_AREA_WIDTH    800
#define GAME_AREA_HEIGHT   600
#define GAME_AREA_X        180
#define GAME_AREA_Y        50

// Game Constants
#define MIN_GUESS 1
#define MAX_GUESS 100
#define MAX_GUESSES 20
#define INPUT_BUFFER 32

// Framebuffer struct
typedef struct Framebuffer {
    unsigned char pixels[MAIN_WINDOW_WIDTH * MAIN_WINDOW_HEIGHT * 4];
} Framebuffer;

// Game struct
typedef struct Game {
    char name[64];
    char path[512];
    void* data;
    void* handle;
    int active;
    void (*init)(struct Game* game);
    void (*handle_key)(struct Game* game, int key_code, int pressed);
    void (*handle_click)(struct Game* game, int x, int y);
    void (*update)(struct Game* game);
    void (*render)(struct Game* game, Framebuffer* fb);
    void (*cleanup)(struct Game* game);
} Game;

// Bitmap font 5x7
static const unsigned char font_5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00},{0x00,0x00,0x5f,0x00,0x00},{0x00,0x07,0x00,0x07,0x00},
    {0x14,0x7f,0x14,0x7f,0x14},{0x24,0x2a,0x7f,0x2a,0x12},{0x23,0x13,0x08,0x64,0x62},
    {0x36,0x49,0x55,0x22,0x50},{0x00,0x05,0x03,0x00,0x00},{0x00,0x1c,0x22,0x41,0x00},
    {0x00,0x41,0x22,0x1c,0x00},{0x08,0x2a,0x1c,0x2a,0x08},{0x08,0x08,0x3e,0x08,0x08},
    {0x00,0x50,0x30,0x00,0x00},{0x08,0x08,0x08,0x08,0x08},{0x00,0x60,0x60,0x00,0x00},
    {0x20,0x10,0x08,0x04,0x02},{0x3e,0x51,0x49,0x45,0x3e},{0x00,0x42,0x7f,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46},{0x21,0x41,0x45,0x4b,0x31},{0x18,0x14,0x12,0x7f,0x10},
    {0x27,0x45,0x45,0x45,0x39},{0x3c,0x4a,0x49,0x49,0x30},{0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36},{0x06,0x49,0x49,0x29,0x1e},{0x00,0x36,0x36,0x00,0x00},
    {0x00,0x56,0x36,0x00,0x00},{0x00,0x08,0x14,0x22,0x41},{0x14,0x14,0x14,0x14,0x14},
    {0x41,0x22,0x14,0x08,0x00},{0x02,0x01,0x51,0x09,0x06},{0x32,0x49,0x79,0x41,0x3e},
    {0x7e,0x11,0x11,0x11,0x7e},{0x7f,0x49,0x49,0x49,0x36},{0x3e,0x41,0x41,0x41,0x22},
    {0x7f,0x41,0x41,0x22,0x1c},{0x7f,0x49,0x49,0x49,0x41},{0x7f,0x09,0x09,0x01,0x01},
    {0x3e,0x41,0x41,0x51,0x32},{0x7f,0x08,0x08,0x08,0x7f},{0x00,0x41,0x7f,0x41,0x00},
    {0x20,0x40,0x41,0x3f,0x01},{0x7f,0x08,0x14,0x22,0x41},{0x7f,0x40,0x40,0x40,0x40},
    {0x7f,0x02,0x04,0x02,0x7f},{0x7f,0x04,0x08,0x10,0x7f},{0x3e,0x41,0x41,0x41,0x3e},
    {0x7f,0x09,0x09,0x09,0x06},{0x3e,0x41,0x51,0x21,0x5e},{0x7f,0x09,0x19,0x29,0x46},
    {0x46,0x49,0x49,0x49,0x31},{0x01,0x01,0x7f,0x01,0x01},{0x3f,0x40,0x40,0x40,0x3f},
    {0x1f,0x20,0x40,0x20,0x1f},{0x7f,0x20,0x18,0x20,0x7f},{0x63,0x14,0x08,0x14,0x63},
    {0x03,0x04,0x78,0x04,0x03},{0x61,0x51,0x49,0x45,0x43},{0x00,0x00,0x7f,0x41,0x41},
    {0x02,0x04,0x08,0x10,0x20},{0x41,0x41,0x7f,0x00,0x00},{0x04,0x02,0x01,0x02,0x04},
    {0x40,0x40,0x40,0x40,0x40},{0x00,0x01,0x02,0x04,0x00},{0x20,0x54,0x54,0x54,0x78},
    {0x7f,0x48,0x44,0x44,0x38},{0x38,0x44,0x44,0x44,0x20},{0x38,0x44,0x44,0x48,0x7f},
    {0x38,0x54,0x54,0x54,0x18},{0x08,0x7e,0x09,0x01,0x02},{0x08,0x14,0x54,0x54,0x3c},
    {0x7f,0x08,0x04,0x04,0x78},{0x00,0x44,0x7d,0x40,0x00},{0x20,0x40,0x44,0x3d,0x00},
    {0x00,0x7f,0x10,0x28,0x44},{0x00,0x41,0x7f,0x40,0x00},{0x7c,0x04,0x18,0x04,0x78},
    {0x7c,0x08,0x04,0x04,0x78},{0x38,0x44,0x44,0x44,0x38},{0x7c,0x14,0x14,0x14,0x08},
    {0x08,0x14,0x14,0x18,0x7c},{0x7c,0x08,0x04,0x04,0x08},{0x48,0x54,0x54,0x54,0x20},
    {0x04,0x3f,0x44,0x40,0x20},{0x3c,0x40,0x40,0x20,0x7c},{0x1c,0x20,0x40,0x20,0x1c},
    {0x3c,0x40,0x30,0x40,0x3c},{0x44,0x28,0x10,0x28,0x44},{0x0c,0x50,0x50,0x50,0x3c},
    {0x44,0x64,0x54,0x4c,0x44},{0x00,0x08,0x36,0x41,0x00},{0x00,0x00,0x7f,0x00,0x00},
    {0x00,0x41,0x36,0x08,0x00},{0x08,0x08,0x2a,0x1c,0x08}
};

// Screen states
#define SCREEN_GAME   0
#define SCREEN_SCORES 1
#define MAX_SCORES    100

// Game data structure
typedef struct {
    int screen;
    int target;
    int guesses[MAX_GUESSES];
    int num_guesses;
    int game_over;
    char current_input[INPUT_BUFFER];
    int input_len;
    int cursor_visible;
    int frame_count;
    char message[256];
    int game_active;
    int new_game_key;
    int scores_key;
    int quit_stats_key;
} GuessingData;

// History sessions
typedef struct {
    time_t timestamp;
    int target;
    int num_guesses;
    int guesses[MAX_GUESSES];
} Session;

static Session sessions[MAX_SCORES];
static int num_sessions = 0;

// =============================================================================
// Drawing primitives
// =============================================================================

static inline void fb_pixel(Framebuffer* fb, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= MAIN_WINDOW_WIDTH || y < 0 || y >= MAIN_WINDOW_HEIGHT) return;
    int i = (y * MAIN_WINDOW_WIDTH + x) * 4;
    fb->pixels[i] = r; fb->pixels[i+1] = g; fb->pixels[i+2] = b; fb->pixels[i+3] = 255;
}

static void fb_fill(Framebuffer* fb, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b) {
    for (int dy = 0; dy < h; dy++)
        for (int dx = 0; dx < w; dx++)
            fb_pixel(fb, x+dx, y+dy, r, g, b);
}

static void fb_rect(Framebuffer* fb, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b) {
    for (int dx = 0; dx < w; dx++) { 
        fb_pixel(fb, x+dx, y, r, g, b); 
        fb_pixel(fb, x+dx, y+h-1, r, g, b); 
    }
    for (int dy = 1; dy < h-1; dy++) { 
        fb_pixel(fb, x, y+dy, r, g, b); 
        fb_pixel(fb, x+w-1, y+dy, r, g, b); 
    }
}

static void fb_char(Framebuffer* fb, int x, int y, char c, unsigned char r, unsigned char g, unsigned char b) {
    if (c < 32 || c > 126) return;
    const unsigned char* glyph = font_5x7[c-32];
    for (int row = 0; row < 7; row++)
        for (int col = 0; col < 5; col++)
            if (glyph[col] & (1<<row)) 
                fb_pixel(fb, x+col, y+row, r, g, b);
}

static void fb_text(Framebuffer* fb, int x, int y, const char* s, unsigned char r, unsigned char g, unsigned char b) {
    if (!s) return;
    for (int i = 0; s[i]; i++) 
        fb_char(fb, x+i*6, y, s[i], r, g, b);
}

// =============================================================================
// Custom square root using Newton's method
// =============================================================================
static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double guess = x / 2.0;
    for (int i = 0; i < 20; i++) {
        double new_guess = (guess + x / guess) / 2.0;
        if (new_guess == guess) break;
        guess = new_guess;
    }
    return guess;
}

// =============================================================================
// RF03 - Save session to file
// =============================================================================
static void save_session(GuessingData* d) {
    FILE* f = fopen("guessing_history.txt", "a");
    if (!f) {
        printf("ERROR: Cannot open history file for writing!\n");
        return;
    }
    
    time_t now = time(NULL);
    fprintf(f, "%ld %d %d", (long)now, d->target, d->num_guesses);
    for (int i = 0; i < d->num_guesses; i++) {
        fprintf(f, " %d", d->guesses[i]);
    }
    fprintf(f, "\n");
    fclose(f);
    printf("Session saved: target=%d, guesses=%d\n", d->target, d->num_guesses);
}

// =============================================================================
// RF04 - Load history from file
// =============================================================================
static void load_history() {
    FILE* f = fopen("guessing_history.txt", "r");
    if (!f) {
        printf("No history file found\n");
        num_sessions = 0;
        return;
    }
    
    num_sessions = 0;
    char line[1024];
    
    while (fgets(line, sizeof(line), f) && num_sessions < MAX_SCORES) {
        Session* s = &sessions[num_sessions];
        char* token = strtok(line, " \n");
        if (!token) continue;
        s->timestamp = (time_t)atol(token);
        
        token = strtok(NULL, " \n");
        if (!token) continue;
        s->target = atoi(token);
        
        token = strtok(NULL, " \n");
        if (!token) continue;
        s->num_guesses = atoi(token);
        
        int i = 0;
        while ((token = strtok(NULL, " \n")) != NULL && i < MAX_GUESSES) {
            s->guesses[i++] = atoi(token);
        }
        
        if (s->num_guesses > 0 && i > 0) {
            num_sessions++;
        }
    }
    fclose(f);
    printf("Loaded %d sessions from history\n", num_sessions);
}

// =============================================================================
// RF06 - Recursive functions for statistics
// =============================================================================
static int recursive_sum(int* arr, int n) {
    if (n <= 0) return 0;
    return arr[0] + recursive_sum(arr + 1, n - 1);
}

static int recursive_min(int* arr, int n) {
    if (n <= 0) return 999999;
    if (n == 1) return arr[0];
    int min_rest = recursive_min(arr + 1, n - 1);
    return arr[0] < min_rest ? arr[0] : min_rest;
}

static int recursive_max(int* arr, int n) {
    if (n <= 0) return -1;
    if (n == 1) return arr[0];
    int max_rest = recursive_max(arr + 1, n - 1);
    return arr[0] > max_rest ? arr[0] : max_rest;
}

static double recursive_sum_sq_diff(int* arr, int n, double mean) {
    if (n <= 0) return 0.0;
    double diff = arr[0] - mean;
    return diff * diff + recursive_sum_sq_diff(arr + 1, n - 1, mean);
}

// =============================================================================
// RF05 - Calculate statistics
// =============================================================================
static void calculate_statistics(GuessingData* d) {
    if (num_sessions == 0) {
        snprintf(d->message, sizeof(d->message), "Nenhum historico de jogo encontrado!");
        return;
    }
    
    int guess_counts[MAX_SCORES];
    for (int i = 0; i < num_sessions; i++) {
        guess_counts[i] = sessions[i].num_guesses;
    }
    
    int total_sessions = num_sessions;
    int sum_guesses = recursive_sum(guess_counts, num_sessions);
    double mean = (double)sum_guesses / total_sessions;
    int best = recursive_min(guess_counts, num_sessions);
    int worst = recursive_max(guess_counts, num_sessions);
    double variance = recursive_sum_sq_diff(guess_counts, num_sessions, mean) / total_sessions;
    double std_dev = my_sqrt(variance);
    
    snprintf(d->message, sizeof(d->message), 
             "Stats: %d sessoes | Media:%.1f | Melhor:%d | Pior:%d | DP:%.2f", 
             total_sessions, mean, best, worst, std_dev);
}

// =============================================================================
// RF07 - Strategy suggestions
// =============================================================================
static const char* get_strategy_message() {
    if (num_sessions == 0) return "Sem dados - tente busca binaria!";
    
    int total_guesses = 0;
    for (int i = 0; i < num_sessions; i++) {
        total_guesses += sessions[i].num_guesses;
    }
    
    double avg = (double)total_guesses / num_sessions;
    
    if (avg < 6) return "Especialista! Busca binaria perfeita!";
    if (avg < 10) return "Boa estrategia! Quase otimo.";
    return "Tente busca binaria: comece em 50, depois 25 ou 75...";
}

// =============================================================================
// Game Logic
// =============================================================================

static void new_game(GuessingData* d) {
    d->game_active = 1;
    d->target = (rand() % MAX_GUESS) + 1;
    d->num_guesses = 0;
    d->game_over = 0;
    d->input_len = 0;
    d->current_input[0] = 0;
    snprintf(d->message, sizeof(d->message), 
             "Pensei em um numero entre 1 e %d. Digite seu palpite!", MAX_GUESS);
    printf("New game started! Target: %d\n", d->target);
}

static void submit_guess(GuessingData* d) {
    if (d->game_over || d->input_len == 0) return;
    
    int guess = atoi(d->current_input);
    
    if (guess < MIN_GUESS || guess > MAX_GUESS) {
        snprintf(d->message, sizeof(d->message), 
                 "Por favor, digite um numero entre %d e %d!", MIN_GUESS, MAX_GUESS);
        d->input_len = 0;
        d->current_input[0] = 0;
        return;
    }
    
    if (d->num_guesses >= MAX_GUESSES) {
        snprintf(d->message, sizeof(d->message), 
                 "Fim de jogo! Muitas tentativas. O numero era %d.", d->target);
        d->game_over = 1;
        d->game_active = 0;
        return;
    }
    
    d->guesses[d->num_guesses++] = guess;
    
    if (guess < d->target) {
        snprintf(d->message, sizeof(d->message), "MUITO BAIXO! Tente um numero maior.");
    } else if (guess > d->target) {
        snprintf(d->message, sizeof(d->message), "MUITO ALTO! Tente um numero menor.");
    } else {
        snprintf(d->message, sizeof(d->message), 
                 "ACERTOU! Voce encontrou %d em %d tentativas! N=Novo Jogo S=Estatisticas", 
                 d->target, d->num_guesses);
        d->game_over = 1;
        d->game_active = 0;
        save_session(d);
    }
    
    d->input_len = 0;
    d->current_input[0] = 0;
}

// =============================================================================
// Game State Update
// =============================================================================
static void guessing_update(Game* game) {
    if (!game || !game->data) return;
    GuessingData* d = (GuessingData*)game->data;
    
    d->frame_count++;
    
    // Process pending actions
    if (d->new_game_key) {
        d->new_game_key = 0;
        new_game(d);
    }
    
    if (d->scores_key) {
        d->scores_key = 0;
        load_history();
        calculate_statistics(d);
        d->screen = SCREEN_SCORES;
    }
    
    if (d->quit_stats_key) {
        d->quit_stats_key = 0;
        snprintf(d->message, sizeof(d->message), 
                 "Voltou ao jogo! %s", 
                 d->game_over ? "Pressione N para novo jogo" : "Digite seu palpite");
        d->screen = SCREEN_GAME;
    }
    
    // Cursor blink
    if (d->frame_count % 30 == 0) {
        d->cursor_visible = !d->cursor_visible;
    }
}

// =============================================================================
// Input Handlers - CORRIGIDO
// =============================================================================
static void guessing_handle_key(Game* game, int key_code, int pressed) {
    if (!game || !game->data || !pressed) return;
    GuessingData* d = (GuessingData*)game->data;
    
    printf("Key pressed: %d (active=%d, game_over=%d, screen=%d)\n", 
           key_code, d->game_active, d->game_over, d->screen);
    
    // Global controls (work on all screens)
    if (key_code == 78 || key_code == 110) { // N
        printf("N key - starting new game\n");
        d->new_game_key = 1;
        d->screen = SCREEN_GAME;
        return;
    }
    if (key_code == 83 || key_code == 115) { // S
        printf("S key - showing stats\n");
        d->scores_key = 1;
        return;
    }
    
    // Screen-specific controls
    if (d->screen == SCREEN_SCORES) {
        if (key_code == 81 || key_code == 113) { // Q
            printf("Q key - returning to game\n");
            d->quit_stats_key = 1;
        }
        return;
    }
    
    // Game screen controls
    // IMPORTANT: Even if game_over, allow Enter to start new game
    if (d->game_over) {
        if (key_code == 13) { // Enter
            printf("Enter key - starting new game\n");
            d->new_game_key = 1;
        }
        return;
    }
    
    // Active game input - THIS IS THE KEY PART
    if (d->game_active && !d->game_over) {
        if (key_code == 13) { // Enter - submit guess
            printf("Enter - submitting guess: %s\n", d->current_input);
            submit_guess(d);
        } else if (key_code == 8 || key_code == 127) { // Backspace
            if (d->input_len > 0) {
                d->current_input[--d->input_len] = 0;
                printf("Backspace - input now: %s\n", d->current_input);
            }
        } else if (key_code >= 48 && key_code <= 57) { // Digits 0-9
            if (d->input_len < INPUT_BUFFER - 1) {
                d->current_input[d->input_len++] = (char)key_code;
                d->current_input[d->input_len] = 0;
                printf("Digit %c - input now: %s\n", (char)key_code, d->current_input);
            }
        }
    }
}

static void guessing_handle_click(Game* game, int x, int y) {
    if (!game || !game->data) return;
    GuessingData* d = (GuessingData*)game->data;
    
    printf("Click at %d,%d (screen=%d, game_over=%d)\n", x, y, d->screen, d->game_over);
    
    if (d->screen == SCREEN_SCORES) {
        d->quit_stats_key = 1;
    } else if (d->game_over) {
        d->new_game_key = 1;
    }
}

// =============================================================================
// Rendering Functions
// =============================================================================
static void render_game_screen(Game* game, Framebuffer* fb) {
    GuessingData* d = (GuessingData*)game->data;
    int bx = GAME_AREA_X + 30;
    int by = GAME_AREA_Y + 30;
    
    // Title
    fb_text(fb, GAME_AREA_X + 30, GAME_AREA_Y + 10, 
            "JOGO DE ADIVINHACAO", 0xFF, 0xCC, 0x00);
    
    // Instructions
    fb_text(fb, GAME_AREA_X + 300, GAME_AREA_Y + 10, 
            "N:Novo Jogo  S:Estatisticas  Enter:Enviar Palpite", 0x88, 0x88, 0x88);
    
    // Strategy hint
    char strat[128];
    snprintf(strat, sizeof(strat), "Dica: %s", get_strategy_message());
    fb_text(fb, GAME_AREA_X + 30, GAME_AREA_Y + 30, strat, 0x00, 0xCC, 0xCC);
    
    // Input box
    fb_fill(fb, bx, by + 50, 400, 45, 0x22, 0x22, 0x33);
    fb_rect(fb, bx, by + 50, 400, 45, 
            d->game_over ? 0x44 : 0x00, 
            d->game_over ? 0x44 : 0x88, 
            d->game_over ? 0x44 : 0xFF);
    
    // Show input with prompt
    char input_display[64];
    if (d->game_active && !d->game_over) {
        snprintf(input_display, sizeof(input_display), "Palpite: %s", d->current_input);
        fb_text(fb, bx + 15, by + 65, input_display, 0xFF, 0xFF, 0xFF);
        
        // Blinking cursor
        if (d->cursor_visible) {
            int cx = bx + 15 + (10 + strlen(d->current_input)) * 6;
            fb_fill(fb, cx, by + 70, 8, 2, 0xFF, 0xFF, 0xFF);
        }
    } else {
        fb_text(fb, bx + 15, by + 65, "Pressione ENTER para novo jogo", 0x88, 0x88, 0x88);
    }
    
    // Message area
    unsigned char mr = 0xFF, mg = 0xFF, mb = 0x00;
    if (strstr(d->message, "BAIXO") || strstr(d->message, "baixo")) {
        mr = 0xFF; mg = 0x66; mb = 0x66;
    } else if (strstr(d->message, "ALTO") || strstr(d->message, "alto")) {
        mr = 0xFF; mg = 0x66; mb = 0x66;
    } else if (strstr(d->message, "ACERTOU") || strstr(d->message, "encontrou")) {
        mr = 0x00; mg = 0xFF; mb = 0x00;
    }
    fb_text(fb, bx, by + 110, d->message, mr, mg, mb);
    
    // Guess history panel
    int hx = bx + 450;
    fb_fill(fb, hx, by + 50, 200, 350, 0x15, 0x15, 0x20);
    fb_rect(fb, hx, by + 50, 200, 350, 0x33, 0x33, 0x44);
    fb_text(fb, hx + 10, by + 60, "Historico de Palpites:", 0xAA, 0xAA, 0xAA);
    
    for (int i = 0; i < d->num_guesses; i++) {
        char gs[32];
        snprintf(gs, sizeof(gs), "#%d: %d %s", i+1, d->guesses[i],
                 d->guesses[i] < d->target ? "(baixo)" : 
                 d->guesses[i] > d->target ? "(alto)" : "(CERTO!)");
        
        unsigned char r = 0xFF, g = 0xCC, b = 0xCC;
        if (d->guesses[i] < d->target) {
            r = 0xFF; g = 0x88; b = 0x88;
        } else if (d->guesses[i] > d->target) {
            r = 0xFF; g = 0x88; b = 0x88;
        } else {
            r = 0x00; g = 0xFF; b = 0x00;
        }
        fb_text(fb, hx + 10, by + 85 + i*20, gs, r, g, b);
    }
    
    // Game over message
    if (d->game_over && d->num_guesses >= 0) {
        fb_fill(fb, GAME_AREA_X + 10, GAME_AREA_Y + 500, 
                GAME_AREA_WIDTH - 20, 60, 0x11, 0x33, 0x11);
        fb_rect(fb, GAME_AREA_X + 10, GAME_AREA_Y + 500, 
                GAME_AREA_WIDTH - 20, 60, 0x00, 0xFF, 0x00);
        fb_text(fb, GAME_AREA_X + 40, GAME_AREA_Y + 525, 
                "PRESSIONE ENTER OU CLIQUE PARA NOVO JOGO", 0x00, 0xFF, 0x00);
    }
}

static void render_scores_screen(Game* game, Framebuffer* fb) {
    GuessingData* d = (GuessingData*)game->data;
    int sx = GAME_AREA_X + 30;
    int sy = GAME_AREA_Y + 20;
    
    fb_text(fb, sx, sy, "ESTATISTICAS DO JOGO", 0xFF, 0xCC, 0x00);
    fb_text(fb, sx + 250, sy, "Q:Voltar ao Jogo", 0x88, 0x88, 0x88);
    
    // Statistics summary
    fb_text(fb, sx, sy + 30, d->message, 0x00, 0xFF, 0xFF);
    
    // Strategy on stats page
    char strat[128];
    snprintf(strat, sizeof(strat), "Estrategia: %s", get_strategy_message());
    fb_text(fb, sx, sy + 50, strat, 0x00, 0xCC, 0xCC);
    
    // Table header
    char header[128];
    snprintf(header, sizeof(header), "%-20s %-10s %-10s %-20s", 
             "Data/Hora", "Alvo", "Tentativas", "Sequencia de Palpites");
    fb_text(fb, sx, sy + 80, header, 0xCC, 0xCC, 0xCC);
    
    // Show last 20 sessions
    int start = num_sessions > 20 ? num_sessions - 20 : 0;
    for (int i = start; i < num_sessions; i++) {
        char line[512];
        char timestr[32];
        struct tm* tm_info = localtime(&sessions[i].timestamp);
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);
        
        // Build guesses sequence
        char seq_str[256] = "";
        for (int j = 0; j < sessions[i].num_guesses; j++) {
            char num[8];
            snprintf(num, sizeof(num), "%d%s", sessions[i].guesses[j],
                     j < sessions[i].num_guesses - 1 ? ", " : "");
            strcat(seq_str, num);
        }
        
        snprintf(line, sizeof(line), "%-20s %-10d %-10d %s", 
                 timestr, sessions[i].target, 
                 sessions[i].num_guesses, seq_str);
        fb_text(fb, sx, sy + 100 + (i-start)*18, line, 0xAA, 0xAA, 0xAA);
    }
    
    fb_text(fb, sx, sy + 480, 
            "Clique ou pressione Q para voltar ao jogo", 0x88, 0x88, 0x88);
}

static void guessing_render(Game* game, Framebuffer* fb) {
    if (!game || !game->data || !fb) return;
    GuessingData* d = (GuessingData*)game->data;
    
    // Clear game area
    fb_fill(fb, GAME_AREA_X, GAME_AREA_Y, 
            GAME_AREA_WIDTH, GAME_AREA_HEIGHT, 0x11, 0x11, 0x1A);
    fb_rect(fb, GAME_AREA_X, GAME_AREA_Y, 
            GAME_AREA_WIDTH, GAME_AREA_HEIGHT, 0x33, 0x55, 0x77);
    
    if (d->screen == SCREEN_GAME) {
        render_game_screen(game, fb);
    } else {
        render_scores_screen(game, fb);
    }
}

// =============================================================================
// Initialization and Cleanup - CORRIGIDO
// =============================================================================
static void guessing_init(Game* game) {
    if (!game) return;
    
    GuessingData* d = (GuessingData*)calloc(1, sizeof(GuessingData));
    if (!d) {
        printf("ERROR: Failed to allocate game data!\n");
        return;
    }
    
    srand(time(NULL));
    d->screen = SCREEN_GAME;
    
    // CORREÇÃO PRINCIPAL: Iniciar jogo ativo imediatamente
    d->target = (rand() % MAX_GUESS) + 1;
    d->num_guesses = 0;
    d->game_over = 0;
    d->game_active = 1;
    d->input_len = 0;
    d->current_input[0] = 0;
    
    d->frame_count = 0;
    d->cursor_visible = 1;
    d->new_game_key = 0;
    d->scores_key = 0;
    d->quit_stats_key = 0;
    
    snprintf(d->message, sizeof(d->message), 
             "Pensei em um numero de 1 a %d. Digite seu palpite e pressione ENTER!", 
             MAX_GUESS);
    
    game->data = d;
    game->active = 1;
    
    printf("Jogo de Adivinhacao inicializado - Target: %d, game_active: %d, game_over: %d\n", 
           d->target, d->game_active, d->game_over);
}

static void guessing_cleanup(Game* game) {
    if (game && game->data) {
        free(game->data);
        game->data = NULL;
        printf("Jogo de Adivinhacao finalizado\n");
    }
}

// =============================================================================
// EXPORTED ENTRY POINT
// =============================================================================
__attribute__((visibility("default"))) Game* create_game() {
    printf("Criando nova instancia do Jogo de Adivinhacao\n");
    
    Game* game = (Game*)calloc(1, sizeof(Game));
    if (!game) {
        printf("ERROR: Failed to allocate Game struct!\n");
        return NULL;
    }
    
    // Setup function pointers
    game->init = guessing_init;
    game->handle_key = guessing_handle_key;
    game->handle_click = guessing_handle_click;
    game->update = guessing_update;
    game->render = guessing_render;
    game->cleanup = guessing_cleanup;
    
    // Initialize name
    strncpy(game->name, "Jogo de Adivinhacao", sizeof(game->name) - 1);
    game->name[sizeof(game->name) - 1] = '\0';
    
    // Don't set active here - init will do it
    game->active = 0;
    game->data = NULL;
    
    printf("create_game: Jogo criado com sucesso em %p\n", (void*)game);
    
    return game;
}