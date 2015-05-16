from datasets import XORDataSet 
from pybrain.tools.shortcuts import buildNetwork
from pybrain.supervised import BackpropTrainer
from pybrain.structure import TanhLayer 
from pybrain.datasets import SupervisedDataSet
from pybrain.tools.customxml.networkwriter import NetworkWriter
from pybrain.tools.customxml.networkreader import NetworkReader
import numpy
d = XORDataSet()
n = buildNetwork(d.indim, 4, d.outdim, bias=True, hiddenclass=TanhLayer)
t = BackpropTrainer(n, learningrate=0.01, momentum=0.99, verbose=False)
t.trainOnDataset(d, 1000)
t.testOnData(verbose=True)
print "-------------------------"
for i in xrange(1000):
    t.train()
for i in d:
	print i
print numpy.array([int(n.activate(x)) for x, _ in d])
print "-------------------------"
print long(n.activate([1,1]))
print long(n.activate([0,1]))
print long(n.activate([0,0]))
print long(n.activate([1,0]))
