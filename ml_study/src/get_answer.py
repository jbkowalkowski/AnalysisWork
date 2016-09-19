
import sys
import glob
import numpy as np

import split_range as sr

def process(inp_r, inp_s):
    num_samples = sr.get_num_samples(inp_s)
    d_range = sr.get_ranges(inp_r) # ,num_samples,size)
    f_sig = open(inp_s,'r')
    d_sig = np.fromfile(f_sig,dtype=int,sep=' ')
    d_sig.shape = ( d_sig.size/num_samples,num_samples )
    d_sig = np.zeros_like(d_sig)
    print d_sig.shape, d_sig.size

    for r in d_range:
        d_sig[r[0]][r[1]:r[2]] = 1
    
    d_sig.tofile(open('ans_'+inp_s,'w'),sep=' ',format="%d")


def do_main():
        
    if len(sys.argv)<2:
        print "bad arg list - how about an event number?"
        sys.exit(1)

    event_num = sys.argv[1]
    inp_sig = glob.glob("%s_*no_noise*"%event_num)
    inp_range = glob.glob("newsig_%s_*no_noise*"%event_num)

    for n in  zip(inp_range,inp_sig):
        print n
        process(n[0],n[1])
    
            
if "__main__" == __name__:
    do_main()

