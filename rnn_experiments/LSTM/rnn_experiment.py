#-----------------------------------------------------------
# simplified training and testing of Keras RNN
#----------------------------------------------------------

import os
import yaml
import timeit
import h5py
import numpy as np
from tqdm import tqdm
import itertools
from keras.models import Sequential
from keras.layers import SimpleRNN, LSTM, GRU, Dense, Merge
from keras.layers import Dropout, Activation, Embedding
from keras.utils import np_utils
from keras.optimizers import SGD, RMSprop, Adam, Nadam
from keras.layers.embeddings import Embedding
from keras.layers.wrappers import TimeDistributed
import keras.backend as K
import sys
import random
from keras.callbacks import EarlyStopping
from keras.callbacks import ModelCheckpoint
from keras.models import model_from_yaml

#----------------------------------------------------------
# parameters
#----------------------------------------------------------

loadtrainedmodel = False
modelname = 'savedmodel.yaml'
modelweights = 'modelweights.h5'
bestweights = 'best_val_loss_weights.h5'

traindata = 'datafile'
traintruth = 'truthfile'
testdata = 'test1'
testtruth = 'test2'

sequencelength = 21
windowsize = 1
batchsize = 10

#----------------------------------------------------------
# build model - load previously saved model or build new
#	model defined in build_model()
#----------------------------------------------------------

#-----function containing model structure------------------

def build_model():
	model = Sequential()
	model.add(
		GRU(16,
		input_shape = (sequencelength,windowsize),
		return_sequences = True,
		activation = ‘softsign’,
		dropout_W = 0.2,
		dropout_U = 0.2))
	model.add(
		GRU(16,
		return_sequences = True,
		activation = ‘softsign’, 
       		dropout_W = 0.2, 
		dropout_U = 0.2))
	model.add(TimeDistributed(Dense(2)))
	model.add(Activation(‘softmax’))
	return model

#-----load previous model or build new model and save------

if loadtrainedmodel:
        try:
                print 'Reconstructing previously saved model'
                with open(modelname, 'r') as infile:
                        model = model_from_yaml(yaml.load(infile))
        except IOError as e:
                print 'Error: loading previously saved model failed'
                print e
                sys.exit(0)
else:
        model = build_model()
	if os.path.isfile(modelname):
                print 'Overwriting previous model file'
                os.remove(modelname)
        yaml_string = model.to_yaml()
        with open(modelname, 'w') as outfile:
                outfile.write(yaml.dump(yaml_string, default_flow_style=True))

#-----compile model and display stats----------------------

model.compile(loss='binary_crossentropy', optimizer=Nadam(), metrics=['accuracy'])
model.summary()

#----------------------------------------------------------
# load data - load and shape data and truth from files
#----------------------------------------------------------

def load_data(datafile,truthfile):
	data = np.fromfile(datafile,dtype=np.short)
	truth = np.fromfile(truthfile,dtype=np.short)
	
	numsamples = len(data)//sequencelength//windowsize
	data = data.reshape(numsamples,sequencelenth,windowsize)
	assert len(data) == len(truth)
	truth = np_utils.to_categorical(truth,numlabels)

	return data, truth

#----------------------------------------------------------
# train model - use a previously trained model or train
# 	new model with training data; saves weights as 
#	it trains
#----------------------------------------------------------

if loadtrainedmodel:
	try:
		print 'Loading weights'
		model.load_weights(modelweights)
	except IOError as e:
		print 'Error: loading previously saved weights failed'
		print e
		sys.exit(0)
else:
	callbacklist = list()
	callbacklist.append(ModelCheckpoint(bestweights, monitor='val_loss', verbose=0, save_best_only=True, mode='auto'))
	callbacklist.append(ModelCheckpoint(modelweights, verbose=0, save_best_only=False, mode='auto'))
	X, Y = load_data(traindata,traintruth)
	model.fit(X, Y,
		batch_size = batchsize,
                nb_epoch = 25,
                verbose = 1,
                validation_split = 0.1,
                callbacks = callbacklist)

#--------------------------------------------------------
# test model - load test data and run evaluation
#--------------------------------------------------------

X_test, Y_test = load_data(testdata,testtruth)
score = model.evaluate(X_test, Y_test, batch_size = batchsize)
# probabilities = model.predict_proba(X_test,batch_size=batchsize)
