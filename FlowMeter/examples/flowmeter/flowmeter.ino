#include <FlowMeter.h>

const uint8_t FLOW_METER_PIN = 2;

FlowMeter fm = FlowMeter(FLOW_METER_PIN);

volatile bool kiloflag = false;

#ifdef ESP32
// Note: the Flow Meter library doesn't use the Pulse Counter (PCNT) peripheral
void IRAM_ATTR handleFlowMeterTick() {
  fm.increment();
}
#else
void handleFlowMeterTick() {
  fm.increment();
}
#endif

void setup() {
  Serial.begin(115200);
  Serial.println("FlowMeter example");
  fm.begin();
  fm.flagEveryTicks(&kiloflag, 1000, 0);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), handleFlowMeterTick, FALLING);
  Serial.println("setup complete");
}

void loop() {
    if (kiloflag) {
      Serial.print("Total ticks: ");
      Serial.println(fm.getTicks());
      Serial.print("Ticks in last 1s:");

      int recentTicks = fm.getTicksInPast(1000);
      if (recentTicks >= 0) {
        Serial.println(recentTicks);
      } else {
        Serial.println("N/A");
      }
      Serial.println();
      kiloflag = false;
    }
}
