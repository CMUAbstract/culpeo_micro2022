import numpy as np

NUM_TAPS = 32
#DIVIDER = 4.22/14.22
DIVIDER = 10/20

refs = [1.2,2.0,2.5]

ref_regs = {1.2:1, 2.0:2, 2.5:3}

def calc_tap(val,divider,num_taps):
  diff = 1
  best_ref = 0
  best_tap = 0
  for ref in refs:
    tap_val =  np.floor(num_taps*val*divider/ref)
    test = tap_val*ref/(num_taps*divider)
    while(test < val):
      tap_val += 1
      test = tap_val*ref/(num_taps*divider)
    if tap_val > num_taps - 2:
      continue
    if (abs(val - test) < diff):
      diff = abs(val - test)
      best_ref = ref
      best_tap = tap_val
    #print(abs(val - test))
  return [int(best_tap),best_ref]



total = 0

if __name__ == "__main__":
  for val in np.arange(1.4,2.5,.02):
    total += 1
    [tap,ref] = calc_tap(val,DIVIDER,NUM_TAPS)
    #CEREFL_2 | CEREF0_19 | CEREF1_20, // 10
    val_str = "{:.2f}".format(val)
    cerefl_str = "CEREFL_"+str(ref_regs[ref])
    ceref0_str = "CEREF0_"+str(tap)
    cerefl1_str = "CEREF1_"+str(tap+1)
    print("/*",val_str,"*/",cerefl_str,"|",ceref0_str,"|",cerefl1_str,",")
  for count,val in enumerate(np.arange(1.4,2.5,.02)):
    dec_str = str(int(100*(val - np.floor(val))))
    dec_str = dec_str.zfill(2)
    volt_str = "V_"+str(int(np.floor(val)))+"_"+dec_str
    print(volt_str,"=",count,",")
  for val in np.arange(1.4,2.5,.02):
    hndrds_str = str(int(100*val))
    print(hndrds_str,",")
  for val in np.arange(1.4,2.5,.02):
    print("Val:",val)
    e_str = str(int((val*100)*(val*100)))
    print(e_str,",")

  print("Total: ",total)



