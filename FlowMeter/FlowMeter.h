#ifndef FLOWMETER_H
#define FLOWMETER_H

#ifndef FLOW_METER_RING_SIZE
#define FLOW_METER_RING_SIZE 16
#endif

class FlowMeter
{
    public:
        FlowMeter(int pin);
        void begin();
        void reset();
        void increment();
        int getTicks();
        int getTicksInPast(unsigned long period_ms);
        int getRingSize();

        bool callbackAfterDeltaTicks(unsigned long delta, void (*callback)(void));
        bool callbackAtTicks(unsigned long ticks, void (*callback)(void));

        bool flagAfterDeltaTicks(volatile bool* flag, unsigned long ticks);
        bool flagAtTicks(volatile bool* flag, unsigned long ticks);
        bool flagEveryTicks(volatile bool* flag, unsigned long ticks, unsigned long startTicks);
    private:
        int _pin;
        unsigned long _ticks;
        unsigned long _tickMillis[FLOW_METER_RING_SIZE]; //volatile
        int _tickMillisIndex; // last index written to

        long _flag_at_delta_target;
        bool* _flag_at_delta_ptr;

        void (*_delta_cb)(void);
        long _delta_ticks_target;

        void (*_abs_cb)(void);
        long _abs_ticks_target;

        // Flags and flag targets
        volatile bool* _flag_after_delta_ptr;
        unsigned long _flag_after_delta_target;
        volatile bool* _flag_at_ptr;
        unsigned long _flag_at_target;

        volatile bool* _repeating_flag_ptr;
        unsigned long _repeating_flag_target;
        unsigned long _repeating_flag_increment;
};

#endif // FLOWMETER_H
