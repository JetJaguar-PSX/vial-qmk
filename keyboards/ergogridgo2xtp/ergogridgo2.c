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

#include "ergogridgo2.h"
#include "bmp.h"
#include "i2c_mod.h"
#include "bmp_host_driver.h"
#include "pointing_device.h"
#include "iqs5xx.h"

void send_mouse(report_mouse_t *report);
// int hktkb_cnt = 0;
bool init_flag = true;
bool check_flag = true;
uint32_t touch_time = 0;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    bool cont = process_record_bmp(keycode, record);

    if (cont) {
        process_record_user(keycode, record);
    }

    return cont;
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
    if (*command_id == id_set_keyboard_value) {
        // This version does not handle save flag
    } else {
        *command_id = id_unhandled;
    }
}


static void dummy_func(uint8_t btn){};
void (*pointing_device_set_button)(uint8_t btn) = dummy_func;
void (*pointing_device_clear_button)(uint8_t btn) = dummy_func;

bool mouse_send_flag = false;

keyevent_t get_u_3 = {
    .key = (keypos_t){.row = 5, .col = 0},
    .pressed = false
};

keyevent_t get_d_3 = {
    .key = (keypos_t){.row = 5, .col = 1},
    .pressed = false
};

keyevent_t get_r_2 = {
    .key = (keypos_t){.row = 6, .col = 0},
    .pressed = false
};

keyevent_t get_r_3 = {
    .key = (keypos_t){.row = 6, .col = 1},
    .pressed = false
};

keyevent_t get_l_2 = {
    .key = (keypos_t){.row = 6, .col = 2},
    .pressed = false
};

keyevent_t get_l_3 = {
    .key = (keypos_t){.row = 6, .col = 3},
    .pressed = false
};

keyevent_t get_i_2 = {
    .key = (keypos_t){.row = 7, .col = 0},
    .pressed = false
};

keyevent_t get_o_2 = {
    .key = (keypos_t){.row = 7, .col = 1},
    .pressed = false
};

keyevent_t get_t_3 = {
    .key = (keypos_t){.row = 7, .col = 2},
    .pressed = false
};


void gesture_press_key(keyevent_t k) {
    k.pressed = true;
    k.time = (timer_read() | 1);
    action_exec(k);   
    k.pressed = false;
    k.time = (timer_read() | 1);
    action_exec(k);  
    ges_time = timer_read32();
    hold_drag_mode = false;
}

// void test1(void){
//     hktkb_cnt++;
//     if(hktkb_cnt >= 8){
//         iqs5xx_data_t iqs5xx_data;
//         bool is_valid = false;

//         is_valid = read_iqs5xx(&iqs5xx_data);

//         if (is_valid) {
//             if(iqs5xx_data.finger_cnt >= 1){
//                 if (!BMPAPI->app.has_schedule_in_range(0, MAINTASK_INTERVAL << 1)) {
//                     BMPAPI->app.schedule_next_task(MAINTASK_INTERVAL);
//                 }
//             }
//         }
//         hktkb_cnt = 0;
//     }
// }

void housekeeping_task_kb(void) {
    // hktkb_cnt++;
    // if(hktkb_cnt >= 10000){
    //     BMPAPI->bootloader_jump();
    // }
    if(timer_elapsed32(touch_time) >= MAINTASK_INTERVAL * 2){
    // if(hktkb_cnt >= 17){
        // if(init_flag){
        //     if(!i2c_init()){
        //         init_flag = false;
        //     }
        // }
        // if(check_flag){
        //     if(check_iqs5xx()){
        //         if(!init_iqs5xx()){
        //             check_flag = false;
        //         }
        //     }
        // }else{
            iqs5xx_data_t iqs5xx_data;
            bool is_valid = false;

            is_valid = read_iqs5xx(&iqs5xx_data);
            if(!is_valid){
                read_iqs5xx(&iqs5xx_data);
            }
            if (is_valid) {
                if(iqs5xx_data.finger_cnt >= 1){
                    BMPAPI->app.schedule_next_task(MAINTASK_INTERVAL * 2);
                }
            }
        // }
        // hktkb_cnt = 0;
        touch_time = timer_read32();
    }
}

report_mouse_t mouse_rep = {0};

bool pointing_device_send(void) {
    if(mouse_send_flag){
        send_mouse(&mouse_rep);
    }
    return false;
}

bool pointing_device_task(void) {
    if (mouse_send_flag) {
        pointing_device_send();
        mouse_send_flag = false;
        return true;
    }
    return false;
}

void keyboard_pre_init_kb() {
    // if(is_usb_connected()){
        if(!i2c_init()){
            init_flag = false;
        }
    // }
}

void keyboard_post_init_kb() {
    // if(is_usb_connected()){
        if(check_iqs5xx()){
            if(!init_iqs5xx()){
                check_flag = false;
            }
        }
    // }
    pointing_device_set_button = pointing_device_set_button_iqs5xx;
    pointing_device_clear_button = pointing_device_clear_button_iqs5xx;
    ges_time = timer_read32();
    hold_drag_time = timer_read32();
    hold_drag_mode = false;
    touch_time = timer_read32();
    wait_ms(300);    
    keyboard_post_init_user();
}

void matrix_scan_kb() {
    iqs5xx_data_t iqs5xx_data;
    bool is_valid = false;

    is_valid = read_iqs5xx(&iqs5xx_data);

    if (is_valid) {
        static iqs5xx_processed_data_t iqs5xx_processed_data;
        static iqs5xx_gesture_data_t iqs5xx_gesture_data;
        bool send_flag = process_iqs5xx(&iqs5xx_data, &iqs5xx_processed_data, &mouse_rep, &iqs5xx_gesture_data);
        bool is_passed_ges_timer = timer_elapsed32(ges_time) > GES_TIME_MS;
        if(iqs5xx_processed_data.tap_cnt == 3) {
            gesture_press_key(get_t_3);
        } 
        switch (iqs5xx_gesture_data.multi.gesture_state) {
            case GESTURE_SWIPE_U:
                if(iqs5xx_data.finger_cnt == 2){
                    mouse_rep.v = 1;
                    send_flag = true;
                } else if (is_passed_ges_timer && iqs5xx_data.finger_cnt == 3) {
                    gesture_press_key(get_u_3);
                }
                break;
            case GESTURE_SWIPE_D:
                if(iqs5xx_data.finger_cnt == 2){
                    mouse_rep.v = -1;
                    send_flag = true;
                } else if (is_passed_ges_timer && iqs5xx_data.finger_cnt == 3) {
                    gesture_press_key(get_d_3);
                }
                break;
            case GESTURE_SWIPE_R:
                if(is_passed_ges_timer){
                    if(iqs5xx_data.finger_cnt == 2){
                        gesture_press_key(get_r_2);
                    } else if(iqs5xx_data.finger_cnt == 3){
                        gesture_press_key(get_r_3);
                    }
                }
                break;
            case GESTURE_SWIPE_L:
                if(is_passed_ges_timer){
                    if(iqs5xx_data.finger_cnt == 2){
                        gesture_press_key(get_l_2);
                    } else if(iqs5xx_data.finger_cnt == 3){
                        gesture_press_key(get_l_3);
                    }
                }
                break;
            case GESTURE_PINCH_IN:
                if(is_passed_ges_timer){
                    gesture_press_key(get_i_2);
                }
                break;
            case GESTURE_PINCH_OUT:
                if(is_passed_ges_timer){
                    gesture_press_key(get_o_2);
                }
                break;
            default:
                break;
        }
        if (send_flag) {
            mouse_send_flag = true;
        }
    }
}
