![FSM](FSM.png)

O código implementa um sistema de detecção de múltiplos cliques do mouse e associa cada tipo de clique a uma animação do personagem.
A FSM modelada tem dois estados: `READY` (aguarda o primeiro clique) e `CLICKING` (conta cliques dentro do tempo/área de clique limite). Quando o tempo expira ou há movimentação do mouse, um evento é disparado com o número de cliques acumulados.

## Estados :
### STATE_READY
* Transição ocorre no primeiro clique (SDL_MOUSEBUTTONDOWN).
* Armazena a posição dp clique (x0,y0) e inicia o contador de cliques (click.counter).
### STATE_CLICKING
* Se houver um novo clique dentro dos 250ms: incrementa o contador de cliques;
* Senão: envia evento SDL_USEREVENT com o número de cliques acumulados, zera o contador e volta para STATE_READY.
* Se houver detecção de movimento do mouse (SDL_MOUSEMOTION) além de um limite de tolerância de 5 pixels: dispara evento com o número de cliques acumulados até então, zera o contador e volta para STATE_READY.

## Eventos disparados:
* `1 clique` → o personagem se desloca horizontalmente até a posição do clique (*walking*);
* `2 cliques` → o personagem realiza um "salto" (*jumping*);
* `3 cliques` → o personagem executa a animação do golpe especial (*special*).

## Comentários adicionais
- As mecânicas de *caminhar* e *saltar* estão simplificadas (velocidade constante, ausência de gravidade, valores arbitrários etc.).
- A implementação permite que o mouse se mexa por até 5 pixels em x/y antes de abortar a contagem de cliques.
