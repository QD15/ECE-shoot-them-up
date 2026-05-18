#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#define ETAT_PAUSE    5

#define ETAT_MENU     0
#define ETAT_JEU      1
#define ETAT_BOSS     2
#define ETAT_FIN      3
#define ETAT_VICTOIRE 4

#define DUREE_NIVEAU  3600   // 60 s x 60 fps

#define NB_ETOILES  40

typedef struct {
    float x, y;
    float rayon;
    unsigned char luminosite; //0à255
} Etoile;

static Etoile etoiles[NB_ETOILES];

#define VITESSE_ETOILES  1.2f

static void etoiles_init(void) {
    int i;
    for (i = 0; i < NB_ETOILES; i++) {
        etoiles[i].x          = (float)(rand() % 800);
        etoiles[i].y          = (float)(rand() % 600);
        etoiles[i].rayon      = 1.5f + (float)(rand() % 15) / 10.0f;
        etoiles[i].luminosite = 180 + rand() % 76;
    }
}

static void etoiles_update(void) {
    int i;
    for (i = 0; i < NB_ETOILES; i++) {
        etoiles[i].x -= VITESSE_ETOILES;
        if (etoiles[i].x < 0.0f) {
            etoiles[i].x = 800.0f;
            etoiles[i].y = (float)(rand() % 600);
        }
    }
}

static void etoiles_draw(void) {
    int i;
    unsigned char l;
    for (i = 0; i < NB_ETOILES; i++) {
        l = etoiles[i].luminosite;
        al_draw_filled_circle(etoiles[i].x, etoiles[i].y,
            etoiles[i].rayon,
            al_map_rgb(l, l, (unsigned char)(l < 30 ? 0 : l - 30))); /* teinte bleutee */
    }
}

static ALLEGRO_FONT *font_titre = NULL;
static ALLEGRO_FONT *font_menu  = NULL;
static ALLEGRO_FONT *font_hud   = NULL;

static Player player;
static Enemy  ennemis[MAX_ENNEMIS];

static int etat;
static int score;
static int frame;
static int intervalle_spawn;
static int niveau;
static int vies;

#define VIES_MAX        3
#define INVINCIBLE_DUREE 120  // 2 s d'invincibilite apres un degat
static int invincible;        // compteur frames d'invincibilite

static void demarrer_niveau(void);

void game_init(void) {
    font_titre = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", 52, 0);
    font_menu  = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", 30, 0);
    font_hud   = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", 20, 0);

    if (!font_titre) font_titre = al_create_builtin_font();
    if (!font_menu)  font_menu  = al_create_builtin_font();
    if (!font_hud)   font_hud   = al_create_builtin_font();

    etat   = ETAT_MENU;
    score  = 0;
    frame  = 0;
    niveau = 1;
    vies   = VIES_MAX;
    invincible = 0;
    intervalle_spawn = 90;

    etoiles_init();

    player_init(&player);
    ennemis_init(ennemis);
    balles_ene_init();
}
static void demarrer_niveau(void) {
    score = 0;
    frame = 0;
    vies  = VIES_MAX;
    invincible = 0;

    player_init(&player);
    ennemis_init(ennemis);
    balles_ene_init();

    if      (niveau == 1) intervalle_spawn = 120;
    else if (niveau == 2) intervalle_spawn = 70;
    else                  intervalle_spawn = 40;

    etat = ETAT_JEU;
}

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
                player.balles[i].active = 0;
                ennemis[j].active       = 0;
                score++;
            }
        }
    }
}

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


// Balle joueur touche le boss -> retire 1 PV
static void verifier_collisions_boss(void) {
    int i;
    float dx, dy;
    if (!boss.active) return;

    for (i = 0; i < MAX_BALLES; i++) {
        if (!player.balles[i].active) continue;
        dx = player.balles[i].x - boss.x;
        dy = player.balles[i].y - boss.y;
        if (dx > 0 && dx < BOSS_W && dy > 0 && dy < BOSS_H) {
            player.balles[i].active = 0;
            boss.hp--;
            score++;
            if (boss.hp <= 0) {
                boss.active = 0;   // boss mort -> victoire
                etat = ETAT_VICTOIRE;
            }
        }
    }
}

// Balle du boss touche le joueur
static int boss_balle_touche_joueur(void) {
    int i;
    float dx, dy;
    for (i = 0; i < MAX_BOSS_BALLES; i++) {
        if (!boss_balles[i].active) continue;
        dx = boss_balles[i].x - player.x;
        dy = boss_balles[i].y - player.y;
        if (dx > -7 && dx < PLAYER_W && dy > -7 && dy < PLAYER_H) {
            boss_balles[i].active = 0;
            return 1;
        }
    }
    return 0;
}

