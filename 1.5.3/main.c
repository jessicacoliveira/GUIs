#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define RECT_SIZE 50
#define LARGURA_TELA 200
#define ALTURA_TELA 200
#define TIMEOUT 200

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
void fit_rect_in_screen (SDL_Rect* rect);

int main (int argc, char* args[])
{

    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.5.3",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    srand(time(NULL));

    /* EXECUÇÃO */
    int aux = 0, running = 1, aux_sentido = 1, aux_stop = 1, velocidade_pixels = 2;
    int red = 0, green = 0, blue = 255;
    SDL_Rect r = { 0,0, RECT_SIZE ,RECT_SIZE };
    Uint32 espera = TIMEOUT;
    while (running) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, red, green, blue, 255);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);
        
        SDL_Event evt;
        
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            switch (evt.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (evt.key.keysym.sym) {
                        case SDLK_a: //sentido anti-horário
                            aux_sentido = -1;
                            break;
                            
                        case SDLK_z: //sentido horário
                            aux_sentido = 1;
                            break;
                            
                        case SDLK_s: //aumenta o retângulo
                            if (r.w < LARGURA_TELA && r.h < ALTURA_TELA) {
                                r.w++;
                                r.h++;
                            }
                            break;
                            
                        case SDLK_x: //diminui o retângulo
                            if (r.w > 5 && r.h > 5) { //tamanho mínimo: 5 pixels
                                r.w--;
                                r.h--;
                            }
                            break;
                            
                        case SDLK_t: //muda a cor do retângulo
                            red = rand() % 255;
                            green = rand() % 255;
                            blue = rand() % 255;
                            break;
                        
                        case SDLK_SPACE: //para a animação
                            aux_stop = !aux_stop;
                            break;
                            
                        case SDLK_d: //aumenta velocidade
                            velocidade_pixels +=2;
                            break;
                            
                        case SDLK_c: //diminui velocidade
                            if (velocidade_pixels >1)
                            velocidade_pixels -=2;
                            break;
                    }
              }
        }
        else{     
            if(aux == 0 && aux_stop == 1){
               r.x += velocidade_pixels*aux_sentido;
                if (r.x >= LARGURA_TELA - r.w && aux_sentido == 1) aux=1;
                if (r.x <= 0 && aux_sentido == -1){ r.x = 0; aux = 3;}
            }
            else if(aux == 1 && aux_stop == 1){
                r.y += velocidade_pixels*aux_sentido;
                if (r.y >= ALTURA_TELA - r.h && aux_sentido == 1) aux=2;
                else if (r.y <= 0 && aux_sentido == -1){ r.y = 0; aux = 0;}
            }
            else if(aux == 2 && aux_stop == 1){
                r.x += -velocidade_pixels*aux_sentido;
                if (r.x <= 0 && aux_sentido == 1) aux=3;
                else if (r.x>=LARGURA_TELA - r.w && aux_sentido == -1) { r.x = LARGURA_TELA - r.w; aux = 1; }
            }
            else if(aux == 3 && aux_stop == 1){
               r.y += -velocidade_pixels*aux_sentido;
               if (r.y <= 0 && aux_sentido == 1) aux=0;
               if (r.y >= ALTURA_TELA - r.h && aux_sentido == -1) { r.y = ALTURA_TELA - r.h; aux = 2; }
            }               
        }
        fit_rect_in_screen (&r);
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
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
