
# in:  id view plane cell energy
# out: x/I,y/I,z/I,xerr/F,yerr/F,zerr/F,energy/F,eid/F,view/I

# in
# eid/I,type/S,channel/I,plane,wireindex,wiresz,samples/I,ped/F,sigma/F
# out
# channel,value

f = open("testrun_di.csv","r")
# f = open("output_di.csv","r")

first = f.readline()

for line in f:
    i=line.split(',')
    # print "%4.4x"%int(i[2]), i[6], i[7]
    print int(i[2]), i[3], i[4], i[5], i[9], i[10]
