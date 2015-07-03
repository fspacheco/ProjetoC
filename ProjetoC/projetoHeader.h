/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||Pedro dos Santos Guerreiro                                                                                           ||
||Thyago Augusto Reboledo                                                                                              ||
|| projetoHeader.h                                                                                                     ||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*# -> os comentarios que tiverem isso ainda estao em desenvolvimento */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

const int LINHA_MAX = 45;
const int COLUNA_MAX = 20;

int change_bkg = 0;
int stopwatch = 120;

bool iddle = false;
bool jump = false;
bool falling = false;

int gravity = 10;
float force = 0;

const int ENEMY_MAX = 35;
const int NUM_BULLET = 3;

int trap = 0;
int chance_enemy_shoot = 0;

int enemyKilled = 0;
int scores = 0;

int i=0;
int j=0;
int k=0;

enum
{
    KEY_RIGHT,
    KEY_LEFT,
    KEY_UP,
    KEY_SPACE,
    KEY_MAX
};

bool keys[KEY_MAX];

int mapa[45][20] =
{
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0,2},
    {2,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,2},
    {2,0,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0,2},
    {2,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,6,0,0,0,6,0,0,0,6,0,0,5,6,0,2},
    {2,0,0,0,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,2},
    {2,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,5,2},
    {2,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,1,0,3,7,0,7,0,7,0,7,9,7,0,7,0,0,0,0,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
};

/* ------------------------------Estruturas------------------------------ */

typedef struct
{
    int x;
    int y;
    int direction;

    float speed;

    int life;
    bool live;
} s_object;

typedef struct
{
    int x;
    int y;
    int ID;
    int speed;
    int direction; /* -1 = Esquerda, 1 = Direita */

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
        if(player->direction == -1)
        {
            bullet->live = true;
            bullet->x = player->x - 8;
            bullet->y = player->y + 10;
            al_play_sample_instance(instance_spl);
        }
        if(player->direction == 1)
        {
            bullet->live = true;
            bullet->x = player->x + 40;
            bullet->y = player->y + 10;
            al_play_sample_instance(instance_spl);
        }
    }
}

void enemyShoot(s_object *player, s_object *enemy, s_bullet *bullet)
{
    if(player->x > enemy->x + 40)
    {
        enemy->direction = 1;
    }
    if(player->x + 40 < enemy->x)
    {
        enemy->direction = -1;
    }
    if(player->y >= enemy->y && player->y + 35 <= enemy->y + 45 && enemy->live == true && bullet->live == false)
    {
        if(chance_enemy_shoot == 1 && enemy->direction == -1)
        {
            bullet->live = true;
            bullet->x = enemy->x - 8;
            bullet->y = enemy->y + 10;
            printf("%d", enemy->direction);
        }
        if(chance_enemy_shoot == 1 && enemy->direction == 1)
        {
            bullet->live = true;
            bullet->x = enemy->x + 40;
            bullet->y = enemy->y + 10;
            printf("%d", enemy->direction);
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~Colisoes~~~~~~~~~~~~~~~~~~~~ */

void collision_player_wall(s_object *player, s_animation *jumping, ALLEGRO_BITMAP* img_block)
{
    if (player->x < al_get_bitmap_width(img_block))
    {
        player->x = al_get_bitmap_width(img_block);
        keys[KEY_LEFT] = false;
    }
    if (player->x > WORLD_W - (al_get_bitmap_width(img_block)+40))
    {
        player->x = WORLD_W - (al_get_bitmap_width(img_block)+40);
        keys[KEY_RIGHT] = false;
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

void collision_player_tiles(s_object *player, s_object *block, s_animation *jumping, ALLEGRO_BITMAP* img_block)
{
    if(player->y < block->y + al_get_bitmap_height(img_block) && player->y > block->y && player->x + 35 > block->x && player->x < block->x + al_get_bitmap_width(img_block) - 5)
    {
        player->y = block->y + al_get_bitmap_height(img_block);
    }
    if(player->y + 40 > block->y && player->y + 40 < block->y + al_get_bitmap_height(img_block) && player->x + 35 > block->x && player->x < block->x + al_get_bitmap_width(img_block) - 5)
    {
        player->y = block->y - 40;
        jump = false;
        jumping->curFrame = 0;
    }
    if (player->x + 40 > block->x && player->x + 40 < block->x + al_get_bitmap_width(img_block) && player->y + 40 > block->y && player->y < block->y + al_get_bitmap_height(img_block))
    {
        player->x = block->x - 40;
    }
    if(player->x > block->x && player->x + 5 < block->x + al_get_bitmap_width(img_block) && player->y + 40 > block->y && player->y < block->y + al_get_bitmap_height(img_block))
    {
        player->x = block->x + al_get_bitmap_width(img_block) - 5;
    }
}

void collision_player_enemy(s_object *player, s_object *enemy, int enemy_width, int enemy_height)
{
    if(enemy->live == true)
    {
        if (player->x + enemy_width > enemy->x && player->x + 40 < enemy->x + enemy_width && player->y + enemy_width > enemy->y && player->y < enemy->y + enemy_height)
        {
            player->x = enemy->x - 40;
            player->life--;

        }
        if(player->x > enemy->x && player->x < enemy->x + enemy_width && player->y + 40 > enemy->y && player->y < enemy->y + enemy_width)
        {
            player->x = enemy->x + enemy_width ;
            player->life--;
        }
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

void collision_bullet_enemy(s_bullet *bullet, s_object *enemy, ALLEGRO_BITMAP* img_bullet, int enemy_width, int enemy_height)
{
    if(bullet->x < enemy->x + enemy_width && enemy->x < bullet->x + al_get_bitmap_width(img_bullet) && bullet->y < enemy->y + enemy_height && enemy->y < bullet->y + al_get_bitmap_height(img_bullet) && enemy->live && bullet->live)
    {
        enemy->life--;
        if(enemy->life <= 0)
        {
            enemyKilled++;
            enemy->live = false;
            if(enemyKilled<=34)
            {
                scores+=10;
            }
            if(enemyKilled==35)
            {
                scores+=100;
            }
        }
        bullet->live = false;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~Armadilhas~~~~~~~~~~~~~~~~~~~~ */
void check_trap(s_object *player, s_object *block, s_object *enemy, int num_mapa)
{
    if(enemyKilled == 4)
    {
        trap = 1;
    }
    if(enemyKilled == 10)
    {
        trap = 2;
    }
    if(enemyKilled == 34)
    {
        trap = 3;
    }
    /* Blocos */
    if(trap == 1 && num_mapa == 4)
    {
        block->live = false;
        trap = 0;
    }
    if(trap == 2 && num_mapa == 5)
    {
        block->live = true;
        trap = 0;
    }
    /* Inimigos */
    if(trap == 1 && num_mapa == 7)
    {
        enemy->live = true;
        trap = 0;
    }
    if(trap == 2 && num_mapa == 8)
    {
        enemy->live = true;
        trap = 0;
    }
    if(trap == 3 && num_mapa == 9)
    {
        enemy->live = true;
        trap = 0;
    }
}
