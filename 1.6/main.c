#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define LARGURA_TELA 1200
#define ALTURA_TELA 600
#define RECT_MAX 200
#define TIMEOUT 50

void rect_generator2 (int x, int y, SDL_Rect* rect, int rect_w, int rect_h);
void fit_rect_in_screen (SDL_Rect* rect);
int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
void fix_mouse_bug (SDL_Rect* rect); //mantem rect do mouse nos limites da pista
void fill_rect_with_pattern (SDL_Renderer* ren, SDL_Texture* pattern, SDL_Rect* rect); //funciona apenas com retangulos

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* win = SDL_CreateWindow("Exercicio 1.6",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND); //habilita transparencia
    
    // Textos
    TTF_Font* fnt = TTF_OpenFont("tiny.ttf", 20);
    assert(fnt != NULL);
    SDL_Color clr = {0xFF,0xFF,0xFF,0xFF};
    
    SDL_Surface* sfc_mouse = TTF_RenderText_Blended(fnt, "Mouse", clr);
    assert(sfc_mouse != NULL);
    SDL_Surface* sfc_anima = TTF_RenderText_Blended(fnt, "Animacao", clr);
    assert(sfc_anima != NULL);
    SDL_Surface* sfc_keyboard = TTF_RenderText_Blended(fnt, "Teclado", clr);
    assert(sfc_keyboard != NULL);
    SDL_Surface* sfc_message = TTF_RenderText_Blended(fnt, "Press SPACEBAR to restart", clr);
    assert(sfc_message != NULL);
    
    SDL_Texture* txt_mouse = SDL_CreateTextureFromSurface(ren, sfc_mouse);
    assert(txt_mouse != NULL);
    SDL_Texture* txt_anima = SDL_CreateTextureFromSurface(ren, sfc_anima);
    assert(txt_anima != NULL);
    SDL_Texture* txt_keyboard = SDL_CreateTextureFromSurface(ren, sfc_keyboard);
    assert(txt_keyboard != NULL);
    SDL_Texture* txt_message = SDL_CreateTextureFromSurface(ren, sfc_message);
    assert(txt_message != NULL);
    
    SDL_FreeSurface(sfc_message);
    SDL_FreeSurface(sfc_keyboard);
    SDL_FreeSurface(sfc_anima);
    SDL_FreeSurface(sfc_mouse);

    /* EXECUÇÃO */
    SDL_Rect r1, r2, r3;
    
    rect_generator2 (50, 50, &r1, RECT_MAX, RECT_MAX/2); //carro1
    rect_generator2 (50, 250, &r2, RECT_MAX, RECT_MAX/2); //carro2
    rect_generator2 (50, 450, &r3, RECT_MAX, RECT_MAX/2); //carro3
    SDL_Texture* carros = IMG_LoadTexture(ren, "carro4.png");
    assert(carros != NULL);
    
    // Pistas
    SDL_Rect r6,r7,r8;
    rect_generator2(0, 0, &r6, LARGURA_TELA, ALTURA_TELA/3);
    rect_generator2(0, ALTURA_TELA/3, &r7, LARGURA_TELA, ALTURA_TELA/3);
    rect_generator2(0, 2*(ALTURA_TELA/3), &r8, LARGURA_TELA, ALTURA_TELA/3);
    SDL_Texture* pistas = IMG_LoadTexture(ren, "pista_teste2.jpg");
    assert(pistas != NULL);
    
    
    // Largada
    SDL_Rect r5;
    rect_generator2 (0, 0, &r5, 50, ALTURA_TELA);
    
    // Linha de chegada
    SDL_Rect r4;
    rect_generator2 (LARGURA_TELA - 100, 0, &r4, 100, ALTURA_TELA);
    SDL_Texture* chegada = IMG_LoadTexture(ren, "chegada_teste.jpg");
    assert(chegada != NULL);
    
    // Placar
    SDL_Rect r9, r10, r11;
    rect_generator2 (100, 50, &r9, 100, 100);
    rect_generator2 (100, 150, &r10, 100, 100);
    rect_generator2 (100, 250, &r11, 100, 100);
    SDL_Texture* trofeus = IMG_LoadTexture(ren, "trofeu_sprite.png");
    assert(trofeus != NULL);
    
    int running = 1, placar = 0;
    int go1 = 1, go2 = 1, go3 = 1, winner = 0;
    
    Uint32 espera = TIMEOUT;
    while (running) {
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        
        if(placar==0){
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
            fix_mouse_bug(&r3);
                  
            if (go1 == 0 && go2 == 0 && go3 ==0) placar = 1;
        }
        
        else if (placar==1){
            if (isevt) {
                switch (evt.type){
                    case SDL_QUIT:
                        running = 0;
                        break;
                    case SDL_KEYDOWN:
                        if (evt.key.keysym.sym == SDLK_SPACE) {
                            go1 = 1;
                            go2 = 1;
                            go3 = 1;
                            winner = 0;
                            r1.x = 50;
                            r2.x = 50;
                            r3.x = 50;
                            placar=0;
                        }
                        break;
                }
            }
        }
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,255);
        SDL_RenderClear(ren);
        fill_rect_with_pattern(ren, pistas, &r6);
        fill_rect_with_pattern(ren, pistas, &r7);
        fill_rect_with_pattern(ren, pistas, &r8);
        fill_rect_with_pattern(ren, chegada, &r4); //desenhar chegada
        fill_rect_with_pattern(ren, chegada, &r5); //desenhar largada
        SDL_RenderCopy(ren, carros, NULL, &r1);
        SDL_RenderCopy(ren, carros, NULL, &r2);
        SDL_RenderCopy(ren, carros, NULL, &r3);

        if(placar==1){   
            SDL_SetRenderDrawColor(ren, 0,0,255,100);
            SDL_Rect rscore = { LARGURA_TELA/2 - 200, ALTURA_TELA/2 - 80, 400,160 };
            SDL_RenderFillRect(ren, &rscore);
        
            SDL_Rect ricon = {LARGURA_TELA/2 - 200, ALTURA_TELA/2 - 40, 80,80};
            SDL_Rect area_icon = {0, 0, 160,148};
            SDL_RenderCopy(ren, trofeus, &area_icon, &ricon);
        
            SDL_Rect r = { LARGURA_TELA/2 - 100, ALTURA_TELA/2 - 40, 280,80 };
            if (winner == 3) SDL_RenderCopy(ren, txt_mouse, NULL, &r);
            else if (winner == 2) SDL_RenderCopy(ren, txt_keyboard, NULL, &r);
            else SDL_RenderCopy(ren, txt_anima, NULL, &r);
        
            SDL_Rect rmessage = { LARGURA_TELA/2 - 200, ALTURA_TELA/2 +150, 400,30 };
            SDL_RenderCopy(ren, txt_message, NULL, &rmessage);
        }
        SDL_RenderPresent(ren);
        
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(trofeus);
    SDL_DestroyTexture(chegada);
    SDL_DestroyTexture(pistas);
    SDL_DestroyTexture(carros);
    SDL_DestroyTexture(txt_message);
    SDL_DestroyTexture(txt_keyboard);
    SDL_DestroyTexture(txt_anima);
    SDL_DestroyTexture(txt_mouse);
    TTF_CloseFont(fnt);
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

