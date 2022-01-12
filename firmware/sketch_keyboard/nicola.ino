/* Copyright 2018-2019 eswai <@eswai>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * introduced NICOLA state machine
 * (C) 2020 Sadao Ikebe @bonyarou
 */

#include "keymap.h"

static bool is_nicola = false; // 親指シフトがオンかオフか
static uint8_t n_modifier = 0; // 押しているmodifierキーの数

#define TIMEOUT_THRESHOLD (150)
#define OVERLAP_THRESHOLD (20)

typedef enum {
  NICOLA_STATE_S1_INIT,
  NICOLA_STATE_S2_M,
  NICOLA_STATE_S3_O,
  NICOLA_STATE_S4_MO,
  NICOLA_STATE_S5_OM
} nicola_state_t;

static nicola_state_t nicola_int_state = NICOLA_STATE_S1_INIT;
static int nicola_m_key;
static int nicola_o_key;
static uint16_t nicola_m_time;
static uint16_t nicola_o_time;

#define IF_TIMEOUT(x) if(x)

// 親指シフトをオンオフ
void nicola_on(void) {
  is_nicola = true;
  nicola_clear();

  Keyboard.press(KEY_CONVERT);
  Keyboard.press(KEY_LANG1);
  delayMicroseconds(10);
  Keyboard.release(KEY_CONVERT);
  Keyboard.release(KEY_LANG1);
}

void nicola_off(void) {
  is_nicola = false;
  nicola_clear();

  Keyboard.press(KEY_NOCONVERT);
  Keyboard.press(KEY_LANG2);
  delayMicroseconds(10);
  Keyboard.release(KEY_NOCONVERT);
  Keyboard.release(KEY_LANG2);
}

// 親指シフトの状態
bool nicola_state(void) {
  return is_nicola;
}

// バッファをクリアする
void nicola_clear(void) {
  nicola_int_state = NICOLA_STATE_S1_INIT;
}

// 入力モードか編集モードかを確認する
bool is_nicola_layer(uint16_t keycode, bool pressed) {
  if (!is_nicola) return false;

  // modifierが押されたらレイヤーをオフ
  switch (keycode) {
    case KEY_LEFT_CTRL:
    case KEY_LEFT_SHIFT:
    case KEY_LEFT_ALT:
    case KEY_LEFT_GUI:
    case KEY_RIGHT_CTRL:
    case KEY_RIGHT_SHIFT:
    case KEY_RIGHT_ALT:
    case KEY_RIGHT_GUI:
      if (pressed) {
        if(n_modifier == 0) {
          return false;
        }
        n_modifier++;
      } else {
        n_modifier--;
        if (n_modifier == 0) {
          return true;
        }
      }
      break;
  }

  return true;
}

