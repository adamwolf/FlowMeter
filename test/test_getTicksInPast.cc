#include <gtest/gtest.h>
#include "../FlowMeter/FlowMeter.h"

TEST(GetTicksInPast, RingSizeDefaultsTo16) {
    FlowMeter fm = FlowMeter(3);
    ASSERT_EQ(16, fm.getRingSize());
}