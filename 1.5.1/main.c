#include <SDL2/SDL.h>
#include <assert.h>

#define LARGURA_TELA 200
#define ALTURA_TELA 120
#define RECT_MAX 20
#define TIMEOUT 500

void rect_generator2 (int x, int y, SDL_Rect* rect, int rect_w, int rect_h); //v2: nao gera cores aleatorias
void fit_rect_in_screen (SDL_Rect* rect);

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.5.1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r1, r2, r3;
    rect_generator2 (40, 20, &r1, RECT_MAX, RECT_MAX);
    rect_generator2 (40, 50, &r2, RECT_MAX, RECT_MAX);
    rect_generator2 (40, 80, &r3, RECT_MAX, RECT_MAX);
    
    int running = 1, aux_direcao = 1, espera = TIMEOUT;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r1);
        SDL_RenderFillRect(ren, &r2);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);
        if (isevt) {
            espera -= (SDL_GetTicks() - antes);
            if (espera < 0) espera = 0;
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        r2.y -= 5;
                        break;
                    case SDLK_DOWN:
                        r2.y += 5;
                        break;
                    case SDLK_LEFT:
                        r2.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        r2.x += 5;
                        break;
                }
            }
            else if(evt.type == SDL_MOUSEMOTION){
                r3.x = evt.motion.x;
                r3.y = evt.motion.y;
            }
            else if(evt.type == SDL_QUIT){
                running = 0;
            }
        } else {
            espera = TIMEOUT;
            r1.x += 5*aux_direcao;
            fit_rect_in_screen(&r1);
            fit_rect_in_screen(&r2);
            fit_rect_in_screen(&r3);
            if (r1.x == LARGURA_TELA-RECT_MAX) aux_direcao = -1;
            else if (r1.x == 0) aux_direcao = 1;
          }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void rect_generator2(int x, int y, SDL_Rect* rect, int rect_w, int rect_h){
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
