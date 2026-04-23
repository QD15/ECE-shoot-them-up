#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

void player_init(Player *p) {
    int i;
    p->x = 100;
    p->y = 300;
    p->vitesse = 4;
    p->cooldown_tir = 0;
    for (i = 0; i < MAX_BALLES; i++)
        p->balles[i].active = 0;

    p->sprite = al_load_bitmap("pngegg.png");
}

void player_update(Player *p, int up, int down, int left, int right, int tir) {
    int i;

    if (up)    p->y -= p->vitesse;
    if (down)  p->y += p->vitesse;
    if (left)  p->x -= p->vitesse;
    if (right) p->x += p->vitesse;

    if (p->x < 0)              p->x = 0;
    if (p->x > 800 - PLAYER_W) p->x = 800 - PLAYER_W;
    if (p->y < 0)              p->y = 0;
    if (p->y > 600 - PLAYER_H) p->y = 600 - PLAYER_H;

    if (p->cooldown_tir > 0) p->cooldown_tir--;

    if (tir && p->cooldown_tir == 0) {
        for (i = 0; i < MAX_BALLES; i++) {
            if (!p->balles[i].active) {
                p->balles[i].active = 1;
                p->balles[i].x = p->x + PLAYER_W;
                p->balles[i].y = p->y + PLAYER_H / 2;
                p->cooldown_tir = 15;
                break;
            }
        }
    }

    for (i = 0; i < MAX_BALLES; i++) {
        if (p->balles[i].active) {
            p->balles[i].x += 8;
            if (p->balles[i].x > 800)
                p->balles[i].active = 0;
        }
    }
}

void player_draw(Player *p) {
    int i;

    if (p->sprite) {
        al_draw_scaled_bitmap(
            p->sprite,
            0, 0,
            al_get_bitmap_width(p->sprite),
            al_get_bitmap_height(p->sprite),
            p->x, p->y,
            PLAYER_W, PLAYER_H,
            0
        );
    } else {
        al_draw_filled_rectangle(p->x, p->y, p->x + PLAYER_W, p->y + PLAYER_H,
            al_map_rgb(100, 200, 255));
    }

    for (i = 0; i < MAX_BALLES; i++) {
        if (p->balles[i].active)
            al_draw_filled_rectangle(
                p->balles[i].x, p->balles[i].y,
                p->balles[i].x + 10, p->balles[i].y + 4,
                al_map_rgb(0, 255, 80));
    }
}

void player_destroy(Player *p) {
    if (p->sprite) {
        al_destroy_bitmap(p->sprite);
        p->sprite = NULL;
    }
}

