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
import pickle
from matplotlib.ticker import AutoMinorLocator

EVT_LEN = 0.4
DEADLINE = 3.0
TIME_INDEX = 9

if __name__ == "__main__":
  num_files = len(sys.argv)
  i = 1
  all_files = []
  while i < num_files:
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
    # Only look at time
    times = vals[:,TIME_INDEX:TIME_INDEX+2]
    times = times[times[:,1] == 1]
    times = times[:,0]
    last = times[0]
    first = times[0]
    second = times[1]
    violations = []
    for i in range(1,len(times)):
      # Assume we already have the first
      if times[i] < first + EVT_LEN:
        second = times[i]
        last = first
      else:
        # We're on the first
        first = times[i]
      if (first > last + DEADLINE):
        print("Violation!",first,last,i)
        violations.append(first-last)
    print("Total:",len(violations))
    print("Average:",np.average(violations))




