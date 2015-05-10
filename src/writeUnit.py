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
def writeTraining(req):
	print "[Preparing] to write Training Data..."
	mydata = req.data
	print mydata

	#append the target here
	imageNumber = (req.imgNum)+1 	#increment number of images taken so far
	file = open("data.txt",'a+')
	json.dump(mydata,file)			#write in json format to file
	file.write('\n')
	file.close()
	print "[Data Written]"
	print "[Returning] Count Number..."
	return ArrayDataResponse(imageNumber)

def trainNetwork():
	print "[Training] Network has Started..."
	inputSize = 0
	with open('data.', 'r') as f:			#automatically closes file at the end of the block
  		first_line = f.readline()
  		featureNumber = len(first_line)
		dataset = SupervisedDataSet(inputSize, 1)	 #specify size of data and target
		f.seek(0) 							#Move back to beginnning of file
	    for line in f:						#iterate through the file. 1 picture per line
	    	mylist = json.loads(line)		#list object
	    	target = mylist[-1]				#retrieve and then delete the target classification
	    	del a[-1]
	    	dataset.addSample(tuple(mylist), (target,))
	        #print json.loads(line)
	skynet = buildNetwork(dataset.indim, int((inputSize+1)/2), dataset.outdim, bias=True, hiddenclass=TanhLayer) #input,hidden,output
	#SoftmaxLayer, SigmoidLayer, LinearLayer, GaussianLayer
	#Note hidden neuron number is arbitrary, can try 1 or 4 or 3 or 5 if this methods doesnt work out
	trainer = BackpropTrainer(skynet, ds,learningrate = 0.3, weightdecay = 0.01)
	#trainer.trainUntilConvergence()
	for i in xrange(1000):
    	trainer.train()
    #trainer.trainEpochs(1000)

    #Save the now trained neural network
    NetworkWriter.writeToFile(skynet, 'trainedNetwork.xml')
    print "[Network] has been Written"

def arrayDataServer():
	rospy.init_node('writeUnit')
	s = rospy.Service('array_data',ArrayData,writeTraining)
	print "[Greenlight]: to receive data..."
	rospy.spin()

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('-s', action = 'store_true',default=False,dest = 'boolean_switch',help = "Initiate Node. Obtain training data and write it to file")
	parser.add_argument('-l',action = 'store_false',default = False, dest = 'boolean_switch',help = "Initiate Network learning and saves the trained neural network to file")
	parser.add_argument('-t', action = 'store',dest='simple',type = int, dest='simple_value',help = "Specify either 1 or 0 for data target. 1 is for a broken light, 2 is for a non-broken light.")
	arrayDataServer()
	#####Testing below##########
	#If switch is set to true then run node, if set to false then run training
	#these 2 as flag options to specify what the current data set its writing is
	#If light is broken then 1
	#mylist.append(1)
	#If light is not broken then 0
	#mylist.append(0)