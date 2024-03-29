# Script to pull the timing violations out of atomic_<app> tests.
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

EVT_LEN = .5
DEADLINE = 4.0
TIME_INDEX = 9

if __name__ == "__main__":
  DEADLINE = float(sys.argv[1])
  out_name = sys.argv[2]
  num_files = len(sys.argv) - 2
  i = 3
  all_files = []
  lost_vals = [] # Fraction of total
  delay_vals = []
  while i < num_files + 2:
    all_files.append(sys.argv[i])
    i += 1
  for filename in all_files:
    #print("filename:",filename)
    try:
      df = pd.read_csv(filename, mangle_dupe_cols=True,
           dtype=np.float64, skipinitialspace=True)#skiprows=[0])
    except:
      df = pd.read_csv(filename, mangle_dupe_cols=True,
           dtype=np.float64, skipinitialspace=True,skiprows=[0])

    vals = df.values
    # Only look at time
    times = vals[:,TIME_INDEX:TIME_INDEX+2]
    times = times[times[:,1] == 1]
    times = times[:,0]
    #print(times)
    total = 1
    lost = 0
    last = times[0]
    first = times[0]
    second = times[1]
    violations = []
    for i in range(1,len(times)):
      # Assume we already have the first
      if times[i] < first + EVT_LEN:
        second = times[i]
        last = first
        total += 1
      else:
        # We're on the first
        first = times[i]
      if (first > last + DEADLINE):
        #print("Violation!",first,last,i)
        violations.append(first-last)
        lost += (first-last)/DEADLINE
    if len(violations) > 0:
      vio_avg = np.average(violations)
    else:
      vio_avg = 0
    print(len(violations),total,vio_avg,lost)
    lost_vals.append(lost/total)
    delay_vals.append(vio_avg)
    print("lost:",lost/total,"delay:",vio_avg)
  results = {'lost':np.average(lost_vals),'lost_std':np.std(lost_vals),\
  'delay':np.average(delay_vals),'delay_std':np.std(delay_vals)}
  pkl.dump(results,open(out_name,'wb'))
    #print("Total:",len(violations),"/",len(times))

    #print("Average:",np.average(violations))




