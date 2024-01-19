#include <gtest/gtest.h>
#include "../FlowMeter/FlowMeter.h"

TEST(FlowMeterTest, RingSizeDefaultsTo16) {
    FlowMeter fm = FlowMeter(3);
    ASSERT_EQ(16, fm.getRingSize());
}

//Test(FlowMeterTest, beginCallsPinMode) {
//    FlowMeter fm = FlowMeter(3);
//    fm.begin();
//    TODO assert pinMode was called or pin is set or something
//}

TEST(FlowMeterTest, GetTicksReturnsZeroWithoutTicks) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    ASSERT_EQ(0, fm.getTicks());
}

TEST(FlowMeterTest, GetTicksReturnsOneAfterOneTick) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    fm.increment();
    ASSERT_EQ(1, fm.getTicks());
}

TEST(FlowMeterTest, GetTicksReturnsTwoAfterTwoTicks) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    fm.increment();
    fm.increment();
    ASSERT_EQ(2, fm.getTicks());
}

TEST(FlowMeterTest, GetTicksResets) {
    FlowMeter fm = FlowMeter(3);
    fm.begin();
    for (uint8_t i = 0; i  < 10; i++) {
        fm.increment();
    }
    ASSERT_EQ(10, fm.getTicks());

    fm.reset();

    ASSERT_EQ(0, fm.getTicks());
    fm.increment();
    ASSERT_EQ(1, fm.getTicks());
}