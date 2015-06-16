/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| Pedro dos Santos Guerreiro e Thyago Augusto Reboledo                                                                ||
|| projetoHeader.h                                                                                                     ||
|| Versão 1.0.0 (30/05/2015)                                                                                           ||
|| Versão 1.0.1 (02/06/2015)                                                                                           ||
|| Versão 1.0.2 (08/06/2015)                                                                                           ||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

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

const int LINHA_MAX = 15;
const int COLUNA_MAX = 20;

bool jump = false;
int gravity = 10;
float force = 0;

const int PLAYERBULLET_MAX = 3;
const int ENEMYBULLET_MAX = 3;
const int ENEMY_MAX = 20;
int enemyBulletCount = 0;
int enemyBulletID = 0;

int playerDirection = -1; /* -1 = Direita, 1 = Esquerda */
int playerLife = 100;
int scores = 0;

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
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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

/* ------------------------------Funções------------------------------ */

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

void collision_player_wall(s_object *player, ALLEGRO_BITMAP* img_block)
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
    }
}

void collision_player_tiles(s_object *player, s_object *block, ALLEGRO_BITMAP* img_block)
{
    if(player->x < block->x + al_get_bitmap_width(img_block) && block->x < player->x + 40 && player->y < block->y + al_get_bitmap_height(img_block) && block->y < player->y + 40 && block->live && playerDirection == 1)
    {
        player->x = block->x + al_get_bitmap_width(img_block);
    }
    else if(player->x < block->x + al_get_bitmap_width(img_block) && block->x < player->x + 40 && player->y < block->y + al_get_bitmap_height(img_block) && block->y < player->y + 40 && block->live && playerDirection == -1)
    {
        player->x = block->x - 40;
    }
}
