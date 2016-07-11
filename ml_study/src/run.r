
library(lattice)
a<-read.table("outs.txt",header=TRUE)
b<-read.table("out.txt",header=F,fill=T,col.names=1:370)
b<-as.matrix(b)
f<-function(num) { print(table(is.na(b[num,]))); print(a[num,]) }
subset(a,size>250)
