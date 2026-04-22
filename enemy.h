#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>

/* Dimensions et nombre max d'ennemis */
#define ENEMY_W           50
#define ENEMY_H           50
#define MAX_ENNEMIS       8
#define MAX_BALLES_ENE    16  /* balles tirees par les ennemis */

/* Une balle ennemie : se deplace vers la gauche */
typedef struct {
    float x, y;
    int active;
} BalleEnnemie;

/* Un ennemi : position, drapeau actif, compteur de tir */
typedef struct {
    float x, y;
    int active;
    int timer_tir;  /* decremente chaque frame, tire quand il atteint 0 */
} Enemy;

/* Tableau global des balles ennemies (gere dans enemy.c) */
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
