#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include "enemy.h"

BalleEnnemie balles_ene[MAX_BALLES_ENE];

static ALLEGRO_BITMAP *sprite_ennemi = NULL;

void ennemis_init(Enemy ennemis[]) {
    int i;
    for (i = 0; i < MAX_ENNEMIS; i++) {
        ennemis[i].active = 0;
        ennemis[i].timer_tir = 0;
    }

    if (sprite_ennemi == NULL)
        sprite_ennemi = al_load_bitmap("Image extraterestre.png");
}

void ennemis_spawn(Enemy ennemis[]) {
    int i;
    for (i = 0; i < MAX_ENNEMIS; i++) {
        if (!ennemis[i].active) {
            ennemis[i].active = 1;
            ennemis[i].x = 810;
            ennemis[i].y = 50 + rand() % 500;
            ennemis[i].timer_tir = 60 + rand() % 120;
            return;
        }
    }
}

void ennemis_update(Enemy ennemis[]) {
    int i, j;
    for (i = 0; i < MAX_ENNEMIS; i++) {
        if (!ennemis[i].active) continue;

        ennemis[i].x -= 2;

        if (ennemis[i].x < -ENEMY_W) {
            ennemis[i].active = 0;
            continue;
        }

        ennemis[i].timer_tir--;
        if (ennemis[i].timer_tir <= 0) {
            ennemis[i].timer_tir = 90 + rand() % 60;

            for (j = 0; j < MAX_BALLES_ENE; j++) {
                if (!balles_ene[j].active) {
                    balles_ene[j].active = 1;
                    balles_ene[j].x = ennemis[i].x;
                    balles_ene[j].y = ennemis[i].y + ENEMY_H / 2;
                    break;
                }
            }
        }
    }
}

void ennemis_draw(Enemy ennemis[]) {
    int i;
    for (i = 0; i < MAX_ENNEMIS; i++) {
        if (!ennemis[i].active) continue;

        if (sprite_ennemi) {
            al_draw_scaled_bitmap(
                sprite_ennemi,
                0, 0,
                al_get_bitmap_width(sprite_ennemi),
                al_get_bitmap_height(sprite_ennemi),
                ennemis[i].x, ennemis[i].y,
                ENEMY_W, ENEMY_H,
                0
            );
        } else {
            al_draw_filled_rectangle(
                ennemis[i].x, ennemis[i].y,
                ennemis[i].x + ENEMY_W, ennemis[i].y + ENEMY_H,
                al_map_rgb(255, 60, 60));
        }
    }
}

void ennemis_destroy(void) {
    if (sprite_ennemi) {
        al_destroy_bitmap(sprite_ennemi);
        sprite_ennemi = NULL;
    }
}


void balles_ene_init(void) {
    int i;
    for (i = 0; i < MAX_BALLES_ENE; i++)
        balles_ene[i].active = 0;
}

void balles_ene_update(void) {
    int i;
    for (i = 0; i < MAX_BALLES_ENE; i++) {
        if (!balles_ene[i].active) continue;
        balles_ene[i].x -= 3; 
        if (balles_ene[i].x < -10)
            balles_ene[i].active = 0;
    }
}

void balles_ene_draw(void) {
    int i;
    for (i = 0; i < MAX_BALLES_ENE; i++) {
        if (balles_ene[i].active)
            al_draw_filled_rectangle(
                balles_ene[i].x, balles_ene[i].y,
                balles_ene[i].x + 10, balles_ene[i].y + 4,
                al_map_rgb(255, 140, 0)); 
    }
}
