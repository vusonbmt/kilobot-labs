#include <kilolib.h>

#define THRESH_HIGH 5
#define THRESH_LOW 5

int cur_direction = 0;
int cur_light = 0;
int high_thresh = 0, low_thresh = 1024;

void sample_light() {
    int i;
    long average;

    average = 0;
    for (i = 0 ; i < 300; i++) {
        int sample = get_ambientlight();
        if( sample == -1) { i--; continue;}
        average += sample;
    }

    cur_light = average / 300;
}

void switch_directions() {
    if (cur_direction) {
        spinup_motors();
        set_motors(0, kilo_turn_right);
        set_color(RGB(2,0,0));
    } else {
        spinup_motors();
        set_motors(kilo_turn_left, 0);
        set_color(RGB(0,2,0));
    }
    cur_direction = !cur_direction;
}

void update_thresh() {
    high_thresh = cur_light*(100+THRESH_HIGH)/100;
    low_thresh = cur_light*(100-THRESH_LOW)/100;
}

void compute_direction() {
    if (cur_light < low_thresh)
        update_thresh();
    else if (cur_light > high_thresh) {
        update_thresh();
        switch_directions();
        delay(100); // to avoid fast switches
    }
}

void setup() {
    switch_directions(); // start turning in any direction
}

void loop() {
    static int start = 0;
    if (!start) {
        start = 1;
        switch_directions();
    } else {
        sample_light();
        compute_direction();
    }
}

int main() {
    kilo_init();
    kilo_start(setup, loop);

    return 0;
}
