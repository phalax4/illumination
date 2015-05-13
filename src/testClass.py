import json
from pybrain.tools.customxml.networkreader import NetworkReader
import joblib
skynet = NetworkReader.readFrom('annModel.xml') 

data = []
target = []
output = []
clf = joblib.load('svmModel.pkl') 
clfData = []
with open('file8BL.txt','r') as f:
	for line in f:
		mylist = json.loads(line)
		target.append(mylist[-1])
		del mylist[-1]
		data.append(mylist)
		clfData.append(clf.predict([mylist]))
print "Actual: "
print target
print "Predicted"
for i in data:
	output.append(round(skynet.activate(tuple(i))))
print output
print clfData
#file6L.txt
#file8BL.txt