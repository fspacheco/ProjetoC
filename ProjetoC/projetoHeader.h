/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||Pedro dos Santos Guerreiro                                                                                           ||
||Thyago Augusto Reboledo                                                                                              ||
|| projetoHeader.h                                                                                                     ||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*# -> os comentarios que tiverem isso ainda estao em desenvolvimento */

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

const int LINHA_MAX = 15;
const int COLUNA_MAX = 20;

bool jump = false;
int gravity = 10;
float force = 0;

const int ENEMY_MAX = 20;
const int NUM_BULLET = 3;

int playerDirection = 1; /* -1 = Esquerda, 1 = Direita */
int playerLife = 100;
int scores = 0;

int i=0;
int j=0;
int k=0;

enum
{
    KEY_RIGHT,
    KEY_LEFT,
    KEY_UP,
    KEY_MAX
};

bool keys[KEY_MAX];

int mapa[15][20] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
    int x;
    int y;
    int ID;
    int speed;
    int direction;

    bool live;

    ALLEGRO_BITMAP* image;
} s_bullet;

typedef struct
{
    float x;
    float y;

    int width;
    int height;

    ALLEGRO_BITMAP* image;
} s_background;

typedef struct
{
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
} s_animation;

/* ------------------------------Funcoes------------------------------ */

/* ~~~~~~~~~~~~~~~~~~~~Background~~~~~~~~~~~~~~~~~~~~ */

void drawnBackground(s_background *bkg)
{
    al_draw_bitmap(bkg->image, bkg->x-cameraX, bkg->y-cameraY, 0);

    if(bkg->x + bkg->width < WORLD_W)
    {
        al_draw_bitmap(bkg->image, bkg->x-cameraX + bkg->width, bkg->y-cameraY, 0);
    }
}
/* ~~~~~~~~~~~~~~~~~~~~Shoots~~~~~~~~~~~~~~~~~~~~ */

void playerShoot(s_object *player, s_bullet *bullet, ALLEGRO_SAMPLE_INSTANCE *instance_spl)
{
    if(!bullet->live)
    {
        if(playerDirection == -1)
        {
            bullet->live = true;
            bullet->x = player->x - 8;
            bullet->y = player->y + 20;
            al_play_sample_instance(instance_spl);
        }
        if(playerDirection == 1)
        {
            bullet->live = true;
            bullet->x = player->x + 40;
            bullet->y = player->y + 20;
            al_play_sample_instance(instance_spl);
        }

    }
}

/* ~~~~~~~~~~~~~~~~~~~~Colisoes~~~~~~~~~~~~~~~~~~~~ */

void collision_player_wall(s_object *player, s_animation *jumping, ALLEGRO_BITMAP* img_block)
{
    if (player->x < al_get_bitmap_width(img_block))
    {
        player->x = al_get_bitmap_width(img_block);
    }
    if (player->x > WORLD_W - (al_get_bitmap_width(img_block)+40))
    {
        player->x = WORLD_W - (al_get_bitmap_width(img_block)+40);
    }
    if (player->y < al_get_bitmap_height(img_block))
    {
        player->y = al_get_bitmap_height(img_block);
    }
    if (player->y > WORLD_H - (al_get_bitmap_height(img_block)+40))
    {
        player->y = WORLD_H - (al_get_bitmap_height(img_block)+40);
        jump = false;
        jumping->curFrame = 0;
    }
}

void collision_bullet_tiles(s_bullet *bullet, s_object *block, ALLEGRO_BITMAP* img_bullet,ALLEGRO_BITMAP* img_block)
{
    if(bullet->x < block->x + al_get_bitmap_width(img_block) && block->x < bullet->x + al_get_bitmap_width(img_bullet) && bullet->y < block->y + al_get_bitmap_height(img_block) && block->y < bullet->y + al_get_bitmap_height(img_bullet) && block->live && bullet->live)
    {
        bullet->live = false;
    }
    if(bullet->x > SCREEN_W + cameraX)
    {
        bullet->live = false;
    }
    if(bullet->x < 0 + cameraX)
    {
        bullet->live = false;
    }
}
