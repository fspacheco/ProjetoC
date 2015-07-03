/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| Pedro dos Santos Guerreiro                                                                                          ||
||Thyago Augusto Reboledo                                                                                              ||
||Leukocyte                                                                                                            ||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

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
    srand(time(NULL));

    int change_bkg = 0;
    int stopwatch = 120;
    bool iddle = false;

    int bulletID=0;
    int bulletCount=0;

    ALLEGRO_DISPLAY*            display;
    ALLEGRO_TIMER*              timer_0p2;
    ALLEGRO_TIMER*              timer_1;
    ALLEGRO_TIMER*              timer_60;
    ALLEGRO_EVENT_QUEUE*        event_queue;
    ALLEGRO_EVENT               ev;

    ALLEGRO_BITMAP*             img_home_screen;
    ALLEGRO_BITMAP*             img_dica_h1n1;
    ALLEGRO_BITMAP*             img_background0;
    ALLEGRO_BITMAP*             img_game_over;
    ALLEGRO_BITMAP*             img_you_win;

    ALLEGRO_BITMAP*             img_heart;
    ALLEGRO_BITMAP*             img_medal;
    ALLEGRO_BITMAP*             img_clock;
    ALLEGRO_BITMAP*             img_block1;
    ALLEGRO_BITMAP*             img_block2;

    ALLEGRO_BITMAP*             img_player_walking;
    ALLEGRO_BITMAP*             img_player_walking_shoot;
    ALLEGRO_BITMAP*             img_player_immobile;
    ALLEGRO_BITMAP*             img_player_immobile_shoot;
    ALLEGRO_BITMAP*             img_player_jump;
    ALLEGRO_BITMAP*             img_player_jump_shoot;
    ALLEGRO_BITMAP*             img_player_bullet;

    ALLEGRO_BITMAP*             img_enemy1;
    ALLEGRO_BITMAP*             img_boss1;
    ALLEGRO_BITMAP*             img_enemy_bullet;

    ALLEGRO_SAMPLE*             spl_theme;
    ALLEGRO_SAMPLE*             spl_playerShoot;
    ALLEGRO_SAMPLE*             spl_mlk;

    ALLEGRO_SAMPLE_INSTANCE*    instance_theme;
    ALLEGRO_SAMPLE_INSTANCE*    instance_playerShoot;
    ALLEGRO_SAMPLE_INSTANCE*    instance_mlk;

    ALLEGRO_FONT*           fonte16;

    /* Estruturas */
    s_object player;
    s_object block[LINHA_MAX][COLUNA_MAX];
    s_object enemy1[LINHA_MAX][COLUNA_MAX];

    for (i=0; i<LINHA_MAX; i++)
    {
        for(j=0; j<COLUNA_MAX; j++)
        {
            /* Cria o player */
            if(mapa[i][j] == 1)
            {
                player.y = i*16 - 24;
                player.x = j*64 + 24;
                player.speed = 3;
                player.direction = 1;

                player.live = true;
                player.life = 100;

                block[i][j].live = false;
            }
            /* Cria os Blocos */
            if(mapa[i][j] == 2)
            {
                block[i][j].y = i*16;
                block[i][j].x = j*64;
                block[i][j].live = true;
            }
            if(mapa[i][j] == 3)
            {
                block[i][j].y = i*16;
                block[i][j].x = j*64;
                block[i][j].live = true;
            }
            if(mapa[i][j] == 4)
            {
                block[i][j].y = i*16;
                block[i][j].x = j*64;
                block[i][j].live = true;
            }
            if(mapa[i][j] == 5)
            {
                block[i][j].y = i*16;
                block[i][j].x = j*64;
                block[i][j].live = false;
            }
            /* Cria os Inimigos */
            if(mapa[i][j] == 6)
            {
                enemy1[i][j].y = i*16 - 24;
                enemy1[i][j].x = j*64;
                enemy1[i][j].speed = 2;
                enemy1[i][j].direction = -1;

                enemy1[i][j].life = 3;
                enemy1[i][j].live = true;

                block[i][j].live = false;
            }
            if(mapa[i][j] == 7 || mapa[i][j] == 8)
            {
                enemy1[i][j].y = i*16 - 24;
                enemy1[i][j].x = j*64;
                enemy1[i][j].speed = 2;
                enemy1[i][j].direction = 1;

                enemy1[i][j].life = 3;
                enemy1[i][j].live = false;

                block[i][j].live = false;
            }
            if(mapa[i][j] == 9)
            {
                enemy1[i][j].y = i*16 - 24;
                enemy1[i][j].x = j*64;
                enemy1[i][j].speed = 2;
                enemy1[i][j].direction = -1;

                enemy1[i][j].life = 25;
                enemy1[i][j].live = false;

                block[i][j].live = false;
            }
        }
    }

    s_bullet playerBullet[NUM_BULLET];
    s_bullet enemyBullet[NUM_BULLET];

    for(i=0; i<NUM_BULLET; i++)
    {
        playerBullet[i].x = 0;
        playerBullet[i].y = 0;
        playerBullet[i].speed = 5;
        playerBullet[i].direction = 1;
        playerBullet[i].live = false;

        enemyBullet[i].x = 0;
        enemyBullet[i].y = 0;
        enemyBullet[i].speed = 5;
        enemyBullet[i].direction = 0;
        enemyBullet[i].live = false;
    }
    s_animation walking;
    walking.maxFrame = 8;
    walking.frameDelay = 5;
    walking.frameCount = 0;
    walking.curFrame = 0;
    walking.frameHeight = 40;
    walking.frameWidth = 40;

    s_animation jumping;
    jumping.maxFrame = 7;
    jumping.frameDelay = 5;
    jumping.frameCount = 0;
    jumping.curFrame = 0;
    jumping.frameHeight = 52;
    jumping.frameWidth = 40;

    s_animation immobile;
    immobile.maxFrame = 7;
    immobile.frameDelay = 15;
    immobile.frameCount = 0;
    immobile.curFrame = 0;
    immobile.frameHeight = 40;
    immobile.frameWidth = 40;

    s_animation anim_enemy1;
    anim_enemy1.maxFrame = 3;
    anim_enemy1.frameDelay = 15;
    anim_enemy1.frameCount = 0;
    anim_enemy1.curFrame = 0;
    anim_enemy1.frameHeight = 40;
    anim_enemy1.frameWidth = 40;

    /* Faz com que as teclas comecem em false */
    for(i=0; i<KEY_MAX; i++)
    {
        keys[i] = false;
    }

    /* Carrega as configuracoes (teclado, audio, etc) */
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

    timer_0p2 = al_create_timer(5.0);
    timer_1 = al_create_timer(1.0);
    timer_60 = al_create_timer(1/60.0);

    if(!timer_0p2)
    {
        printf("Erro ao criar o timer de 0.2 FPS");
        exit(-1);
    }
    if(!timer_1)
    {
        printf("Erro ao criar o timer de 1 FPS");
        exit(-1);
    }
    if(!timer_60)
    {
        printf("Erro ao criar o timer de 60 FPS");
        exit(-1);
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        printf("Erro ao criar o event_queue");
        exit(-1);
    }

    /* Carregando as Imagens */
    img_home_screen = al_load_bitmap("Sprites/Background/home_screen.png");
    img_dica_h1n1 = al_load_bitmap("Sprites/Background/dica_h1n1.png");
    img_background0 = al_load_bitmap("Sprites/Background/background_h1n1.png");
    img_you_win = al_load_bitmap("Sprites/Background/you_win.png");
    img_game_over = al_load_bitmap("Sprites/Background/game_over.png");

    img_heart = al_load_bitmap("Sprites/heart.png");
    img_medal = al_load_bitmap("Sprites/medal.png");
    img_clock = al_load_bitmap("Sprites/clock.png");
    img_block1 = al_load_bitmap("Sprites/block1.png");
    img_block2 = al_load_bitmap("Sprites/block2.png");

    img_player_walking = al_load_bitmap("Sprites/Player/player_walking.png");
    img_player_walking_shoot = al_load_bitmap("Sprites/Player/player_walking_shoot.png");
    img_player_immobile = al_load_bitmap("Sprites/Player/player_immobile.png");
    img_player_immobile_shoot = al_load_bitmap("Sprites/Player/player_immobile_shoot.png");
    img_player_jump = al_load_bitmap("Sprites/Player/player_jump.png");
    img_player_jump_shoot = al_load_bitmap("Sprites/Player/player_jump_shoot.png");
    img_player_bullet = al_load_bitmap("Sprites/Player/player_bullet.png");

    img_enemy1 = al_load_bitmap("Sprites/Enemies/enemy_h1n1.png");
    img_boss1 = al_load_bitmap("Sprites/Enemies/boss_h1n1.png");
    img_enemy_bullet = al_load_bitmap("Sprites/Enemies/enemy_bullet.png");

    /* Carregando os Samples */
    al_reserve_samples(10);
    spl_theme = al_load_sample("Sounds/theme.wav");
    spl_playerShoot = al_load_sample("Sounds/shoot.wav");
    spl_mlk = al_load_sample("Sounds/mlk.wav");

    instance_theme = al_create_sample_instance(spl_theme);
    instance_playerShoot = al_create_sample_instance(spl_playerShoot);
    instance_mlk = al_create_sample_instance(spl_mlk);

    al_set_sample_instance_gain(instance_playerShoot, 0.5);

    al_attach_sample_instance_to_mixer(instance_theme, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance_playerShoot, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance_mlk, al_get_default_mixer());

    /* Registra os Eventos */
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer_0p2));
    al_register_event_source(event_queue, al_get_timer_event_source(timer_1));
    al_register_event_source(event_queue, al_get_timer_event_source(timer_60));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    /* Carregando os timers */
    al_start_timer(timer_0p2);
    al_start_timer(timer_1);
    al_start_timer(timer_60);

    /* Carregando a fonte */
    fonte16 = al_load_ttf_font("Joystix.ttf", 16, 0);

    while(!quit)
    {
        switch(gamestate)
        {
        case 0:
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) /* Faz com que o jogo feche ao clicar no botao "X" do display */
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
                    change_bkg++;
                    if(change_bkg >= 2)
                    {
                        gamestate = 1;
                    }
                }
            }
            if(ev.type == ALLEGRO_EVENT_TIMER) /* Mostrar mensagens na tela */
            {
                if(ev.timer.source == timer_60)
                {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    if(change_bkg == 0)
                    {
                        al_draw_bitmap(img_home_screen, 0, 0, 0);
                    }
                    if(change_bkg == 1)
                    {
                        al_draw_bitmap(img_dica_h1n1, 0, 0, 0);

                        if(++anim_enemy1.frameCount >= anim_enemy1.frameDelay)
                        {
                            if(++anim_enemy1.curFrame >= anim_enemy1.maxFrame)
                            {
                                anim_enemy1.curFrame = 0;
                            }
                            anim_enemy1.frameCount = 0;
                        }

                        al_draw_bitmap_region(img_enemy1, anim_enemy1.curFrame * anim_enemy1.frameWidth, 0, anim_enemy1.frameWidth, anim_enemy1.frameHeight, 330, 320, 0);
                        al_draw_bitmap_region(img_boss1, anim_enemy1.curFrame * anim_enemy1.frameWidth, 0, anim_enemy1.frameWidth, anim_enemy1.frameHeight, 450, 320, 0);
                    }
                    al_flip_display();
                }
            }
            break;
        case 1:

            if(force>= -7.5)
            {
                force-=0.5; /* Queda */
            }

            /* Toca a música de fundo */
            if(!al_get_sample_instance_playing(instance_theme) && !al_get_sample_instance_playing(instance_mlk))
            {
                al_play_sample_instance(instance_theme);
            }

            /* Fechar o display */
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                quit = true;
            }

            /* Evento de quando a tecla eh pressionada */
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    quit = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[KEY_SPACE]=true;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[KEY_UP] = true;
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
                case ALLEGRO_KEY_F1:
                    if(!al_get_sample_instance_playing(instance_mlk))
                    {
                        al_play_sample_instance(instance_mlk);
                        al_stop_sample_instance(instance_theme);
                    }
                    break;
                case ALLEGRO_KEY_M:
                    if(al_get_sample_instance_playing(instance_mlk))
                    {
                        al_stop_sample_instance(instance_mlk);
                    }
                    break;
                }
            }

            /* Evento de quando a tecla eh solta */
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_SPACE:
                    keys[KEY_SPACE]=false;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[KEY_UP] = false;
                    break;
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
                if(ev.timer.source == timer_0p2)
                {
                    if((iddle == false) && (keys[KEY_RIGHT] == false) && (keys[KEY_LEFT] == false) && (jump == false))
                    {
                        iddle = true;
                    }
                }
                if(ev.timer.source == timer_1)
                {
                    stopwatch--;
                }
                if(ev.timer.source == timer_60)
                {
                    /* Posicionamento do player*/
                    player.y-=force;

                    if(keys[KEY_RIGHT])
                    {
                        player.direction = 1;
                        player.x+=player.speed;
                    }
                    if(keys[KEY_LEFT])
                    {
                        player.direction = -1;
                        player.x-=player.speed;
                    }

                    if(keys[KEY_SPACE])
                    {
                        for(i=0; i<NUM_BULLET; i++)
                        {
                            if(!al_get_sample_instance_playing(instance_playerShoot))
                            {
                                playerShoot(&player, &playerBullet[i], instance_playerShoot);
                            }
                        }
                    }


                    /*Posicionamento do Inimigo */

                    for (i=0; i<LINHA_MAX; i++)
                    {
                        for(j=0; j<COLUNA_MAX; j++)
                        {
                            if(player.x > enemy1[i][j].x + 40)
                            {
                                enemy1[i][j].direction = 1;
                            }
                            else if(player.x + 40 <= enemy1[i][j].x)
                            {
                                enemy1[i][j].direction = -1;
                            }
                        }
                    }

                    /* ~~Posicionamento do projetil~~ */

                    /* Chance do Inimigo Atirar */
                    chance_enemy_shoot = rand() % 40;
                    for (i=0; i<LINHA_MAX; i++)
                    {
                        for(j=0; j<COLUNA_MAX; j++)
                        {
                            enemyShoot(&player, &enemy1[i][j], enemyBullet, &bulletID, &bulletCount);
                        }
                    }
                    for(i=0; i<NUM_BULLET; i++)
                    {
                        if(!playerBullet[i].live)
                        {
                            playerBullet[i].direction = player.direction;
                        }

                        if(playerBullet[i].live)
                        {
                            if(playerBullet[i].direction == -1)
                            {
                                playerBullet[i].x-=playerBullet[i].speed;
                            }
                            else if(playerBullet[i].direction == 1)
                            {
                                playerBullet[i].x+=playerBullet[i].speed;
                            }
                        }
                        if(enemyBullet[i].live)
                        {
                            if(enemyBullet[i].direction == -1)
                            {
                                enemyBullet[i].x-=enemyBullet[i].speed;
                            }
                            if(enemyBullet[i].direction == 1)
                            {
                                enemyBullet[i].x+=enemyBullet[i].speed;
                            }

                        }
                    }

                    /* Prende a Camera no Personagem */
                    cameraX = player.x-(SCREEN_W/2);
                    cameraY = player.y-(SCREEN_H/2);

                    /* Fazer com que a camera nao passe dos limites do mapa */
                    if (cameraX < 0) cameraX = 0;
                    if (cameraY < 0) cameraY = 0;
                    if (cameraX > WORLD_W - SCREEN_W) cameraX = WORLD_W - SCREEN_W;
                    if (cameraY > WORLD_H - SCREEN_H) cameraY = WORLD_H - SCREEN_H;

                    /* Colisoes + check trap */
                    for (i = 0; i<LINHA_MAX; i++)
                    {
                        for(j = 0; j<COLUNA_MAX; j++)
                        {
                            for(k=0; k<NUM_BULLET; k++)
                            {
                                collision_bullet_player(&player, &enemyBullet[k], img_enemy_bullet, &bulletCount, 40, 40);
                            }
                            if(block[i][j].live == true)
                            {
                                if(mapa[i][j] == 2)
                                {
                                    collision_player_tiles(&player, &block[i][j], &jumping, img_block2);
                                }
                                if(mapa[i][j] == 3)
                                {
                                    collision_player_tiles(&player, &block[i][j], &jumping, img_block2);
                                }
                                if(mapa[i][j] == 4)
                                {
                                    check_trap(&player, &block[i][j], &enemy1[i][j], 4);
                                    collision_player_tiles(&player, &block[i][j], &jumping, img_block2);
                                }
                                if(mapa[i][j] == 5)
                                {
                                    collision_player_tiles(&player, &block[i][j], &jumping, img_block2);
                                }
                                for(k=0; k<NUM_BULLET; k++)
                                {
                                    collision_bullet_tiles(&playerBullet[k], &block[i][j], img_player_bullet, img_block2, 0, &bulletCount);
                                    collision_bullet_tiles(&enemyBullet[k], &block[i][j], img_player_bullet, img_block2, 1, &bulletCount);
                                }
                            }
                            if(block[i][j].live == false)
                            {
                                if(mapa[i][j] == 5)
                                {
                                    check_trap(&player, &block[i][j], &enemy1[i][j], 5);
                                }
                                if(mapa[i][j] == 6)
                                {
                                    collision_player_enemy(&player, &enemy1[i][j], 40, 40);

                                    for(k=0; k<NUM_BULLET; k++)
                                    {
                                        collision_bullet_enemy(&playerBullet[k], &enemy1[i][j], img_player_bullet, 40, 40);
                                    }
                                }
                                if(mapa[i][j] == 7)
                                {
                                    check_trap(&player, &block[i][j], &enemy1[i][j], 7);
                                    collision_player_enemy(&player, &enemy1[i][j], 40, 40);

                                    for(k=0; k<NUM_BULLET; k++)
                                    {
                                        collision_bullet_enemy(&playerBullet[k], &enemy1[i][j], img_player_bullet, 40, 40);
                                    }
                                }
                                if(mapa[i][j] == 8)
                                {
                                    check_trap(&player, &block[i][j], &enemy1[i][j], 8);
                                    collision_player_enemy(&player, &enemy1[i][j], 40, 40);

                                    for(k=0; k<NUM_BULLET; k++)
                                    {
                                        collision_bullet_enemy(&playerBullet[k], &enemy1[i][j], img_player_bullet, 40, 40);
                                    }
                                }
                                if(mapa[i][j] == 9)
                                {
                                    check_trap(&player, &block[i][j], &enemy1[i][j], 9);
                                    collision_player_enemy(&player, &enemy1[i][j], 40, 40);

                                    for(k=0; k<NUM_BULLET; k++)
                                    {
                                        collision_bullet_enemy(&playerBullet[k], &enemy1[i][j], img_player_bullet, 40, 40);
                                    }
                                }
                            }
                        }
                    }

                    collision_player_wall(&player, &jumping, img_block1);

                    /* ~~Desenha o Background~~ */
                    al_draw_bitmap(img_background0, 0 - cameraX, 0 - cameraY, 0);

                    /* ~~Animação dos inimigos~~ */
                    if(++anim_enemy1.frameCount >= anim_enemy1.frameDelay)
                    {
                        if(++anim_enemy1.curFrame >= anim_enemy1.maxFrame)
                        {
                            anim_enemy1.curFrame = 0;
                        }
                        anim_enemy1.frameCount = 0;
                    }

                    /* ~~Desenha os Blocos/Inimigos~~ */
                    for (i = 0; i<LINHA_MAX; i++)
                    {
                        for(j = 0; j<COLUNA_MAX; j++)
                        {
                            if(mapa[i][j] == 2 && block[i][j].live == true)
                            {
                                al_draw_bitmap(img_block1, block[i][j].x - cameraX, block[i][j].y - cameraY, 0);
                            }
                            if(mapa[i][j] == 3 && block[i][j].live == true)
                            {
                                al_draw_bitmap(img_block2, block[i][j].x - cameraX, block[i][j].y - cameraY, 0);
                            }
                            if(mapa[i][j] == 4 && block[i][j].live == true)
                            {
                                al_draw_bitmap(img_block2, block[i][j].x - cameraX, block[i][j].y - cameraY, 0);
                            }
                            if(mapa[i][j] == 5 && block[i][j].live == true)
                            {
                                al_draw_bitmap(img_block2, block[i][j].x - cameraX, block[i][j].y - cameraY, 0);
                            }
                            if((mapa[i][j] == 6 || mapa[i][j] == 7 || mapa[i][j] == 8) && (enemy1[i][j].direction == -1) && (enemy1[i][j].live == true))
                            {
                                al_draw_bitmap_region(img_enemy1, anim_enemy1.curFrame * anim_enemy1.frameWidth, 0, anim_enemy1.frameWidth, anim_enemy1.frameHeight, enemy1[i][j].x - cameraX, enemy1[i][j].y - cameraY, 0);
                            }
                            if((mapa[i][j] == 6 || mapa[i][j] == 7 || mapa[i][j] == 8) && (enemy1[i][j].direction == 1) && (enemy1[i][j].live == true))
                            {
                                al_draw_bitmap_region(img_enemy1, anim_enemy1.curFrame * anim_enemy1.frameWidth, 0, anim_enemy1.frameWidth, anim_enemy1.frameHeight, enemy1[i][j].x - cameraX, enemy1[i][j].y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                            }
                            if(mapa[i][j] == 9 && enemy1[i][j].live == true)
                            {
                                al_draw_bitmap_region(img_boss1, anim_enemy1.curFrame * anim_enemy1.frameWidth, 0, anim_enemy1.frameWidth, anim_enemy1.frameHeight, enemy1[i][j].x - cameraX, enemy1[i][j].y - cameraY, 0);
                            }
                        }
                    }

                    /* ~~Desenho do player~~ */

                    /* Player parado */
                    if(iddle == true)
                    {
                        if(++immobile.frameCount >= immobile.frameDelay)
                        {
                            if(++immobile.curFrame >= immobile.maxFrame)
                            {
                                immobile.curFrame = 0;
                                iddle = false;
                            }
                            immobile.frameCount = 0;
                        }
                    }
                    if((keys[KEY_SPACE] == false) && (jump == false) && ((!keys[KEY_LEFT] && player.direction == -1) || player.x == 64))
                    {
                        al_draw_bitmap_region(img_player_immobile, immobile.curFrame * immobile.frameWidth, 0, immobile.frameWidth, immobile.frameHeight, player.x - cameraX, player.y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    if((keys[KEY_SPACE] == false) && (jump == false) && ((!keys[KEY_RIGHT] && player.direction == 1) || player.x == WORLD_W - (64-immobile.frameWidth)))
                    {
                        al_draw_bitmap_region(img_player_immobile, immobile.curFrame * immobile.frameWidth, 0, immobile.frameWidth, immobile.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                    }
                    if((keys[KEY_SPACE] == true) && (jump == false) && ((!keys[KEY_LEFT] && player.direction == -1)))
                    {
                        al_draw_bitmap(img_player_immobile_shoot, player.x - cameraX, player.y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                    }
                    if((keys[KEY_SPACE] == true) && (jump == false) && ((!keys[KEY_RIGHT] && player.direction == 1)))
                    {
                        al_draw_bitmap(img_player_immobile_shoot, player.x - cameraX, player.y - cameraY, 0);
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

                    if(keys[KEY_SPACE] == false)
                    {
                        if(jump == false && keys[KEY_LEFT] && player.direction == -1)
                        {
                            al_draw_bitmap_region(img_player_walking, walking.curFrame * walking.frameWidth, 0, walking.frameWidth, walking.frameHeight, player.x - cameraX, player.y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                        }
                        if(jump == false && keys[KEY_RIGHT] && player.direction == 1)
                        {
                            al_draw_bitmap_region(img_player_walking, walking.curFrame * walking.frameWidth, 0, walking.frameWidth, walking.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                        }
                    }
                    if(keys[KEY_SPACE] == true)
                    {
                        if(jump == false && keys[KEY_LEFT] && player.direction == -1)
                        {
                            al_draw_bitmap_region(img_player_walking_shoot, walking.curFrame * walking.frameWidth, 0, walking.frameWidth, walking.frameHeight, player.x - cameraX, player.y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                        }
                        if(jump == false && keys[KEY_RIGHT] && player.direction == 1)
                        {
                            al_draw_bitmap_region(img_player_walking_shoot, walking.curFrame * walking.frameWidth, 0, walking.frameWidth, walking.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                        }
                    }

                    /* Player pulando */
                    if(jump == true)
                    {
                        if(++jumping.frameCount >= jumping.frameDelay)
                        {
                            if(++jumping.curFrame >= jumping.maxFrame)
                            {
                                jumping.curFrame = 0;
                            }
                            jumping.frameCount = 0;
                        }

                        if(keys[KEY_SPACE] == false)
                        {
                            if(player.direction == -1)
                            {
                                al_draw_bitmap_region(img_player_jump, jumping.curFrame * jumping.frameWidth, 0, jumping.frameWidth, jumping.frameHeight, player.x - cameraX, player.y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                            }
                            if(player.direction == 1)
                            {
                                al_draw_bitmap_region(img_player_jump, jumping.curFrame * jumping.frameWidth, 0, jumping.frameWidth, jumping.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                            }
                        }
                        else if(keys[KEY_SPACE] == true)
                        {
                            if(player.direction == -1)
                            {
                                al_draw_bitmap_region(img_player_jump_shoot, jumping.curFrame * jumping.frameWidth, 0, jumping.frameWidth, jumping.frameHeight, player.x - cameraX, player.y - cameraY, ALLEGRO_FLIP_HORIZONTAL);
                            }
                            if(player.direction == 1)
                            {
                                al_draw_bitmap_region(img_player_jump_shoot, jumping.curFrame * jumping.frameWidth, 0, jumping.frameWidth, jumping.frameHeight, player.x - cameraX, player.y - cameraY, 0);
                            }
                        }

                    }

                    /* ~~Desenho dos projeteis~~ */
                    for(i=0; i<NUM_BULLET; i++)
                    {
                        if(playerBullet[i].live)
                        {
                            al_draw_bitmap(img_player_bullet, playerBullet[i].x - cameraX, playerBullet[i].y - cameraY, 0);
                        }
                        if(enemyBullet[i].live)
                        {
                            al_draw_bitmap(img_enemy_bullet, enemyBullet[i].x - cameraX, enemyBullet[i].y - cameraY, 0);
                        }
                    }

                    /* Pontuacao e Porcentagem de Vida */
                    al_draw_bitmap(img_heart, 0, 0, 0);
                    al_draw_textf(fonte16, al_map_rgb(255, 255, 255), 20, 0, ALLEGRO_ALIGN_LEFT, ("%03d"), player.life);

                    al_draw_bitmap(img_medal, 64, 0, 0);
                    al_draw_textf(fonte16, al_map_rgb(255, 255, 255), 80, 0, ALLEGRO_ALIGN_LEFT, ("%04d"), scores);

                    al_draw_bitmap(img_clock, 144, 0, 0);
                    al_draw_textf(fonte16, al_map_rgb(255, 255, 255), 160, 0, ALLEGRO_ALIGN_LEFT, ("%03d"), stopwatch);
                }

                /* Termino da Fase */
                if(enemyKilled == ENEMY_MAX) /* You Win! */
                {
                    scores = scores + (25 * stopwatch) + (10*player.life);
                    gamestate = 2;
                }
                if(player.life <= 0 || stopwatch == 0) /* Game Over! */
                {
                    gamestate = 3;
                }

                /* Troca o display */
                al_flip_display();
            }
            break;
        case 2:
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) /* Faz com que o jogo feche ao clicar no botao "X" do display */
            {
                quit = true;
            }

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) /* Faz com que o jogo feche ao pressionar esc */
                {
                    quit = true;
                }
            }
            if(ev.type == ALLEGRO_EVENT_TIMER) /* Mostrar mensagens na tela */
            {
                if(ev.timer.source == timer_60)
                {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_draw_bitmap(img_you_win, 0, 0, 0);
                    al_draw_textf(fonte16, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H - 48, ALLEGRO_ALIGN_CENTER, "Seus pontos: %d", scores);
                    al_draw_textf(fonte16, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H - 32, ALLEGRO_ALIGN_CENTER, "Pressione Esc para sair");
                    al_flip_display();
                }
            }
            break;

        case 3:
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) /* Faz com que o jogo feche ao clicar no botao "X" do display */
            {
                quit = true;
            }

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) /* Faz com que o jogo feche ao pressionar esc */
                {
                    quit = true;
                }
            }
            if(ev.type == ALLEGRO_EVENT_TIMER) /* Mostrar mensagens na tela */
            {
                if(ev.timer.source == timer_60)
                {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_draw_bitmap(img_game_over, 0, 0, 0);
                    al_draw_textf(fonte16, al_map_rgb(255, 0, 0), SCREEN_W/2, SCREEN_H - 48, ALLEGRO_ALIGN_CENTER, "Seus pontos: %d", scores);
                    al_draw_textf(fonte16, al_map_rgb(255, 0, 0), SCREEN_W/2, SCREEN_H - 32, ALLEGRO_ALIGN_CENTER, "Pressione Esc para sair");
                    al_flip_display();
                }
            }
            break;
        }
    }
    /* Destruindo as variaveis */
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer_0p2);
    al_destroy_timer(timer_1);
    al_destroy_timer(timer_60);

    al_destroy_bitmap(img_home_screen);
    al_destroy_bitmap(img_dica_h1n1);
    al_destroy_bitmap(img_background0);

    al_destroy_bitmap(img_heart);
    al_destroy_bitmap(img_medal);
    al_destroy_bitmap(img_clock);
    al_destroy_bitmap(img_block1);
    al_destroy_bitmap(img_block2);

    al_destroy_bitmap(img_player_walking);
    al_destroy_bitmap(img_player_walking_shoot);
    al_destroy_bitmap(img_player_immobile);
    al_destroy_bitmap(img_player_immobile_shoot);
    al_destroy_bitmap(img_player_jump);
    al_destroy_bitmap(img_player_jump_shoot);
    al_destroy_bitmap(img_player_bullet);

    al_destroy_bitmap(img_enemy1);
    al_destroy_bitmap(img_boss1);
    al_destroy_bitmap(img_enemy_bullet);

    al_destroy_sample(spl_theme);
    al_destroy_sample(spl_playerShoot);
    al_destroy_sample(spl_mlk);

    return 0;
}
