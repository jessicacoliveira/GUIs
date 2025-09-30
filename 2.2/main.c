#include<SDL2/SDL.h>

#define LARGURA_TELA 600
#define ALTURA_TELA 600
#define RECT_SIZE 100
#define TIMEOUT 500

enum State{
	STATE_READY,
	STATE_PRESSING,
	STATE_DRAGGING
};

typedef struct {
    SDL_Rect *r;
    int x0;
	int y0;
    int offset_x;
	int offset_y;
} DragInfo;

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
void fit_rect_in_screen (SDL_Rect* rect);

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("2.2 - Drag, Click, or Cancel",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	
    /* EXECUÇÃO */

	int running=1, x=0, y=0, cor = 0;
	enum State state_ = STATE_READY;
	SDL_Log("Ready");
	SDL_Color colors[3] = {
							{0,   0, 255, 255}, // azul: STATE_READY
							{0, 255,   0, 255}, // verde: STATE_PRESSING
							{255, 0,   0, 255}  // vermelho: STATE_DRAGGING
						  };
	
	SDL_Rect rect = {(LARGURA_TELA - RECT_SIZE)/2, (ALTURA_TELA - RECT_SIZE)/2, RECT_SIZE, RECT_SIZE};
	DragInfo info = {&rect, rect.x, rect.y, 0, 0};
	
	Uint32 espera = TIMEOUT;
	while(running){
		SDL_Event evt;
		int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
		
		if(isevt){
			if (evt.type == SDL_QUIT) running=0;
			
			switch(state_){
				
				case STATE_READY:
					if(evt.type == SDL_MOUSEBUTTONDOWN){
						x = evt.button.x;
						y = evt.button.y;
						SDL_Point p = {x, y};
						if (SDL_PointInRect(&p, &rect)){
							state_ = STATE_PRESSING;
							cor = 1;
						}
					}
					break;
					
				case STATE_PRESSING:					
					if (evt.type == SDL_MOUSEBUTTONUP){
							state_ = STATE_READY;
							SDL_Log("Clicked!");
							cor = 0;
					}
					else if(evt.type == SDL_MOUSEMOTION){
						SDL_Point p = {x, y};
						int x1,y1;
						Uint32 button_state = SDL_GetMouseState(&x1, &y1);
						if (SDL_PointInRect(&p, &rect) && (button_state & SDL_BUTTON(SDL_BUTTON_LEFT))){
							state_ = STATE_DRAGGING;
							info.x0 = rect.x;
							info.y0 = rect.y;
							info.offset_x = x - rect.x;
							info.offset_y = y - rect.y;
							SDL_Log("Dragging...");
							cor = 2;
						}
					}
					else if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_ESCAPE){
							state_ = STATE_READY;
							SDL_Log("Cancelled!");
							cor = 0;
						}
					}
					break;
		
				case STATE_DRAGGING:					
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_ESCAPE){
							rect.x = info.x0;
							rect.y = info.y0;
							info.offset_x = 0;
							info.offset_y = 0;
							state_ = STATE_READY;
							SDL_Log("Cancelled!");
							cor = 0;
						}
					}
					else if (evt.type == SDL_MOUSEBUTTONUP){
						state_ = STATE_READY;
						SDL_Log("Dropped!");
						cor = 0;
					}
					
					if(evt.type == SDL_MOUSEMOTION){
						rect.x = evt.motion.x - info.offset_x;
						rect.y = evt.motion.y - info.offset_y;
					}
						
					break;
			
			}
		}
		else{
			//vazio
		}
		SDL_SetRenderDrawColor(ren, 255,255,255,255);
    	SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, colors[cor].r, colors[cor].g, colors[cor].b, colors[cor].a);
		fit_rect_in_screen(&rect);
		SDL_RenderFillRect(ren, &rect);
		SDL_RenderPresent(ren);
	}
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
