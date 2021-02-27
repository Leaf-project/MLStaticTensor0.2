from model.neurones import Neu	#Neu is 1st order neurone : f(a*x**1 + b)
from model.network import Network, Link
from model.data import writecsv, loadcsv, plumcsv
#from model.simple import SimpleTrain, SimpleShow

inputs = [Neu(label='i0'), Neu(label='i1')]	#without funcs => default:identity and no ax+b
layer = [Neu('tanh', label='lay0'), Neu('logistic', label='lay1'), Neu('logistic', label='lay2')]
outputs = [Neu('logistic', label='out')]

xor_net = Network(
	inputs,
	outputs,
	links=[
		Link(inputs[0],0, layer[0],0),	#defaul link is : out = 1*inp + 0
		Link(inputs[0],0, layer[1],0),
		Link(inputs[0],0, layer[2],0),
		#
		Link(inputs[1],0, layer[0],0),
		Link(inputs[1],0, layer[1],0),
		Link(inputs[1],0, layer[2],0),
		#
		Link(layer[0],0, outputs[0],0),
		Link(layer[1],0, outputs[0],0),
		Link(layer[2],0, outputs[0],0),
	]
)	#Network convert itself to simple Model class

xor_net.rndw()
xor_net.write('Test/xor.bin', 'Test/xor.bin')	#is converted to neurone then written to file
writecsv('Test/xordata.csv', """
0,0;0
0,1;1
1,0;1
1,1;0
""")
plumcsv(xor_data := loadcsv('Test/xordata.csv'))

#SimpleTrain(xor_net, xor_data, time='20s', target_error=1e4)
#SimpleShow(xor_net, xor_data)

############### Train Btc ###############
'''
from model.neurones import InpNeu, OutNeu, Neu
from model.network import Network, Patern, Link
from model.server import Server
from model.data import loaddata

inputs = [InpNeu(name='BrutBtc'), InpNeu(name='Rsi'), InpNeu(name='VolumBtc'), InpNeu(name='VolumUsdt')]
outputs = [OutNeu('bin', name='Buy or cell')]

btc_net = Network(
	inputs=inputs,
	outputs=outputs,
	links=[
		Link(Patern(
			inputs[0],
			,
			'pyramid', (2,5,2,10)
		), ),
	]
)'''