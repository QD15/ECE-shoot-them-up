#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "input.h"


#define ETAT_MENU 0
#define ETAT_JEU  1
#define ETAT_FIN  2

static Player player;
static Enemy ennemis[MAX_ENNEMIS];
static ALLEGRO_FONT *font;

static int etat;
static int score;
static int frame;
static int intervalle_spawn; 


void game_init(void) {
    font = al_create_builtin_font();
    etat = ETAT_MENU;
    score = 0;
    frame = 0;
    intervalle_spawn = 90;
    player_init(&player);
    ennemis_init(ennemis);
    balles_ene_init();
}

/* ---------- Detection de collision (AABB) ----------
   On verifie si deux rectangles se chevauchent.
   dx = ecart horizontal entre balle et ennemi.
   Si dx est compris entre 0 et ENEMY_W, il y a overlap horizontal.
   Idem pour dy en vertical.
*/
static void verifier_collisions(void) {
    int i, j;
    float dx, dy;

    for (i = 0; i < MAX_BALLES; i++) {
        if (!player.balles[i].active) continue;

        for (j = 0; j < MAX_ENNEMIS; j++) {
            if (!ennemis[j].active) continue;

            dx = player.balles[i].x - ennemis[j].x;
            dy = player.balles[i].y - ennemis[j].y;

            if (dx > 0 && dx < ENEMY_W && dy > 0 && dy < ENEMY_H) {
                /* Collision : on desactive les deux et on incremente le score */
                player.balles[i].active = 0;
                ennemis[j].active = 0;
                score++;
            }
        }
    }
}

/* Verifie si une balle ennemie touche le joueur */
static int balle_ene_touche_joueur(void) {
    int i;
    float dx, dy;
    for (i = 0; i < MAX_BALLES_ENE; i++) {
        if (!balles_ene[i].active) continue;
        dx = balles_ene[i].x - player.x;
        dy = balles_ene[i].y - player.y;
        if (dx > -10 && dx < PLAYER_W && dy > -4 && dy < PLAYER_H) {
            balles_ene[i].active = 0;
            return 1;
        }
    }
    return 0;
}

/* Verifie si le joueur touche un ennemi (fin de partie) */
static int joueur_touche_ennemi(void) {
    int j;
    float dx, dy;

    for (j = 0; j < MAX_ENNEMIS; j++) {
        if (!ennemis[j].active) continue;
        dx = player.x - ennemis[j].x;
        dy = player.y - ennemis[j].y;
        if (dx > -PLAYER_W && dx < ENEMY_W && dy > -PLAYER_H && dy < ENEMY_H)
            return 1;
    }
    return 0;
}

/* ---------- Mise a jour ---------- */

void game_update(void) {
    if (etat == ETAT_MENU) {
        if (key_enter) {
            /* Reinitialiser et demarrer */
            score = 0;
            frame = 0;
            intervalle_spawn = 90;
            player_init(&player);
            ennemis_init(ennemis);
            balles_ene_init();
            etat = ETAT_JEU;
        }
        return;
    }

    if (etat == ETAT_FIN) {
        if (key_enter)
            etat = ETAT_MENU;
        return;
    }

    /* --- Etat ETAT_JEU --- */

    player_update(&player, key_up, key_down, key_left, key_right, key_space);
    ennemis_update(ennemis);
    balles_ene_update();

    frame++;

    /* Faire apparaitre un ennemi toutes les <intervalle_spawn> frames */
    if (frame % intervalle_spawn == 0)
        ennemis_spawn(ennemis);

    /* Augmenter la difficulte toutes les 600 frames */
    if (frame % 600 == 0 && intervalle_spawn > 30)
        intervalle_spawn -= 10;

    verifier_collisions();

    if (joueur_touche_ennemi() || balle_ene_touche_joueur())
        etat = ETAT_FIN;
}

/* ---------- Dessin ---------- */

void game_draw(void) {
    char buf[64];

    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (etat == ETAT_MENU) {
        al_draw_text(font, al_map_rgb(100, 200, 255),
            400, 240, ALLEGRO_ALIGN_CENTRE, "STELLAR STRIKE");
        al_draw_text(font, al_map_rgb(200, 200, 200),
            400, 280, ALLEGRO_ALIGN_CENTRE, "Fleches : deplacer   Espace : tirer");
        al_draw_text(font, al_map_rgb(255, 255, 100),
            400, 320, ALLEGRO_ALIGN_CENTRE, "Appuyez sur ENTREE pour commencer");
        return;
    }

    if (etat == ETAT_FIN) {
        al_draw_text(font, al_map_rgb(255, 80, 80),
            400, 240, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
        sprintf(buf, "Score final : %d", score);
        al_draw_text(font, al_map_rgb(255, 255, 255),
            400, 280, ALLEGRO_ALIGN_CENTRE, buf);
        al_draw_text(font, al_map_rgb(200, 200, 200),
            400, 320, ALLEGRO_ALIGN_CENTRE, "Appuyez sur ENTREE pour rejouer");
        return;
    }

    /* ETAT_JEU */
    ennemis_draw(ennemis);
    balles_ene_draw();
    player_draw(&player);

    sprintf(buf, "Score : %d", score);
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, buf);
}

void game_destroy(void) {
    player_destroy(&player);
    ennemis_destroy();   /* libere le sprite alien */
    al_destroy_font(font);
}
