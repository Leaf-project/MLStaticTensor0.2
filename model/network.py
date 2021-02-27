from .neurones import Model, Tensor, TYPES
import model.etc as etc
from json import dumps

flatten = lambda lst: [x for it in lst for x in it]

scalar = lambda type,lbl: Tensor(1, *[None]*3, type, label=lbl)
ts = lambda shape, type, lbl: Tensor(len(shape),shape, *[None]*2, type, label=lbl)

class Link:
	#	FromMdl -----> ToMdl
	def __init__(self, from_obj, from_port, to_obj, to_port, label='link'):
		self.from_obj = from_obj		#
		self.from_port = from_port		#	port is just id in mdl.inputs[id] of the tensor
		self.to_obj = to_obj			#
		self.to_port = to_port			#

class WLink(Link):
	def __init__(self, frmo, frmp, too, top, label='wlink', scalar=True):
		inp = ts(frmo.outputs[frmp].shape, TYPE['var'], label+'.inp')
		if scalar:
			mid = ts(inp.shape, TYPE['var'], label+'.mid')
			out = ts([1], TYPE['var'], label+'.out')
		else:
			out = ts(inp.shape, TYPE['var'], label+'.out')
		w = ts(inp.shape, TYPE['w'], 'W':label)
		#
		self.weight = Model(
			tensors=[inp]+([mid] if scalar else [])+[out,w],
			inputs=[inp],
			outputs=[out],
			commands=([getcmd('i+', )] if scalar else [])+[[getcmd('*'), ]]
		)
		self.links = [
			Link(),
			Link()
		]

def network(inputs, outputs, links:list=None, mulinks={}):
	def add_obj(self, obj):
		self.tensors += [ts for ts in obj.tensors if not ts in self.tensors]
		self.inputs += [ts for ts in obj.inputs if not ts in self.inputs]
		self.outputs += [ts for ts in obj.outputs if not ts in self.outputs]
		self.commands += obj.commands

	self = Model()
	'''mulinks = {
		#target (to_obj)
		mdl2 : {	#can be a list, but easyer to understand dict, 0 is ToPort
			#port target (to_port)
			0 : [(mdl0,3), (mdl1,2)], #MdlOutPortID<int> : (MdlInpObj<Mdl>,MdlInpPortID<int>)
			1 : [(mdl0,1)]
		},
		mdl3 : {
			0 : [(mdl0,0)],
		}
	}'''

	if links:
		for link in links:
			if type(link) is Link:
				if not link.to_obj in mulinks:
					mulinks[link.to_obj] = {i:[] for i in range(len(link.to_obj.inputs))}	#i:([(link.from_obj,form_port)] if link.to_port == i else [])
				#
				mulinks[link.to_obj][link.to_port] += [(link.from_obj,link.from_port)]
			elif type(link) is WLink:
				pass
			else:
				pass
	##########################
	added = []
	to_del = []
	
	for obj in inputs:
		self.add_obj(obj)
	added += [obj]

	for to_obj,ports in mulinks.items():
		for to_port, from_objs in ports.items():
			for obj,from_port in from_objs:
				if not obj in added:
					self.add_obj(obj)
					added += [obj]
			#
			self.commands.append([
				etc.getcmd('+'),
				to_obj.inputs[to_port],0,
				len(from_objs),
				*[elm for obj,from_port in from_objs for elm in (obj.outputs[from_port],0)]
			])
			#
		del to_obj.inputs[to_port]
		if not to_obj in added:
			self.add_obj(to_obj)
			added += [to_obj]

	for obj in outputs:
		if not obj in added:
			self.add_obj(obj)
			added += [obj]

	for obj in added:
		if not obj in inputs:
			for tensor in obj.inputs:
				del self.inputs[self.inputs.index(tensor)]
		if not obj in outputs:
			for tensor in obj.outputs:
				del self.outputs[self.outputs.index(tensor)]
	#in result only tensors from inputs and outputs will be respectivly inputs and outputs of the all network (in fact Network is Model)