void nicola_m_type(void) {
    switch(nicola_m_key) {
        case NG_1   : Keyboard.write((const uint8_t*)"1", 1); break;
        case NG_2   : Keyboard.write((const uint8_t*)"2", 1); break;
        case NG_3   : Keyboard.write((const uint8_t*)"3", 1); break;
        case NG_4   : Keyboard.write((const uint8_t*)"4", 1); break;
        case NG_5   : Keyboard.write((const uint8_t*)"5", 1); break;
        case NG_6   : Keyboard.write((const uint8_t*)"6", 1); break;
        case NG_7   : Keyboard.write((const uint8_t*)"7", 1); break;
        case NG_8   : Keyboard.write((const uint8_t*)"8", 1); break;
        case NG_9   : Keyboard.write((const uint8_t*)"9", 1); break;
        case NG_0   : Keyboard.write((const uint8_t*)"0", 1); break;
        case NG_MINS: Keyboard.write((const uint8_t*)"-", 1); break;
        case NG_EQL : Keyboard.write((const uint8_t*)"=", 1); break;

        case NG_Q   : Keyboard.write((const uint8_t*)"." ,1); break;
        case NG_W   : Keyboard.write((const uint8_t*)"ka",2); break;
        case NG_E   : Keyboard.write((const uint8_t*)"ta",2); break;
        case NG_R   : Keyboard.write((const uint8_t*)"ko",2); break;
        case NG_T   : Keyboard.write((const uint8_t*)"sa",2); break;
        case NG_Y   : Keyboard.write((const uint8_t*)"ra",2); break;
        case NG_U   : Keyboard.write((const uint8_t*)"ti",2); break;
        case NG_I   : Keyboard.write((const uint8_t*)"ku",2); break;
        case NG_O   : Keyboard.write((const uint8_t*)"tu",2); break;
        case NG_P   : Keyboard.write((const uint8_t*)"," ,1); break;
        case NG_LBRC: Keyboard.write((const uint8_t*)"," ,1); break;
        case NG_RBRC: Keyboard.write((const uint8_t*)";" ,1); break;
        case NG_BSLS: Keyboard.write((const uint8_t*)"\\",1); break;

        case NG_A   : Keyboard.write((const uint8_t*)"u" ,1); break;
        case NG_S   : Keyboard.write((const uint8_t*)"si",2); break;
        case NG_D   : Keyboard.write((const uint8_t*)"te",2); break;
        case NG_F   : Keyboard.write((const uint8_t*)"ke",2); break;
        case NG_G   : Keyboard.write((const uint8_t*)"se",2); break;
        case NG_H   : Keyboard.write((const uint8_t*)"ha",2); break;
        case NG_J   : Keyboard.write((const uint8_t*)"to",2); break;
        case NG_K   : Keyboard.write((const uint8_t*)"ki",2); break;
        case NG_L   : Keyboard.write((const uint8_t*)"i" ,1); break;
        case NG_SCLN: Keyboard.write((const uint8_t*)"nn",22); break;
        case NG_QUOT: Keyboard.write((const uint8_t*)"\xb2",1); break;

        case NG_Z   : Keyboard.write((const uint8_t*)"." ,1); break;
        case NG_X   : Keyboard.write((const uint8_t*)"hi",2); break;
        case NG_C   : Keyboard.write((const uint8_t*)"su",2); break;
        case NG_V   : Keyboard.write((const uint8_t*)"hu",2); break;
        case NG_B   : Keyboard.write((const uint8_t*)"he",2); break;
        case NG_N   : Keyboard.write((const uint8_t*)"me",2); break;
        case NG_M   : Keyboard.write((const uint8_t*)"so",2); break;
        case NG_COMM: Keyboard.write((const uint8_t*)"ne",2); break;
        case NG_DOT : Keyboard.write((const uint8_t*)"ho",2); break;
        case NG_SLSH: Keyboard.write((const uint8_t*)"/" ,1); break;
    }
}

void nicola_o_type(void) {
    if(nicola_o_key != 0) {
        Keyboard.write((const uint8_t*)" ", 1);
    }
}

