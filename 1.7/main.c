#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

#define LARGURA_TELA 400
#define ALTURA_TELA 400
#define TIMEOUT 150

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
void fit_rect_in_screen (SDL_Rect* rect);
void create_frames (SDL_Rect* frames, int num_frames, int start_x, int start_y, int width_frame, int height_frame); //funciona apenas se o sprite sheet estiver na horizontal

int main (int argc, char* args[]){

    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.7: Animacao Aladin",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img_up = IMG_LoadTexture(ren, "001.png");
    assert(img_up != NULL);
    SDL_Texture* img_left = IMG_LoadTexture(ren, "002.png");
    assert(img_left != NULL);
    SDL_Texture* img_down = IMG_LoadTexture(ren, "003.png");
    assert(img_down != NULL);
    SDL_Texture* img_center = IMG_LoadTexture(ren, "004.png");
    assert(img_center != NULL);
    

    /* EXECUÇÃO */
    int aux = 0, running = 1;
    SDL_Rect r = { 0,0, 50 ,140};
    SDL_Rect r_center = {(LARGURA_TELA - 186)/2, (ALTURA_TELA - 128)/2, 186, 128};
    
    SDL_Rect rects_corte_up[4];
    SDL_Rect rects_corte_sides[3];
    SDL_Rect rects_corte_down[5];
    SDL_Rect rects_corte_center[2];
    
    create_frames (rects_corte_up, 4, 0, 0, 36, 61);
    create_frames (rects_corte_sides, 3, 0, 0, 39, 61);
    create_frames (rects_corte_down, 5, 0, 0, 42, 73);
    
    int delta;
    /* delta=36;
    for (int i=0; i<4; i++){
        rects_corte_up [i] = (SDL_Rect) { i*delta,0, delta ,61};
    }
    
    delta=39;
    for (int i=0; i<3; i++){
        rects_corte_sides [i] = (SDL_Rect) { i*delta,0, delta ,61};
    }
    
    delta=42;
    for (int i=0; i<5; i++){
        rects_corte_down [i] = (SDL_Rect) { i*delta,0, delta ,73 };
    } */
    
    delta=85;
    for (int i=0; i<2; i++){
        rects_corte_center [i] = (SDL_Rect) { 0,i*delta, 124 , delta }; //sprite sheet está na vertical!
    }
    
    
    int frame = 0, isopen=1, frame2=0;
    SDL_Texture* textura;
    SDL_Rect sprite;
    Uint32 espera = TIMEOUT;
    while (running) {
        SDL_SetRenderDrawColor(ren, 250,244,150,0x00);
        SDL_RenderClear(ren);
        
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            switch (evt.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (evt.key.keysym.sym == SDLK_SPACE) {
                        isopen = !isopen;
                        if (isopen){
                            frame2 = 0;
                        }
                        else{
                            frame2 = 1;
                        }
                    }
                    break; 
            }
        }
        else{     
            if(aux == 0){
                r.x += 15;
               if (r.x >= LARGURA_TELA - r.w){ aux=1; frame = 0;}
               if (frame>=4) frame =0; //rects_corte_up tem 4 frames
               textura = img_up;
               sprite = rects_corte_up[frame];
            }
            else if(aux == 1){
                r.y += 15;
                if (r.y >= ALTURA_TELA - r.h){ aux=2; frame = 0;}
                if (frame>=3) frame =0;
                textura = img_left;
                sprite = rects_corte_sides[frame];
            }
            else if(aux == 2){
                r.x += -15;
                if (r.x <= 0) {aux=3; frame = 0;}
                if (frame>=5) frame =0;
                textura = img_down;
                sprite = rects_corte_down[frame];
            }
            else if(aux == 3){
                r.y += -15;
                if (r.y <= 0) {aux=0; frame = 0;}
                if (frame>=3) frame =0;
                textura = img_left;
                sprite = rects_corte_sides[frame];
            }
            frame++; 
        }
        fit_rect_in_screen (&r);
        fit_rect_in_screen (&r_center);
        SDL_RenderCopy(ren, textura, &sprite, &r);
        SDL_RenderCopy(ren, img_center, &rects_corte_center[frame2], &r_center);
        SDL_RenderPresent(ren);  
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(img_center);
    SDL_DestroyTexture(img_down);
    SDL_DestroyTexture(img_left);
    SDL_DestroyTexture(img_up);
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

void create_frames (SDL_Rect* frames, int num_frames, int start_x, int start_y, int width_frame, int height_frame){
    for(int i = 0; i < num_frames; i++){
        frames[i] = (SDL_Rect) { start_x + i*width_frame, start_y, width_frame, height_frame };
    }
}
