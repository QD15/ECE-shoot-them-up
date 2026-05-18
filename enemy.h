#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#define ENEMY_W        50
#define ENEMY_H        50
#define MAX_ENNEMIS     8
#define MAX_BALLES_ENE 16

typedef struct {
    float x, y;
    int active;
} BalleEnnemie;

typedef struct {
    float x, y;
    int active;
    int timer_tir;
} Enemy;

extern BalleEnnemie balles_ene[MAX_BALLES_ENE];

void ennemis_init(Enemy ennemis[]);
void ennemis_spawn(Enemy ennemis[]);
void ennemis_update(Enemy ennemis[]);
void ennemis_draw(Enemy ennemis[]);
void ennemis_destroy(void);

void balles_ene_init(void);
void balles_ene_update(void);
void balles_ene_draw(void);

#define BOSS_W        150//taille
#define BOSS_H        150//taille
#define BOSS_HP_MAX    20//vie
#define MAX_BOSS_BALLES 40

typedef struct {
    float x, y;
    float vx, vy;   // direction de la balle
    int active;
} BossBalle;

typedef struct {
    float x, y;
    float vitesse_y;
    int active;
    int hp;
    int timer_tir;
    ALLEGRO_BITMAP *sprite;
} Boss;

extern Boss       boss;
extern BossBalle  boss_balles[MAX_BOSS_BALLES];

void boss_init(void);
void boss_update(void);
void boss_draw(void);
void boss_destroy(void);

void boss_balles_init(void);
void boss_balles_update(void);
void boss_balles_draw(void);

#endif
