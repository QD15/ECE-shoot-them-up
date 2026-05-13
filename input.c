#include <allegro5/allegro.h>
#include "input.h"

int key_up = 0, key_down = 0, key_left = 0, key_right = 0;
int key_space = 0, key_enter = 0, key_escape = 0;
int key_1 = 0, key_2 = 0, key_3 = 0;
int key_p = 0;   // ← NOUVEAU

void input_handle_event(ALLEGRO_EVENT *ev) {
    int val;

    if (ev->type == ALLEGRO_EVENT_KEY_DOWN)      val = 1;
    else if (ev->type == ALLEGRO_EVENT_KEY_UP)   val = 0;
    else return;

    switch (ev->keyboard.keycode) {
        case ALLEGRO_KEY_UP:     key_up     = val; break;
        case ALLEGRO_KEY_DOWN:   key_down   = val; break;
        case ALLEGRO_KEY_LEFT:   key_left   = val; break;
        case ALLEGRO_KEY_RIGHT:  key_right  = val; break;
        case ALLEGRO_KEY_SPACE:  key_space  = val; break;
        case ALLEGRO_KEY_ENTER:  key_enter  = val; break;
        case ALLEGRO_KEY_ESCAPE: key_escape = val; break;
        case ALLEGRO_KEY_1:      key_1      = val; break;
        case ALLEGRO_KEY_2:      key_2      = val; break;
        case ALLEGRO_KEY_3:      key_3      = val; break;
        case ALLEGRO_KEY_P:      key_p      = val; break;  // ← NOUVEAU
    }
}
