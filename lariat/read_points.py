
f = open("piglets.txt","r")

t=0
u=0
v=0

for line in f:
    i=float(line)
    if i!=0:
        print t,v,u,i
    u = (u+1)%240
    if u==0: v = (v+1)%240
    if u==0 and v==0: t = (t+1)%3072


        
    
    
