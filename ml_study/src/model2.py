
import numpy as np
from keras.datasets import imdb
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import GRU
from keras.layers import Dropout
from keras.layers.embeddings import Embedding
from keras.preprocessing import sequence
from theano.tensor.shared_randomstreams import RandomStreams

import make_windows as mw
import sys

# fix random seed for reproducibility
np.random.seed(7)
srng = RandomStreams(7)

# load the dataset but only keep the top n words, zero the rest
# read all the training data
run_num = '184686'
plane = 'U'
win_size=21
d_rec_count=12000
n_rec_count=12000
d_mycount=win_size*d_rec_count
n_mycount=win_size*n_rec_count
fX = open("%s_%s_train_X.csv"%(run_num,plane),'r')
fy = open("%s_%s_train_y.csv"%(run_num,plane),'r')
fXn = open("%s_%s_train_Xn.csv"%(run_num,plane),'r')
fyn = open("%s_%s_train_yn.csv"%(run_num,plane),'r')

X_data = np.fromfile(fX,dtype=int,sep=' ',count=d_mycount)
X_noise = np.fromfile(fXn,dtype=int,sep=' ',count=n_mycount)
y_data = np.fromfile(fy,dtype=int,sep=' ',count=d_mycount)
y_noise = np.fromfile(fyn,dtype=int,sep=' ',count=n_mycount)

X_data.shape=(X_data.size/win_size,win_size,1)
X_noise.shape=(X_noise.size/win_size,win_size,1)
y_data.shape=(y_data.size/win_size,win_size,1)
y_noise.shape=(y_noise.size/win_size,win_size,1)

y_data = y_data[:,10] # center point
y_noise = y_noise[:,10] # center point

# concatenate data+noise for X,y
n_sz=n_rec_count
d_sz=d_rec_count

X_train = np.concatenate((X_data[0:d_sz],X_noise[0:n_sz]),axis=0)
y_train = np.concatenate((y_data[0:d_sz],y_noise[0:n_sz]),axis=0)
#X_train_d = X_data[0:d_sz]
#X_train_n = X_noise[0:n_sz]
#y_train_d = y_data[0:d_sz]
#y_train_n = y_noise[0:n_sz]

#X_test = np.concatenate((X_data[d_sz:d_rec_count],X_noise[n_sz:n_rec_count]),axis=0)
#y_test = np.concatenate((y_data[d_sz:d_rec_count],y_noise[n_sz:n_rec_count]),axis=0)

# create the model
model = Sequential()
model.add(LSTM(42, input_shape=(X_train.shape[1], X_train.shape[2]), return_sequences=True,dropout_U=.2, dropout_W=.2))
model.add(LSTM(84))
model.add(Dense(1, activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
print(model.summary())
model.fit(X_train, y_train, nb_epoch=14, batch_size=32) #, validation_split=0.2) #,shuffle=False)
#model.fit(X_train_d, y_train_d, nb_epoch=2, batch_size=1) #21*3)#, validation_split=0.25) #,shuffle=False)
# Final evaluation of the model
# scores = model.evaluate(X_test, y_test, verbose=0)
# print("Accuracy: %.2f%%" % (scores[1]*100))

#probs = model.predict(X_test)
#print probs

# read in all the test data for another run number
print "reading in all the data for the test event"
ad = mw.SigData(184687,'U',21)
k=0
ct=[0,0,0,0] # tp, tn, fp,fn
cf=[0,0,0,0]
total=0
print ct
print cf

print "starting up the eval"
for t,f,a in zip(ad.inp_noise_time[800:], ad.inp_noise_freq[800:], ad.answers[800:]):
    # calculate median for the wires
    med_t = np.median(t)
    med_f = np.median(f)
    for i in range(0,ad.num_samples - ad.window+1):
        tmp_t = t[i:i+21]-med_t
        tmp_f = f[i:i+21]-med_f
        tmp_t.shape=(1,21,1)
        tmp_f.shape=(1,21,1)
        rt=model.predict_classes(tmp_f,batch_size=1,verbose=0)[0][0]
        rf=model.predict_classes(tmp_f,batch_size=1,verbose=0)[0][0]
        ans=a[i:i+21]
        #print rt,rf,ans[10]
        ct[0] = ct[0] + int(rt==True and rt==ans[10])
        ct[1] = ct[1] + int(rt==False and rt==ans[10])
        ct[2] = ct[2] + int(rt==False and rt!=ans[10])
        ct[3] = ct[3] + int(rt==True and rt!=ans[10])
        cf[0] = cf[0] + int(rf==True and rf==ans[10])
        cf[1] = cf[1] + int(rf==False and rf==ans[10])
        cf[2] = cf[2] + int(rf==False and rf!=ans[10])
        cf[3] = cf[3] + int(rf==True and rf!=ans[10])
        total = total + 1
    k = k + 1
    print ct
    print cf
    print k

print total

