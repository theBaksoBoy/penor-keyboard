// Copyright 2026 baksoBoy (@theBaksoBoy)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

#define SPLIT_HAND_PIN GP29

#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_SERIAL
#define SPLIT_USB_DETECT

// specify the keys used for bootmagic. On the left half it is the top-left key. On the right half it is the top-right key
#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 0
#define BOOTMAGIC_ROW_RIGHT 4
#define BOOTMAGIC_COLUMN_RIGHT 5

#define QUICK_TAP_TERM 0
#define TAPPING_TERM 175

// Mouse key speed and acceleration.
#define MOUSEKEY_INTERVAL       16
#define MOUSEKEY_MAX_SPEED      12
#define MOUSEKEY_TIME_TO_MAX    80

// stuff for Tri Layers
#define TRI_LAYER_LOWER_LAYER   2
#define TRI_LAYER_UPPER_LAYER   3
#define TRI_LAYER_ADJUST_LAYER  4
