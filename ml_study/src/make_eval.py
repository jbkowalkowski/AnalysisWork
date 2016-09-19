
import sys
import make_windows as mw

def do_main():
    if len(sys.argv)<3:
        print "bad arg list, want run_number and plane (U,V,Y)"
        sys.exit(1)

    run_num = sys.argv[1]
    plane = sys.argv[2]
    win_size = 21
    total=10

    sigdata = mw.SigData(run_num, plane,win_size)
    Xt = mw.make_wins(sigdata.inp_noise_time,win_size,total)
    Xf = mw.make_wins(sigdata.inp_noise_freq,win_size,total)
    y = mw.make_wins(sigdata.answers,win_size,total)

    fXt = open("%s_%s_eval_Xt.csv"%(run_num,plane),'w')
    fXf = open("%s_%s_eval_Xf.csv"%(run_num,plane),'w')
    fy = open("%s_%s_eval_y.csv"%(run_num,plane),'w')

    for r in Xt:
        r.tofile(fXt,sep=' ',format="%d")
        print >>fXt,"\n",
    for r in Xf:
        r.tofile(fXf,sep=' ',format="%d")
        print >>fXf,"\n",
    for r in y:
        r.tofile(fy,sep=' ',format="%d")
        print >>fy,"\n",
    print "Done with train_X write"




if "__main__" == __name__:
    do_main()

