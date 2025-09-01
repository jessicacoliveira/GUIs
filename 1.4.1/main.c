#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define RECT_MAX 10

void rect_generator (int x, int y, SDL_Rect* rect, SDL_Color* color, int rect_w, int rect_h);
void fit_rect_in_screen (SDL_Rect* rect);

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.4.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    int rect_count = 0, running = 1;
    SDL_Rect rects[RECT_MAX];
    SDL_Color colors[RECT_MAX];
    srand(time(NULL));

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 100,100 };
    SDL_Event evt;
    
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);

        for (int i = 0; i < rect_count && i < RECT_MAX; i++) {
            SDL_SetRenderDrawColor(ren, colors[i].r, colors[i].g, colors[i].b, 255);
            SDL_RenderFillRect(ren, &rects[i]);
        }

    SDL_RenderPresent(ren);

    SDL_WaitEvent(&evt);
    switch(evt.type) {
        case SDL_QUIT:
            running = 0;
            break;
        case SDL_KEYDOWN:
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    r.y -= 5; break;
                case SDLK_DOWN:
                    r.y += 5; break;
                case SDLK_LEFT:
                    r.x -= 5; break;
                case SDLK_RIGHT:
                    r.x += 5; break;
            }
          fit_rect_in_screen(&r);
          break;
    case SDL_MOUSEBUTTONDOWN:
        if (rect_count >= RECT_MAX) break;
        rect_generator(evt.button.x, evt.button.y, &rects[rect_count], &colors[rect_count], (rand()%100),(rand()%100));
	rect_count++;
        break;
    }
  }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void rect_generator(int x, int y, SDL_Rect* rect, SDL_Color* color, int rect_w, int rect_h){
    if (rect_w <= 0) rect_w = 50;
    if (rect_h <= 0) rect_h = 50;
    
    rect->x = x;
    rect->y = y;
    rect->w = rect_w;
    rect->h = rect_h;

    if (rect->x + rect->w > LARGURA_TELA)
        rect->x = LARGURA_TELA - rect->w;
    if (rect->x < 0)
        rect->x = 0;

    if (rect->y + rect->h > ALTURA_TELA)
        rect->y = ALTURA_TELA - rect->h;
    if (rect->y < 0)
        rect->y = 0;

    color->r = rand() % 150;
    color->g = rand() % 150;
    color->b = rand() % 150;
    color->a = 255;
}

void fit_rect_in_screen (SDL_Rect* rect) {
    if (rect->x + rect->w > LARGURA_TELA)
        rect->x = LARGURA_TELA - rect->w;
    if (rect->x < 0)
        rect->x = 0;

    if (rect->y + rect->h > ALTURA_TELA)
        rect->y = ALTURA_TELA - rect->h;
    if (rect->y < 0)
        rect->y = 0;
}
