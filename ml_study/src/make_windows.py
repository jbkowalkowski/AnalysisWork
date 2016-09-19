
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


class SigData:
    def __init__(self, run_num, plane, window):
        no_noise = "%s_sig_no_noise_%s.csv"%(run_num,plane)
        ranges = "sig_%s_sig_no_noise_%s.csv"%(run_num,plane)
        time_noise = "%s_sig_time_noise_%s.csv"%(run_num,plane)
        freq_noise = "%s_sig_freq_noise_%s.csv"%(run_num,plane)
        win_fraction = 5

        self.window=window
        self.run_num = run_num
        self.plane = plane
        print "getting range info",ranges
        self.num_samples = sr.get_num_samples(no_noise)
        self.d_range = sr.get_ranges(ranges)
        
        self.inp_sig = self.get_data(no_noise)
        self.inp_noise_time = self.get_data(time_noise)
        self.inp_noise_freq = self.get_data(freq_noise)
        print "done with data reading"
        
        self.d_only_nt = self.inp_noise_time - self.inp_sig
        self.d_only_nf = self.inp_noise_freq - self.inp_sig
        print "done with noise calc"
        
        self.answers = np.zeros(self.inp_sig.size,dtype=np.bool_)
        self.answers.shape = self.inp_sig.shape
        for r in self.d_range: self.answers[r[0]][r[1]:r[2]]=True
        print "done with setting answers"
            
        seq_X = []
        seq_y = []
        seq_n = []

        ns=(self.d_only_nt,self.d_only_nf)
        
        for r in self.d_range:
            wire=r[0]
            mid=r[5]
            start=max(r[1]-(self.window/win_fraction), 0)
            end=min(r[2]+self.window/win_fraction, self.num_samples)

            if end-start < self.window:
                # the window is too small, make it bigger
                if start < self.window: end = start+self.window
                else: start=end-self.window

            rstart=np.random.randint(0,high=self.num_samples-(end-start))
            rend=rstart+(end-start)
            noise_win = ns[np.random.randint(2)][wire][rstart:rend]
            noise = ns[np.random.randint(2)][wire][start:end]
            
            #nt = self.d_only_nt[wire][start:end]
            #nf = self.d_only_nf[wire][start:end]
            #ns = nt #+nf
            #largest = max(np.max(nt),np.max(nf))
            #m=largest / max(1,max(ns))
            #noise = m*ns

            seq_n.append(noise_win)
            seq_X.append( self.inp_sig[wire][start:end]-mid + noise )
            seq_y.append( self.answers[wire][start:end] )
        print "done with sequence generating"
        
        self.train_X = []
        self.train_y = []
        self.train_n = []
            
        for s_X,s_y,s_n in zip(seq_X,seq_y,seq_n):
            for i in range(0,len(s_X)-self.window+1):
                self.train_X.append(s_X[i:i+self.window])
                self.train_y.append(s_y[i:i+self.window])
                self.train_n.append(s_n[i:i+self.window])
        print "done with training data generation"
        print len(self.train_X),len(self.train_y), len(self.train_n)
            
    def get_data(self,fname):
        f_in = open(fname,'r')
        print "reading",fname
        d_in = np.fromfile(f_in,dtype=int,sep=' ')
        d_in.shape = (d_in.size/self.num_samples, self.num_samples)
        return d_in

def make_wins(sig,window,total):
    X=[]
    for i in range(0,min(sig.shape[0],total)):
        for j in range(0,sig.shape[1]-window+1):
            X.append(sig[j:j+window])
    return X
    
    
def do_main():
        
    if len(sys.argv)<3:
        print "bad arg list, want run_number and plane (U,V,Y)"
        sys.exit(1)

    run_num = sys.argv[1]
    plane = sys.argv[2]
    win_size = 21

    sigdata = SigData(run_num, plane,win_size)

    fX = open("%s_%s_train_X.csv"%(run_num,plane),'w')
    fy = open("%s_%s_train_y.csv"%(run_num,plane),'w')
    fXn = open("%s_%s_train_Xn.csv"%(run_num,plane),'w')
    fyn = open("%s_%s_train_yn.csv"%(run_num,plane),'w')

    for r in sigdata.train_X:
        r.tofile(fX,sep=' ',format="%d")
        print >>fX,"\n",
    print "Done with train_X write"

    for r in sigdata.train_n:
        r.tofile(fXn,sep=' ',format="%d")
        print >>fXn,"\n",
    print "Done with train_n write"

    for r in sigdata.train_y:
        r.tofile(fy,sep=' ',format="%d")
        print >>fy,"\n",
    print "Done with train_y write"

    for r in sigdata.train_n:
        tmp=np.zeros(win_size,dtype=np.bool_)
        tmp.tofile(fyn,sep=' ',format="%d")
        print >>fyn,"\n",
    print "Done with train_n write"
    
if "__main__" == __name__:
    do_main()


