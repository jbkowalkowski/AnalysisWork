#----------------------------------------------------------
# classify_5.py
#
# Roberto Santos - updated 7/20/16
#
# proper usage:
#	$ python classify_5.py <yaml file>
#----------------------------------------------------------

import os
import numpy as np
import yaml
import h5py
import sys
import getopt
import timeit
from sklearn.metrics import roc_auc_score
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import LSTM, Dense
from keras.layers import Dropout, Activation, Embedding
from keras.optimizers import SGD, RMSprop, Adam, Nadam
from keras.layers.embeddings import Embedding
from keras.layers.wrappers import TimeDistributed
from keras.models import model_from_yaml
import keras.backend as K
import random
import tqdm as tqdm
import Image

#----------------------------------------------------------
# load yaml file containing parameters
#----------------------------------------------------------

if len(sys.argv) != 2:
        print 'Incorrect number of arguments'
        print 'Correct usage: $ python prep_train_data.py <yaml>'
        sys.exit(1)
elif not os.path.isfile(sys.argv[1]):
        print sys.argv[1],'is not a valid file'
        print 'Correct usage: $ python prep_train_data.py <yaml>'
        sys.exit(1)
else:
        try:
                with open(sys.argv[1],'r') as infile:
                        yamlfile = yaml.load(infile)


		                # experiment name
                #---------------------------------------------------------
                expname = yamlfile['expname']

                # data parameters from yaml
                #----------------------------------------------------------
                wirelength = yamlfile['wirelength']
                numlabels = yamlfile['numlabels']
                numattributes = yamlfile['numattributes']
                batchsize = yamlfile['batchsize']
                sequencelength = wirelength
                seedvalue = yamlfile['prepseed']

                # training parameters from yaml
                #----------------------------------------------------------
                numepochs = yamlfile['numepochs']
                verboseset = yamlfile['verboseset']
                wirelength = yamlfile['wirelength']
                savetrainedmodel = yamlfile['savetrainedmodel']
                batchesinmem = wirelength//sequencelength
                monitormetric = yamlfile['monitormetric']

                # source data files from yaml
                #----------------------------------------------------------
                events = np.asarray(yamlfile['test_events'])
                truthfiles = np.asarray(yamlfile['test_truth_files'])
                datafiles = np.asarray(yamlfile['test_data_file'])
                datafiles = datafiles.reshape(len(truthfiles),len(datafiles)/len(truthfiles))

		# model files
		#----------------------------------------------------------
		modelfile = expname + '_model.yaml'
		loadbest = yamlfile['loadbest']
		if loadbest: modelweightfile = expname +'_best_'+monitormetric+'_'+'weights.h5'
		else: modelweightfile = expname + '_trained_weights.h5'
		algorithm = eval(yamlfile['algorithm'])
		lossfunction = yamlfile['lossfunction']
		dropwires = yamlfile['droptestwires']

		# save predictions
		#---------------------------------------------------------
		savepredictions = yamlfile['savepredictions']
		saveimage = yamlfile['saveimage']

        except yaml.YAMLError as e:
                print(e)
                sys.exit(1)

# green = [0,255,0]
# blue = [0,0,255]
# yellow = [0,255,255]
# red = [255,0,0]
colors = {0:[0,255,0], 1:[255,255,0], 2:[255,0,0], 3:[0,0,255]}

#----------------------------------------------------------
# load model from file
#----------------------------------------------------------

print 'Reconstructing model...'
with open(modelfile, 'r') as infile:
	model = model_from_yaml(yaml.load(infile))
model.load_weights(modelweightfile)
model.compile(loss=lossfunction, optimizer=algorithm)
model.summary()

#----------------------------------------------------------
# calculates and displays performance stats per timestep
# over all sequences tested
#----------------------------------------------------------

