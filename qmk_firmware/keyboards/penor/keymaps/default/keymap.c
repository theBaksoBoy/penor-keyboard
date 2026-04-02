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
    CK_UNICODE_MODE,
    CK_MACRO_DELAY,
};



bool key_multiplication_mode_active = false; // are you in the mode where you type out numbers to determine how many times a key should repeat?
unsigned int key_multiplication_count = 0; // how many times should a key be made to repeat?
#define MAX_KEY_MULTIPLICATION_COUNT 100

int auto_clicker_click_count = 0; // if 0 then don't click. if > 0 then click and decrement value. if < 0 then keep clicking endlessly
uint16_t auto_clicker_timer = 0;
unsigned int auto_clicker_button = 1; // what mouse button should the auto clicker use? 1 == KC_BTN1, 2 == KC_BTN2, 3 == KC_BTN3
#define AUTO_CLICKER_MILLISECONDS_PER_CLICK 34 // 30 clicks per second

bool unicode_mode = false; // when true your key presses get recorded, and then converted into a unicode character if a valid match exists
#define MAX_RECORDED_UNICODE_STRING_LENGTH 32
char recorded_unicode_string[MAX_RECORDED_UNICODE_STRING_LENGTH];
uint8_t recorded_unicode_string_index = 0;

bool mouse_u_held = false;
bool mouse_d_held = false;
bool mouse_l_held = false;
bool mouse_r_held = false;
uint16_t mouse_warping_timer = 0;
bool mouse_warping_timer_running = false; // this is needed as timers overflow after around 65 seconds. This is used to avoid issues related to that
#define MOUSE_WARPING_DELAY 100 // how many milliseconds until the mouse warps in the held direction after all mouse direction keys are released



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Note that this is designed as a Swedish layout!

    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* MAIN
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤  W  │  F  │  P  │  B  │           │  J  │  L  │  U  │  Y  ├─────┬─────┐
     * │ TAB │  Q  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  Ö  │  Å  │
     * ├─────┼─────┤  R  │  S  │  T  │  G  │           │  M  │  N  │  E  │  I  ├─────┼─────┤
     * │L-SFT│  A  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  O  │     │
     * ├─────┼─────┤  X  │  C  │  D  │  V  │           │  K  │  H  │  ,  │  .  ├─────┼─────┤
     * │L-CTL│  Z  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤ ESC │  Ä  │
     * └─────┴─────┘  │L-ALT│S-GUI│ NAV │SPACE│     │ENTER│SYMBL│BACKS│R-ALT│  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_MAIN] = LAYOUT(
        KC_TAB,  SE_Q, SE_W, SE_F, SE_P, SE_B,       SE_J, SE_L, SE_U,    SE_Y,    SE_ODIA, SE_ARNG,
        KC_LSFT, SE_A, SE_R, SE_S, SE_T, SE_G,       SE_M, SE_N, SE_E,    SE_I,    SE_O,    _______,
        KC_LCTL, KC_Z, SE_X, SE_C, SE_D, SE_V,       SE_K, SE_H, SE_COMM, SE_DOT,  KC_ESC,  SE_ADIA,
        KC_LALT, KC_LGUI, QK_TRI_LAYER_UPPER, KC_SPC,KC_ENT, QK_TRI_LAYER_LOWER, KC_BSPC, KC_RALT),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* GAMING
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤  Q  │  W  │  E  │  R  │           │  Y  │  U  │  I  │  O  ├─────┬─────┐
     * │ TAB │  T  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  P  │  Å  │
     * ├─────┼─────┤  A  │  S  │  D  │  F  │           │  H  │  J  │  K  │  L  ├─────┼─────┤
     * │L-SFT│  G  ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  Ö  │     │
     * ├─────┼─────┤  Z  │  X  │  C  │  V  │           │  N  │  M  │  ,  │  .  ├─────┼─────┤
     * │L-CTL│  B  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤ ESC │  Ä  │
     * └─────┴─────┘  │L-ALT│S-GUI│ NAV │SPACE│     │ENTER│SYMBL│BACKS│R-ALT│  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_GAMING] = LAYOUT(
        KC_TAB,  SE_T, SE_Q, SE_W, SE_E, SE_R,       SE_Y, SE_U, SE_I,    SE_O,    SE_P,    SE_ARNG,
        KC_LSFT, SE_G, SE_A, SE_S, SE_D, SE_F,       SE_H, SE_J, SE_K,    SE_L,    SE_ODIA, _______,
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
     * ├─────┼─────┤  ←  │  ↓  │  →  │  ▢↓ │           │     │ DEL │KEYML│  .  ├─────┼─────┤
     * │     │  ⬍↓ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤  ▶  │STENO│
     * ├─────┼─────┤  2  │  3  │  4  │  5  │           │  6  │  7  │  8  │  9  ├─────┼─────┤
     * │     │  1  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤  0  │     │
     * └─────┴─────┘  │     │     │     │     │     │     │MOUSE│     │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_NAV] = LAYOUT(
        _______, KC_WH_U, KC_HOME, KC_UP, KC_END, KC_PGUP, _______, KC_INS, _______, _______, _______, CK_MACRO_DELAY,
        _______, KC_WH_D, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, _______, KC_DEL, CK_KEY_MULTIPLICATION, SE_DOT, _______, CK_STENO_TOGGLE,
        _______, SE_1, SE_2, SE_3, SE_4, SE_5, SE_6, SE_7, SE_8, SE_9, SE_0, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),


    // ────────────────────────────────────────────────────────────────────────────────────────────
    /* MOUSE
     *             ┌─────┬─────┬─────┬─────┐           ┌─────┬─────┬─────┬─────┐
     * ┌─────┬─────┤ ◀▮  │  ▮↑ │  ▮▶ │  ▢↑ │           │Ma/Ga│PRSCR│SCRLL│PAUS ├─────┬─────┐
     * │     │  ⬍↑ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤UNCOD│ MAN │
     * ├─────┼─────┤ ←▮  │  ▮↓ │  ▮→ │  ▢↓ │           │     │  ◧  │  ◨  │  ▣  ├─────┼─────┤
     * │L-SFT│  ⬍↓ ├─────┼─────┼─────┼─────┤           ├─────┼─────┼─────┼─────┤     │ F12 │
     * ├─────┼─────┤ F2  │ F3  │ F4  │ F5  │           │ F6  │ F7  │ F8  │ F9  ├─────┼─────┤
     * │L-CTL│ F1  ├──┬──┴──┬──┴──┬──┴──┬──┴──┐     ┌──┴──┬──┴──┬──┴──┬──┴──┬──┤ F10 │ F11 │
     * └─────┴─────┘  │     │     │     │     │     │     │     │     │     │  └─────┴─────┘
     *                └─────┴─────┴─────┴─────┘     └─────┴─────┴─────┴─────┘
     */
    [_MOUSE] = LAYOUT(
        _______, KC_WH_U, KC_BTN4, KC_MS_U, KC_BTN5, KC_PGUP, TG(_GAMING), KC_PSCR, KC_SCRL, KC_PAUS, CK_UNICODE_MODE, CK_MANUAL,
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



// see if recorded_unicode_string matches any of the specified strings. If it does then send the output string as a unicode character and return true
void MatchAndSendUnicode(const char *output, const char *candidates[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(recorded_unicode_string, candidates[i]) == 0) {

            clear_mods(); // clear mods to make sure that stuff sends correctly
            register_code(KC_LCTL); // LCTL+LSFT+u to activate unicode input on linux
            register_code(KC_LSFT);
            tap_code(KC_U);
            unregister_code(KC_LCTL);
            unregister_code(KC_LSFT);

            send_string(output);

            tap_code(KC_SPACE); // presses space to confirm unicode
        }
    }
}
// make a macro for using MatchAndSendUnicode way easier, where you can just do MATCH_AND_SEND(unicode_id, match1, match2, match3, ...)
#define MATCH_SEND_UNICODE(output, ...) \
    MatchAndSendUnicode(output, \
        (const char*[]){__VA_ARGS__}, \
        sizeof((const char*[]){__VA_ARGS__}) / sizeof(const char*))