void nicola_om_type(void) {
    if(nicola_o_key == NG_SHFTL) {
        switch(nicola_m_key) {
            case NG_1   : Keyboard.write((const uint8_t*)"?", 1); break;
            case NG_2   : Keyboard.write((const uint8_t*)"/", 1); break;
            case NG_3   : Keyboard.write((const uint8_t*)"~", 1); break;
            case NG_4   : Keyboard.write((const uint8_t*)"[", 1); break;
            case NG_5   : Keyboard.write((const uint8_t*)"]", 1); break;
            case NG_6   : Keyboard.write((const uint8_t*)"{", 1); break;
            case NG_7   : Keyboard.write((const uint8_t*)"}", 1); break;
            case NG_8   : Keyboard.write((const uint8_t*)"*", 1); break;
            case NG_9   : Keyboard.write((const uint8_t*)"(", 1); break;
            case NG_0   : Keyboard.write((const uint8_t*)")", 1); break;
            case NG_MINS: Keyboard.write((const uint8_t*)"_", 1); break;
            case NG_EQL : Keyboard.write((const uint8_t*)"+", 1); break;

            case NG_Q   : Keyboard.write((const uint8_t*)"xa", 2); break;
            case NG_W   : Keyboard.write((const uint8_t*)"e" , 1); break;
            case NG_E   : Keyboard.write((const uint8_t*)"ri", 2); break;
            case NG_R   : Keyboard.write((const uint8_t*)"xya", 3);break;
            case NG_T   : Keyboard.write((const uint8_t*)"re", 2); break;
            case NG_Y   : Keyboard.write((const uint8_t*)"pa", 2); break;
            case NG_U   : Keyboard.write((const uint8_t*)"di", 2); break;
            case NG_I   : Keyboard.write((const uint8_t*)"gu", 2); break;
            case NG_O   : Keyboard.write((const uint8_t*)"du", 2); break;
            case NG_P   : Keyboard.write((const uint8_t*)"pi", 2); break;
            case NG_LBRC: Keyboard.write((const uint8_t*)"[" , 1); break;
            case NG_RBRC: Keyboard.write((const uint8_t*)"]" , 1); break;
            case NG_BSLS: Keyboard.write((const uint8_t*)"\\", 1); break;

            case NG_A   : Keyboard.write((const uint8_t*)"wo", 2); break;
            case NG_S   : Keyboard.write((const uint8_t*)"a" , 1); break;
            case NG_D   : Keyboard.write((const uint8_t*)"na", 2); break;
            case NG_F   : Keyboard.write((const uint8_t*)"xyu", 3);break;
            case NG_G   : Keyboard.write((const uint8_t*)"mo", 2); break;
            case NG_H   : Keyboard.write((const uint8_t*)"ba", 2); break;
            case NG_J   : Keyboard.write((const uint8_t*)"do", 2); break;
            case NG_K   : Keyboard.write((const uint8_t*)"gi", 2); break;
            case NG_L   : Keyboard.write((const uint8_t*)"po", 2); break;
            case NG_SCLN:                    break;
            case NG_QUOT: Keyboard.write((const uint8_t*)"\xb2", 1); break;

            case NG_Z   : Keyboard.write((const uint8_t*)"xu", 2); break;
            case NG_X   : Keyboard.write((const uint8_t*)"-" , 1); break;
            case NG_C   : Keyboard.write((const uint8_t*)"ro", 2); break;
            case NG_V   : Keyboard.write((const uint8_t*)"ya", 2); break;
            case NG_B   : Keyboard.write((const uint8_t*)"xi", 2); break;
            case NG_N   : Keyboard.write((const uint8_t*)"pu", 2); break;
            case NG_M   : Keyboard.write((const uint8_t*)"zo", 2); break;
            case NG_COMM: Keyboard.write((const uint8_t*)"pe", 2); break;
            case NG_DOT : Keyboard.write((const uint8_t*)"bo", 2); break;
            case NG_SLSH: Keyboard.write((const uint8_t*)"?" , 1); break;
        }
    } else if(nicola_o_key == NG_SHFTR) {
        switch(nicola_m_key) {
            case NG_1   : Keyboard.write((const uint8_t*)"?", 1); break;
            case NG_2   : Keyboard.write((const uint8_t*)"/", 1); break;
            case NG_3   : Keyboard.write((const uint8_t*)"~", 1); break;
            case NG_4   : Keyboard.write((const uint8_t*)"[", 1); break;
            case NG_5   : Keyboard.write((const uint8_t*)"]", 1); break;
            case NG_6   : Keyboard.write((const uint8_t*)"{", 1); break;
            case NG_7   : Keyboard.write((const uint8_t*)"}", 1); break;
            case NG_8   : Keyboard.write((const uint8_t*)"*", 1); break;
            case NG_9   : Keyboard.write((const uint8_t*)"(", 1); break;
            case NG_0   : Keyboard.write((const uint8_t*)")", 1); break;
            case NG_MINS: Keyboard.write((const uint8_t*)"_", 1); break;
            case NG_EQL : Keyboard.write((const uint8_t*)"+", 1); break;

            case NG_Q   :                    break;
            case NG_W   : Keyboard.write((const uint8_t*)"ga", 2); break;
            case NG_E   : Keyboard.write((const uint8_t*)"da", 2); break;
            case NG_R   : Keyboard.write((const uint8_t*)"go", 2); break;
            case NG_T   : Keyboard.write((const uint8_t*)"za", 2); break;
            case NG_Y   : Keyboard.write((const uint8_t*)"yo", 2); break;
            case NG_U   : Keyboard.write((const uint8_t*)"ni", 2); break;
            case NG_I   : Keyboard.write((const uint8_t*)"ru", 2); break;
            case NG_O   : Keyboard.write((const uint8_t*)"ma", 2); break;
            case NG_P   : Keyboard.write((const uint8_t*)"xe", 2); break;
            case NG_LBRC: Keyboard.write((const uint8_t*)"[" , 1); break;
            case NG_RBRC: Keyboard.write((const uint8_t*)"]" , 1); break;
            case NG_BSLS: Keyboard.write((const uint8_t*)"\\", 1); break;

            case NG_A   : Keyboard.write((const uint8_t*)"vu", 2); break;
            case NG_S   : Keyboard.write((const uint8_t*)"zi", 2); break;
            case NG_D   : Keyboard.write((const uint8_t*)"de", 2); break;
            case NG_F   : Keyboard.write((const uint8_t*)"ge", 2); break;
            case NG_G   : Keyboard.write((const uint8_t*)"ze", 2); break;
            case NG_H   : Keyboard.write((const uint8_t*)"mi", 2); break;
            case NG_J   : Keyboard.write((const uint8_t*)"o" , 1); break;
            case NG_K   : Keyboard.write((const uint8_t*)"no", 2); break;
            case NG_L   : Keyboard.write((const uint8_t*)"xyo", 3);break;
            case NG_SCLN: Keyboard.write((const uint8_t*)"xtu", 3);break;
            case NG_QUOT: Keyboard.write((const uint8_t*)"'" , 1); break;

            case NG_Z   :                    break;
            case NG_X   : Keyboard.write((const uint8_t*)"bi", 2); break;
            case NG_C   : Keyboard.write((const uint8_t*)"zu", 2); break;
            case NG_V   : Keyboard.write((const uint8_t*)"bu", 2); break;
            case NG_B   : Keyboard.write((const uint8_t*)"be", 2); break;
            case NG_N   : Keyboard.write((const uint8_t*)"nu", 2); break;
            case NG_M   : Keyboard.write((const uint8_t*)"yu", 2); break;
            case NG_COMM: Keyboard.write((const uint8_t*)"mu", 2); break;
            case NG_DOT : Keyboard.write((const uint8_t*)"wa", 2); break;
            case NG_SLSH: Keyboard.write((const uint8_t*)"xo", 2); break;
        }
    }
}

