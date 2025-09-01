#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define RECT_NUM 3
#define RECT_SIZE 50
#define LARGURA_TELA 300
#define ALTURA_TELA 300

void rect_generator(int x, int y, SDL_Rect* rect, SDL_Color* color, int rect_w, int rect_h);

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.3.1: Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    srand(time(NULL));
    SDL_Rect rects[RECT_NUM];
    SDL_Color colors[RECT_NUM];

    /* EXECUÇÃO */
    rect_generator(0, 0, &rects[0], &colors[0], RECT_SIZE, RECT_SIZE);
    rect_generator(RECT_SIZE, 0, &rects[1], &colors[1], RECT_SIZE, RECT_SIZE);
    rect_generator(2*RECT_SIZE, 0, &rects[2], &colors[2], RECT_SIZE, RECT_SIZE);
    
    int aux[3] = {0,0,0};
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

        for (int i = 0; i < RECT_NUM; i++) {
            SDL_SetRenderDrawColor(ren, colors[i].r, colors[i].g, colors[i].b, 255);
            SDL_RenderFillRect(ren, &rects[i]);
        }
        SDL_RenderPresent(ren);
        SDL_Delay(200);  
        
        for(int i=0;i<RECT_NUM;i++){
            if(aux[i] == 0){
                rects[i].x += 2;
                if (rects[i].x >= LARGURA_TELA - RECT_SIZE) aux[i]=1;
            }
            else if(aux[i] == 1){
                rects[i].y += 2;
                if (rects[i].y >= ALTURA_TELA - RECT_SIZE) aux[i]=2;
            }
            else if(aux[i] == 2){
                rects[i].x -= 2;
                if (rects[i].x <= 0) aux[i]=3;
            }
            else if(aux[i] == 3){
                rects[i].y -= 2;
                if (rects[i].y <= 0) aux[i]=0;
            }
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
