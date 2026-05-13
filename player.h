#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

/* Dimensions hitbox et balles */
#define PLAYER_W    80
#define PLAYER_H    50
#define MAX_BALLES  10

/* Une balle : position et drapeau actif */
typedef struct {
    float x, y;
    int active;
} Balle;

/* Le joueur */
typedef struct {
    float x, y;
    float vitesse;
    int cooldown_tir;
    Balle balles[MAX_BALLES];
    ALLEGRO_BITMAP *sprite;  /* image du vaisseau */
} Player;

void player_init(Player *p);
void player_update(Player *p, int up, int down, int left, int right, int tir);
void player_draw(Player *p);
void player_destroy(Player *p);

#endif
