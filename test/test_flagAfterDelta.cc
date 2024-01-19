#include <gtest/gtest.h>
#include "../FlowMeter/FlowMeter.h"

TEST(FlagAfterDelta, DoesNotClearFlag) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagAfterDeltaTicks(&flag, 10);
    ASSERT_EQ(true, status);

    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 9; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
    fm.increment();
    ASSERT_EQ(true, flag);

    // Should not reset flag
    fm.increment();
    ASSERT_EQ(true, flag);
}

TEST(FlagAfterDelta, DoesntRepeat) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;
    fm.flagAfterDeltaTicks(&flag, 10);
    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 9; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
    fm.increment();
    ASSERT_EQ(true, flag);

    flag = false;

    for (uint8_t i = 0; i < 30; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
}

TEST(FlagAfterDelta, WorksWhenNotStartingAtZero) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    for (uint8_t i = 0; i < 25; i++) {
        fm.increment();
    }
    volatile bool flag = false;

    bool status = fm.flagAfterDeltaTicks(&flag, 10);
    ASSERT_EQ(true, status);

    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 9; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
    fm.increment();
    ASSERT_EQ(true, flag);
}

TEST(FlagAfterDelta, CancelsUpcomingFlagWhenReset) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    volatile bool flag = false;

    bool status = fm.flagAfterDeltaTicks(&flag, 15);
    ASSERT_EQ(true, status);

    fm.increment();
    fm.reset();


    ASSERT_EQ(false, flag);
    for (uint8_t i = 0 ; i < 20; i++) {
        fm.increment();
        ASSERT_EQ(false, flag);
    }
}

TEST(FlagAfterDelta, ReturnsFalseWithNullFlag) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();

    bool status = fm.flagAfterDeltaTicks(NULL, 15);
    ASSERT_EQ(false, status);

    for (uint8_t i = 0; i < 20; i++) {
        fm.increment();
    }
}