library(lattice)
library(rgl)
library(doBy)

#di.u<-subset(di,V3<240)
#di.v<-subset(di,V3>=240)
#di.um<-as.matrix(di.u[,7:3078])
#di.vm<-as.matrix(di.v[,7:3078])

                                        # cut value is 10

                                        # x<-data.frame(x=1:3072, y=di.um[343,])
                                        # test.all<-data.frame(x=test.x, y=test.y, z=test.z)
# x<-data.frame(x=1:3072, y=di.um[117,])
# x1<-data.frame(x=1:3072, y=di.vm[117,])
# x2<-data.frame(x=x$x, y1=x$y, y2=x1$y) 

mydiff<-function(ua,va)
{
  u=ua #+2048
  v=va #+2048

  floor(1/(abs(u-v)%/%15+1)) * u
}

#di.uv1<-outer(di.um[,1], di.vm[,1], FUN=mydiff)
#di.uv2<-outer(di.um[,2], di.vm[,2], FUN=mydiff)
#persp3d(1:240,1:240,di.uv1)

norm.plane<-function(p)
    {
        t.p<-p
        cut.p<-mean(p) + sd(p) * 2
        t.p[abs(p) < cut.p] <- 0
        t.p
    }

norm.plane.orig<-function(p)
    {
        t.p<-p
        cut.p<-mean(p) + sd(p) * 2
        t.p[abs(p) < cut.p] <- 0
        max.p<-apply(t.p,1, function(r) { max(r) })
        max.p <- max.p + .00001
        t <- t.p / max.p
        t[ abs(t)>1000 ] <- 0
        t * 100
    }

                                        # yikes
                                        # persp3d(1:240,1:3,t.p[,1647:1649]/max.p)
                                        # this does not plot.
                                        # if you do it at 1648, it works!

mymatrix<-function(i, um, vm)
{
    uv<-outer(um[,i], vm[,i], FUN=mydiff)
                                        # uv[uv==0]<-NA
    uv
}


myplot<-function(i, um, vm)
{
  uv<-mymatrix(i,um,vm)
  levelplot(uv)
}

myprint<-function(i,um,vm)
    {
        u=norm.plane(um)
        v=norm.plane(vm)
        uv<-mymatrix(i,u,v)

        d<-data.frame(t=rep(i,length(uv)), v=rep(1:ncol(uv),each=nrow(uv)), u=rep(1:nrow(uv),ncol(uv)),c=as.vector(uv) )
        d<-subset(d,c!=0)
        write.table(d,file="piglets.txt", append=TRUE, row.names=FALSE,col.names=FALSE)
        
                                        #cat(i,sep=' ')
                                        #cat(uv, file="piglets.txt", append=TRUE, sep='\n')
        cat(paste(i,'\n'))
        d
    }

mywrite<-function(um,vm)
    {
        for(i in 1:ncol(um)) myprint(i,um,vm)
    }

justtest <- function()
    {
        di.temp<-matrix(di.m<15&di.m>(-15),480,3072)
        nrow(di.temp)
        # di.m[di.temp]<-NA
        temp<-di.m
        temp[di.temp]=NA
        persp3d(1:480,1:3072,di.filt)
        nrow(temp)
        ncol(temp)
        persp3d(1:480,1:3072,temp)
    }

                                        # scatterplot3d(a$V1,a$V2,a$V3, color=(a$V4+900)/70,pch='.',angle=45)

