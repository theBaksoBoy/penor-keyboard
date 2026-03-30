// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// PENOR: Portable ergonomic NKRO ortholinear receiver

#include QMK_KEYBOARD_H
#include <keymap_swedish.h>
#include <sendstring_swedish.h>



enum layer_number {_MAIN = 0, _GAMING, _SYMBOL, _NAV, _MOUSE, _STENO};

enum custom_keycodes {
    CK_MANUAL = SAFE_RANGE,
    CK_STENO_TOGGLE,
    CK_KEY_MULTIPLICATION,
    CK_MACRO_DELAY,
};



bool key_multiplication_mode_active = false; // are you in the mode where you type out numbers to determine how many times a key should repeat?
unsigned int key_multiplication_count = 0; // how many times should a key be made to repeat?
#define MAX_KEY_MULTIPLICATION_COUNT 100

int auto_clicker_click_count = 0; // if 0 then don't click. if > 0 then click and decrement value. if < 0 then keep clicking endlessly
uint16_t auto_clicker_timer = 0;
unsigned int auto_clicker_button = 1; // what mouse button should the auto clicker use? 1 == KC_BTN1, 2 == KC_BTN2, 3 == KC_BTN3
#define AUTO_CLICKER_MILLISECONDS_PER_CLICK 34 // 30 clicks per second



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
        KC_LSFT, SE_A, SE_R, SE_S, SE_T, SE_G,       SE_M, SE_N, SE_E,    SE_I,    SE_O,    CK_STENO_TOGGLE,
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
        KC_LSFT, SE_G, SE_A, SE_S, SE_D, SE_F,       SE_H, SE_J, SE_K,    SE_L,    SE_ODIA, CK_STENO_TOGGLE,
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
     * ┌─────┬─────┤HOME │  ↑  │ END │  ▢↑ │           │     │INSRT│     │     ├─────┬─────┐
     * │     │  ⬍↑ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  ◉  │DELAY│
     * ├─────┼─────┤  ←  │  ↓  │  →  │  ▢↓ │           │Ma/Ga│ DEL │KEYML│  .  ├─────┼─────┤
     * │     │  ⬍↓ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  ▶  │     │
     * ├─────┼─────┤  2  │  3  │  4  │  5  │           │  6  │  7  │  8  │  9  ├─────┼─────┤
     * │     │  1  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤  0  │     │
     * └─────┴─────┘  │     │     │     │     │     │     │MOUSE│     │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_NAV] = LAYOUT(
        _______, KC_WH_U, KC_HOME, KC_UP, KC_END, KC_PGUP, _______, KC_INS, _______, _______, _______, CK_MACRO_DELAY,
        _______, KC_WH_D, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, TG(_GAMING), KC_DEL, CK_KEY_MULTIPLICATION, SE_DOT, _______, _______,
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
                          _______, _______, SE_C, SE_V, SE_N, SE_M, CK_STENO_TOGGLE, _______),
};



void fixed_tap_code(uint16_t keycode) {
    switch (keycode) {

        // if not a special keycode that need extra functionality to work then just register the keycode directly or whatever
        default:
            tap_code(keycode);
            break;

        // wait for 0.25 seconds when getting to this keycode. Used when macros need a delay between presses to for example wait for animations to finish first
        case CK_MACRO_DELAY:
            wait_ms(250);
            break;

        // Swedish keycodes for characters don't work as they do special logic like holding down shift whilst pressing 6 and shit like that which doesn't work in tap_code().
        case SE_AMPR: SEND_STRING("&"); break;
        case SE_PIPE: SEND_STRING("|"); break;
        case SE_CIRC: SEND_STRING("^"); break;
        case SE_LABK: SEND_STRING("<"); break;
        case SE_LCBR: SEND_STRING("{"); break;
        case SE_RCBR: SEND_STRING("}"); break;
        case SE_RABK: SEND_STRING(">"); break;
        case SE_MINS: SEND_STRING("-"); break;
        case SE_PLUS: SEND_STRING("+"); break;
        case SE_GRV: SEND_STRING("`"); break;
        case SE_ACUT: SEND_STRING("´"); break;
        case SE_HASH: SEND_STRING("#"); break;
        case SE_QUOT: SEND_STRING("'"); break;
        case SE_BSLS: SEND_STRING("\\"); break;
        case SE_DOT: SEND_STRING("."); break;
        case SE_LPRN: SEND_STRING("("); break;
        case SE_RPRN: SEND_STRING(")"); break;
        case SE_SCLN: SEND_STRING(";"); break;
        case SE_SLSH: SEND_STRING("/"); break;
        case SE_ASTR: SEND_STRING("*"); break;
        case SE_EXLM: SEND_STRING("!"); break;
        case SE_DLR: SEND_STRING("$"); break;
        case SE_PERC: SEND_STRING("%"); break;
        case SE_DQUO: SEND_STRING("\""); break;
        case SE_TILD: SEND_STRING("~"); break;
        case SE_COMM: SEND_STRING(","); break;
        case SE_LBRC: SEND_STRING("["); break;
        case SE_RBRC: SEND_STRING("]"); break;
        case SE_COLN: SEND_STRING(":"); break;
        case SE_UNDS: SEND_STRING("_"); break;
        case SE_EQL: SEND_STRING("="); break;
        case SE_QUES: SEND_STRING("?"); break;
        case SE_AT: SEND_STRING("@"); break;
    }
}



