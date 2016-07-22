
# example run:
# python only_noise.py sig_no_noise sig_time_noise noise_time.csv

import sys
import glob
import numpy as np

if len(sys.argv)<4:
    print "bad arg list"
    sys.exit(1)

inp_sig = sys.argv[1]
inp_noise = sys.argv[2]
out_sig_noise = sys.argv[3]

f_sig = open(inp_sig,'r')
f_noise = open(inp_noise,'r')
f_sig_noise = open(out_sig_noise,'w')

for rsig in f_sig:
    sig=rsig.split()
    noise = f_noise.readline().split()
    isig = [int(i) for i in sig]
    inoise = [int(i) for i in noise]
    snoise_sig = [str(a - b) for a, b in zip(isig, inoise)]
    print >>f_sig_noise,','.join(snoise_sig)

