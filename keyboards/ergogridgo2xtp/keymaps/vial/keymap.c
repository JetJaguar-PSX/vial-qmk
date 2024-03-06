/* Copyright 2021 daraku-neko
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
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "iqs5xx.h"

typedef union {
  uint32_t raw;
  struct {
    bool     init : 1;
    bool     tap : 1;
    bool     drag_mode : 1;
    uint32_t drag_time : 12;
    bool     auto_trackpad_layer : 1;
  };
} user_config_t;
user_config_t user_config;

// Defines names for use in layer keycodes and the keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                KC_ESC, 
                KC_Q, 
                KC_W, 
                KC_E, 
                KC_R, 
                KC_T, 
                KC_7, 
                KC_8, 
                KC_9, 
                KC_Y, 
                KC_U,
                KC_I, 
                KC_O, 
                KC_P, 
                KC_DELETE, 
                KC_TAB, 
                KC_A, 
                KC_S, 
                KC_D, 
                KC_F, 
                KC_G, 
                KC_4, 
                KC_5, 
                KC_6, 
                KC_H, 
                KC_J, 
                KC_K, 
                KC_L, 
                KC_BACKSPACE, 
                KC_MINUS, 
                KC_LSFT, 
                KC_Z, 
                KC_X, 
                KC_C, 
                KC_V, 
                KC_B, 
                KC_1, 
                KC_2, 
                KC_3, 
                KC_N, 
                KC_M, 
                KC_COMMA, 
                KC_DOT, 
                KC_ENTER, 
                KC_RSFT, 
                KC_LCTL, 
                KC_INTERNATIONAL_5, 
                KC_LGUI, 
                KC_EQUAL, 
                DB_TOGG, 
                KC_SLASH, 
                KC_SPACE, 
                KC_INTERNATIONAL_4, 
                KC_LALT
    ),
    [1] = LAYOUT(
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO
    ),
    [2] = LAYOUT(
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO
    ),
    [3] = LAYOUT(
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO, 
                KC_NO
    )
};

// keyevent_t encoder1_ccw = {
//     .key = (keypos_t){.row = 4, .col = 2},
//     .pressed = false
// };

// keyevent_t encoder1_cw = {
//     .key = (keypos_t){.row = 4, .col = 3},
//     .pressed = false
// }; 

// void matrix_scan_user(void) {
//     if (IS_PRESSED(encoder1_ccw)) {
//         encoder1_ccw.pressed = false;
//         encoder1_ccw.time = (timer_read() | 1);
//         action_exec(encoder1_ccw);
//     }

//     if (IS_PRESSED(encoder1_cw)) {
//         encoder1_cw.pressed = false;
//         encoder1_cw.time = (timer_read() | 1);
//         action_exec(encoder1_cw);
//     }
// }

bool is_hold = false;

// void encoder_layer_up(void) { 
//   if (get_highest_layer(layer_state|default_layer_state) == 3 ) {
//     layer_clear();
//   } else {
//     layer_move(get_highest_layer(layer_state)+1); 
//   }
// } 

// void encoder_layer_down(void) { 
//     if (get_highest_layer(layer_state|default_layer_state) == 0 ) {
//       layer_move(3);
//     } else {
//       layer_move(get_highest_layer(layer_state)-1); 
//     }
// }  

// void encoder_ccw(void) {
//   encoder1_ccw.pressed = true;
//   encoder1_ccw.time = (timer_read() | 1);
//   is_hold ? encoder_layer_down() : action_exec(encoder1_ccw);
// }

// void encoder_cw(void) {
//   encoder1_cw.pressed = true;
//   encoder1_cw.time = (timer_read() | 1);
//   is_hold ? encoder_layer_up() : action_exec(encoder1_cw);
// }

// bool encoder_update_user(uint8_t index, bool clockwise) {
//     if (index == 0) { clockwise ? encoder_cw() : encoder_ccw(); }
//     return true;
// }

void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  if(!user_config.init) {
    user_config.init = true;  
    user_config.tap = false; 
    user_config.drag_mode = true;  
    user_config.drag_time = 700;
    eeconfig_update_user(user_config.raw); 
  }
  tap_mode = user_config.tap;
  is_drag_mode = user_config.drag_mode;  
  drag_time = user_config.drag_time; 
}

void send_setting_string(int i){
  char buf[12]; 
  snprintf(buf, sizeof(buf), "%d", i);
  const char *s = buf;
  send_string(s);
}

void update_drag_time(uint32_t dt){
  user_config.drag_time = dt;
  eeconfig_update_user(user_config.raw); 
  drag_time = user_config.drag_time;
  send_setting_string(dt);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t pressed_time = 0;
  switch (keycode) {  
      case KC_BTN1 ... KC_BTN5:
        if (record->event.pressed) {
          pointing_device_set_button(1 << (keycode - KC_BTN1));
        } else {
          pointing_device_clear_button(1 << (keycode - KC_BTN1));
        }
      return false;
      break;
      case KC_F20: 
        if (record->event.pressed) {
          user_config.drag_mode = !is_drag_mode;  
          eeconfig_update_user(user_config.raw); 
          is_drag_mode = user_config.drag_mode;        
        }
        return false;             
      case KC_F21: 
        if (record->event.pressed) {
          drag_time = drag_time + 10;
          if(drag_time > 3000) {
              drag_time = 3000;
          }
          update_drag_time(drag_time);
        }
        return false;
      case KC_F22: 
        if (record->event.pressed) {
          if(drag_time != 0) {
            drag_time = drag_time - 10;
          }
          update_drag_time(drag_time);
        }
        return false;   
      case KC_F23:
        if (record->event.pressed) {    
          user_config.tap = !user_config.tap;  
          eeconfig_update_user(user_config.raw); 
          tap_mode = user_config.tap;
        } 
      return false;             
      case KC_F24:
        if (record->event.pressed) {    
          pressed_time = record->event.time;
          if(!is_hold){
            // encoder_layer_up();
          }
          is_hold = false;
        } else {
          if((record->event.time - pressed_time) > TAPPING_TERM) {
            is_hold = true;
          }
        }
      return false;
 
    default:
      return true;
  }
};
