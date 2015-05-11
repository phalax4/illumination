from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.tools.shortcuts import buildNetwork
from pybrain.structure import TanhLayer 
from pybrain.tools.customxml.networkwriter import NetworkWriter
from pybrain.tools.customxml.networkreader import NetworkReader

ds = SupervisedDataSet(2, 1)

ds.addSample((0, 0), (0,))
ds.addSample((0, 1), (1,))
ds.addSample((1, 0), (1,))
ds.addSample((1, 1), (0,))
ds.addSample((0, 0), (0,))
ds.addSample((0, 1), (1,))
ds.addSample((1, 0), (1,))
ds.addSample((1, 1), (0,))
ds.addSample((0, 0), (0,))
ds.addSample((0, 1), (1,))
ds.addSample((1, 0), (1,))
ds.addSample((1, 1), (0,))
net = buildNetwork(2, 3, 1, bias=True, hiddenclass=TanhLayer)
trainer = BackpropTrainer(net, ds,learningrate = 0.3, weightdecay = 0.01)

#trainer.trainUntilConvergence()
for i in xrange(1000):
    trainer.train()
#trainer.trainEpochs(1000)

c = net.activate([0,0])
d = net.activate([0,1])
f = net.activate([1,0])
e = net.activate([1,1])

print '0,0 : {0}'.format(round(c))
print '0,1 : {0}'.format(round(d))
print '1,0 : {0}'.format(round(f))
print '1,1 : {0}'.format(round(e))
print "....................."
NetworkWriter.writeToFile(net, 'data.xml')
net2 = NetworkReader.readFrom('data.xml') 
c2 = net2.activate([0,0])
d2 = net2.activate([0,1])
f2 = net2.activate([1,0])
e2 = net2.activate([1,1])
print '..0,0 : {0}'.format(round(c2))
print '..0,1 : {0}'.format(round(d2))
print '..1,0 : {0}'.format(round(f2))
print '..1,1 : {0}'.format(round(e2))

ds2 = SupervisedDataSet(2, 1)

ds2.addSample((0, 0), (0,))
ds2.addSample((0, 1), (1,))
ds2.addSample((1, 0), (1,))
ds2.addSample((1, 1), (0,))
ds2.addSample((0, 0), (0,))
ds2.addSample((0, 1), (1,))
ds2.addSample((1, 0), (1,))
ds2.addSample((1, 1), (0,))
ds2.addSample((0, 0), (0,))
ds2.addSample((0, 1), (1,))
ds2.addSample((1, 0), (1,))
ds2.addSample((1, 1), (0,))
net3 = buildNetwork(2, 3, 1, bias=True, hiddenclass=TanhLayer)
print "--------------------------------------------"
c3 = net3.activate([0,0])
d3 = net3.activate([0,1])
f3 = net3.activate([1,0])
e3 = net3.activate([1,1])

print '..0,0 : {0}'.format(round(c3))
print '..0,1 : {0}'.format(round(d3))
print '..1,0 : {0}'.format(round(f3))
print '..1,1 : {0}'.format(round(e3))
