#include <SDL2/SDL.h>
#include <assert.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define RECT_MAX 50
#define TIMEOUT 30

void rect_generator2 (int x, int y, SDL_Rect* rect, int rect_w, int rect_h);
void fit_rect_in_screen (SDL_Rect* rect);
int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.6",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r1, r2, r3;
    rect_generator2 (40, 20, &r1, RECT_MAX, RECT_MAX);
    rect_generator2 (40, 100, &r2, RECT_MAX, RECT_MAX);
    rect_generator2 (40, 160, &r3, RECT_MAX, RECT_MAX);
    
    // Linha de chegada
    SDL_Rect r4;
    rect_generator2 (LARGURA_TELA - 100, 0, &r4, 100, ALTURA_TELA);
    
    
    int running = 1;
    int go1 = 1, go2 = 1, go3 = 1, winner = 0;
    
    Uint32 espera = TIMEOUT;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r4);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r1);
        SDL_RenderFillRect(ren, &r2);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            switch (evt.type){
                case SDL_KEYDOWN:
                    switch (evt.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (SDL_HasIntersection(&r2, &r4)){
                                go2 = 0;
                                if (winner == 0) winner = 2;
                            }
                            else if (go2) r2.x -= 5;
                            break;
                        case SDLK_RIGHT:
                            if (SDL_HasIntersection(&r2, &r4)){
                                go2 = 0;
                                if (winner == 0) winner = 2;
                            }
                            else if (go2) r2.x += 5;
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (go3) r3.x = evt.motion.x;
                    if (SDL_HasIntersection(&r3, &r4)){
                                go3 = 0;
                                if (winner == 0) winner = 3;
                            }
                    
                    break;
                
                case SDL_QUIT:
                    running = 0;
                    break;
            }
        } else {
            if (SDL_HasIntersection(&r1, &r4)){
                                go1 = 0;
                                if (winner == 0) winner = 1;
                            }
            if (go1 == 1)
            r1.x += 5;
        }
        fit_rect_in_screen(&r1);
        fit_rect_in_screen(&r2);
        fit_rect_in_screen(&r3);
    }
    
    printf("Vencedor: %d\n", winner);

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

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes, espera;
	antes = SDL_GetTicks();
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if (isevt){
		espera = (SDL_GetTicks() - antes);
		if (espera >= *ms) *ms = 0;
		else *ms -= espera;
	}
	else *ms = TIMEOUT;
	
	return isevt;
}
