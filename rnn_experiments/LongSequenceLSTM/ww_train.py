#-----------------------------------------------------------
# script to create and train multilayer RNN network -
#
# Roberto Santos - updated 8/4/16
#
# proper usage:
#	$ python rnn_train.py <yaml file>
#
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
# load yaml file containing parameters
#----------------------------------------------------------

if len(sys.argv) != 2:
        print 'Incorrect number of arguments'
        print 'Correct usage: $ python prep_train_data.py <yaml>'
        sys.exit(0)
elif not os.path.isfile(sys.argv[1]):
        print sys.argv[1],'is not a valid file'
        print 'Correct usage: $ python prep_train_data.py <yaml>'
        sys.exit(0)
else:
        try:
                with open(sys.argv[1],'r') as infile:
                        yamlfile = yaml.load(infile)

		# experiment name
		#---------------------------------------------------------
		expname = yamlfile['expname']
		savemodelname = expname + '_model.yaml'
		trainedweightname = expname + '_trained_weights.h5' 
                
		# training data file from yaml
                #---------------------------------------------------------
                events = np.asarray(yamlfile['train_events'])
                truthfiles = np.asarray(yamlfile['train_truth_files'])
                datafiles = np.asarray(yamlfile['train_data_file'])
                datafiles = datafiles.reshape(len(truthfiles),len(datafiles)/len(truthfiles))

                # data parameters from yaml
                #----------------------------------------------------------
                wirelength = yamlfile['wirelength']
                numlabels = yamlfile['numlabels']
                numattributes = yamlfile['numattributes']
 		batchsize = yamlfile['batchsize']
                sequencelength = wirelength
                reservevalidation = yamlfile['reservevalidation']
                seedvalue = yamlfile['prepseed']
                droptrainingwires = yamlfile['droptrainingwires']

		# training parameters from yaml
		#----------------------------------------------------------
		numepochs = yamlfile['numepochs']
		verboseset = yamlfile['verboseset']
		wirelength = yamlfile['wirelength']
		savetrainedmodel = yamlfile['savetrainedmodel']
		batchesinmem = wirelength//sequencelength

		# callbacks
		#----------------------------------------------------------
		earlystopping = yamlfile['earlystopping']
		stopmetricmonitor = yamlfile['stopmetricmonitor']
		setpatience = yamlfile['patience']
		savebest = yamlfile['savebest']
		monitormetric = yamlfile['monitormetric']
		everyepochsave = yamlfile['everyepochsave']
		bestmodelfile = expname +'_best_'+monitormetric+'_'+'weights.h5'


		# load saved model parameters
		#----------------------------------------------------------
		loadsavedmodel = yamlfile['loadsavedmodel']
		loadname = yamlfile['previousexperiment']
		loadbest = yamlfile['loadbest']
		previousmodelfile = loadname + '_model.yaml'
		if loadbest: previousweightfile = loadname + '_best_'+monitormetric+'_weights.h5'
		else: previousweightfile = loadname + '_trained_weights.h5'

		# forward model parameters
		#----------------------------------------------------------
		layertype = yamlfile['layertype']
		numlayers = yamlfile['numlayers']
		numnodes = yamlfile['numnodes']
		unroll = yamlfile['unroll']
		inneractivation = yamlfile['inneractivation']
		dropoutW = yamlfile['dropoutW']
		dropoutU = yamlfile['dropoutU']
		stateful = yamlfile['stateful']
		consumeless = yamlfile['consumeless']
		gobackwards = yamlfile['gobackwards']			
		algorithm = eval(yamlfile['algorithm'])
		lastactivation = yamlfile['lastactivation']
		lossfunction = yamlfile['lossfunction']
		metricslist = yamlfile['metricslist']
		mergemode = yamlfile['mergemode']   

	except yaml.YAMLError as e:
		print'Error trying to open',sys.argv[1]
		print(e)
                sys.exit(1)


def wires_to_batches(truth,data):
        t = list()
        d = list()
        for i in range(sequencelength,wirelength+1,sequencelength):
                start = i-sequencelength
                end = i
                temp = truth[:,start:end]
                for j in range(0,len(temp)):
                        t.append(np_utils.to_categorical(temp[j],numlabels))
                d.extend(data[:,start:end,:])
        return t,d



#----------------------------------------------------------
# training data generator
#----------------------------------------------------------