// Le joueur touche le boss directement
static int joueur_touche_boss(void) {
    float dx, dy;
    if (!boss.active) return 0;
    dx = player.x - boss.x;
    dy = player.y - boss.y;
    return (dx > -PLAYER_W && dx < BOSS_W && dy > -PLAYER_H && dy < BOSS_H);
}

void game_update(void) {

    static int prev_p = 0;
    if (key_p && !prev_p) {
        if (etat == ETAT_JEU  || etat == ETAT_BOSS)
            etat = ETAT_PAUSE;
        else if (etat == ETAT_PAUSE)
            etat = ETAT_JEU;   // ou ETAT_BOSS selon d'où on vient
    }
    prev_p = key_p;

    if (etat == ETAT_PAUSE) return;   //  rien ne bouge en pause
    // Les etoiles defilent dans tous les etats (menu inclus)
    etoiles_update();

    //MENU
    if (etat == ETAT_MENU) {
        static int prev_1 = 0, prev_2 = 0, prev_3 = 0;
        if (key_1 && !prev_1) { niveau = 1; demarrer_niveau(); }
        if (key_2 && !prev_2) { niveau = 2; demarrer_niveau(); }
        if (key_3 && !prev_3) { niveau = 3; demarrer_niveau(); }
        prev_1 = key_1; prev_2 = key_2; prev_3 = key_3;
        return;
    }

    // GAME OVER
    if (etat == ETAT_FIN) {
        if (key_enter) etat = ETAT_MENU;
        return;
    }

    // VICTOIRE
    if (etat == ETAT_VICTOIRE) {
        if (key_enter) etat = ETAT_MENU;
        return;
    }

//Boss après 60 secondes
    if (etat == ETAT_BOSS) {
        player_update(&player, key_up, key_down, key_left, key_right, key_space);
        boss_update();
        boss_balles_update();

        verifier_collisions_boss();

        if (invincible > 0) {
            invincible--;
        } else if (boss_balle_touche_joueur() || joueur_touche_boss()) {
            vies--;
            if (vies <= 0)
                etat = ETAT_FIN;
            else {
                invincible = INVINCIBLE_DUREE;
                player_init(&player);  /* repositionne le joueur */
            }
        }

        return;
    }

    player_update(&player, key_up, key_down, key_left, key_right, key_space);
    ennemis_update(ennemis);
    balles_ene_update();

    frame++;

    if (frame % intervalle_spawn == 0)
        ennemis_spawn(ennemis);

    if (niveau < 3 && frame % 600 == 0 && intervalle_spawn > 30)
        intervalle_spawn -= 10;

    verifier_collisions();

    if (invincible > 0) {
        invincible--;
    } else if (joueur_touche_ennemi() || balle_ene_touche_joueur()) {
        vies--;
        if (vies <= 0) {
            etat = ETAT_FIN;
            return;
        }
        invincible = INVINCIBLE_DUREE;
        player_init(&player);  /* repositionne le joueur */
    }

    // Fin des 60 secondes
    if (frame >= DUREE_NIVEAU) {
        if (niveau == 3) {
            // Niveau 3 passer au combat boss
            boss_init();
            etat = ETAT_BOSS;
        } else {
            etat = ETAT_VICTOIRE;
        }
    }
}

//affichage des vies
static void draw_vies(void) {
    int i;
    char coeur[4] = "\x03";
    float x = 10.0f;
    float y = 570.0f;
    for (i = 0; i < VIES_MAX; i++) {
        ALLEGRO_COLOR couleur = (i < vies)
            ? al_map_rgb(255, 60, 80)    // vie restante : rouge vif
            : al_map_rgb(60, 60, 60);    // vie perdue: gris sombre
        // Dessine un petit cercle plein comme icone de vie
        al_draw_filled_circle(x + 10, y + 8, 8, couleur);
        al_draw_circle(x + 10, y + 8, 8, al_map_rgb(200, 200, 200), 1.0f);
        x += 26.0f;
    }
    (void)coeur;
}


//Barre de vie du Boss
static void draw_boss_hp_bar(void) {
    float ratio    = (float)boss.hp / (float)BOSS_HP_MAX;
    float bar_w    = 300.0f;
    float bar_x    = 400 - bar_w / 2;
    float bar_y    = 10;
    float bar_h    = 18;

    // Fond gris
    al_draw_filled_rectangle(bar_x, bar_y,
        bar_x + bar_w, bar_y + bar_h,
        al_map_rgb(60, 60, 60));

    // Portion de vie restante : vert -> jaune -> rouge selon les PV
    ALLEGRO_COLOR couleur;
    if (ratio > 0.6f)       couleur = al_map_rgb(80,  220, 80);
    else if (ratio > 0.3f)  couleur = al_map_rgb(255, 200,  0);
    else                    couleur = al_map_rgb(255,  50, 50);

    al_draw_filled_rectangle(bar_x, bar_y,
        bar_x + bar_w * ratio, bar_y + bar_h,
        couleur);

    //Contour
    al_draw_rectangle(bar_x, bar_y,
        bar_x + bar_w, bar_y + bar_h,
        al_map_rgb(200, 200, 200), 1.5f);

    // Label
    al_draw_text(font_hud, al_map_rgb(255, 255, 255),
        400, bar_y + 22, ALLEGRO_ALIGN_CENTRE, "BOSS");
}

