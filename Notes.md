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


# Experiment configs
- The atomic_ble experiment where we show that Catnap's energy based estimates
  for Vsafe are bad uses a regulator dropping a launchpad's 3.3V input to 2.3V
  output, and then uses a 470Ohm and 220Ohm resistors in parallel before
  Vharv. This high current is necessary to get Capy to charge while the booster
  is turned on, given that it's not really meant for this
- atomic_sense was run with a potentiometer set at 243 Ohms (same LDO config as
  atomic_ble), 2 edlc caps + 2 330uF caps, and used -O1 optimization with *no*
  -g flag
