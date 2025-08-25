#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Programa 1",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 227,66,245,0x00);
    SDL_Rect r1 = { 150,50, 100,100 }; //posicao tam
    SDL_RenderFillRect(ren, &r1);
    SDL_SetRenderDrawColor(ren, 66,197,245,0x00);
    SDL_Rect r2 = { 150,150, 100,100 };
    SDL_RenderFillRect(ren, &r2);
    SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
    SDL_Rect r3 = { 150,250, 100,100 };
    SDL_RenderFillRect(ren, &r3);
    SDL_SetRenderDrawColor(ren, 173,245,66,0x00);
    SDL_Rect r4 = { 250,150, 100,100 };
    SDL_RenderFillRect(ren, &r4);
    SDL_SetRenderDrawColor(ren, 245,161,66,0x00);
    SDL_Rect r5 = { 50,150, 50,50 };
    SDL_RenderFillRect(ren, &r5);
    
    // Linhas pretas
    SDL_SetRenderDrawColor(ren, 0,0,0,0x00);
    SDL_RenderDrawLine(ren, 0, 0, 500, 500);
    SDL_RenderDrawLine(ren, 250, 250, 500, 0);
    SDL_RenderDrawLine(ren, 0, 500, 300, 300);
    SDL_RenderDrawLine(ren, 300, 500, 300, 300);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