void fix_mouse_bug (SDL_Rect* rect) {
    if (rect->x + rect->w > LARGURA_TELA-100)
        rect->x = LARGURA_TELA - rect->w - 100;
    if (rect->x < 50)
        rect->x = 50;
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

void fill_rect_with_pattern(SDL_Renderer* ren, SDL_Texture* pattern, SDL_Rect* rect) {
    int pattern_w, pattern_h;
    SDL_QueryTexture(pattern, NULL, NULL, &pattern_w, &pattern_h); //recupera largura e altura da textura

    SDL_Rect aux_pattern, aux_rect;
    aux_pattern.x = 0;
    aux_pattern.y = 0;
    
    for (int y = rect->y; y < (rect->y + rect->h); y = (y + pattern_h)) {
        for (int x = rect->x; x < rect->x + rect->w; x = (x + pattern_w)) {
            aux_rect.x = x;
            aux_rect.y = y;
            
            if (x + pattern_w > rect->x + rect->w)
                aux_rect.w = rect->x + rect->w - x;
            else
                aux_rect.w = pattern_w;
            
            if(y + pattern_h > rect->y + rect->h)
                aux_rect.h = rect->y + rect->h - y;
            else
                aux_rect.h = pattern_h;

            aux_pattern.w = aux_rect.w;
            aux_pattern.h = aux_rect.h;

            SDL_RenderCopy(ren, pattern, &aux_pattern, &aux_rect);
        }
    }
}
