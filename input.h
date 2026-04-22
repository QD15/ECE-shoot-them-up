#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>

/* Variables globales : etat des touches (1 = enfoncee, 0 = relachee) */
extern int key_up, key_down, key_left, key_right;
extern int key_space, key_enter, key_escape;

/* Met a jour les variables selon l'evenement recu */
void input_handle_event(ALLEGRO_EVENT *ev);

#endif