def train_generator():
	random.seed(seedvalue)
	valtruth = list()
	valdata = list()
	endflag = False	
	for i in range(0,len(events)):
        	for j in range(0,len(truthfiles)):
                	filename = events[i]+truthfiles[j]
                	try:
                        	truth = np.fromfile(filename,dtype=np.short)
                        	truth = truth.reshape(len(truth)//wirelength,wirelength)
                        	for k in range(0,len(datafiles[j])):
                                	filename = events[i]+datafiles[j][k]
                                	try:
                                        	data = np.fromfile(filename,dtype=np.short)
                                        	data = data.reshape(len(data)//wirelength,wirelength,numattributes)
                                        	for l in range(batchsize,len(data)+1,batchsize):
							if i>=len(events)-1 and j>=len(truthfiles)-1 and k>=len(datafiles[j])-1 and l>=len(data): endflag=True
                                                	if random.uniform(0,1) > droptrainingwires or endflag:
                                                        	start = l - batchsize
                                                        	end = l
								t = list()
								for m in range(start,end): t.append((np_utils.to_categorical(truth[m],numlabels)))
								d = data[start:end]
                                                        	if random.uniform(0,1) < reservevalidation and not endflag:
									valtruth.extend(t)
									valdata.extend(d)
                                                        	else:
									yield np.asarray(d), np.asarray(t), np.asarray(valdata), np.asarray(valtruth), endflag 
                                	except IOError:
                                        	print 'No file named',filename
                	except IOError:
                        	print 'No truth file for',events[i]


#----------------------------------------------------------
# create bidirectional model
#----------------------------------------------------------

def construct_bidirectional_model():
	print 'Validating new model syntax...'
	forward = Sequential()
	backward = Sequential()
	layerstring = layertype +'('+str(numnodes)+','
	layerstring += 'batch_input_shape=('+str(batchsize)+','+str(sequencelength)+','+str(numattributes)+'),'
	layerstring += "activation='"+inneractivation+"',"
	layerstring += 'return_sequences=True,'
	layerstring += 'stateful=False,'
	layerstring += 'dropout_W='+str(dropoutW)+','
	layerstring += 'dropout_U='+str(dropoutU)+','
	layerstring += 'go_backwards=False,'
	layerstring += 'unroll=False,'
	layerstring += "consume_less='"+consumeless+"')"
	forward.add(eval(layerstring))

	layerstring = layertype +'('+str(numnodes)+','
        layerstring += 'batch_input_shape=('+str(batchsize)+','+str(sequencelength)+','+str(numattributes)+'),'
        layerstring += "activation='"+inneractivation+"',"
        layerstring += 'return_sequences=True,'
        layerstring += 'stateful=False,'
        layerstring += 'dropout_W='+str(dropoutW)+','
        layerstring += 'dropout_U='+str(dropoutU)+','
        layerstring += 'go_backwards=True,'
        layerstring += 'unroll=False,'
        layerstring += "consume_less='"+consumeless+"')"
        backward.add(eval(layerstring))

        for i in range(0,numlayers-1):
		layerstring = layertype +'('+str(numnodes)+','
                layerstring += "activation='"+inneractivation+"',"
                layerstring += 'return_sequences=True,'
                layerstring += 'stateful=False,'
                layerstring += 'dropout_W='+str(dropoutW)+','
                layerstring += 'dropout_U='+str(dropoutU)+','
                layerstring += 'go_backwards=False,'
                layerstring += 'unroll=False,'
                layerstring += "consume_less='"+consumeless+"')"
		forward.add(eval(layerstring))
		
		layerstring = layertype +'('+str(numnodes)+','
                layerstring += "activation='"+inneractivation+"',"
                layerstring += 'return_sequences=True,'
                layerstring += 'stateful=False,'
                layerstring += 'dropout_W='+str(dropoutW)+','
                layerstring += 'dropout_U='+str(dropoutU)+','
                layerstring += 'go_backwards=True,'
                layerstring += 'unroll=False,'
                layerstring += "consume_less='"+consumeless+"')"
                backward.add(eval(layerstring))

	model = Sequential()
	model.add(Merge([forward,backward], mode = mergemode))
        model.add(TimeDistributed(Dense(numlabels)))
        model.add(Activation(lastactivation))
	return model

#----------------------------------------------------------
# load previously trained model or define from scratch
#----------------------------------------------------------

if loadsavedmodel:
	try:
		print 'Reconstructing previously saved model...'
		with open(previousmodelfile, 'r') as infile:
        		model = model_from_yaml(yaml.load(infile))
		model.load_weights(previousweightfile)
	except IOError as e:
		print 'Error: loading previously saved model failed.'
		print e
		sys.exit(0)
else:
	model = construct_bidirectional_model()
model.compile(loss=lossfunction, optimizer=algorithm, metrics=metricslist)
if savetrainedmodel:
        if os.path.isfile(savemodelname):
                print 'Overwriting previous model file.'
                os.remove(savemodelname)
        yaml_string = model.to_yaml()
        with open(savemodelname, 'w') as outfile:
                outfile.write(yaml.dump(yaml_string, default_flow_style=True))
model.summary()

#----------------------------------------------------------
# train single direction model
#----------------------------------------------------------

print('Compiling model...')
start_time = timeit.default_timer()

callbacklist = list()
if earlystopping:
	callbacklist.append(EarlyStopping(monitor=stopmetricmonitor, patience=setpatience, verbose=0,mode='min'))
if savebest:
	callbacklist.append(ModelCheckpoint(bestmodelfile, monitor=monitormetric, verbose=0, save_best_only=True, mode='auto'))
if everyepochsave:
	callbacklist.append(ModelCheckpoint(trainedweightname, verbose=0, save_best_only=False, mode='auto'))

for i in range(numepochs):
	print 'Epoch',i+1,'of',numepochs 
	for X,Y,valX,valY,flag in train_generator():
		if flag:
			sys.stdout.write('last data load in epoch. ')
			model.fit([X,X], Y,
				batch_size = batchsize,
				nb_epoch=1,
				verbose=verboseset,
				validation_data=([valX,valX],valY),
				callbacks = callbacklist)
			model.reset_states()
			elapsed = (timeit.default_timer() - start_time)/60
			print 'Total elapsed time:{:8.1f}'.format(elapsed),'minutes'
		else:
			model.fit([X,X], Y,
				batch_size = batchsize,
				nb_epoch=1,
				verbose=0)
			model.reset_states()
			sys.stdout.write('>')

#----------------------------------------------------------
# save weights
#----------------------------------------------------------

if savetrainedmodel:
	if os.path.isfile(trainedweightname):
        	os.remove(trainedweightname)
	model.save_weights(trainedweightname)
