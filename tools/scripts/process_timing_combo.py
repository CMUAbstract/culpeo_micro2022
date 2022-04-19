# Script to pull the timing violations out of atomic_ble tests where start and
# end are separated and never happen twice.
# Those data live over in culpeo_measurements/runtime_tests

import pandas as pd
import sys
import matplotlib
import numpy as np
#matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
import re
import glob
import pickle as pkl
from matplotlib.ticker import AutoMinorLocator

EVT_LEN = 4
DEADLINE = 15
MIC_DEADLINE = 7
INTERRUPT_INDEX = 7
EVENT_INDEX = 9
VDD_INDEX = 13

SHORT = .0001


def is_on(Vdds,time):
  if Vdds[Vdds[:,0] < time][-1,1] == 1:
    return 1
  else:
    return 0




if __name__ == "__main__":
  num_files = len(sys.argv)
  out_name = sys.argv[1]
  num_files = len(sys.argv) - 1
  i = 2
  all_files = []
  lost_vals_ble = [] # Fraction of total
  delay_vals_ble = []
  lost_vals_mic = [] # Fraction of total
  delay_vals_mic = []
  while i < num_files + 1:
    all_files.append(sys.argv[i])
    i += 1
  for filename in all_files:
    print(filename)
    try:
      df = pd.read_csv(filename, mangle_dupe_cols=True,
           dtype=np.float64, skipinitialspace=True)#skiprows=[0])
    except:
      df = pd.read_csv(filename, mangle_dupe_cols=True,
           dtype=np.float64, skipinitialspace=True,skiprows=[0])
    vals = df.values
    start_time = vals[0,0]
    vals = vals[vals[:,0] < (start_time + 300)]
    # Get Vdds
    Vdd = vals[:,VDD_INDEX:VDD_INDEX+2]
    Vdd = Vdd[Vdd[:,0] < (start_time + 300)]
    # Get triggers
    triggers = vals[:,INTERRUPT_INDEX:INTERRUPT_INDEX+2]
    triggers = triggers[triggers[:,0] < (start_time + 300)]
    triggers = triggers[triggers[:,1] == 1]
      # Only look at time
    triggers = triggers[:,0]
    # Get ble/mic
    events = vals[:,EVENT_INDEX:EVENT_INDEX+2]
    events = events[events[:,0] < (start_time + 300)]
    events = events[events[:,1] == 1]
    events = events[:,0]
    i = 0
    mics = []
    bles = []
    #print("Gather mics:")
    while i < len(events) - 2:
      if events[i+1] - events[i] < SHORT:
        mics.append(events[i])
        #print(events[i])
        i += 2
      else:
        bles.append(events[i])
        i += 1
    ble_violations = []
    ble_times = []
    total = 0
    print("Len mics:",len(mics))
# Count missed response deadlines
    for count, trigger in enumerate(triggers):
      if is_on(Vdd,trigger) == 0:
        ble_violations.append(-1)
        print("Missed trigger!",trigger)
        continue
      ble_time = 0
      for ble in bles:
        if ble > trigger:
          ble_time = ble
          break;
      if ble_time - trigger > DEADLINE:
        ble_violations.append(ble_time - trigger)
        print("Violation! ",trigger,ble_time)
      if (ble_time - trigger) > 0:
        ble_times.append(ble_time - trigger)
        print("Adding time",ble_time - trigger)
# Count missed mic events
    i = 0
    mic_violations = 0
    while i < len(mics) - 2:
      if mics[i+1] - mics[i] > MIC_DEADLINE:
        # We only worry if the device is powered down. There are bugs in both the
        # scheduler implementations that are occasionally missing events for no
        # reason
        if is_on(Vdd,mics[i]+MIC_DEADLINE) == 0:
          print("Mic:",mics[i])
          mic_violations += (mics[i+1] - mics[i])/MIC_DEADLINE

      i += 1
    print("Total:",len(ble_violations),"/",len(triggers))
    print("Average for violations:",np.average(ble_violations))
    print("Mic vals lost:",mic_violations,"/",len(mics)+mic_violations)
# Use ble count for total responses
    lost_vals_ble.append(len(ble_violations)/len(triggers))
    #delay_vals_ble.append(np.average([i for i in ble_times if i > -1]))
    delay_vals_ble.append(np.average(ble_times))

    lost_vals_mic.append( mic_violations / (len(mics) + mic_violations))
  results_mic = {'lost':np.average(lost_vals_mic),
  'lost_std':np.std(lost_vals_mic)}
  pkl.dump(results_mic,open(out_name+"_mic.pkl","wb"))

  results_ble = {'lost':np.average(lost_vals_ble),\
  'lost_std':np.std(lost_vals_ble),\
  'delay':np.average(delay_vals_ble),'delay_std':np.std(delay_vals_ble)}
  print(results_ble)
  pkl.dump(results_ble,open(out_name+"_ble.pkl","wb"))



