from illumination.srv import *
import rospy
import json
from pybrain.tools.customxml.networkreader import NetworkReader
from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.tools.shortcuts import buildNetwork
from select import select
import sys
from sklearn import svm
from sklearn.externals import joblib

#requires pybrain, and ?sklearn
#need flag to specify to load saved neural network or to train neural network
#If light is broken then 1
#If light is not broken then 0
def runClassifier(req):
	mylist = req.data
	imageNumber = (req.imgNum)+1
	#read in the trained Network
	skynet = NetworkReader.readFrom('data.xml') 
	output = round(skynet.activate(tuple(mylist)))

############SVM Classification below#############
	clf = joblib.load('svmModel.pkl')
	clf.predict([mylist])

	if output == 1:
		print "There is a Broken Light here." #Maybe record the information of where it is at
	else:
		print "ALL OK, no Broken Lights here"

	return ArrayDataResponse([imageNumber,ouput]);#Will send both the image number and the result of classification



def arrayClassificationServer():
	rospy.init_node('writeUnit')
	s = rospy.Service('array_data',ArrayData,runClassifier)
	print "[Greenlight]: to receive data..."

	rospy.spin()
if __name__ == "__main__":
	arrayClassificationServer()