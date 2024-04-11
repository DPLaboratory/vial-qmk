
#include<stdio.h>
#include "features/helper.h"

#define CLICK_PERIOD_MS 500

static deferred_token click_token = INVALID_DEFERRED_TOKEN;
static int n_loop1 = 0;
static int n_loop2 = 0;

// Callback used with deferred execution. It alternates between registering and
// unregistering (pressing and releasing) the mouse button.
static uint32_t helper_callback(uint32_t trigger_time, void* cb_arg) {
  n_loop1++;
  SEND_STRING(" -(o)- ");

  if (n_loop1 >= 10) {
    tap_code(KC_ENT);
    n_loop1=0;
    n_loop2++;
  }

  if (n_loop2 >= 2) {
    register_code(KC_LCTL);
    tap_code(KC_A);
    unregister_code(KC_LCTL);
    tap_code(KC_DEL);
    n_loop1=0;
    n_loop2=0;
  }

  return CLICK_PERIOD_MS / 2;  // Execute callback again in half a period.
}

// Starts Turbo Click, begins the `turbo_click_callback()` callback.
static void helper_start(void) {
  if (click_token == INVALID_DEFERRED_TOKEN) {
    uint32_t next_delay_ms = helper_callback(0, NULL);
    click_token = defer_exec(next_delay_ms, helper_callback, NULL);
  }
}

// Stops Turbo Click, cancels the callback.
static void helper_stop(void) {
  if (click_token != INVALID_DEFERRED_TOKEN) {
    cancel_deferred_exec(click_token);
    click_token = INVALID_DEFERRED_TOKEN;
  }
}

bool process_helper(keyrecord_t* record) {
  static bool locked = false;
  //static bool tapped = false;
  //static uint16_t tap_timer = 0;

    if (record->event.pressed) {  // Turbo Click key was pressed.
      if (!locked) {
        locked = true;
        helper_start();
      }
      else
      {
        locked = false;
        n_loop1=0;
        n_loop2=0;
        helper_stop();
      }

    } else {
      // If not currently locked, stop on key release.

    }

    return false;
}
