#include <stdio.h>

#define COLORS 8
typedef enum {
    RED     = 0,
    GREEN   = 1,
    BLUE    = 2,
    YELLOW  = 3,
    PINK    = 4,
    CYAN    = 5,
    WHITE   = 6,
    BLACK   = 7,
} color_t;

short colors[COLORS][3];

#define set_color(x, r, g, b) colors[x][0] = r; colors[x][1] = g; colors[x][2] = b; 
void calibrate_sensor() {
    set_color(RED,    255,   0,   0);
    set_color(GREEN,    0, 255,   0);
    set_color(BLUE,     0,   0, 255);
    set_color(YELLOW, 255, 255,   0);
    set_color(PINK,   255,   0, 255);
    set_color(CYAN,     0, 255, 255);
    set_color(WHITE,  255, 255, 255);
    set_color(BLACK ,   0,   0,   0);
}

#define ifprint(c, s) case c: printf(#s); break;
void print_color(color_t color) {
    switch (color) {
        ifprint(RED,    Red);
        ifprint(GREEN,  Green);
        ifprint(BLUE,   Blue);
        ifprint(YELLOW, Yellow);
        ifprint(PINK,   Pink);
        ifprint(CYAN,   Cyan);
        ifprint(WHITE,  White);
        ifprint(BLACK,  Black);
    }
}

int isqrt(int num) {
    int res = 0;
    int bit = 1 << 30; // The second-to-top bit is set: 1 << 30 for 32 bits
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > num)
        bit >>= 2;
        
    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    
    return res;
}

int euclidean_distance_3d(short* vec1, short* vec2) {
    int res1 = vec1[0] - vec2[0];
    int res2 = vec1[1] - vec2[1];
    int res3 = vec1[2] - vec2[2];

    return isqrt(res1*res1 + res2*res2 + res3*res3);
}

color_t determin_color(short* rgb) {
    int i, min = 2147483647;
    color_t res;

    for (i = 0; i < COLORS; i++) {
        int dist = euclidean_distance_3d(colors[i], rgb);
        
        if (dist < min) {
            res = i;
            min = dist;
        }
    }

    return res;
}

#define determin_and_print(name, r, g, b)   \
short name[3] = { r, g, b };                \
printf(#name);                              \
printf(": ");                               \
print_color(determin_color(name));          \
printf("\n");                               

int main() {
    calibrate_sensor();
    determin_and_print(redish,    204,   0,   0 );
    determin_and_print(greenish,    0, 153,  51 );
    determin_and_print(blueish,     0,  51, 204 );
    determin_and_print(yellowish, 204, 204,   0 );
    determin_and_print(pinkish,   255,  77, 255 );
    determin_and_print(cyanish,     0, 255, 153 );
    determin_and_print(brownish,  102,  51,   0 );
    determin_and_print(darkgray,   64,  64,  64 );
    determin_and_print(lightgray, 191, 191, 191 );
    return 0;
}