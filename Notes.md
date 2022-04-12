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
  output, and then uses a 243Ohm potentiometer before Vharv. It also relies on
  an arduino hooked up over a level shifter sending gpio flips to P1.5 with a
  poisson distribution of lambda=45 seconds. Also, a saleae needs to be
  measuring the input to the adc/comparator otherwise it goes haywire. The
  results are stored in
  culpeo_measurements/runtime_tests/atomic_ble_culpeo_latest_10min and
  atomic_ble_catnap_15min. You'll need to trim catnap since we lost the first
  100s of culpeo to its profiling.
- atomic_sense was run with a potentiometer set at 243 Ohms (same LDO config as
  atomic_ble), 2 edlc caps + 2 330uF caps, and used -O1 optimization with *no*
  -g flag. Also, we were running on the ACLK timer configured (using maker to a
  frequency of 32768 with a divider of 8. This isn't slow enough for other apps
  though