void game_draw(void) {
    char buf[64];
    int  temps_restant;
    if (etat == ETAT_PAUSE) {
        al_draw_text(font_titre, al_map_rgb(255, 200, 0),
            400, 220, ALLEGRO_ALIGN_CENTRE, "PAUSE");
        al_draw_text(font_menu, al_map_rgb(200, 200, 200),
            400, 310, ALLEGRO_ALIGN_CENTRE, "P  ->  Reprendre");
        al_draw_text(font_menu, al_map_rgb(200, 200, 200),
            400, 360, ALLEGRO_ALIGN_CENTRE, "ECHAP  ->  Quitter");
        return;
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Fond etoile dessine en premier (dans tous les etats)
    etoiles_draw();

    //MENU
    if (etat == ETAT_MENU) {
        al_draw_text(font_titre, al_map_rgb(100, 200, 255),
            400, 140, ALLEGRO_ALIGN_CENTRE, "L'INVASION");
        al_draw_text(font_menu, al_map_rgb(80, 220, 80),
            400, 260, ALLEGRO_ALIGN_CENTRE, "1  -  Niveau 1  (Facile)");
        al_draw_text(font_menu, al_map_rgb(255, 200, 0),
            400, 320, ALLEGRO_ALIGN_CENTRE, "2  -  Niveau 2  (Moyen)");
        al_draw_text(font_menu, al_map_rgb(255, 80, 80),
            400, 380, ALLEGRO_ALIGN_CENTRE, "3  -  Niveau 3  (Difficile)");
        return;
    }

    //GAME OVER
    if (etat == ETAT_FIN) {
        al_draw_text(font_titre, al_map_rgb(255, 80, 80),
            400, 200, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
        sprintf(buf, "Score : %d", score);
        al_draw_text(font_menu, al_map_rgb(255, 255, 255),
            400, 300, ALLEGRO_ALIGN_CENTRE, buf);
        al_draw_text(font_menu, al_map_rgb(150, 150, 150),
            400, 370, ALLEGRO_ALIGN_CENTRE, "ENTREE  ->  Menu");
        return;
    }

    //VICTOIRE
    if (etat == ETAT_VICTOIRE) {
        // Message different si on vient de tuer le boss
        if (niveau == 3)
            al_draw_text(font_titre, al_map_rgb(255, 215, 0),
                400, 180, ALLEGRO_ALIGN_CENTRE, "BOSS VAINCU !");
        else
            al_draw_text(font_titre, al_map_rgb(80, 255, 120),
                400, 180, ALLEGRO_ALIGN_CENTRE, "NIVEAU TERMINE !");

        sprintf(buf, "Score : %d", score);
        al_draw_text(font_menu, al_map_rgb(255, 255, 255),
            400, 290, ALLEGRO_ALIGN_CENTRE, buf);
        al_draw_text(font_menu, al_map_rgb(150, 150, 150),
            400, 360, ALLEGRO_ALIGN_CENTRE, "ENTREE  ->  Menu");
        return;
    }

    //COMBAT BOSS
    if (etat == ETAT_BOSS) {
        boss_draw();
        boss_balles_draw();
        player_draw(&player);
        draw_boss_hp_bar();

        sprintf(buf, "Score : %d", score);
        al_draw_text(font_hud, al_map_rgb(255, 255, 255), 10, 10, 0, buf);
        draw_vies();
        return;
    }

    //EN JEU
    ennemis_draw(ennemis);
    balles_ene_draw();
    player_draw(&player);

    sprintf(buf, "Score : %d", score);
    al_draw_text(font_hud, al_map_rgb(255, 255, 255), 10, 10, 0, buf);

    temps_restant = (DUREE_NIVEAU - frame) / 60;
    sprintf(buf, "Temps : %ds", temps_restant);
    al_draw_text(font_hud, al_map_rgb(255, 255, 100),
        790, 10, ALLEGRO_ALIGN_RIGHT, buf);

    sprintf(buf, "Niveau %d", niveau);
    al_draw_text(font_hud, al_map_rgb(150, 200, 255),
        400, 10, ALLEGRO_ALIGN_CENTRE, buf);

    draw_vies();
}
void game_destroy(void) {
    player_destroy(&player);
    ennemis_destroy();
    boss_destroy();
    al_destroy_font(font_titre);
    al_destroy_font(font_menu);
    al_destroy_font(font_hud);
}