def calc_test_stats(tp,tn,fp,fn,signaldist,noisedist,filename,AUCtruth,prob):
	
	sumtp = tp.sum(dtype=np.int32)
	sumtn = tn.sum(dtype=np.int32)
	sumfp = fp.sum(dtype=np.int32)
	sumfn = fn.sum(dtype=np.int32)
	if sumtp+sumfn==0.0: tpr=0.0
	else: tpr = float(sumtp)/(sumtp+sumfn)			# recall/sensitivity
	if sumfp+sumtn==0.0: tnr=0.0
	else: tnr = float(sumtn)/(sumfp+sumtn)			# specificity
	if sumtp+sumfp==0.0: ppv=0.0
	else: ppv = float(sumtp)/(sumtp+sumfp)			# precision
	acc = float(sumtp+sumtn)/(sumtp+sumtn+sumfp+sumfn)	# accuracy
        if ppv+tpr == 0.0: f1 = 0.0
        else: f1 = 2*float(ppv*tpr)/(ppv+tpr) 			# F1 score
	print 'Overall wireset performance:', filename
	print '-----------------------------------------------------------------------'
	print 'True Positive:    {:11d}'.format(sumtp)
	print 'False Positive:   {:11d}'.format(sumfp)
	print 'True Negative:    {:11d}'.format(sumtn)
	print 'False Negative:   {:11d}'.format(sumfn)
	print 'Recall (TPR):     {:11.4f}'.format(tpr)
	print 'Specificity (TNR):{:11.4f}'.format(tnr)
	print 'Precision:        {:11.4f}'.format(ppv)
	print 'Accuracy:         {:11.4f}'.format(acc)
	print 'F1 Score:         {:11.4f}'.format(f1)
	print 'AUC:              {:11.4f}'.format(roc_auc_score(AUCtruth,np.reshape(prob,len(AUCtruth))))
	print ''

	if os.path.isfile(filename): os.remove(filename)
	outfile = open(filename, "w")
	outfile.write('Overall wireset performance: '+ filename+'\n')
        outfile.write('-----------------------------------------------------------------------\n')
        outfile.write('True Positive:    {:11d}'.format(sumtp)+'\n')
        outfile.write('False Positive:   {:11d}'.format(sumfp)+'\n')
        outfile.write('True Negative:    {:11d}'.format(sumtn)+'\n')
        outfile.write('False Negative:   {:11d}'.format(sumfn)+'\n')
        outfile.write('Recall (TPR):     {:11.4f}'.format(tpr)+'\n')
        outfile.write('Specificity (TNR):{:11.4f}'.format(tnr)+'\n')
        outfile.write('Precision:        {:11.4f}'.format(ppv)+'\n')
        outfile.write('Accuracy:         {:11.4f}'.format(acc)+'\n')
        outfile.write('F1 Score:         {:11.4f}'.format(f1)+'\n')
        outfile.write('\n')
	outfile.write('            True       False        True       False      Recall Specificity \n')
	outfile.write('        positive    positive    negative    negative       (TPR)       (TNR)   Precision    Accuracy    F1 Score \n')
	outfile.write('     ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- \n')
	for i in range (0,len(tp)):
		if tp[i]+fn[i]==0.0: tpr=0.0
		else: tpr = float(tp[i])/(tp[i]+fn[i])                 	# recall/sensitivity
        	if fp[i]+tn[i]==0.0: tnr=0.0
		else: tnr = float(tn[i])/(fp[i]+tn[i])                 	# specificity   
        	if tp[i]+fp[i]==0.0: ppv=0.0
		else: ppv = float(tp[i])/(tp[i]+fp[i])                 	# precision
        	acc = float(tp[i]+tn[i])/(tp[i]+tn[i]+fp[i]+fn[i])      # accuracy
        	if ppv+tpr == 0.0: f1 = 0.0
        	else: f1 = 2*float(ppv*tpr)/(ppv+tpr) 			# F1 score
		outfile.write("{:4d}".format(i)+
 				" {:11d}".format(int(tp[i]))+
        			" {:11d}".format(int(fp[i]))+
        			" {:11d}".format(int(tn[i]))+
        			" {:11d}".format(int(fn[i]))+
       	 			" {:11.4f}".format(tpr)+
        			" {:11.4f}".format(tnr)+
        			" {:11.4f}".format(ppv)+
        			" {:11.4f}".format(acc)+
        			" {:11.4f}".format(f1)+
				" {:4d}".format(i)+'\n')
		if ((i+1)%5) == 0:
 			outfile.write('     ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- \n')   
	outfile.write('\n')
	outfile.write('Probability Distribution (Signal)\n')
	outfile.write('        <0.10     <0.20     <0.30     <0.40     <0.50     <0.60     <0.70     <0.80     <0.90     <1.00  \n')
	outfile.write('       -------   -------   -------   -------   -------   -------   -------   -------   -------   ------- \n')
	for i in range (0, len(signaldist)):
		outfile.write("{:4d} ".format(i))
		for j in range(0, 20):
			denominator = np.sum(signaldist[i],dtype=np.int32)
			if j%2 == 1:
				output = signaldist[i][j] + signaldist[i][j-1]
				if denominator==0: outfile.write('{:9.4f} '.format(0.0))
				else: outfile.write('{:9.4f} '.format(float(output)/denominator))
		outfile.write("{:4d}".format(i)+'\n')
		if ((i+1)%5) == 0:
			outfile.write('       -------   -------   -------   -------   -------   -------   -------   -------   -------   ------- \n')
	outfile.write('\n')
        outfile.write('Probability Distribution (Noise)\n')
        outfile.write('        <0.10     <0.20     <0.30     <0.40     <0.50     <0.60     <0.70     <0.80     <0.90     <1.00  \n')
        outfile.write('       -------   -------   -------   -------   -------   -------   -------   -------   -------   ------- \n')
        for i in range (0, len(noisedist)):
		outfile.write("{:4d} ".format(i),)
                for j in range(0, 20):
			denominator = np.sum(noisedist[i],dtype=np.int32)
                        if j%2 == 1:
                                output = noisedist[i][j] + noisedist[i][j-1]
				if denominator==0: outfile.write('{:9.4f} '.format(0.0))
                                else: outfile.write('{:9.4f} '.format(float(output)/denominator))
                outfile.write("{:4d}".format(i)+'\n')
		if ((i+1)%5) == 0:
			outfile.write('       -------   -------   -------   -------   -------   -------   -------   -------   -------   ------- \n')
        outfile.close()

