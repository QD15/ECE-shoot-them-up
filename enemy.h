#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>

#define ENEMY_W           50
#define ENEMY_H           50
#define MAX_ENNEMIS       8
#define MAX_BALLES_ENE    16  /* balles tirees par les ennemis */

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

#endif