// keys that shouldn't be recorded such as layer switches
bool IsIgnoredDuringRecording(uint16_t keycode) {

    // TG(layer)
    if (QK_TOGGLE_LAYER <= keycode && keycode <= QK_TOGGLE_LAYER_MAX) {
        return true;
    }

    // other keycodes
    switch (keycode) {
        default:
            return false;
        case QK_TRI_LAYER_UPPER:
        case QK_TRI_LAYER_LOWER:
        case CK_STENO_TOGGLE:
            return true;
    }
}



bool KeyMultiplicationModeLogic(uint16_t keycode) {

    // cancel the mode if ESC is pressed
    if (keycode == KC_ESC) {
        key_multiplication_mode_active = false;
        return false;
    }

    // increase the key multiplication count if numbers are typed
    if ((keycode >= KC_1 && keycode <= KC_9) || keycode == KC_0) {
        unsigned int digit = (keycode == KC_0) ? 0 : (keycode - KC_1 + 1);
        key_multiplication_count = key_multiplication_count * 10 + digit;
        return false;
    }

    // if both if-checks failed then the user pressed a key that should be multiplied

    // cancel if the count is higher than the max allowed count
    if (key_multiplication_count > MAX_KEY_MULTIPLICATION_COUNT) {
        key_multiplication_mode_active = false;
        return false;
    }

    // if you start holding down shift for example, then the key multiplication mode should ignore that, so that you can hold keys like shift as keys are being mulitplied
    if (IS_MODIFIER_KEYCODE(keycode)) return true;
    // ignore stuff like layer changes. You don't want it to mulitply a layer change because wtf is the benifit of that?
    if (IsIgnoredDuringRecording(keycode)) return true;

    // special logic for if it is a mouse button
    if (keycode == KC_BTN1 || keycode == KC_BTN2 || keycode == KC_BTN3) {
        if (keycode == KC_BTN1) auto_clicker_button = 1;
        if (keycode == KC_BTN2) auto_clicker_button = 2;
        if (keycode == KC_BTN3) auto_clicker_button = 3;
        auto_clicker_click_count = (key_multiplication_count == 0) ? -1 : key_multiplication_count;
        key_multiplication_mode_active = false;
        return false;
    }

    // multiply the key
    for (int i = 0; i < key_multiplication_count; i++) {
        fixed_tap_code(keycode);
    }
    key_multiplication_mode_active = false;
    return false;
}



void keyboard_post_init_user(void) {
    // enable n-key rollover on startup
    keymap_config.nkro = 1;
}



// stuff that is run constantly in a loop. Make sure to not make this function very intensive as it will easily slow down your entire keyboard!
// it should be fine to add anything as long as it first does a check to see if it should be run or not. If you need some logic to actually always run on a loop
// then concider using a timer to make the logic only run for instance once every 1000 milliseconds
void matrix_scan_user(void) {

    if (auto_clicker_click_count != 0 && timer_elapsed(auto_clicker_timer) >= AUTO_CLICKER_MILLISECONDS_PER_CLICK) {
        switch (auto_clicker_button) {
            case 1: tap_code(KC_BTN1); break;
            case 2: tap_code(KC_BTN2); break;
            case 3: tap_code(KC_BTN3); break;
        }
        if (auto_clicker_click_count > 0)
            auto_clicker_click_count--;
        auto_clicker_timer = timer_read(); // reset timer
    }
}



// do stuff whenever keys get pressed down
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) { // if any key was pressed down



        if (key_multiplication_mode_active) {
            return KeyMultiplicationModeLogic(keycode);
        }
        if (keycode == CK_KEY_MULTIPLICATION) {
            key_multiplication_mode_active = true;
            key_multiplication_count = 0;
            return false;
        }
        if (keycode == KC_BTN1 || keycode == KC_BTN2 || keycode == KC_BTN3) {
            if (auto_clicker_click_count != 0) {
                auto_clicker_click_count = 0;
                return false;
            }
        }

        if (keycode == CK_STENO_TOGGLE) {
            layer_invert(_STENO); // toggle the steno layer
            return false;
        }


    }

    return true;
}