#----------------------------------------------------------
# bins and displays probabilities from binary class
# prediction
#----------------------------------------------------------

def analyze_wire(truth,prob,signaldist,noisedist,timestep):
        tp = 0
        tn = 0
        fp = 0
        fn = 0
	for i in range(0,len(prob)):
		if truth[i,1] == 1.:
			index = int(prob[i][1]*20)
			if index == 20: index = 19
			signaldist[index] += 1
			if prob[i][0]<prob[i][1]: tp += 1
                       	else: fn += 1
		else:
			index = int(prob[i][0]*20)
			if index == 20: index = 19
			noisedist[index] += 1
			if prob[i][0]>=prob[i][1]: tn += 1
                       	else: fp += 1
        return tp,tn,fp,fn

#----------------------------------------------------------
# iterates through events/truthfiles/datafiles/wires and
# converts wire to windowed segments which are saved to a
# hdf5 file
#----------------------------------------------------------

start_time = timeit.default_timer()              
for i in range(0,len(events)):
	random.seed(10)
        for j in range(0,len(truthfiles)):           
                filename = events[i]+truthfiles[j]  
                truth = np.fromfile(filename,dtype=np.short)
		AUCtruth = np.copy(truth)                              
                truth = truth.reshape(len(truth)//wirelength,wirelength)       
		for k in range(0,len(datafiles[j])):
                        filename = events[i]+datafiles[j][k]
                        print 'Processing',filename,'...'
			data = np.fromfile(filename,dtype=np.short)        
                        data = data.reshape(len(data)//wirelength,wirelength,numattributes)
			signaldist = np.zeros((sequencelength,20))
        		noisedist = np.zeros((sequencelength,20))
			tp = np.zeros(sequencelength)
			tn = np.zeros(sequencelength)
			fp = np.zeros(sequencelength)
			fn = np.zeros(sequencelength)
			probset=list()
			for l in range(batchsize,len(data)+1,batchsize):
					start = l - batchsize
					end = l
				 	t = list()
                                        for n in range(start,end): t.append((np_utils.to_categorical(truth[n],numlabels)))
                                        d = data[start:end]
					wireprobabilities =(model.predict_proba([d,d],batch_size=batchsize,verbose=0))
					t = np.asarray(t)	
					for m in range(0,sequencelength):
						temptruth = np.reshape(t[:,m,:],(len(t),numlabels))
						tempprob = np.reshape(wireprobabilities[:,m,:],(len(t),numlabels))
						stp,stn,sfp,sfn = analyze_wire(temptruth,tempprob,signaldist[m],noisedist[m],m)
						tp[m] += stp
						tn[m] += stn
						fp[m] += sfp
						fn[m] += sfn
					probset.extend(wireprobabilities)
			print 'Prediction performance for',filename
			print 'Testing on',(tp[0]+tn[0]+fp[0]+fn[0])//wirelength,'full wires'
			statfilename = filename + '_' + expname + '_performance.txt'
			probset = np.reshape(probset,(len(truth)//batchsize,wirelength//sequencelength, batchsize, sequencelength,numlabels)).swapaxes(1,2).reshape(len(truth),wirelength,numlabels)[:,:,1]
			calc_test_stats(tp,tn,fp,fn,signaldist,noisedist,statfilename,AUCtruth,probset)
			if savepredictions:
				predictfilename = filename + '_' + expname + '_predictions.h5'
				if os.path.isfile(predictfilename): os.remove(predictfilename)
				savefile = h5py.File(predictfilename,"a")	
                              	probwriteset = savefile.create_dataset("probabilities",data=probset, dtype=float,chunks = True)
				savefile.close()
			if saveimage:
				print 'Generating image...'
                       	 	imagefilename = filename + '_' + expname + '_image.bmp'
                        	imagemap = np.add(truth,np.rint(probset)*2)
                        	image=np.zeros((len(imagemap),len(imagemap[0]),3),dtype=np.uint8)
                        	for index,color in colors.iteritems():
                                	image[imagemap==index]=color
                        	img = Image.fromarray(image, 'RGB')
                        	img.save(imagefilename,format='bmp')
			elapsed = (timeit.default_timer() - start_time)/60
                        print 'Total elapsed time:{:8.1f}'.format(elapsed),'minutes'



