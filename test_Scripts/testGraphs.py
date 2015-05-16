import plotly.plotly as py
from plotly.graph_objs import *
import json
import numpy as np
import itertools
data = []
with open('afile.txt', 'r') as f:
    for line in f:
        data.append(json.loads(line)[:-2])
data2 = []
with open('bfile.txt', 'r') as f:
    for line in f:
        data2.append(json.loads(line)[:-2])
data3 = []
with open('cfile.txt', 'r') as f:
    for line in f:
        data3.append(json.loads(line)[:-2])



chain1 = itertools.chain(*data)
chain2 = itertools.chain(*data2)
chain3 = itertools.chain(*data3)
x0 = list(chain1)#0 lights
x1 = list(chain2)#2 lights
x2 = list(chain3)#1 lights

trace1 = Histogram(
    x=x0,
    histnorm='count',
    name='0 Lamps',
    autobinx=False,
    xbins=XBins(
        start=0,
        end=256,
        size=0.2
    ),
    marker=Marker(
        color='fuchsia',
        line=Line(
            color='grey',
            width=0
        ),
        opacity=0.75
    )
)
trace2 = Histogram(
    x=x1,
    name='2 Lamps',
    autobinx=False,
    xbins=XBins(
        start=0,
        end=256,
        size=0.2
    ),
    marker=Marker(
        color='rgb(255, 217, 102)'
    ),
    opacity=0.75
)
trace3 = Histogram(
    x=x2,
    name='1 Lamps',
    autobinx=False,
    xbins=XBins(
        start=0,
        end=256,
        size=0.2
    ),
    marker=Marker(
        color='red'
    ),
    opacity=0.75
)
data = Data([trace1, trace2,trace3])
layout = Layout(
    title='Sampled Results',
    xaxis=XAxis(
        title='Value'
    ),
    yaxis=YAxis(
        title='Count'
    ),
    barmode='overlay',
    bargap=0.25,
    bargroupgap=0.3
)
fig = Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='style-histogram')
'''
trace1 = Histogram(
    x=x0,
    opacity=0.75
)
trace2 = Histogram(
    x=x1,
    opacity = 0.5
)
trace3 = Histogram(
    x=x2,
    opacity = 0.3
)
data = Data([trace1, trace2,trace3])
layout = Layout(
    barmode='overlay'
)
fig = Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='overlay-histogram')
'''