void ParseUnicodeString(void) {

    // this site categorizes all unicode characters https://symbl.cc/en/unicode-table

    // superscript and subscript for numbers
    /* ⁰ */ MATCH_SEND_UNICODE("2070",   "0", "super0"); /* ₀ */ MATCH_SEND_UNICODE("2080",   "sub0");
    /* ¹ */ MATCH_SEND_UNICODE("b9",   "1", "super1");   /* ₁ */ MATCH_SEND_UNICODE("2081",   "sub1");
    /* ² */ MATCH_SEND_UNICODE("b2",   "2", "super2");   /* ₂ */ MATCH_SEND_UNICODE("2082",   "sub2");
    /* ³ */ MATCH_SEND_UNICODE("b3",   "3", "super3");   /* ₃ */ MATCH_SEND_UNICODE("2083",   "sub3");
    /* ⁴ */ MATCH_SEND_UNICODE("2074",   "4", "super4"); /* ₄ */ MATCH_SEND_UNICODE("2084",   "sub4");
    /* ⁵ */ MATCH_SEND_UNICODE("2075",   "5", "super5"); /* ₅ */ MATCH_SEND_UNICODE("2085",   "sub5");
    /* ⁶ */ MATCH_SEND_UNICODE("2076",   "6", "super6"); /* ₆ */ MATCH_SEND_UNICODE("2086",   "sub6");
    /* ⁷ */ MATCH_SEND_UNICODE("2077",   "7", "super7"); /* ₇ */ MATCH_SEND_UNICODE("2087",   "sub7");
    /* ⁸ */ MATCH_SEND_UNICODE("2078",   "8", "super8"); /* ₈ */ MATCH_SEND_UNICODE("2088",   "sub8");
    /* ⁹ */ MATCH_SEND_UNICODE("2079",   "9", "super9"); /* ₉ */ MATCH_SEND_UNICODE("2089",   "sub9");

    // math
    /* ∏ */ MATCH_SEND_UNICODE("220f",   "prod", "product");
    /* ∑ */ MATCH_SEND_UNICODE("2211",   "sum");
    /* ∎ */ MATCH_SEND_UNICODE("220e",   "qed");
    /* √ */ MATCH_SEND_UNICODE("221a",   "sqrt", "root");
    /* ∞ */ MATCH_SEND_UNICODE("221e",   "infty", "inf", "infinity");
    /* ∩ */ MATCH_SEND_UNICODE("2229",   "cap", "and", "intersect", "intersection");
    /* ∪ */ MATCH_SEND_UNICODE("222a",   "cup", "or", "union");
    /* ∫ */ MATCH_SEND_UNICODE("222b",   "int", "integral");
    /* ∬ */ MATCH_SEND_UNICODE("222c",   "iint", "iintegral");
    /* ∭ */ MATCH_SEND_UNICODE("222d",   "iiint", "iiintegral");
    /* ≈ */ MATCH_SEND_UNICODE("2248",   "approx", "roughly", "about", "approximately");
    /* ≠ */ MATCH_SEND_UNICODE("2260",   "neq", "not", "isnot", "notequal", "notequalto");
    /* ± */ MATCH_SEND_UNICODE("b1",   "pm", "plusminus");

    // greek symbols
    /* α */ MATCH_SEND_UNICODE("3b1",   "alpha");
    /* β */ MATCH_SEND_UNICODE("3b2",   "beta");
    /* γ */ MATCH_SEND_UNICODE("3b3",   "gamma"); /* Γ */ MATCH_SEND_UNICODE("393",   "Gamma");
    /* δ */ MATCH_SEND_UNICODE("3b4",   "delta"); /* Δ */ MATCH_SEND_UNICODE("394",   "Delta");
    /* ε */ MATCH_SEND_UNICODE("3b5",   "epsilon");
    /* ζ */ MATCH_SEND_UNICODE("3b6",   "zeta");
    /* η */ MATCH_SEND_UNICODE("3b7",   "eta");
    /* θ */ MATCH_SEND_UNICODE("3b8",   "theta"); /* Θ */ MATCH_SEND_UNICODE("398",   "Theta");
    /* λ */ MATCH_SEND_UNICODE("3bb",   "lambda");
    /* μ */ MATCH_SEND_UNICODE("3bc",   "mu");
    /* π */ MATCH_SEND_UNICODE("3c0",   "pi"); /* Π */ MATCH_SEND_UNICODE("3a0",   "Pi");
    /* ρ */ MATCH_SEND_UNICODE("3c1",   "rho");
    /* σ */ MATCH_SEND_UNICODE("3c3",   "sigma"); /* Σ */ MATCH_SEND_UNICODE("3a3",   "Sigma");
    /* τ */ MATCH_SEND_UNICODE("3c4",   "tau");
    /* φ */ MATCH_SEND_UNICODE("3c6",   "phi"); /* Φ */ MATCH_SEND_UNICODE("3a6",   "Phi");
    /* ψ */ MATCH_SEND_UNICODE("3c8",   "psi"); /* Ψ */ MATCH_SEND_UNICODE("3a8",   "Psi");
    /* ω */ MATCH_SEND_UNICODE("3c9",   "omega"); /* Ω */ MATCH_SEND_UNICODE("3a9",   "Omega");

    // arrows
    /* ← */ MATCH_SEND_UNICODE("2190",   "leftarrow", "arrowleft", "larrow", "arrowl", "left", "l");
    /* ↑ */ MATCH_SEND_UNICODE("2191",   "uparrow", "arrowup", "uarrow", "arrowu", "up", "u");
    /* → */ MATCH_SEND_UNICODE("2192",   "rightarrow", "arrowright", "rarrow", "arrowr", "right", "r", "to");
    /* ↓ */ MATCH_SEND_UNICODE("2193",   "downarrow", "arrowdown", "darrow", "arrowd", "down", "d");
    /* ↔ */ MATCH_SEND_UNICODE("2194",   "leftrightarrow", "arrowleftright", "lrarrow", "arrowlr", "leftright", "lr");
    /* ↕ */ MATCH_SEND_UNICODE("2195",   "updownarrow", "arrowupdown", "udarrow", "arrowud", "updown", "ud");
    /* ↖ */ MATCH_SEND_UNICODE("2196",   "nwarrow");
    /* ↗ */ MATCH_SEND_UNICODE("2197",   "nearrow");
    /* ↘ */ MATCH_SEND_UNICODE("2198",   "searrow");
    /* ↙ */ MATCH_SEND_UNICODE("2199",   "nwarrow");
    /* ⇐ */ MATCH_SEND_UNICODE("21d0",   "Leftarrow", "Arrowleft", "Larrow", "ArrowL", "Left", "L");
    /* ⇑ */ MATCH_SEND_UNICODE("21d1",   "Uparrow", "Arrowup", "Uarrow", "Arrowu", "Up", "U");
    /* ⇒ */ MATCH_SEND_UNICODE("21d2",   "Rightarrow", "Arrowright", "Rarrow", "Arrowr", "Right", "R", "implication", "implies");
    /* ⇓ */ MATCH_SEND_UNICODE("21d3",   "Downarrow", "Arrowdown", "Darrow", "Arrowd", "Down", "D");
    /* ⇔ */ MATCH_SEND_UNICODE("21d4",   "Leftrightarrow", "Arrowleftright", "Lrarrow", "Arrowlr", "Leftright", "Lr", "equivalent", "equivalence");
    /* ⇕ */ MATCH_SEND_UNICODE("21d5",   "Updownarrow", "Arrowupdown", "Udarrow", "Arrowud", "Updown", "Ud");

    // misc
    /* é */ MATCH_SEND_UNICODE("e9",   "e");
    /* É */ MATCH_SEND_UNICODE("c9",   "E");
    /* © */ MATCH_SEND_UNICODE("a9",   "c", "cp", "copyright");
    /* ™ */ MATCH_SEND_UNICODE("2122",   "tm", "trademark", "trademarked");
    /* ◉ */ MATCH_SEND_UNICODE("25c9",   "bullet");
    /* ° */ MATCH_SEND_UNICODE("00b0",   "degree", "degrees", "deg");
    /* ⌀ */ MATCH_SEND_UNICODE("2300",   "diameter");
    /* (zero-width joiner) */ MATCH_SEND_UNICODE("200d",   "zwj");
    /* ඞ */ MATCH_SEND_UNICODE("d9e"   , "amongus", "amogus", "amogos");

    // blocks
    /* █ */  MATCH_SEND_UNICODE("2588",   "block", "block1", "block1111");
    /* ▓ */  MATCH_SEND_UNICODE("2593",   "block2");
    /* ▒ */  MATCH_SEND_UNICODE("2592",   "block3");
    /* ░ */  MATCH_SEND_UNICODE("2591",   "block4");
    /* ▘ */  MATCH_SEND_UNICODE("2598",   "block1000");
    /* ▝ */  MATCH_SEND_UNICODE("259d",   "block0100");
    /* ▀ */  MATCH_SEND_UNICODE("2580",   "block1100");
    /* ▖ */  MATCH_SEND_UNICODE("2596",   "block0010");
    /* ▌ */  MATCH_SEND_UNICODE("258c",   "block1010");
    /* ▞ */  MATCH_SEND_UNICODE("259e",   "block0110");
    /* ▛ */  MATCH_SEND_UNICODE("259b",   "block1110");
    /* ▗ */  MATCH_SEND_UNICODE("2597",   "block0001");
    /* ▚ */  MATCH_SEND_UNICODE("259a",   "block1001");
    /* ▐ */  MATCH_SEND_UNICODE("2590",   "block0101");
    /* ▜ */  MATCH_SEND_UNICODE("259c",   "block1101");
    /* ▄ */  MATCH_SEND_UNICODE("2584",   "block0011");
    /* ▙ */  MATCH_SEND_UNICODE("2599",   "block1011");
    /* ▟ */  MATCH_SEND_UNICODE("259f",   "block0111");

    // clear recorded string
    memset(recorded_unicode_string, 0, sizeof(recorded_unicode_string));
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


    if (mouse_warping_timer_running && timer_elapsed(mouse_warping_timer) >= MOUSE_WARPING_DELAY) {

        mouse_warping_timer_running = false;

        // don't run if both up and down is held down at the same time
        if (!(mouse_u_held && mouse_d_held)) {
            if (mouse_u_held) {
                report_mouse_t mouse_report = {};
                mouse_report.y = -127;
                for (int i = 0; i < 35; i++)
                    host_mouse_send(&mouse_report);
            }
            if (mouse_d_held) {
                report_mouse_t mouse_report = {};
                mouse_report.y = 127;
                for (int i = 0; i < 35; i++)
                    host_mouse_send(&mouse_report);
            }
        }

        // don't run if both left and right is held down at the same time
        if (!(mouse_l_held && mouse_r_held)) {
            if (mouse_l_held) {
                report_mouse_t mouse_report = {};
                mouse_report.x = -127;
                for (int i = 0; i < 35; i++)
                    host_mouse_send(&mouse_report);
            }
            if (mouse_r_held) {
                report_mouse_t mouse_report = {};
                mouse_report.x = 127;
                for (int i = 0; i < 35; i++)
                    host_mouse_send(&mouse_report);
            }
        }
    }
}



