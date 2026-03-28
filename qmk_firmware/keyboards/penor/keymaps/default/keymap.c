// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// PENOR: Portable ergonomic NKRO ortholinear receiver

#include QMK_KEYBOARD_H
#include <keymap_swedish.h>
#include <sendstring_swedish.h>



enum layer_number {_MAIN = 0, _GAMING, _SYMBOL, _NAV, _MOUSE, _STENO};

enum custom_keycodes {
    MANUAL = SAFE_RANGE,
    STENO_TOGGLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Note that this is designed as a Swedish layout!

    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* MAIN
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤  W  │  F  │  P  │  B  │           │  J  │  L  │  U  │  Y  ├─────┬─────┐
     * │ TAB │  Q  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  Ö  │  Å  │
     * ├─────┼─────┤  R  │  S  │  T  │  G  │           │  M  │  N  │  E  │  I  ├─────┼─────┤
     * │L-SFT│  A  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  O  │STENO│
     * ├─────┼─────┤  X  │  C  │  D  │  V  │           │  K  │  H  │  ,  │  .  ├─────┼─────┤
     * │L-CTL│  Z  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤ ESC │  Ä  │
     * └─────┴─────┘  │L-ALT│S-GUI│ NAV │SPACE│     │ENTER│SYMBL│BACKS│R-ALT│  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_MAIN] = LAYOUT(
        KC_TAB,  SE_Q, SE_W, SE_F, SE_P, SE_B,       SE_J, SE_L, SE_U,    SE_Y,    SE_ODIA, SE_ARNG,
        KC_LSFT, SE_A, SE_R, SE_S, SE_T, SE_G,       SE_M, SE_N, SE_E,    SE_I,    SE_O,    STENO_TOGGLE,
        KC_LCTL, KC_Z, SE_X, SE_C, SE_D, SE_V,       SE_K, SE_H, SE_COMM, SE_DOT,  KC_ESC,  SE_ADIA,
        KC_LALT, KC_LGUI, QK_TRI_LAYER_UPPER, KC_SPC,KC_ENT, QK_TRI_LAYER_LOWER, KC_BSPC, KC_RALT),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* GAMING
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤  Q  │  W  │  E  │  R  │           │  Y  │  U  │  I  │  O  ├─────┬─────┐
     * │ TAB │  T  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  P  │  Å  │
     * ├─────┼─────┤  A  │  S  │  D  │  F  │           │  H  │  J  │  K  │  L  ├─────┼─────┤
     * │L-SFT│  G  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  Ö  │STENO│
     * ├─────┼─────┤  Z  │  X  │  C  │  V  │           │  N  │  M  │  ,  │  .  ├─────┼─────┤
     * │L-CTL│  B  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤ ESC │  Ä  │
     * └─────┴─────┘  │L-ALT│S-GUI│ NAV │SPACE│     │ENTER│SYMBL│BACKS│R-ALT│  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_GAMING] = LAYOUT(
        KC_TAB,  SE_T, SE_Q, SE_W, SE_E, SE_R,       SE_Y, SE_U, SE_I,    SE_O,    SE_P,    SE_ARNG,
        KC_LSFT, SE_G, SE_A, SE_S, SE_D, SE_F,       SE_H, SE_J, SE_K,    SE_L,    SE_ODIA, STENO_TOGGLE,
        KC_LCTL, KC_B, SE_Z, SE_X, SE_C, SE_V,       SE_N, SE_M, SE_COMM, SE_DOT,  KC_ESC,  SE_ADIA,
        KC_LALT, KC_LGUI, QK_TRI_LAYER_UPPER, KC_SPC,KC_ENT, QK_TRI_LAYER_LOWER, KC_BSPC, KC_RALT),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* SYMBOL
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤  |  │  ^  │  <  │  {  │           │  }  │  >  │  -  │  +  ├─────┬─────┐
     * │  §  │  &  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  `  │  ´  │
     * ├─────┼─────┤  '  │  \  │  .  │  (  │           │  )  │  ;  │  /  │  *  ├─────┼─────┤
     * │CAPS │  #  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  !  │  $  │
     * ├─────┼─────┤  "  │  ~  │  ,  │  [  │           │  ]  │  :  │  _  │  =  ├─────┼─────┤
     * │     │  %  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤  ?  │  @  │
     * └─────┴─────┘  │     │     │MOUSE│     │     │     │     │     │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_SYMBOL] = LAYOUT(
        SE_SECT, SE_AMPR, SE_PIPE, SE_CIRC, SE_LABK, SE_LCBR, SE_RCBR, SE_RABK, SE_MINS, SE_PLUS, SE_GRV, SE_ACUT,
        KC_CAPS, SE_HASH, SE_QUOT, SE_BSLS, SE_DOT, SE_LPRN, SE_RPRN, SE_SCLN, SE_SLSH, SE_ASTR, SE_EXLM, SE_DLR,
        _______, SE_PERC, SE_DQUO, SE_TILD, SE_COMM, SE_LBRC, SE_RBRC, SE_COLN, SE_UNDS, SE_EQL, SE_QUES, SE_AT,
                        _______, _______, _______, _______, _______, _______, _______, _______),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* NAV
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤HOME │  ↑  │ END │  ▢↑ │           │     │INSRT│AUTOL│AUTOR├─────┬─────┐
     * │     │  ⬍↑ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  ◉  │DELAY│
     * ├─────┼─────┤  ←  │  ↓  │  →  │  ▢↓ │           │Ma/Ga│ DEL │ARMUL│  .  ├─────┼─────┤
     * │     │  ⬍↓ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  ▶  │     │
     * ├─────┼─────┤  2  │  3  │  4  │  5  │           │  6  │  7  │  8  │  9  ├─────┼─────┤
     * │     │  1  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤  0  │     │
     * └─────┴─────┘  │     │     │     │     │     │     │MOUSE│     │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_NAV] = LAYOUT(
        _______, KC_WH_U, KC_HOME, KC_UP, KC_END, KC_PGUP, _______, KC_INS, _______, _______, _______, _______,
        _______, KC_WH_D, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, _______, KC_DEL, _______, SE_DOT, _______, _______,
        _______, SE_1, SE_2, SE_3, SE_4, SE_5, SE_6, SE_7, SE_8, SE_9, SE_0, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* MOUSE
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤ ◀▮  │  ▮↑ │  ▮▶ │  ▢↑ │           │Ma/Ga│PRSCR│SCRLL│PAUS ├─────┬─────┐
     * │     │  ⬍↑ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤     │ MAN │
     * ├─────┼─────┤ ←▮  │  ▮↓ │  ▮→ │  ▢↓ │           │RESET│  ◧  │  ◨  │  ▣  ├─────┼─────┤
     * │L-SFT│  ⬍↓ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤     │ F12 │
     * ├─────┼─────┤ F2  │ F3  │ F4  │ F5  │           │ F6  │ F7  │ F8  │ F9  ├─────┼─────┤
     * │L-CTL│ F1  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤ F10 │ F11 │
     * └─────┴─────┘  │     │     │     │     │     │     │     │     │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_MOUSE] = LAYOUT(
        _______, KC_WH_U, KC_BTN4, KC_MS_U, KC_BTN5, KC_PGUP, TG(_GAMING), KC_PSCR, KC_SCRL, KC_PAUS, _______, _______,
        KC_LSFT, KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, KC_PGDN, _______, KC_BTN1, KC_BTN2, KC_BTN3, _______, KC_F12,
        KC_LCTL, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,
        _______, _______, _______, _______, _______, _______, _______, _______),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* STENO
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤  W  │  E  │  R  │  T  │           │  Y  │  U  │  I  │  O  ├─────┬─────┐
     * │     │  Q  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  P  │  Z  │
     * ├─────┼─────┤  S  │  D  │  F  │  G  │           │  H  │  J  │  K  │  L  ├─────┼─────┤
     * │     │  A  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  X  │  B  │
     * ├─────┼─────┤     │     │     │     │           │     │     │     │     ├─────┼─────┤
     * │     │     ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤     │     │
     * └─────┴─────┘  │     │     │  C  │  V  │     │  N  │  M  │MAIN │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_STENO] = LAYOUT(
        _______, SE_Q, SE_W, SE_E, SE_R, SE_T, SE_Y, SE_U, SE_I, SE_O, SE_P, SE_Z,
        _______, SE_A, SE_S, SE_D, SE_F, SE_G, SE_H, SE_J, SE_K, SE_L, SE_X, SE_B,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                          _______, _______, SE_C, SE_V, SE_N, SE_M, STENO_TOGGLE, _______),
};



void keyboard_post_init_user(void) {
    // enable n-key rollover on startup
    keymap_config.nkro = 1;
}



// do stuff whenever keys get pressed down
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) { // if any key was pressed down

        if (keycode == STENO_TOGGLE) {
            layer_invert(_STENO); // toggle the steno layer
            return false;
        }
    }

    return true;
}
