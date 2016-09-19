
# example run:
# python only_noise.py run-number
# old - python only_noise.py sig_no_noise sig_time_noise noise_time.csv

# 0123456789012345678901234
#             +
# ..........xxxxx..........
# ooooooooooIoooooooooo          
#  ooooooooooIoooooooooo         
#   ooooooooooIoooooooooo        
#    ooooooooooIoooooooooo       
#     ooooooooooIoooooooooo      
#     ......xxxxx..........
#    .......xxxxx.........
#   ........xxxxx........
#  .........xxxxx.......
# ..........xxxxx......

# need to start from right side (window making) and move left
# centering the signal point of interest.
# should one on either side be considered, where the answer is false?
# in the example, window length is 20+1
# extend interval .5 * (window length - 1) on left and right
# will generate one window per signal value in interval (in order)
# assume signal interval start,end defined
# win_center = length(window)/2
# int_center = (end-start)/2
# for i in (end-start)
#  X = array of length(window)
#  s=max(start-win_center+i,0)
#  e=min(s+length(window),length(real signal window))
#  X[s:e] = from real signal wo/noise + random section of noise
#  # do this for both frequency and time-based noise
#  ys.append(True)
#  Xs.append(X)

import sys
import glob
import numpy as np

import split_range as sr

def process(inp,size):
    print inp

    num_samples = sr.get_num_samples(inp[0])
    d_range = sr.get_ranges(inp[3]) # ,num_samples,size)

    #print num_samples, d_range.size
    
    f_sig = open(inp[0],'r')
    f_nt  = open(inp[1],'r')
    f_nf  = open(inp[2],'r')

    print 'start reads'
    d_sig = np.fromfile(f_sig,dtype=int,sep=' ')
    print 'done with sig'
    d_nt  = np.fromfile(f_nt,dtype=int,sep=' ')
    print 'done with np'
    d_nf  = np.fromfile(f_nf,dtype=int,sep=' ')
    print 'done with nf'

    d_sig.shape = ( d_sig.size/num_samples,num_samples )
    d_nt.shape  = ( d_nt.size/num_samples,num_samples )
    d_nf.shape  = ( d_nf.size/num_samples,num_samples )

    d_only_nt = d_nt - d_sig
    d_only_nf = d_nf - d_sig

    def func(a,dset):
        b=np.zeros(size,dtype=long)
        start=a[1]
        end=a[2]
        offset=(size/2)-(end-start+1)/2
        b[(offset):(offset+end-start)]=dset[a[0]][ start:end ]-a[5]
        return b

    def func_with_nt(a):
        return func(a,d_sig) + func(a,d_nt)

    def func_with_nf(a):
        return func(a,d_sig) + func(a,d_nf)

    def func_with_both(a):
        return func(a,d_sig) + func(a,d_nf) + func(a,d_nt)

    def func_answers(a):
        return func(a,d_sig) != 0
    
    # this works for the close and smaller than 50 intervals
    all_answers=np.apply_along_axis(func_answers,1,d_range)
    all_both=np.apply_along_axis(func_with_both,1,d_range)
    all_nt=np.apply_along_axis(func_with_nt,1,d_range)
    all_nf=np.apply_along_axis(func_with_nf,1,d_range)

    rstart=10
    rend=14
    print all_nf[rstart:rend]
    print all_nt[rstart:rend]
    print all_both[rstart:rend]
    print all_answers[rstart:rend]

    all_nf.tofile(open('all_'+inp[2],'w'),sep=' ',format="%d")
    all_nt.tofile(open('all_'+inp[1],'w'),sep=' ',format="%d")
    all_both.tofile(open('all_'+inp[0],'w'),sep=' ',format="%d")
    all_answers.tofile(open('all_'+inp[0],'w'),sep=' ',format="%d")

    sys.exit(0)

def do_main():
        
    if len(sys.argv)<2:
        print "bad arg list - how about an event number?"
        sys.exit(1)

    # names = glob.glob("%s_*"%(sys.argv[1]))

    inp_sig = glob.glob("%s_*no_noise*"%sys.argv[1])
    inp_noise_time = glob.glob("%s_*time_noise*"%sys.argv[1])
    inp_noise_freq = glob.glob("%s_*freq_noise*"%sys.argv[1])
    inp_range = glob.glob("newsig_%s_*no_noise*"%sys.argv[1])
    inp_ans = glob.glob("ans_%s_*no_noise*"%sys.argv[1])
    # out_sig_noise = glob.glob("%s_*freq_noise*"%sys.argv[1])

    for a in zip(inp_sig,inp_noise_time,inp_noise_freq,inp_range,in_ans):
        process(a,50)
    
if "__main__" == __name__:
    do_main()

# this is just here for reference, not meant to ever be run!
def func():

    f_sig = open(inp_sig,'r')
    f_noise = open(inp_noise,'r')
    f_sig_noise = open(out_sig_noise,'w')

    d_sig = np.fromfile(f_sig,' ')
    d_noise = np.fromfile(f_noise,' ')

    for rsig in f_sig:
        sig=rsig.split()
        noise = f_noise.readline().split()
        isig = [int(i) for i in sig]
        inoise = [int(i) for i in noise]
        snoise_sig = [str(a - b) for a, b in zip(isig, inoise)]
        print >>f_sig_noise,','.join(snoise_sig)

