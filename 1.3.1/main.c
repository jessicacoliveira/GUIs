#include <SDL2/SDL.h>

#define RECT_SIZE 50
#define LARGURA_TELA 300
#define ALTURA_TELA 300

int main (int argc, char* args[])
{

    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 0,0, RECT_SIZE,RECT_SIZE };
    int aux = 0;
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);
        SDL_Delay(200);
       
        if(aux == 0){
            r.x += 2;
            if (r.x >= LARGURA_TELA - RECT_SIZE) aux=1;
        }
        else if(aux == 1){
            r.y += 2;
            if (r.y >= ALTURA_TELA - RECT_SIZE) aux=2;
        }
        else if(aux == 2){
            r.x += -2;
            if (r.x <= 0) aux=3;
        }
        else if(aux == 3){
            r.y += -2;
            if (r.y <= 0) aux=0;
        }        
       
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
