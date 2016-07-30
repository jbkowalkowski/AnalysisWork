
# example run:
# python only_noise.py run-number
# old - python only_noise.py sig_no_noise sig_time_noise noise_time.csv

import sys
import glob
import numpy as np

import split_range as sr


def num_windows(start,end,size): return (end-start-1)/(size) + 1

def calc_stride(start,end,size):
    return float(end-start-size)/(num_windows(start,end,size)-1)

def calc_range(start,end,size):
    nw=num_windows(start,end,size)
    s=calc_stride(start,end,size)
    offset=nw*s
    return np.array(np.arange(start,start+offset,s),dtype=int)
                    
def split_big_ones(ranges, size):
    for r in ranges:
        sr=calc_range(r[1],r[2].size)
        temp=np.array(shape=(sr.size,r.size))
        for si in sr.size:
            temp[si]=np.like(r)
            temp[si][1]=sr[si]
            temp[si][2]=sr[si]+size
        
def process(inp):
    print inp
    f_sig = open(inp[0],'r')
    f_nt  = open(inp[1],'r')
    f_nf  = open(inp[2],'r')
    f_range = open(inp[3],'r')

    # calculate line length
    temp=f_sig.readline()
    temps=temp.split()
    num_samples = len(temps)
    f_sig.seek(0)

    print 'start reads'
    d_sig = np.fromfile(f_sig,dtype=int,sep=' ')
    print 'done with sig'
    #d_nt  = np.fromfile(f_nt,dtype=int,sep=' ')
    print 'done with np'
    #d_nf  = np.fromfile(f_nf,dtype=int,sep=' ')
    print 'done with nf'

    d_sig.shape = ( d_sig.size/num_samples,num_samples )
    #d_nt.shape  = ( d_sig.size/num_samples,num_samples )
    #d_nf.shape  = ( d_sig.size/num_samples,num_samples )

    # ranges has six columns (wire,start,end,length,sum,median)
    d_range = np.fromfile(f_range,dtype=int,sep=' ')
    r_shape1=(d_range.size/6,6)
    r_shape2=(6,d_range.size/6)
    d_range.shape=r_shape1
    d_range_t=np.transpose(d_range)
    wire_pos=np.argsort(d_range_t[0])

    gt50=(d_range_t[2]-d_range_t[1])>50
    lt50=(d_range_t[2]-d_range_t[1])<=50
    near_start=d_range_t[1]<25
    near_end=(d_range_t[2] > (num_samples-25))
    big=d_range[gt50]
    small=d_range[lt50 & np.logical_not(near_start) & np.logical_not(near_end)]
    close0=d_range[ near_start & lt50 ]
    close_end=d_range[ near_end & lt50 ]
    zeros=d_range[ d_range_t[3] ==0 ]

    split_ranges = split_big_ones(big,50)

    print "close0=",close0.size/6,"closeend=",close_end.size/6
    print "big=",big.size/6,"small=",small.size/6,"d_range size=",d_range_t[0].size
    print "zeros=",zeros.size

    def func(a):
        b=np.zeros(50,dtype=long)
        start=a[1]
        end=a[2]
        offset=25-(end-start+1)/2
        b[(offset):(offset+end-start)]=d_sig[a[0]][ start:end ]
        return b

    # this works for the close and smaller than 50 intervals
    close_start=np.apply_along_axis(func,1,close0)

    # need to break up larger intervals into size 50 ones
    
    
    print close_start
    print close_start.shape
    # print close0
    #print d_range[0:3][np.array([True,False,False])]
    
    sys.exit(0)
    
    for i in range(0,5):
        print d_range[i]
    
if len(sys.argv)<2:
    print "bad arg list"
    sys.exit(1)

names = glob.glob("%s_*"%(sys.argv[1]))

inp_sig = glob.glob("%s_*no_noise*"%sys.argv[1])
inp_noise_time = glob.glob("%s_*time_noise*"%sys.argv[1])
inp_noise_freq = glob.glob("%s_*freq_noise*"%sys.argv[1])
inp_range = glob.glob("sig_%s_*no_noise*"%sys.argv[1])
out_sig_noise = glob.glob("%s_*freq_noise*"%sys.argv[1])

for a in zip(inp_sig,inp_noise_time,inp_noise_freq,inp_range):
    process(a)
    
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

