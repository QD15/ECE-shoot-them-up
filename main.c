#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "input.h"

int main(void) {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_EVENT ev;
    int running, redraw;

 
    if (!al_init()) return -1;
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);
    al_init_image_addon();  

   
    display = al_create_display(800, 600);
    al_set_window_title(display, "Space Shooter");

   
    timer = al_create_timer(1.0 / 60);

  
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    game_init();
    al_start_timer(timer);

    running = 1;
    redraw = 0;

    while (running) {
        al_wait_for_event(queue, &ev);

    
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = 0;

      
        input_handle_event(&ev);
        if (key_escape)
            running = 0;

       
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            game_update();
            redraw = 1;
        }

      
        if (redraw && al_is_event_queue_empty(queue)) {
            game_draw();
            al_flip_display();
            redraw = 0;
        }
    }

    game_destroy();
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    return 0;
}
