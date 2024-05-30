#include <stdio.h>
#include <allegro5/allegro5.h>

int main(void) {

    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;

    al_init();
    display = al_create_display(640, 480);
    queue = al_create_event_queue();
    timer = al_create_timer(1.0/60);

    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    int running = 1;
    while (running ){
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || event.type == ALLEGRO_EVENT_KEY_UP) running = 0;
        if(event.type == ALLEGRO_EVENT_TIMER) al_flip_display();
    }

    al_destroy_display(display);
    al_uninstall_keyboard();
    return 0;
}

