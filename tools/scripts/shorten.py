import pandas as pd
import sys 
import matplotlib
import numpy as np
#matplotlib.use("TkAgg")
import matplotlib.pyplot as plt 
import re
import glob
import pickle

START_TIME = 0.0 
STOP_TIME = 300

if __name__ == "__main__":
  filename = sys.argv[1]
  print(filename)
  try:
    df = pd.read_csv(filename, mangle_dupe_cols=True,
         dtype=np.float64, skipinitialspace=True)#skiprows=[0])
  except:
    df = pd.read_csv(filename, mangle_dupe_cols=True,
         dtype=np.float64, skipinitialspace=True,skiprows=[0])
  vals = df.values
  print("Min time is:",min(vals[:,0]))
  STOP_TIME = min(vals[:,0]) + STOP_TIME
  vals = vals[vals[:,0] < STOP_TIME]
  vals = vals[vals[:,0] > START_TIME]
  np.savetxt("shorten.csv",vals,delimiter=",")

