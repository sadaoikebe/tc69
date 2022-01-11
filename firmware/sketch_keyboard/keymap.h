#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include <Keyboard.h>
#include "nicola_keyboard.h"

#define LAYOUT( \
      K05,   K07,   K06,   K04,   K02,   K01,   K00,   K03,   K0D,   K0F,   K0E,   K0C,   K0A,  K09,        K0B, \
      K15,   K17,   K16,   K14,   K12,   K11,   K10,   K13,   K1D,   K1F,   K1E,   K1C,   K1A,        K18,  K1B, \
      K25,   K27,   K26,   K24,   K22,   K21,   K20,   K23,   K2D,   K2F,   K2E,   K2C,         K29,        K2B, \
      K35,          K36,   K34,   K32,   K31,   K30,   K33,   K3D,   K3F,   K3E,   K3C,         K39,  K38,  K3B, \
      K45,   K47,          K44,          K41,                 K4D,          K4E,   K4C,   K4A,  K49,  K48,  K4B  \
) { \
    { K00,   K01,   K02,   K03,   K04,   K05,   K06,   K07,     0,   K09,   K0A,   K0B,   K0C,  K0D,  K0E,  K0F }, \
    { K10,   K11,   K12,   K13,   K14,   K15,   K16,   K17,   K18,     0,   K1A,   K1B,   K1C,  K1D,  K1E,  K1F }, \
    { K20,   K21,   K22,   K23,   K24,   K25,   K26,   K27,     0,   K29,     0,   K2B,   K2C,  K2D,  K2E,  K2F }, \
    { K30,   K31,   K32,   K33,   K34,   K35,   K36,     0,   K38,   K39,     0,   K3B,   K3C,  K3D,  K3E,  K3F }, \
    {   0,   K41,     0,     0,   K44,   K45,     0,   K47,   K48,   K49,   K4A,   K4B,   K4C,  K4D,  K4E,    0 }, \
}

#define KEY_PRNT_SCRN 206
#define KEY_PAUSE 212
#define KEY_FN (0x101)
#define KEY_HENKAN (0x102)
#define KEY_MUHENKAN (0x103)

typedef enum nicola_keycodes {
  NG_TOP = (0x102),
  NG_M_TOP = NG_TOP,
  NG_1 = NG_TOP,
  NG_2,
  NG_3,
  NG_4,
  NG_5,
  NG_6,
  NG_7,
  NG_8,
  NG_9,
  NG_0,
  NG_MINS,
  NG_EQL,
  NG_BSLS,

  NG_Q,
  NG_W,
  NG_E,
  NG_R,
  NG_T,
  NG_Y,
  NG_U,
  NG_I,
  NG_O,
  NG_P,
  NG_LBRC,
  NG_RBRC,

  NG_A,
  NG_S,
  NG_D,
  NG_F,
  NG_G,
  NG_H,
  NG_J,
  NG_K,
  NG_L,
  NG_SCLN,
  NG_QUOT,

  NG_Z,
  NG_X,
  NG_C,
  NG_V,
  NG_B,
  NG_N,
  NG_M,
  NG_COMM,
  NG_DOT,
  NG_SLSH,

  NG_M_BOTTOM = NG_SLSH,

  NG_SHFTL,
  NG_SHFTR,

  NG_BOTTOM = NG_SHFTR,
} NGKEYS;

const uint16_t keymap_base[MATRIX_ROWS][MATRIX_COLS] = LAYOUT( \
    KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE, KEY_HOME, \
    KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\', KEY_PAGE_UP, \
    KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', KEY_RETURN, KEY_PAGE_DOWN, \
    KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_RIGHT_SHIFT, KEY_UP_ARROW, KEY_END, \
    0, KEY_LEFT_GUI, KEY_LEFT_ALT, ' ', ' ', KEY_RIGHT_ALT, KEY_RIGHT_GUI, 0, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW);

const uint16_t keymap_fn[MATRIX_ROWS][MATRIX_COLS] = LAYOUT( \
    '`', KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_DELETE, 1, \
    1, 1, 1, 1, 1, 1, KEY_HOME, KEY_PAGE_DOWN, KEY_PAGE_UP, KEY_END, KEY_PRNT_SCRN, 1, KEY_PAUSE, 1, 1, \
    1, 1, 1, 1, 1, 1, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_INSERT, KEY_DELETE, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    0, 1, 1, KEY_MUHENKAN, KEY_HENKAN, 1, 1, 1, 1, 1, 1);

const uint16_t keymap_nicola[MATRIX_ROWS][MATRIX_COLS] = LAYOUT( \
    1, NG_1, NG_2, NG_3, NG_4, NG_5, NG_6, NG_7, NG_8, NG_9, NG_0, NG_MINS, NG_EQL, 1, 1, \
    1, NG_Q, NG_W, NG_E, NG_R, NG_T, NG_Y, NG_U, NG_I, NG_O, NG_P, NG_LBRC, NG_RBRC, NG_BSLS, 1, \
    1, NG_A, NG_S, NG_D, NG_F, NG_G, NG_H, NG_J, NG_K, NG_L, NG_SCLN, NG_QUOT, 1, 1, \
    1, NG_Z, NG_X, NG_C, NG_V, NG_B, NG_N, NG_M, NG_COMM, NG_DOT, NG_SLSH, 1, 1, 1, \
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    
#endif
