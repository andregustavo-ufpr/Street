#include <stdio.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#define FPS 60

#define GROUNDED_STATE 1
#define JUMPING_STATE 2

#define GROUND 50
#define JUMP_HEIGHT 360
#define MAX_HEALTH 1000
#define JUMP_VELOCITY 50
#define MOVE_VELOCITY 10
#define GRAVITY 5

typedef struct fighter {
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    float health;
    int state; // grounded = 1, jumping = 2
    ALLEGRO_BITMAP *img;
} fighter;

fighter *create_fighter(char *image, float x, float y) {
    fighter *new_fighter = (fighter *) malloc(sizeof(fighter));
    if(new_fighter == NULL) {
        printf("Could not create fighter");
        return NULL;
    }

    new_fighter->img = al_load_bitmap(image);
    new_fighter->pos_x = x;
    new_fighter->pos_y = y - al_get_bitmap_height(new_fighter->img);
    new_fighter->vel_x = 0;
    new_fighter->vel_y = 0;
    new_fighter->state = GROUNDED_STATE;
    new_fighter->health = MAX_HEALTH;

    return new_fighter;
}

void destroy_fighter(fighter *p) {
    al_destroy_bitmap(p->img);
    free(p);
}

void gravity(fighter *p, float ground) {
    if(p->pos_y < ground) p->vel_y += GRAVITY;
}

void jump(fighter *p) {
    if(p->state == GROUNDED_STATE) {
        p->vel_y -= JUMP_VELOCITY;
        p->state = JUMPING_STATE;
    }
}

void move(fighter *p, float value) {
    p->vel_x += value;
}

void update_pos(fighter *p, float ground) {
    p->pos_y += p->vel_y;
    p->pos_x += p->vel_x;

    if(p->pos_y >= ground) {
        p->state = GROUNDED_STATE;
        p->pos_y = ground;
        p->vel_y = 0;
    }
}

void pause_game(int *paused) {
    if(!*paused) *paused = 1;
    else *paused = 0;
}

void init_game() {
    al_init();
    al_init_image_addon();
    al_install_keyboard();
}

void end_game(ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue) {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

int main(void) {

    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;

    init_game(queue);

    queue = al_create_event_queue();
    display = al_create_display(1600, 900);
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    float disp_h = al_get_display_height(display),
        disp_w = al_get_display_width(display);

    fighter *char1;
    char1 = create_fighter("menu.png", 90, disp_h - GROUND);

    int running = 1, paused = 0;
    while (running ){
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        float ground = disp_h - al_get_bitmap_height(char1->img) - GROUND;

        gravity(char1, ground);
        update_pos(char1, ground);

        if(!paused) {
            switch (event.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = 0;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) pause_game(&paused);
                    if(event.keyboard.keycode == ALLEGRO_KEY_W) {
                        jump(char1);
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_D) {
                        int direction = 1;
                        if(event.keyboard.keycode == ALLEGRO_KEY_A) direction = -1;

                        move(char1, direction*MOVE_VELOCITY);
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_D) {
                        int direction = -1;
                        if(event.keyboard.keycode == ALLEGRO_KEY_A) direction = 1;

                        move(char1, direction*MOVE_VELOCITY);
                    }
                    break;
                case ALLEGRO_EVENT_TIMER:
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    al_draw_bitmap(char1->img, char1->pos_x, char1->pos_y, 0);
                    al_flip_display();
                    break;
                default:
                    break;
            }
        }
        else {
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                pause_game(&paused);
            }
        }
    }

    end_game(timer, display, queue);
    destroy_fighter(char1);

    return 0;
}

