#include "Arduino.h"
#include "FlowMeter.h"

FlowMeter::FlowMeter(int pin) {
    _pin = pin;
}

int FlowMeter::getRingSize() {
    return FLOW_METER_RING_SIZE;
}

void FlowMeter::begin() {
    pinMode(_pin, INPUT_PULLUP);
    reset();
}

void FlowMeter::reset() {
    _ticks = 0;
    _tickMillisIndex = 0;
    for (int i = 0; i < FLOW_METER_RING_SIZE; i++) {
        _tickMillis[i] = 0;
    }

    // Callbacks
    _flag_at_delta_target = 0;
    _flag_at_delta_ptr = NULL;
    _delta_cb = NULL;
    _delta_ticks_target = 0;
    _abs_cb = NULL;
    _abs_ticks_target = 0;

    // Flags and flag targets
    _flag_after_delta_ptr = NULL;
    _flag_after_delta_target = 0;
    _flag_at_ptr = NULL;
    _flag_at_target = 0;
    _repeating_flag_ptr = NULL;
    _repeating_flag_target = 0;
    _repeating_flag_increment = 0;
}

void FlowMeter::increment() {
    _ticks++;
    _tickMillisIndex = (_tickMillisIndex + 1) % FLOW_METER_RING_SIZE;
    _tickMillis[_tickMillisIndex] = millis();

// TODO what if this gets interrupted? Is that a valid concern?

    if (_flag_after_delta_target <= _ticks && _flag_after_delta_ptr != NULL) {
        *_flag_after_delta_ptr = true;
        _flag_after_delta_ptr = NULL;
    }

    if (_flag_at_target <= _ticks && _flag_at_ptr != NULL) {
        *_flag_at_ptr = true;
        _flag_at_ptr = NULL;
    }

    if (_repeating_flag_target <= _ticks && _repeating_flag_ptr != NULL) {
        *_repeating_flag_ptr = true;
        _repeating_flag_target += _repeating_flag_increment;
    }

    if (_delta_cb != NULL && _delta_ticks_target <= _ticks) {
        _delta_cb();
        _delta_cb = NULL;
    }

    if (_abs_cb != NULL && _abs_ticks_target <= _ticks) {
        _abs_cb();
        _abs_cb = NULL;
    }
}

int FlowMeter::getTicks() {
    return _ticks;
}

// Returns number of ticks in the past X milliseconds
// if it's positive, that means we have a record of ticks from before the period
// if it's negative, that means we don't have the timing of those ticks, so it's likely an incomplete count
// (Look into FLOW_METER_RING_SIZE.)
int FlowMeter::getTicksInPast(unsigned long period_ms) {

    unsigned long now = millis();
    unsigned long start = now - period_ms;

    int end_millis_index = _tickMillisIndex;

    bool haveOlderTicks = false;

    // we want to start at the most recent tick, and go back in time.
    // if we get a value of 0, that means we've gone back to, effectively, uninitialized data in the array
    // if we go before the index is 0, we need to wrap around to the end of the array
    uint16_t num_ticks = 0;
    for (int num = 0; num < FLOW_METER_RING_SIZE; num++) {
        int index = end_millis_index - num;
        if (index < 0) {
            index += FLOW_METER_RING_SIZE;
        }

        if (_tickMillis[index] == 0) {
            // effectively uninitialized data
            break;
        } else if (_tickMillis[index] < start) {
            // we've gone back too far in time
            haveOlderTicks = true;
            break;
        }

        num_ticks++;
    }

    if (!haveOlderTicks) {
        num_ticks *= -1;
    }
    return num_ticks;
}

bool FlowMeter::callbackAfterDeltaTicks(unsigned long delta, void (*callback)(void)) {
    // BE CAREFUL WITH THE CALLBACK!
    // Maybe you want flagAfterDeltaTicks instead.

    if (_delta_cb != NULL) {
        return false;
    }

    if (delta == 0) {
        return false;
    }

    _delta_cb = callback;
    _delta_ticks_target = delta;

    return true;
}

bool FlowMeter::callbackAtTicks(unsigned long ticks, void (*callback)(void)) {
    // BE CAREFUL WITH THE CALLBACK!
    // Maybe you want flagAtTicks instead.

    if (_abs_cb != NULL) {
        return false;
    }

    if (ticks == 0 || ticks < _ticks) {
        return false;
    }

    _abs_cb = callback;
    _abs_ticks_target = ticks;

    return true;
}

bool FlowMeter::flagAfterDeltaTicks(volatile bool *flag, unsigned long deltaTicks) {
    if (flag == NULL) {
        return false;
    }

    if (deltaTicks == 0) {
        return false;
    }

    _flag_after_delta_target = _ticks + deltaTicks;
    _flag_after_delta_ptr = flag;

    return true;
}

bool FlowMeter::flagAtTicks(volatile bool *flag, unsigned long ticks) {
    if (flag == NULL) {
        return false;
    }

    if (_ticks >= ticks) {
        *flag = true;
        // TODO should this clear out any existing target? Probably, I guess?
    } else {
        _flag_at_target = ticks;
        _flag_at_ptr = flag;
    }

    return true;
}

bool FlowMeter::flagEveryTicks(volatile bool *flag, unsigned long ticks, unsigned long startTicks) {
    if (flag == NULL) {
        return false;
    }

    if (ticks == 0) {
        return false;
    }

    _repeating_flag_increment = ticks;
    bool alreadyPast = false;
    unsigned long next_target = startTicks + ticks;

    while (next_target < _ticks) {
        next_target += ticks;
        alreadyPast = true;
    }

    _repeating_flag_target = next_target;
    _repeating_flag_ptr = flag;

    if (alreadyPast) {
        *flag = true;
    }

    return true;
}

