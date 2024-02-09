/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0x504B
#define PRODUCT_ID      0x4A58
#define MANUFACTURER    "Shiitake"
#define PRODUCT "ergogridgo"

#define BMP_BOOTPIN_AS_RESET
/* key matrix size */
#define MATRIX_ROWS_DEFAULT 7
#define MATRIX_COLS_DEFAULT 9
#define THIS_DEVICE_ROWS 7
#define THIS_DEVICE_COLS 9

// // wiring of each half
#define MATRIX_ROW_PINS { 20, 19, 18, 17, 16, 15, 14 }
#define MATRIX_COL_PINS { 1, 2, 5, 6, 7, 8, 9, 10, 11 }

// #define IS_LEFT_HAND  true
#define BMP_DEFAULT_MODE SINGLE
#define ACTION_DEBUG
// /* key matrix size */

#define DIODE_DIRECTION COL2ROW

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define RGBLED_NUM_DEFAULT 128