// do stuff whenever keys get pressed down
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // logic to run when a key is pressed down
    if (record->event.pressed) {

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


        if (unicode_mode) {
            // hitting space or entershould confirm the unicode input mode
            if (keycode == KC_SPACE || keycode == KC_ENT) {
                unicode_mode = false;
                ParseUnicodeString();
                return false;
            }
            // hitting escape should cancel the unicode input mode
            if (keycode == KC_ESC) {
                unicode_mode = false;
                return false;
            }

            bool shift_held = get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));

            if (recorded_unicode_string_index < sizeof(recorded_unicode_string) - 1) {
                char key = 0;
                if (keycode >= KC_A && keycode <= KC_Z)
                    key = (shift_held) ? 'A' + (keycode - KC_A) : 'a' + (keycode - KC_A);
                else if (keycode >= KC_1 && keycode <= KC_9)
                    key = '1' + (keycode - KC_1);
                else if (keycode == KC_0)
                    key = '0';

                // record key to string if it was a valid key
                if (key != 0) {
                    recorded_unicode_string[recorded_unicode_string_index++] = key;
                    recorded_unicode_string[recorded_unicode_string_index] = '\0'; // null-terminate
                    return false;
                }
                // make stuff like layer changing and holding down shift get through
                else if (IsIgnoredDuringRecording(keycode) || IS_MODIFIER_KEYCODE(keycode)) {
                    return true;
                }
                // if invalid key pressed then cancel unicode mode
                else {
                    unicode_mode = false;
                    return true;
                }
            }
            // cancel unicode mode if it goes above the max length
            unicode_mode = false;
            return true;
        }
        if (keycode == CK_UNICODE_MODE) {
            unicode_mode = true;
            recorded_unicode_string_index = 0; // reset recorded string
            return false;
        }


        if (keycode == CK_STENO_TOGGLE) {

            layer_invert(_STENO); // toggle the steno layer

            clear_mods(); // make sure that stuff like control isn't held when it does PHROLG, as that could fuck shit up and make a bunch of unintended shortcuts happen

            // do PHROLG to toggle Plover
            register_code(KC_E); register_code(KC_R); register_code(KC_F); register_code(KC_V); register_code(KC_O); register_code(KC_L);
            unregister_code(KC_E); unregister_code(KC_R); unregister_code(KC_F); unregister_code(KC_V); unregister_code(KC_O); unregister_code(KC_L);
            return false;
        }
    }


    // logic to run when a key is released
    else {

        // if all mouse direction keys WERE held down, where one of the keys was just released
        if ((mouse_u_held && mouse_d_held && mouse_l_held && mouse_r_held) && (keycode == KC_MS_U || keycode == KC_MS_D || keycode == KC_MS_L || keycode == KC_MS_R)) {
            mouse_warping_timer = timer_read();
            mouse_warping_timer_running = true;
        }
    }



    // store the held-down state of the keys used to control the mouse position
    // this is done after all the other key logic as it needs to detect if all mouse direction keys WERE held down when one gets released
    switch (keycode) {
        case KC_MS_U: mouse_u_held = record->event.pressed; break;
        case KC_MS_D: mouse_d_held = record->event.pressed; break;
        case KC_MS_L: mouse_l_held = record->event.pressed; break;
        case KC_MS_R: mouse_r_held = record->event.pressed; break;
    }



    return true;
}
