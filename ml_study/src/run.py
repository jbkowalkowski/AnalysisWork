
f = open("184685_width_U.csv",'r')
out=open("out.txt",'w')
outs=open("outs.txt",'w')

# 6=nothing, 0=signal, 1=no signal, 5=enough no sigs after signal
# 6->0 : mark start
# 1->2 : mark end
# 5->6 : report range

no_sig=3

print >>outs,"start end size sum"

for w in f:
    samps=w.split()
    state=no_sig
    curr=int(samps[0])
    sum=0
    start=0
    end=0
    # print s[0], type(w), len(w),len(s)
    for i in range(1,len(samps)):
        sample = int(samps[i])
        d=sample-curr
        curr=sample
        if state==0:
            if d==0:
                state = state + 1
                end=i
        elif state==no_sig:
            if d!=0:
                state=0
                start=i
        elif state==(no_sig-1):
            if d==0:
                print >>outs,start,end,(end-start),sum
                for x in range(start,end): print >>out,samps[x],
                print >>out,' '
                start=0
                end=0
                state = state + 1
                #sum=0
            else:
                state=0
        else:
            if d==0:
                state = state + 1
            else:
                state=0
        sum = sum + d

    if start!=0 or end!=0:
        if end<start: end=len(samps)
        print >>outs,start,end,(end-start),sum
        for x in range(start,end): print >>out,samps[x],
        print >>out,' '
    #print '----'

