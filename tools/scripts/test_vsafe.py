import numpy as np
import sys
import matplotlib
import pickle
import re
import math
import glob


Vs_const_float=6.8589815
n_ratio_float=1.1907954
Voff_sq_float=2.56
Vd_const_float=1.0887018
m_float=0.1622807
b_float=0.4207894

def calc_vsafe_msp430(Vmin,Vfinal):
  Vd = Vfinal - Vmin;
  scale = Vmin*(m_float*Vmin + b_float);
  scale /= Vd_const_float;
  Vd_scaled = Vd*scale;
  scale = 1- Vd_scaled;
  Ve_squared = Vs_const_float + Voff_sq_float;
  temp = Vfinal*Vfinal;
  temp *= n_ratio_float;
  Ve_squared -= temp;
  Ve = Ve_squared**(1/2)
  return Ve + Vd_scaled

eff = [.8,.73,.56]
vi = [2.4,1.8,.9]

# Vs = Vstart 
# This function generates the #defines we need for our code
def get_vsafe_floats(Vs,Voff):
  m,b= np.polyfit(vi,eff,1)
  Vd_const = (Voff*(m*Voff + b)) 
  n_voff = m*Voff + b 
  n_vs = m*Vs + b 
  print("#define m_float ","{:.7f}".format(m))
  print("#define b_float ","{:.7f}".format(b))
  print("#define Voff_sq_float ","{:.2f}".format(Voff**2))
  print("#define n_ratio_float ","{:.7f}".format(n_vs/n_voff))
  print("#define Vs_const_float ","{:.7f}".format(n_vs/n_voff*(Vs**2)))
  print("#define Vd_const_float ","{:.7f}".format(Voff*(m*Voff+b)))
  return

