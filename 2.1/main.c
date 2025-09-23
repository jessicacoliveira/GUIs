#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<assert.h>

#define LARGURA_TELA 316
#define ALTURA_TELA 200
#define RECT_SIZE 100
#define TIMEOUT 350
#define TIMEOUT_CLICK 250

enum State{
	STATE_READY,
	STATE_CLICKING
};

enum AnimaMode{
        MODE_STANDING,
        MODE_WALKING,
        MODE_JUMPING,
        MODE_SPECIAL
};

typedef struct{
    SDL_Texture* textura;
    SDL_Rect *frames_corte;
    int num_frames;
    int width_frame;
    int height_frame;
}Sprite;

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
int AUX_ClickTimeoutCount (Uint32* last_click_time, Uint32 timeout_ms);
void rect_generator2 (int x, int y, SDL_Rect* rect, int rect_w, int rect_h);
void fit_rect_in_screen (SDL_Rect* rect);
void create_frames (SDL_Rect* frames, int num_frames, int start_x, int start_y, int width_frame, int height_frame);

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("2.1 - Múltiplos Cliques",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    
    SDL_Texture* img_idle = IMG_LoadTexture(ren, "idle_ryu.png");
    assert(img_idle != NULL);
    SDL_Texture* img_jump = IMG_LoadTexture(ren, "jump_ryu.png");
    assert(img_jump != NULL);
    SDL_Texture* img_special = IMG_LoadTexture(ren, "special_ryu.png");
    assert(img_special != NULL);
    
    SDL_Texture* img_background = IMG_LoadTexture(ren, "background.jpeg");
    assert(img_special != NULL);
    

    /* EXECUÇÃO */

	int running=1, counter_click=0, score=0, aux_special=0, aux_jump = 1;
	int x, y;
	Uint32 last_click_time;
	enum State state_=STATE_READY;
	enum AnimaMode animamode_ = MODE_STANDING;
  SDL_Rect rect, rect2, player, background;
	rect_generator2 (0, ALTURA_TELA - RECT_SIZE, &rect, RECT_SIZE, RECT_SIZE);
	rect_generator2 (0, ALTURA_TELA/2, &rect2, RECT_SIZE, RECT_SIZE);
	rect_generator2 (0, ALTURA_TELA - RECT_SIZE, &player, RECT_SIZE, RECT_SIZE);
	rect_generator2 (0, 0, &background, LARGURA_TELA, ALTURA_TELA);
	
	SDL_Rect rects_corte_idle[1], rects_corte_jump[4], rects_corte_special[2];
  create_frames (rects_corte_idle, 1, 0, 0, 60, 95);
  create_frames (rects_corte_jump, 4, 0, 0, 55, 116);
  create_frames (rects_corte_special, 2, 0, 0, 103, 95);
        
  Sprite standing = {img_idle,rects_corte_idle, 1, 60, 95};
  Sprite walking = {img_idle,rects_corte_idle, 1, 60, 95};
  Sprite jumping = {img_jump,rects_corte_jump, 4, 55, 116};
  Sprite special = {img_special,rects_corte_special, 2, 103, 95};
	
	
	Uint32 espera = TIMEOUT;
	int frame = 0;
	SDL_Texture* textura;
  SDL_Rect sprite;
	while(running){
		SDL_Event evt;
		int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
		
		if(isevt){
			switch(state_){
				case STATE_READY:
					if(evt.type == SDL_MOUSEBUTTONDOWN){
						state_=STATE_CLICKING;
						last_click_time = SDL_GetTicks();
						counter_click++;
						x = evt.motion.x;
						y = evt.motion.y;
					}
					break;
				case STATE_CLICKING:
					if(evt.type == SDL_MOUSEBUTTONDOWN){
						if(AUX_ClickTimeoutCount(&last_click_time, TIMEOUT_CLICK)){
							counter_click++;
						}
						else {
							if (counter_click>0){
								SDL_Event multiclick;
								multiclick.type = SDL_USEREVENT;
								multiclick.user.code = counter_click;
								SDL_PushEvent(&multiclick);
							}
							counter_click = 0;
							state_=STATE_READY;
						}
					}
					break;
			}
			if (evt.type == SDL_QUIT) running=0;
			
			else if (evt.type == SDL_USEREVENT){
			    int n = evt.user.code;
					printf("N= %d\n", n);
		      
				if (n == 1){
				    if (animamode_ == MODE_JUMPING){ aux_jump=1; player.y = ALTURA_TELA - RECT_SIZE;}
				    if (animamode_ == MODE_SPECIAL) aux_special = 0;
				    animamode_ = MODE_WALKING;    
				}
				
				else if (n == 2){
				    if (animamode_ == MODE_JUMPING){ aux_jump=1; player.y = ALTURA_TELA - RECT_SIZE;}
				    if (animamode_ == MODE_SPECIAL) aux_special = 0;
				    animamode_ = MODE_JUMPING;
				    
				}
				else if (n == 3){
				    if (animamode_ == MODE_JUMPING){ aux_jump=1; player.y = ALTURA_TELA - RECT_SIZE;}
				    if (animamode_ == MODE_SPECIAL) aux_special = 0;
				    animamode_ = MODE_SPECIAL;
				}
			}
		}
		else{
			if (state_ == STATE_CLICKING) {
                            if (!AUX_ClickTimeoutCount(&last_click_time, TIMEOUT_CLICK)) {
                                  if (counter_click > 0) {
                                          SDL_Event multiclick;
                                          multiclick.type = SDL_USEREVENT;
                                          multiclick.user.code = counter_click;
                                          SDL_PushEvent(&multiclick);
                                    }
                                    counter_click = 0;
                                    state_ = STATE_READY;
                            }
                        }
                        
                        switch (animamode_){
                            case MODE_STANDING:
                                if (frame>=standing.num_frames) frame =0;
                                textura = standing.textura;
                                sprite = standing.frames_corte[frame];
                                break;
    
                            case MODE_WALKING:
							                	int player_center = player.x + player.w/2;

							                	if (player_center < x) {
								                  	player.x += 10;
								                  	if (player.x + player.w/2 > x) player.x = x - player.w/2;
								                  	if (player.x + player.w/2 == x) animamode_ = MODE_STANDING;
							                	}
							                	else if (player_center > x) {
								                  	player.x -= 10;
								                  	if (player.x + player.w/2 < x) player.x = x - player.w/2;
								                  	if (player.x + player.w/2 == x) animamode_ = MODE_STANDING;
							                	}
                                
                                else animamode_ = MODE_STANDING;

                                if (frame>=walking.num_frames) frame =0;
                                textura = walking.textura;
                                sprite = walking.frames_corte[frame];
                                break;
        
                            case MODE_JUMPING:
                                if (aux_jump == 1) {
                                     player.y -= 10;
                                      if (player.y <= ALTURA_TELA - 130) aux_jump = 0;
                                  }
                                  
                                else if (aux_jump == 0){
                                    player.y += 10;
                                    if (player.y >= ALTURA_TELA - RECT_SIZE) {
                                        player.y = ALTURA_TELA - RECT_SIZE;
                                        aux_jump = 1;
                                        animamode_ = MODE_STANDING;
                                        }
                                }
                                
                                if (frame>=jumping.num_frames) frame =0;
                                textura = jumping.textura;
                                sprite = jumping.frames_corte[frame];
                                break;
        
                            case MODE_SPECIAL:
                                aux_special++;
                                if (aux_special >= special.num_frames){
                                    animamode_ = MODE_STANDING;
                                    aux_special = 0;
									                  player.w = 60;
								                  	player.h = 95;
                                }
                                if (frame>=special.num_frames) frame =0;
                                textura = special.textura;
                                sprite = special.frames_corte[frame];
							                	player.w = sprite.w;
							                	player.h = sprite.h;
                                break;
                        }
						frame++;                
		}
		
		SDL_SetRenderDrawColor(ren, 255,255,255,255);
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, img_background, NULL, &background);
		fit_rect_in_screen (&player);
    SDL_RenderCopy(ren, textura, &sprite, &player);
    SDL_RenderPresent(ren); 
	}
	
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_special);
  SDL_DestroyTexture(img_jump);
  SDL_DestroyTexture(img_idle);
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

int AUX_ClickTimeoutCount(Uint32* last_click_time, Uint32 timeout_ms) {
    Uint32 delta = SDL_GetTicks() - *last_click_time;
    if (delta < timeout_ms) {
	*last_click_time = SDL_GetTicks();	
        return 1; 
    } else {
        *last_click_time = 0;
        return 0;
    }
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

void create_frames (SDL_Rect* frames, int num_frames, int start_x, int start_y, int width_frame, int height_frame){
    for(int i = 0; i < num_frames; i++){
        frames[i] = (SDL_Rect) { start_x + i*width_frame, start_y, width_frame, height_frame };
    }
}
