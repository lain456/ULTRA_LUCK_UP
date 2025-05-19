#include "keymap.h"
#include <string.h>
#include <ctype.h>

SDLKey key_name_to_sdlkey(const char *name) {
    if (!name || !name[0]) return SDLK_UNKNOWN;

    // Convert name to lowercase for case-insensitive comparison
    char lower[32];
    strncpy(lower, name, sizeof(lower) - 1);
    lower[sizeof(lower) - 1] = '\0';
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }

    // Map common key names to SDLKey
    if (strcmp(lower, "space") == 0) return SDLK_SPACE;
    if (strcmp(lower, "left") == 0) return SDLK_LEFT;
    if (strcmp(lower, "right") == 0) return SDLK_RIGHT;
    if (strcmp(lower, "up") == 0) return SDLK_UP;
    if (strcmp(lower, "down") == 0) return SDLK_DOWN;
    if (strcmp(lower, "return") == 0) return SDLK_RETURN;
    if (strcmp(lower, "escape") == 0) return SDLK_ESCAPE;
    if (strcmp(lower, "a") == 0) return SDLK_a;
    if (strcmp(lower, "b") == 0) return SDLK_b;
    if (strcmp(lower, "c") == 0) return SDLK_c;
    if (strcmp(lower, "d") == 0) return SDLK_d;
    if (strcmp(lower, "e") == 0) return SDLK_e;
    if (strcmp(lower, "f") == 0) return SDLK_f;
    if (strcmp(lower, "g") == 0) return SDLK_g;
    if (strcmp(lower, "h") == 0) return SDLK_h;
    if (strcmp(lower, "i") == 0) return SDLK_i;
    if (strcmp(lower, "j") == 0) return SDLK_j;
    if (strcmp(lower, "k") == 0) return SDLK_k;
    if (strcmp(lower, "l") == 0) return SDLK_l;
    if (strcmp(lower, "m") == 0) return SDLK_m;
    if (strcmp(lower, "n") == 0) return SDLK_n;
    if (strcmp(lower, "o") == 0) return SDLK_o;
    if (strcmp(lower, "p") == 0) return SDLK_p;
    if (strcmp(lower, "q") == 0) return SDLK_q;
    if (strcmp(lower, "r") == 0) return SDLK_r;
    if (strcmp(lower, "s") == 0) return SDLK_s;
    if (strcmp(lower, "t") == 0) return SDLK_t;
    if (strcmp(lower, "u") == 0) return SDLK_u;
    if (strcmp(lower, "v") == 0) return SDLK_v;
    if (strcmp(lower, "w") == 0) return SDLK_w;
    if (strcmp(lower, "x") == 0) return SDLK_x;
    if (strcmp(lower, "y") == 0) return SDLK_y;
    if (strcmp(lower, "z") == 0) return SDLK_z;
    if (strcmp(lower, "0") == 0) return SDLK_0;
    if (strcmp(lower, "1") == 0) return SDLK_1;
    if (strcmp(lower, "2") == 0) return SDLK_2;
    if (strcmp(lower, "3") == 0) return SDLK_3;
    if (strcmp(lower, "4") == 0) return SDLK_4;
    if (strcmp(lower, "5") == 0) return SDLK_5;
    if (strcmp(lower, "6") == 0) return SDLK_6;
    if (strcmp(lower, "7") == 0) return SDLK_7;
    if (strcmp(lower, "8") == 0) return SDLK_8;
    if (strcmp(lower, "9") == 0) return SDLK_9;
    if (strcmp(lower, "f1") == 0) return SDLK_F1;
    if (strcmp(lower, "f2") == 0) return SDLK_F2;
    if (strcmp(lower, "f3") == 0) return SDLK_F3;
    if (strcmp(lower, "f4") == 0) return SDLK_F4;
    if (strcmp(lower, "f5") == 0) return SDLK_F5;
    if (strcmp(lower, "f6") == 0) return SDLK_F6;
    if (strcmp(lower, "f7") == 0) return SDLK_F7;
    if (strcmp(lower, "f8") == 0) return SDLK_F8;
    if (strcmp(lower, "f9") == 0) return SDLK_F9;
    if (strcmp(lower, "f10") == 0) return SDLK_F10;
    if (strcmp(lower, "f11") == 0) return SDLK_F11;
    if (strcmp(lower, "f12") == 0) return SDLK_F12;
    if (strcmp(lower, "tab") == 0) return SDLK_TAB;
    if (strcmp(lower, "capslock") == 0) return SDLK_CAPSLOCK;
    if (strcmp(lower, "lshift") == 0) return SDLK_LSHIFT;
    if (strcmp(lower, "rshift") == 0) return SDLK_RSHIFT;
    if (strcmp(lower, "lctrl") == 0) return SDLK_LCTRL;
    if (strcmp(lower, "rctrl") == 0) return SDLK_RCTRL;
    if (strcmp(lower, "lalt") == 0) return SDLK_LALT;
    if (strcmp(lower, "ralt") == 0) return SDLK_RALT;
    if (strcmp(lower, "backspace") == 0) return SDLK_BACKSPACE;







    if (strcmp(lower, "[0]") == 0) return SDLK_KP0;
    if (strcmp(lower, "[1]") == 0) return SDLK_KP1;
    if (strcmp(lower, "[2]") == 0) return SDLK_KP2;
    if (strcmp(lower, "[3]") == 0) return SDLK_KP3;
    if (strcmp(lower, "[4]") == 0) return SDLK_KP4;
    if (strcmp(lower, "[5]") == 0) return SDLK_KP5;
    if (strcmp(lower, "[6]") == 0) return SDLK_KP6;
    if (strcmp(lower, "[7]") == 0) return SDLK_KP7;
    if (strcmp(lower, "[8]") == 0) return SDLK_KP8;
    if (strcmp(lower, "[9]") == 0) return SDLK_KP9;
    if (strcmp(lower, "[+]") == 0) return SDLK_KP_PLUS;
    if (strcmp(lower, "[-]") == 0) return SDLK_KP_MINUS;
    if (strcmp(lower, "[*]") == 0) return SDLK_KP_MULTIPLY;
    if (strcmp(lower, "[/]") == 0) return SDLK_KP_DIVIDE;
    if (strcmp(lower, "[.]") == 0) return SDLK_KP_PERIOD;
    if (strcmp(lower, "enter") == 0) return SDLK_KP_ENTER;

    printf("Warning: Unknown key name '%s'\n", name);
    return SDLK_UNKNOWN;
}