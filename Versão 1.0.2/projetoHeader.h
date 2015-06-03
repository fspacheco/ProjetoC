/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| Pedro dos Santos Guerreiro e Thyago Augusto Reboledo                                                                     ||
|| projetoHeader.h                                                                                                          ||
|| Versão 1.0.0 (30/05/2015)                                                                                                  ||
|| Versão 1.0.1 (02/06/2015)                                                                                                  ||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*# -> os comentários que tiverem isso ainda estão em desenvolvimento */

#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/* ------------------------------Constantes------------------------------ */
const int WORLD_W = 1280;
const int WORLD_H = 720;

const int SCREEN_W = 640;
const int SCREEN_H = 400;

int cameraX = 0;
int cameraY = 0;

const int BLOCK_MAX = 300;

const int PLAYERBULLET_MAX = 3;
const int ENEMYBULLET_MAX = 3;
const int ENEMY_MAX = 20;
/*#int enemyBulletCount = 0;*/
/*#int enemyBulletID = 0;*/

const int maxFrame = 8;
int curFrame = 0;
int frameCount = 0;
int frameDelay = 5;

/*int playerLife = 100;*/
/*#int scores = 0;*/

enum
{
    KEY_RIGHT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_ESC,
    KEY_MAX
};

bool keys[KEY_MAX];

int mapa[20][15] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

/* ------------------------------Estruturas------------------------------ */

typedef struct
{
    int x;
    int y;
    bool live;
} s_object;

typedef struct
{
    float x;
    float y;

    int width;
    int height;

    ALLEGRO_BITMAP* image;
} background;

/* ------------------------------Funções------------------------------ */

/* ~~~~~~~~~~~~~~~~~~~~Background~~~~~~~~~~~~~~~~~~~~ */

void initBackground(background *back, float x, float y, int width, int height, ALLEGRO_BITMAP* image)
{
    back->x = x;
    back->y = y;
    back->width = width;
    back->height = height;
    back->image = image;
}

void drawnBackground(background *back)
{
    al_draw_bitmap(back->image, back->x-cameraX, back->y-cameraY, 0);

    if(back->x + back->width < WORLD_W)
    {
        al_draw_bitmap(back->image, back->x-cameraX + back->width, back->y-cameraY, 0);
    }
}
/* ~~~~~~~~~~~~~~~~~~~~Shoots~~~~~~~~~~~~~~~~~~~~ */

void playerShoot(s_object *player, s_object *bullet, ALLEGRO_SAMPLE *spl)
{
    if(!bullet->live)
    {
        bullet->live = true;
        bullet->x = player->x + 12;
        bullet->y = player->y - 8;
        al_play_sample(spl, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}

/* ~~~~~~~~~~~~~~~~~~~~Colisões~~~~~~~~~~~~~~~~~~~~ */

void collision_player_wall(s_object *player, ALLEGRO_BITMAP* img_player)
{
    if (player->x < 92)
    {
        player->x = 92;
    }
    if (player->x > WORLD_W-102)
    {
        player->x = WORLD_W-102;
    }
    if (player->y < 68)
    {
        player->y = 68;
    }
    if (player->y > WORLD_H - 98)
    {
        player->y = WORLD_H - 98;
    }
}
