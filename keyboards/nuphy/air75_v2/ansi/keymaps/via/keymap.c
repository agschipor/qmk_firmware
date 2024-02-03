/*
Copyright 2023 @ Nuphy <https://nuphy.com/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "features/layer_lock.h"


// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TD_LAYR, // Our custom tap dance key; add any other tap dance keys to this enum 
};


// Function to determine the current tapdance state
td_state_t cur_dance(tap_dance_state_t *state);

void dance_finished(tap_dance_state_t *state, void *user_data);
void dance_reset(tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// layer Mac
[0] = LAYOUT_ansi_84(
    KC_ESC,     KC_BRID,    KC_BRIU,    MAC_TASK,   MAC_SEARCH, MAC_VOICE,  MAC_DND,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,    KC_VOLU,    MAC_PRTA,   KC_INS,     KC_DEL,
    KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_BSPC,    KC_PGUP,
    KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,                KC_BSLS,    KC_PGDN,
    KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                            KC_ENT,     KC_HOME,
    KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,                KC_RSFT,    KC_UP,      KC_END,
    KC_LCTL,    KC_LALT,    KC_LGUI,                                        KC_SPC,                             KC_RGUI,    _______,      QK_REP,                KC_LEFT,    KC_DOWN,    KC_RGHT),

// layer Mac Fn
[1] = LAYOUT_ansi_84(
    _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     MAC_PRT,    _______,    _______,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,   LNK_RF,     _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    DEV_RESET,  SLEEP_MODE,             BAT_SHOW,   _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,
    _______,                _______,    _______,    _______,    _______,    _______,    _______,    MO(4),      RGB_SPD,    RGB_SPI,    _______,                _______,    RGB_VAI,    _______,
    _______,    _______,    _______,                                        _______,                            _______,    _______,    _______,                RGB_MOD,    RGB_VAD,    RGB_HUI),

// layer win
[2] = LAYOUT_ansi_84(
    KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_PSCR,    KC_INS,     KC_DEL,
    KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_BSPC,    KC_PGUP,
    KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,                KC_BSLS,    KC_PGDN,
    MO(5),    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                            KC_ENT,     KC_HOME,
    KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,                KC_RSFT,    KC_UP,      KC_END,
    KC_LCTL,    KC_LGUI,    KC_LALT,                                        KC_SPC,                             QK_REP,    TD(TD_LAYR),      KC_RCTL,                KC_LEFT,    KC_DOWN,    KC_RGHT),

// layer win Fn
[3] = LAYOUT_ansi_84(
    _______,    KC_BRID,    KC_BRIU,    _______,    _______,    _______,    _______,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,    KC_VOLU,    _______,    _______,    _______,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,   LNK_RF,     _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    DEV_RESET,  SLEEP_MODE,             BAT_SHOW,   _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,
    _______,                _______,    _______,    _______,    _______,    _______,    _______,    MO(4),      RGB_SPD,    RGB_SPI,    _______,                _______,    RGB_VAI,    _______,
    _______,    _______,    _______,                                        _______,                            _______,    _______,      _______,                RGB_MOD,    RGB_VAD,    RGB_HUI),

// layer 4
[4] = LAYOUT_ansi_84(
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,
    _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    SIDE_SPD,   SIDE_SPI,   _______,                _______,    SIDE_VAI,   _______,
    _______,    _______,    _______,                                        _______,                            _______,    MO(4),      _______,                SIDE_MOD,   SIDE_VAD,   SIDE_HUI),

// layer 5
[5] = LAYOUT_ansi_84(
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,
    _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,    _______,
    _______,    _______,    _______,                                          LLOCK,                            _______,    _______,      _______,              _______,    _______,    _______),


// layer 6
[6] = LAYOUT_ansi_84(
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,
    _______,                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                _______,    _______,    _______,
    _______,    _______,    _______,                                        _______,                            _______,      TG(6),    _______,                _______,    _______,    _______),

};


// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};


void dance_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_APP);
            break;
        case TD_SINGLE_HOLD:
            layer_on(5);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(6)) {
                // If already set, then switch it off
                layer_off(6);
            } else {
                // If not already set, then switch the layer on
                layer_on(6);
            }
            break;
        default:
            break;
    }
}


void dance_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        if (!is_layer_locked(5))
            layer_off(5);
    }
    ql_tap_state.state = TD_NONE;
}

// // Set a long-ish tapping term for tap-dance keys
// uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
//             return 275;
//         default:
//             return TAPPING_TERM;
//     }
// }


//Tap Dance Definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_finished, dance_reset),
};