// 親指シフトの入力処理
bool process_nicola(uint16_t keycode, bool pressed) {
  bool cont_process = true;
  // if (!is_nicola || n_modifier > 0) return true;
  uint16_t curr_time = millis();

  if (pressed) {
    if(NG_M_TOP <= keycode && keycode <= NG_M_BOTTOM) {
        // M key
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            // no timeout check
            nicola_int_state = NICOLA_STATE_S2_M;
            break;
          case NICOLA_STATE_S2_M:
            // same behavior for time out and sequential stroke
            nicola_m_type();
            break;
          case NICOLA_STATE_S3_O:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_o_time > TIMEOUT_THRESHOLD) {
              // timeout => (output O) => S2
              nicola_o_type();
              nicola_int_state = NICOLA_STATE_S2_M;
            } else {
              // combo => S5
              nicola_int_state = NICOLA_STATE_S5_OM;
            }
            break;
          case NICOLA_STATE_S4_MO:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_o_time > TIMEOUT_THRESHOLD) {
              // timeout => (output MO) => S2
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S2_M;
            } else {
              // combo => three key judge
              uint16_t t1 = nicola_o_time - nicola_m_time;
              uint16_t t2 = curr_time - nicola_o_time;
              if(t1 < t2) {
                // the O key in between is combo with the leading M key
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S2_M;
              } else {
                // the leading M key is single, the O key in between is combo with current key
                nicola_m_type();
                nicola_int_state = NICOLA_STATE_S5_OM;
              }
            }
            break;
          case NICOLA_STATE_S5_OM:
            // same behavior for time out and sequential stroke
            nicola_om_type();
            nicola_int_state = NICOLA_STATE_S2_M;
            break;
        }
        nicola_m_key = keycode;
        nicola_m_time = curr_time;
        cont_process = false;
    } else if(keycode == NG_SHFTL || keycode == NG_SHFTR) {
        // O key
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            // no timeout check
            nicola_int_state = NICOLA_STATE_S3_O;
            break;
          case NICOLA_STATE_S2_M:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_m_time > TIMEOUT_THRESHOLD) {
              // timeout => (output M) => S3
              nicola_m_type();
              nicola_int_state = NICOLA_STATE_S3_O;
            } else {
              // combo => S4
              nicola_int_state = NICOLA_STATE_S4_MO;
            }
            break;
          case NICOLA_STATE_S3_O:
            // same behavior for time out and sequential stroke
            nicola_o_type();
            break;
          case NICOLA_STATE_S4_MO:
            // same behavior for time out and sequential stroke
            nicola_om_type();
            nicola_int_state = NICOLA_STATE_S3_O;
            break;
          case NICOLA_STATE_S5_OM:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_m_time > TIMEOUT_THRESHOLD) {
              // timeout => (output MO) => S3
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S3_O;
            } else {
              // combo => three key judge
              uint16_t t1 = nicola_m_time - nicola_o_time;
              uint16_t t2 = curr_time - nicola_m_time;
              if(t1 < t2) {
                // the M key in between is combo with the leading O key
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S3_O;
              } else {
                // the leading O key is single, the M key in between is combo with current key
                nicola_o_type();
                nicola_int_state = NICOLA_STATE_S4_MO;
              }
            }
            break;
        }
        nicola_o_key = keycode;
        nicola_o_time = curr_time;
        cont_process = false;
    } else {
        // その他のキーが押された
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            break;
          case NICOLA_STATE_S2_M:
            nicola_m_type();
            break;
          case NICOLA_STATE_S3_O:
            nicola_o_type();
            break;
          case NICOLA_STATE_S4_MO:
            nicola_om_type();
            break;
          case NICOLA_STATE_S5_OM:
            nicola_om_type();
            break;
        }
        nicola_int_state = NICOLA_STATE_S1_INIT;
        // continue processing current key, so this path returns true
    }
  } else { // key release
    if(NG_TOP <= keycode && keycode <= NG_BOTTOM) { // key off
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            break;
          case NICOLA_STATE_S2_M:
            if(nicola_m_key == keycode) {
              nicola_m_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
          case NICOLA_STATE_S3_O:
            if(nicola_o_key == keycode) {
              nicola_o_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
          case NICOLA_STATE_S4_MO:
            if(nicola_m_key == keycode) {
              // M ON --> O ON --> M OFF
              uint16_t t1 = nicola_o_time - nicola_m_time;
              uint16_t t2 = curr_time - nicola_o_time;
              if(t1>=t2 && t2 < OVERLAP_THRESHOLD) {
                // M ON --> O ON --> M OFF (M is output, but O is still open to combo)
                nicola_m_type();
                nicola_int_state = NICOLA_STATE_S3_O;
              } else {
                // M ON --> O ON --> M OFF (both M and O are output)
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S1_INIT;
              }
            } else if(nicola_o_key == keycode) {
              // M ON --> O ON --> O OFF (both M and O are output)
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
          case NICOLA_STATE_S5_OM:
            if(nicola_o_key == keycode) {
              // O ON --> M ON --> O OFF
              uint16_t t1 = nicola_m_time - nicola_o_time;
              uint16_t t2 = curr_time - nicola_m_time;
              if(t1>=t2 && t2 < OVERLAP_THRESHOLD) {
                // O ON --> M ON --> O OFF (O is output, but M is still open to combo)
                nicola_o_type();
                nicola_int_state = NICOLA_STATE_S2_M;
              } else {
                // O ON --> M ON --> O OFF (both M and O are output)
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S1_INIT;
              }
            } else if(nicola_m_key == keycode) {
              // O ON --> M ON --> M OFF (both O and M are output)
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
        }
        cont_process = false;
    }
  }
  return cont_process;
}
