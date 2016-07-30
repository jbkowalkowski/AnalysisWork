
import sys
import glob
import numpy as np

def num_windows(start,end,size): return (end-start-1)/(size) + 1

def calc_stride(start,end,size):
    return float(end-start-size)/(num_windows(start,end,size)-1)

def calc_range(start,end,size):
    nw=num_windows(start,end,size)
    s=calc_stride(start,end,size)
    offset=nw*s
    return np.array(np.arange(start,start+offset,s),dtype=int)
                    
def split_big_ones(ranges, size):
    out=[]
    for r in ranges:
        sr=calc_range(r[1],r[2],size)
        
        for si in sr:
            temp=np.copy(r)
            temp[1]=si
            temp[2]=si+size
            out.append(temp)
            #print temp

    return np.array(out)

def get_num_samples(sigs):
    f_sig = open(sigs,'r')
    # calculate line length
    temp=f_sig.readline()
    temps=temp.split()
    num_samples = len(temps)
    f_sig.close()
    return num_samples

def get_ranges(inp,num_samples,size):
    f_range = open(inp,'r')

    # ranges has six columns (wire,start,end,length,sum,median)
    d_range = np.fromfile(f_range,dtype=int,sep=' ')
    r_shape1=(d_range.size/6,6)
    r_shape2=(6,d_range.size/6)
    d_range.shape=r_shape1
    # d_range_t=np.transpose(d_range)
    # wire_pos=np.argsort(d_range_t[0])
    return d_range

def tofile(fout, r):
    #r.tofile(fout,sep=' ',format="%d")
    for a in r:
        print >>fout,a[0],a[1],a[2],a[3],a[4],a[5]

def rewrite_ranges(inp,size,out):
    num_samples = get_num_samples(inp[1])
    d_range = get_ranges(inp[0],num_samples,size)
    d_range_t=np.transpose(d_range)
    
    gt50=(d_range_t[2]-d_range_t[1])>size
    lt50=(d_range_t[2]-d_range_t[1])<=size
    near_start=d_range_t[1]<(size/2)
    near_end=(d_range_t[2] > (num_samples-(size/2)))
    small_ones=lt50 & np.logical_not(near_start) & np.logical_not(near_end)
    start_ones=near_start & lt50
    end_ones=near_end & lt50
    
    # split=1,start=2,end=3,small=4
    d_range_t[4][gt50]=1
    d_range_t[4][small_ones]=4
    d_range_t[4][start_ones]=2
    d_range_t[4][end_ones]=3

    d_range = np.transpose(d_range_t)
    big=d_range[gt50]
    small=d_range[small_ones]
    close_start=d_range[start_ones]
    close_end=d_range[end_ones]
    #zeros=d_range[ d_range_t[3] ==0 ]

    split_ranges = split_big_ones(big,size)

    fout = open(out,'w')
    #split_ranges.tofile(fout,sep=' ')
    tofile(fout,split_ranges)
    #close_start.tofile(fout,sep=' ')
    tofile(fout,close_start)
    #close_end.tofile(fout,sep=' ')
    tofile(fout,close_end)
    #small.tofile(fout,sep=' ')
    tofile(fout,small)
    fout.close()

    
    #print "close_start=",close_start.size/6,"closeend=",close_end.size/6
    #print "big=",big.size/6,"small=",small.size/6,"d_range size=",d_range_t[0].size
    #print "zeros=",zeros.size

    # need to break up larger intervals into size 50 ones
    # expanded_set=split_big_ones(close_start,50)

if "__main__" == __name__:
    if len(sys.argv)<3:
        print "bad arg list, need: event_num size"
        sys.exit(1)

    event_num = sys.argv[1]
    size = int(sys.argv[2])
    inp_range = glob.glob("sig_%s_*no_noise*"%event_num)
    inp_sig = glob.glob("%s_*no_noise*"%event_num)

    for n in zip(inp_range,inp_sig):
        outfile = "new%s"%n[0]
        print n,outfile
        rewrite_ranges(n,size,outfile)
    
    
    
