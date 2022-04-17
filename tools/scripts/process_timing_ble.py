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
import pickle
from matplotlib.ticker import AutoMinorLocator

EVT_LEN = 4
DEADLINE = 6
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
    #print(times)
    first = times[0]
    second = -1
    violations = []
    responses = []
    for i in range(1,len(times)):
      # Assume we already have the first
      if first != -1:
        second = times [i]
      else:
        first = times[i]
      if (first != -1 and second != -1):
        if second - first > DEADLINE:
          violations.append(second - first)
          print("Violation:",first,second,i)
        responses.append(second - first)
        first = -1;
        second = -1;
    print("Total:",len(violations))
    print("Average for violations:",np.average(violations))
    print("Average for all:",np.average(responses))




