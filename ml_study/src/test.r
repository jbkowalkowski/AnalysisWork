
                                        #
library(lattice)

tot.samp<-9600
freq<-60
                                        # 9600 / 2*pi
steps<-seq(0,2*pi * freq, length.out=tot.samp*2*pi)
steps.1<-seq(0,2*pi*193, length.out=tot.samp*2*pi)

a<-xyplot((sin(steps)+sin(steps.1))~steps, pch='.')
print(a)
