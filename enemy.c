#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <math.h>
#include "enemy.h"

/* ============================================================
   ENNEMIS NORMAUX
   ============================================================ */
BalleEnnemie balles_ene[MAX_BALLES_ENE];
static ALLEGRO_BITMAP *sprite_ennemi = NULL;

void ennemis_init(Enemy ennemis[]) {
    int i;
    for (i = 0; i < MAX_ENNEMIS; i++) {
        ennemis[i].active    = 0;
        ennemis[i].timer_tir = 0;
    }
    if (sprite_ennemi == NULL)
        sprite_ennemi = al_load_bitmap("Image extraterestre.png");
}

void ennemis_spawn(Enemy ennemis[]) {
    int i;
    for (i = 0; i < MAX_ENNEMIS; i++) {
        if (!ennemis[i].active) {
            ennemis[i].active    = 1;
            ennemis[i].x        = 810;
            ennemis[i].y        = 50 + rand() % 500;
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
                    balles_ene[j].x     = ennemis[i].x;
                    balles_ene[j].y     = ennemis[i].y + ENEMY_H / 2;
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
            al_draw_scaled_bitmap(sprite_ennemi,
                0, 0,
                al_get_bitmap_width(sprite_ennemi),
                al_get_bitmap_height(sprite_ennemi),
                ennemis[i].x, ennemis[i].y,
                ENEMY_W, ENEMY_H, 0);
        } else {
            al_draw_filled_rectangle(ennemis[i].x, ennemis[i].y,
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
        if (!balles_ene[i].active) continue;
        al_draw_filled_rectangle(
            balles_ene[i].x, balles_ene[i].y,
            balles_ene[i].x + 10, balles_ene[i].y + 4,
            al_map_rgb(255, 140, 0));
    }
}
Boss      boss;
BossBalle boss_balles[MAX_BOSS_BALLES];

void boss_init(void) {
    int i;

    boss.x         = 820;          /* entre depuis la droite */
    boss.y         = 225;          /* centre vertical        */
    boss.vitesse_y = 1.5f;         /* rebondit haut/bas      */
    boss.hp        = BOSS_HP_MAX;
    boss.timer_tir = 60;           /* premier tir apres 1 s  */
    boss.active    = 1;
    boss.sprite    = al_load_bitmap("Boss.png");

    for (i = 0; i < MAX_BOSS_BALLES; i++)
        boss_balles[i].active = 0;
}

/* Tire une rafale de 5 balles en eventail vers la gauche */
static void boss_tirer(void) {
    /* Angles en degres : -40 -20 0 +20 +40 */
    float angles[5] = { -40.0f, -20.0f, 0.0f, 20.0f, 40.0f };
    float vitesse   = 4.5f;
    int k, j;

    for (k = 0; k < 5; k++) {
        float rad = angles[k] * 3.14159f / 180.0f;
        float vx  = -vitesse * cosf(rad);  /* vers la gauche       */
        float vy  =  vitesse * sinf(rad);  /* composante verticale */

        /* Chercher une balle inactive */
        for (j = 0; j < MAX_BOSS_BALLES; j++) {
            if (!boss_balles[j].active) {
                boss_balles[j].active = 1;
                boss_balles[j].x     = boss.x;
                boss_balles[j].y     = boss.y + BOSS_H / 2;
                boss_balles[j].vx    = vx;
                boss_balles[j].vy    = vy;
                break;
            }
        }
    }
}

void boss_update(void) {
    if (!boss.active) return;

    /* Entree progressive depuis la droite */
    if (boss.x > 620)
        boss.x -= 2.0f;

    /* Mouvement vertical avec rebond */
    boss.y += boss.vitesse_y;
    if (boss.y < 20 || boss.y > 600 - BOSS_H - 20)
        boss.vitesse_y = -boss.vitesse_y;

    /* Tir en rafale */
    boss.timer_tir--;
    if (boss.timer_tir <= 0) {
        boss.timer_tir = 50 + rand() % 30;  /* rafale toutes les 50-80 frames */
        boss_tirer();
    }
}

void boss_draw(void) {
    if (!boss.active) return;

    if (boss.sprite) {
        al_draw_scaled_bitmap(boss.sprite,
            0, 0,
            al_get_bitmap_width(boss.sprite),
            al_get_bitmap_height(boss.sprite),
            boss.x, boss.y,
            BOSS_W, BOSS_H, 0);
    } else {
        /* Fallback rectangle violet si PNG absent */
        al_draw_filled_rectangle(boss.x, boss.y,
            boss.x + BOSS_W, boss.y + BOSS_H,
            al_map_rgb(180, 0, 220));
    }
}

void boss_destroy(void) {
    if (boss.sprite) {
        al_destroy_bitmap(boss.sprite);
        boss.sprite = NULL;
    }
    boss.active = 0;
}

void boss_balles_update(void) {
    int i;
    for (i = 0; i < MAX_BOSS_BALLES; i++) {
        if (!boss_balles[i].active) continue;
        boss_balles[i].x += boss_balles[i].vx;
        boss_balles[i].y += boss_balles[i].vy;
        /* Desactiver si hors ecran */
        if (boss_balles[i].x < -10 || boss_balles[i].x > 810 ||
            boss_balles[i].y < -10 || boss_balles[i].y > 610)
            boss_balles[i].active = 0;
    }
}

void boss_balles_draw(void) {
    int i;
    for (i = 0; i < MAX_BOSS_BALLES; i++) {
        if (!boss_balles[i].active) continue;
        /* Balle du boss : cercle rouge plus gros */
        al_draw_filled_circle(
            boss_balles[i].x, boss_balles[i].y,
            7, al_map_rgb(255, 40, 40));
        /* Contour pour mieux les voir */
        al_draw_circle(
            boss_balles[i].x, boss_balles[i].y,
            7, al_map_rgb(255, 150, 0), 1.5f);
    }
}
