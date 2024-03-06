/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0x504B
#define PRODUCT_ID      0x4A59
#define MANUFACTURER    "Shiitake"
#define PRODUCT "ergogridgo2"

#define BMP_BOOTPIN_AS_RESET
/* key matrix size */
#define MATRIX_ROWS_DEFAULT 9
#define MATRIX_COLS_DEFAULT 6
#define THIS_DEVICE_ROWS 9
#define THIS_DEVICE_COLS 6

// // wiring of each half
#define MATRIX_ROW_PINS { F4, F5, F6, F7, B1, B3, B2, D3, D2 }
#define MATRIX_COL_PINS { D4, C6, D7, E6, B4, B5 }

#define DIODE_DIRECTION COL2ROW

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
