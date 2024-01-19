# Flow Meter

FlowMeter is an Arduino library for reading a flow meter.  After connecting a pin interrupt 
to count pulses, it tracks the pulses, keeping track of the total and the recent flow rate.

## Usage

Don't. :) This is extremely early in development and is not "ready" or "stable" and doesn't "work".

## Testing
### Dependencies
You'll need to install CMake, and some sort of C++ toolchain. You'll need gcovr for coverage reports.

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build
ctest
```

### Coverage

You can also install gcovr and run:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target coverage
```

(Coverage in CLion seems to work fine, too.)

## Roadmap / Hopes and Dreams
 * coverage
 * github actions
 * doxygen or something
 * make sure this works great with multiple instances
 * make an example sketch
 * do we want ticks per volume?
 * consider long overflow
 * assert that Arduino.h things are called
 * can we check the example sketches?
 * think about which inputs are invalid (delta ticks of 0...)
 * ask someone who knows anything about C++/cmake about the ctest/googletest stuff (and the rest of it actually)
 * think about interface--how do folks use pulse counting peripherals?
 * Arduino Library Manager stuff?
