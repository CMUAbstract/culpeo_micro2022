# ADC sampling
- Doing nothing else, we can get a max sampling rate of ~70kHz

# Not Charging Checklist
- Is the Capybara configured in PS Mode? that's a resistor on the output booster
- Is the cap connected directly (i.e. not through a beefy voltage divider) to
  anything? We've seen op-amps and adc-pins drain the cap

# Timer nonsense
- I'm pretty sure the sleep timer (~TB0) has higher priority than the timer we
  use for Catnap (TA0). So maybe avoid sleeping in tasks?


# Library configs
- We changed libmsp/sleep.c to go into LPM3 so we get more energy savings when
  we're sleeping
- We extended the time libcapybara waits to do anything after it wakes up so we
  don't start while the capacitor is still recovering from the additional inrush
  current
