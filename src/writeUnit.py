#!/usr/bin/env python
from illumination.srv import *
import rospy
import json
import argparse
from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.tools.shortcuts import buildNetwork
from pybrain.structure import TanhLayer
from pybrain.tools.customxml.networkwriter import NetworkWriter
'''
from sklearn import svm,tree
from sklearn.externals import joblib
'''
from pybrain.tools.customxml.networkreader import NetworkReader

import numpy as np

import os.path
globalTargetClass = -1; #Specify the target of this current dataset here for use in writing training data
writeFile = ""
#Write training data to file
def writeTraining(req):
	print "[Preparing] to write Training Data..."
	mydata = list(req.data)
	#global globalTargetClass
	#append the target here,  no need to append if using SVM
	mean = []
  	quadrants = zip(*[iter(mydata)]*76800) #split it into 4
	#print quadrants[1]
  	for i in quadrants:
		x = np.mean(i)
		mean.append(int(x))
	print mean
	mean.append(req.degNum)
	mean.append(globalTargetClass) #append the target classification

	imageNumber = (req.imgNum)+1 	#increment number of images taken so far

	file = open(writeFile,'a+')
	json.dump(mean,file)			#write in json format to file
	file.write('\n')
	file.close()
	print "[Data Written]"
	print "[Returning] Count Number..."
	return ArrayDataResponse([imageNumber,-1])

def trainNetwork():
	print "[Training] Network has Started..."
	inputSize = 0
	with open('file1.txt', 'r') as f:			#automatically closes file at the end of the block
  		#first_line = f.readline()
  		#inputSize = len(first_line)
		dataset = SupervisedDataSet(4, 1)	 #specify size of data and target
		f.seek(0) 							#Move back to beginnning of file
		#iterate through the file. 1 picture per line
		for line in f:
			mylist = json.loads(line)		#list object
	    	target = mylist[-1]				#retrieve and then delete the target classification
	    	del mylist[-2:]
	    	#print target
	    	dataset.addSample(tuple(mylist), (target,))
	        #print json.loads(line)
	if os.path.isfile('annModel.xml'):
		skynet = NetworkReader.readFrom('annModel.xml')#for use if individual sample files used
	else:
		skynet = buildNetwork(dataset.indim, 8, dataset.outdim, bias=True, hiddenclass=TanhLayer) #input,hidden,output
	#SoftmaxLayer, SigmoidLayer, LinearLayer, GaussianLayer
	#Note hidden neuron number is arbitrary, can try 1 or 4 or 3 or 5 if this methods doesnt work out
	trainer = BackpropTrainer(skynet, dataset,learningrate = 0.3, weightdecay = 0.01,momentum = 0.9)
	#trainer.trainUntilConvergence()
	for i in xrange(1000):
		trainer.train()
    #trainer.trainEpochs(1000)
    #Save the now trained neural network
	NetworkWriter.writeToFile(skynet,'annModel.xml')
	print "[Network] has been Written"

################## SVM Method #######################
#Change append method in write method for target persistence
	dataX = []
	datay = []
	with open(writeFile, 'r') as f:
		for line in f:
			mylist = json.loads(line)
			target2 = mylist[-1]
			dataX.append(mylist[:-2])
			datay.append(target2)
	#datay = [target2] * len(dataX) #Targets, size is n_samples, for use with indiviual sample files with same target
	print [target2]
	print dataX
	print datay
	clf = svm.LinearSVC()
	clf.fit(dataX,datay)
    #Persist the trained model
	joblib.dump(clf,'svmModel.pkl')
    	#joblib.dump(clf,'treeModel.pkl')

def arrayDataServer():
	rospy.init_node('writeUnit')
	s = rospy.Service('array_data',ArrayData,writeTraining)
	print "[Greenlight]: to receive data..."
	print writeFile
	rospy.spin()

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('-s', action = 'store_true',default=False,dest = 'switchNode',help = "Initiate Node. Obtain training data and write it to file")
	parser.add_argument('-l',action = 'store_true',default = False, dest = 'switchTrain',help = "Initiate Network learning and saves the trained neural network to file")
	parser.add_argument('-t', action = 'store', dest='classValue',nargs =1 ,help = "Specify either 1 or 0 for data target. 1 is for a broken light, 2 is for a non-broken light.")
	parser.add_argument('filename')
	commandline = parser.parse_args()
	initNode = commandline.switchNode
	initTrain = commandline.switchTrain
	writeFile = str(commandline.filename)
	 #global globalTargetClass #ensure it is the global variable modified for the target class for this dataset
	target = commandline.classValue
		#return -1
	if initNode:#if the boolean is True then initiate Node
		target = int(target[0])
		#print type(target)
		#if target == (0 or 1):
		globalTargetClass = target
		#print globalTargetClass
		#else:
			#print "[Invalid] target class, select 1 or 0"
		arrayDataServer()
	elif initTrain:#if the boolean is set to False then initiate Training
		trainNetwork()
	else:
		print "Please Specify an action to take using flags. Use help for more details."

	#####Testing below##########

	#If switch is set to true then run node, if set to false then run training
	#these 2 as flag options to specify what the current data set its writing is
	#If light is broken then 1
	#mylist.append(1)
	#If light is not broken then 0
	#mylist.append(0)
