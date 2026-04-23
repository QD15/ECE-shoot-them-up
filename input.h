#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>

extern int key_up, key_down, key_left, key_right;
extern int key_space, key_enter, key_escape;


void input_handle_event(ALLEGRO_EVENT *ev);

#endif
