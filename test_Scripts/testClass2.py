import json
from pybrain.tools.customxml.networkreader import NetworkReader
import joblib
import argparse
import itertools
import numpy as np
parser = argparse.ArgumentParser()
parser.add_argument('filename')
par = parser.parse_args()
skynet = NetworkReader.readFrom('annModel.xml') 
data = []
target = []
output = []
clf = joblib.load('svmModel.pkl') 
clfData = []
dfile = str(par.filename)
with open(dfile,'r') as f:
	for line in f:
		mylist = json.loads(line)
		target.append(mylist[-1])
		if len(mylist) > 5:
			mean = []
			del mylist[-1]
			quadrants = zip(*[iter(mylist)]*76800) #split it into 4
  			for i in quadrants:
				x = np.mean(i)
				mean.append(int(x))
			mylist = mean

		else:
			del mylist[-2:]
		#print mylist
		data.append(mylist)
		clfData.append(list(clf.predict([mylist])))
print "Actual: "
print target
print "Predicted"
for i in data:
	#print i
	output.append(round(skynet.activate(tuple(i))))
print output

chain = itertools.chain(*clfData)
clfData = list(chain)
print "SVM Ouput"
print clfData
#file6L.txt
#file8BL.txt
