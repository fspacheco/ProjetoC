/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| Pedro dos Santos Guerreiro e Thyago Augusto Reboledo                                                                ||
|| Vers�o 1.0.0 (29/05/2015)                                                                                           ||
|| Vers�o 1.0.1 (30/05/2015)                                                                                           ||
|| Vers�o 1.0.2 (02/06/2015)                                                                                           ||
|| Vers�o 1.0.3 (08/06/2015)                                                                                           ||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*# -> os coment�rios que tiverem isso ainda est�o em desenvolvimento */

#include <stdio.h>
#include <stdlib.h>

#include "projetoHeader.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main()
{
    bool quit = false;
    int gamestate = 0;
    int i=0;
    int j=0;
    srand(time(NULL));

    ALLEGRO_DISPLAY*        display;
    ALLEGRO_TIMER*          timer;
    ALLEGRO_EVENT_QUEUE*    event_queue;
    ALLEGRO_EVENT           ev;

    ALLEGRO_BITMAP*         img_block;
    ALLEGRO_BITMAP*         img_player_R;
    ALLEGRO_BITMAP*         img_player_L;
    ALLEGRO_BITMAP*         img_player_immobile_R;
    ALLEGRO_BITMAP*         img_player_immobile_L;
    ALLEGRO_BITMAP*         img_player_jump_R;
    ALLEGRO_BITMAP*         img_player_jump_L;
    /*#ALLEGRO_BITMAP*         img_enemy1;*/
    /*#ALLEGRO_BITMAP*         img_enemy_bullet;*/

    ALLEGRO_SAMPLE*         spl_theme;
    /*#ALLEGRO_SAMPLE*         spl_mlk;*/
    /*#ALLEGRO_SAMPLE*         spl_playerShoot;*/

    ALLEGRO_FONT*           fonte;

    /* Estruturas */
    s_object player;
    s_object block[LINHA_MAX][COLUNA_MAX];

    for (i = 0; i<LINHA_MAX; i++)
    {
        for(j = 0; j<COLUNA_MAX; j++)
        {
            /* Cria o player */
            if(mapa[i][j] == 2)
            {
                player.y = i*48 + 8; /* O "+8" � por causa do tamanho da img do player (40x40) em compara��o ao tamanho do bloco (64x48) */
                player.x = j*64;
                player.live = true;
                block[i][j].live = false;
            }
            /* Cria os Blocos */
            block[i][j].y = i*48;
            block[i][j].x = j*64;
            if(mapa[i][j] == 1)
            {
                block[i][j].live = true;
            }
            if(mapa[i][j] != 1)
            {
                block[i][j].live = false;
            }
        }
    }

    /*#s_object playerBullet;
    playerBullet.x = 0;
    playerBullet.y = 0;
    playerBullet.live = false;*/

    /*#s_object enemy[ENEMY_MAX];
    for (i=0; i<ENEMY_MAX; i++)
    {
        enemy[i].x = ;
        enemy[i].y = ;
        enemy[i].live = false;
    }*/

    /*#s_object enemyBullet[ENEMYBULLET_MAX];
    for(i=0; i < ENEMYBULLET_MAX; i++)
    {
        enemyBullet[i].x = 0;
        enemyBullet[i].y = 0;
        enemyBullet[i].live = false;
    }*/

    s_background background_0;

    background_0.x = 0;
    background_0.y = 0;
    background_0.width = 1280;
    background_0.height = 720;
    background_0.image = NULL;

    s_animation walking;
    walking.maxFrame = 8;
    walking.frameDelay = 5;
    walking.frameCount = 0;
    walking.curFrame = 0;
    walking.frameHeight = 40;
    walking.frameWidth = 40;

    s_animation jumping;
    jumping.maxFrame = 9;
    jumping.frameDelay = 5;
    jumping.frameCount = 0;
    jumping.curFrame = 0;
    jumping.frameHeight = 52;
    jumping.frameWidth = 40;

    /* Faz com que as teclas come�em em false */
    for(i=0; i<KEY_MAX; i++)
    {
        keys[i] = false;
    }

    /* Carrega as configura��es (teclado, audio, etc) */
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();


    /* Erros ao criar algo */
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        printf("Erro ao criar o display");
        exit(-1);
    }

    timer = al_create_timer(1/60.0);
    if(!timer)
    {
        printf("Erro ao criar o timer");
        exit(-1);
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        printf("Erro ao criar o event_queue");
        exit(-1);
    }

    /* Carregando as Imagens */
    background_0.image = al_load_bitmap("Sprites/Background/background_h1n1.png");
    img_block = al_load_bitmap("Sprites/block.png");
    img_player_R = al_load_bitmap("Sprites/Player/player_R.png");
    img_player_L = al_load_bitmap("Sprites/Player/player_L.png");
    img_player_immobile_R = al_load_bitmap("Sprites/Player/player_immobile_R.png");
    img_player_immobile_L = al_load_bitmap("Sprites/Player/player_immobile_L.png");
    img_player_jump_R = al_load_bitmap("Sprites/Player/player_jump_R.png");
    img_player_jump_L = al_load_bitmap("Sprites/Player/player_jump_L.png");
    /*#img_enemy = al_load_bitmap("Sprites/Enemies/enemy_h1n1.png");*/

    /* Carregando os Samples */
    al_reserve_samples(1);
    spl_theme = al_load_sample("Sounds/theme.wav");
    /*#spl_mlk = al_load(Sounds/mlk.wav);*/
    /*#spl_playerShoot = al_load();*/

    /* Registra os Eventos */
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    /* Carregando os Timers */
    al_start_timer(timer);

    /* Carregando a Fonte */
    fonte = al_load_ttf_font("Joystix.ttf", 20, 0);

    while(!quit)
    {
        switch(gamestate)
        {
        case 0:
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) /* Faz com que o jogo feche ao clicar no bot�o "X" do display */
            {
                quit = true;
            }

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) /* Faz com que o jogo feche ao pressionar esc */
                {
                    quit = true;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE) /* Faz com que o jogo inicie ao pressionar space */
                {
                    gamestate = 1;
                }

            }
            if(ev.type == ALLEGRO_EVENT_TIMER) /* Mostrar mensagens na tela */
            {
                if(ev.timer.source == timer)
                {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "Press Space to Play!");
                    al_flip_display();
                }
            }
            break;

        case 1:

            force-=0.5; /* Queda */

            /* Toca a m�sica de fundo */
            al_play_sample(spl_theme, 0.9, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);

            /* Fechar o display */
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                quit = true;
            }

            /* Evento de quando a tecla � pressionada */
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    quit = true;
                    break;
                case ALLEGRO_KEY_UP:
                    if(jump == false)
                    {
                        jump = true;
                        force = gravity;
                    }
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[KEY_RIGHT]=false;
                    keys[KEY_LEFT]=true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[KEY_LEFT]=false;
                    keys[KEY_RIGHT]=true;
                    break;
                    /*#case ALLEGRO_KEY_SPACE:
                        playerShoot(&player, &playerBullet, spl_playerShoot);
                    break;*/
                }
            }

            /* Evento de quando a tecla � solta */
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_LEFT:
                    keys[KEY_LEFT]=false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[KEY_RIGHT]=false;
                    break;
                }
            }

            if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                if(ev.timer.source == timer)
                {
                    /* Posicionamento do player*/
                    if(jump == true)
                    {
                        player.y-=force;

                    }
                    if(keys[KEY_RIGHT])
                    {
                        playerDirection=-1;
                        player.x+=2;
                    }
                    if(keys[KEY_LEFT])
                    {
                        playerDirection=1;
                        player.x-=2;
                    }

                    /* Prende a Camera no Personagem */
                    cameraX = player.x-(SCREEN_W/2);
                    cameraY = player.y-(SCREEN_H/2);

                    /* Fazer com que a camera n�o passe dos limites do mapa */
                    if (cameraX < 0) cameraX = 0;
                    if (cameraY < 0) cameraY = 0;
                    if (cameraX > WORLD_W - SCREEN_W) cameraX = WORLD_W - SCREEN_W;
                    if (cameraY > WORLD_H - SCREEN_H) cameraY = WORLD_H - SCREEN_H;

                    /* Colis�o do player na parede */
                    collision_player_wall(&player, img_block);

                    collision_player_tiles(&player, &block[12][3], img_block);

                    /* Desenha o Background */
                    drawnBackground(&background_0);

                    /* Desenha os Blocos */
                    for (i = 0; i<LINHA_MAX; i++)
                    {
                        for(j = 0; j<COLUNA_MAX; j++)
                        {
                            if(block[i][j].live == true)
                            {
                                al_draw_bitmap(img_block, block[i][j].x - cameraX, block[i][j].y - cameraY, 0);
                            }
                        }
                    }

                    /* Desenho do player */

                    /* Player parado */
                    if(jump == false && ((!keys[KEY_LEFT] && playerDirection == 1) || player.x == 64))
                    {
                        al_draw_bitmap(img_player_immobile_L, player.x-cameraX, player.y-cameraY, 0);
                    }
                    else if(jump == false && ((!keys[KEY_RIGHT] && playerDirection == -1) || player.x == WORLD_W - (64+walking.frameWidth)))
                    {
                        al_draw_bitmap(img_player_immobile_R, player.x-cameraX, player.y-cameraY, 0);
                    }

                    /* Player andando */
                    if(++walking.frameCount >= walking.frameDelay)
                    {
                        if(++walking.curFrame >= walking.maxFrame)
                        {
                            walking.curFrame = 0;
                        }
                        walking.frameCount = 0;
                    }

                    if(jump == false && keys[KEY_LEFT] && playerDirection == 1 && player.x != 64)
                    {
                        al_draw_bitmap_region(img_player_L, walking.curFrame * walking.frameWidth, 0, walking.frameWidth, walking.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                    }
                    else if(jump == false && keys[KEY_RIGHT] && playerDirection == -1 && player.x != WORLD_W - (64+walking.frameWidth))
                    {
                        al_draw_bitmap_region(img_player_R, walking.curFrame * walking.frameWidth, 0, walking.frameWidth, walking.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                    }

                    /* Player pulando */
                    if(++jumping.frameCount >= jumping.frameDelay)
                    {
                        if(++jumping.curFrame >= jumping.maxFrame)
                        {
                            jumping.curFrame = 0;
                        }
                        jumping.frameCount = 0;
                    }

                    if(jump == true && playerDirection == 1)
                    {
                        al_draw_bitmap_region(img_player_jump_L, jumping.curFrame * jumping.frameWidth, 0, jumping.frameWidth, jumping.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                    }
                    else if(jump == true && playerDirection == -1)
                    {
                        al_draw_bitmap_region(img_player_jump_R, jumping.curFrame * jumping.frameWidth, 0, jumping.frameWidth, jumping.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                    }
                }
                /* Troca o display */
                al_flip_display();
            }
        }
    }
    /* Destruindo as variaveis */
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(background_0.image);
    al_destroy_bitmap(img_block);
    al_destroy_bitmap(img_player_R);
    al_destroy_bitmap(img_player_L);
    al_destroy_bitmap(img_player_immobile_R);
    al_destroy_bitmap(img_player_immobile_L);
    al_destroy_bitmap(img_player_jump_R);
    al_destroy_bitmap(img_player_jump_L);
    /*#al_destroy_bitmap(img_enemy1);*/
    /*#al_destroy_bitmap(img_enemy_bullet);*/
    al_destroy_sample(spl_theme);
    /*#al_destroy_sample(spl_playerShoot);*/

    return 0;
}
