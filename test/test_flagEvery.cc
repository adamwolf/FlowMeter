#include <gtest/gtest.h>
#include "../FlowMeter/FlowMeter.h"

TEST(FlagEvery, DoesNotClearFlag) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagEveryTicks(&flag, 15, 0);
    ASSERT_EQ(true, status);

    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 14; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
    fm.increment();
    ASSERT_EQ(true, flag);

    // Should not reset flag
    fm.increment();
    ASSERT_EQ(true, flag);
}

TEST(FlagEvery, Repeats) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagEveryTicks(&flag, 15, 0);
    ASSERT_EQ(true, status);

    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 14; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
    fm.increment();
    ASSERT_EQ(true, flag);

    flag = false;

    for (uint8_t iterations = 0; iterations < 3; iterations++) {
        for (uint8_t i = 0; i < 14; i++) {
            fm.increment();
            ASSERT_EQ(false, flag);
        }

        fm.increment();
        ASSERT_EQ(true, flag);

        flag = false;
    }

}

TEST(FlagEvery, SetsInitialValue) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    for (uint8_t i = 0; i < 17; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }

    bool status = fm.flagEveryTicks(&flag, 5, 0);
    ASSERT_EQ(true, status);

    ASSERT_EQ(true, flag);

    flag = false;

    for (uint8_t i = 0; i < 2; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }

    fm.increment();
    ASSERT_EQ(true, flag);
}

TEST(FlagEvery, CancelsRepeatingFlagWhenReset) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagEveryTicks(&flag, 15, 0);
    ASSERT_EQ(true, status);

    fm.increment();
    fm.reset();


    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 20; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
}

TEST(FlagEvery, ReturnsFalseAndDoesntWorkWhenCalledWithTicksZero) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagEveryTicks(&flag, 0, 0);
    ASSERT_EQ(false, status);

    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 20; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
}

TEST(FlagEvery, DoesntClobberExistingWhenCalledWithTicksZero) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagEveryTicks(&flag, 10, 0);
    ASSERT_EQ(true, status);

    // ok now call flageveryticks with ticks 0 and see if it still goes at 10

    status = fm.flagEveryTicks(&flag, 0, 0);
    ASSERT_EQ(false, status);

    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 9; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
    fm.increment();
    ASSERT_EQ(true, flag);
}

TEST(FlagEvery, ReturnsFalseWithNullFlag) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();

    bool status = fm.flagEveryTicks(NULL, 15, 0);
    ASSERT_EQ(false, status);

    for (uint8_t i = 0; i < 20; i++) {
        fm.increment();
    }
}