
setup mvapich2 v1_9_0
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
