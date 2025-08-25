#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Programa 2",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         600, 800, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 140, 98, 161,0x00);
    SDL_RenderClear(ren);

    boxRGBA(ren, 0, 0, 600, 100, 140, 98, 161, 255);
    boxRGBA(ren, 0, 100, 600, 200, 255, 255, 255, 255);
    boxRGBA(ren, 0, 200, 600, 300, 140, 98, 161, 255);
    boxRGBA(ren, 0, 300, 600, 400, 255, 255, 255, 255);
    boxRGBA(ren, 0, 400, 600, 500, 140, 98, 161, 255);
    boxRGBA(ren, 0, 500, 600, 600, 255, 255, 255, 255);
    boxRGBA(ren, 0, 600, 600, 700, 140, 98, 161, 255);
    boxRGBA(ren, 0, 700, 600, 800, 255, 255, 255, 255);

    SDL_SetRenderDrawColor(ren, 140, 98, 161, 255); 
    roundedBoxRGBA(ren, 100, 100, 500, 600, 10, 204, 196, 200, 255); //parte cinza
    roundedBoxRGBA(ren, 150, 150, 450, 400, 10, 93, 158, 73, 255); //tela verde
    thickLineColor(ren, 200, 450, 200, 520, 20, 0xff000000); //botao cruz
    thickLineColor(ren, 170, 485, 230, 485, 20, 0xff000000);
    filledCircleRGBA(ren, 420, 475, 15, 99, 11, 33, 255); //botao bolinha
    filledCircleRGBA(ren, 380, 515, 15, 99, 11, 33, 255);
    
    pieRGBA(ren, 300, 275, 100, 180, 360, 0, 0, 0, 255);
    pieRGBA(ren, 300, 275, 100, 0, 180, 0, 0, 0, 255);
    thickLineRGBA(ren, 300 - 100, 275, 300 + 100, 275, 20, 0, 0, 0, 255);
    filledCircleRGBA(ren, 300, 275, 30, 0, 0, 0, 255);

    SDL_RenderPresent(ren);
    SDL_Delay(10000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
