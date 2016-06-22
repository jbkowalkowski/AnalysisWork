
# setup mvapich2 v1_9_0
setup mvapich2 v2_1
setup gcc v4_9_2

mpirun_rsh -rsh -np 2 -hostfile hostfile ./main

# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64

# I=0; while [ $I -lt 150 ] ; do I=$((I+5)); mpirun_rsh -rsh -np $I -hostfile hostfile ./main; done
# mpirun_rsh -rsh -np 64 -hostfile hostfile ./main

mpirun_rsh -rsh -np 100 -hostfile hostfile2 ./main2

rsh grunt1 "echo 3 > /proc/sys/vm/drop_caches ; free"
rsh grunt2 "echo 3 > /proc/sys/vm/drop_caches ; free"
rsh grunt3 "echo 3 > /proc/sys/vm/drop_caches ; free"
rsh grunt4 "echo 3 > /proc/sys/vm/drop_caches ; free"
rsh grunt5 "echo 3 > /proc/sys/vm/drop_caches ; free"



# running on woof.fnal.gov
# iogroup would be something like 128 on Mira.  It is the number of IO gather points
# across the run.  The example before will use writing points 0,1,2,3 across 10 ranks
mpirun_rsh -ssh -np 10 -hostfile hostfile2 ./main --iogroup 3
