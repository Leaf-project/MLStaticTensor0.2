from ctypes import py_object, cast
import model.etc as etc
from random import random

TYPES = {
	'var' : 0,
	'Var' : 0,
	'v' : 0,
	'V' : 0,
	###########
	'mem' : 1,
	'memory' : 1,
	#############
	'Const' : 2,
	'const' : 2,
	#############
	'w' : 3,
	'W' : 3,
	'Weight' : 3,
	'weight' : 3,
	#############
	'useless' : 4,
	#############
	'mut' : 5,
	'mutable' : 5,
	'Mut' : 5
}

class Tensor:
	def __init__(self, shape, arr=None, ptrs=None, importance=None, type_=None, label=None):
		self.shape = shape
		self.arr = ([0 for _ in range(etc.mul(shape))] if arr == None else arr)
		self.ptrs = (list(range(len(self.arr))) if ptrs == None else ptrs)
		self.importance = (1.0 if importance == None else importance)
		self.type_ = (0 if type_ == None else type_)
		self.label = label

	def rnd(self):
		self.arr = [random() for _ in range(len(self.arr))]

	def compile(self):
		return {'shape':self.shape,'arr':self.arr,'ptrs':self.ptrs,'importance':self.importance,'label':self.label,'type':self.type_}
		
	def __str__(self):
		return f'{self.label} {self.shape} {self.type_}'

	def copy(self):
		ts = Tensor()
		ts.shape = self.shape
		ts.arr = self.arr
		ts.ptrs = self.ptrs
		ts.importance = self.importance
		ts.label = self.label
		ts.type_ = self.type_
		return ts

class Model:
	commands = []
	tensors = [] #etc.SetList([])

	inputs = []#etc.SetList([])		#to send to : input port
	outputs = []#etc.SetList([])	#to get from: output port

	def __init__(self, tensors=[], inputs=[], output=[], commands=[]):
		self.tensors = tensors
		self.inputs = inputs
		self.output = output
		self.commands = commands
		
	def rndw(self):
		for i in range(len(self.tensors)):
			if self.tensors[i].type_ == TYPES['weight']:
				self.tensors[i].rnd()

	def _convert_cmd(self, cmd):
		ret = []
		for elm in cmd:
			if type(elm) is Tensor:
				ret += [self.tensors.index(elm)]
			elif type(elm) is int:
				ret += [elm]
		return ret

	def compile(self):
		return [
			ts.compile() for ts in self.tensors
		], {
			'insts' : [self._convert_cmd(cmd) for cmd in self.commands],
			'inputs' : [list(self.tensors).index(ts) for ts in self.inputs],
			'outputs' : [list(self.tensors).index(ts) for ts in self.outputs]
		}

	def write(self, mdlfile, instsfile):
		mdl, insts = self.compile()
		#print(insts)
		if mdlfile == instsfile:
			with open(mdlfile, 'wb') as buff:
				etc.writemdl(mdl, buff)
				etc.writeinsts(insts, buff)
		else:
			with open(mdlfile, 'wb') as buff:
				etc.writemdl(mdl, buff)
			with open(instsfile, 'wb') as buff:
				etc.writeinsts(insts, buff)

	def copy(self):
		mdl = Model()

		inputs = {self.tensors.index(ts) for ts in self.inputs}
		outputs = {self.tensors.index(ts) for ts in self.outputs}

		for cmd in self.commands:
			#mdl.commands += [{int:(lambda:elm),Tensor:(lambda:(mdl.tensors:=(mdl.tensors+[elm.copy()]))[-1])}[type(elm)]() for elm in cmd]
			mdl.commands += []
			for elm in cmd:
				if type(elm) is Tensor:
					mdl.tensors += elm.copy()
					mdl.commands[-1] += [mdl.tensors[-1]]
				elif type(elm) is int:
					mdl.commands[-1] += [elm]

		mdl.inputs = [mdl.tensors[i] for i in inputs]
		mdl.outputs = [mdl.tensors[i] for i in outputs]
		
		return mdl

'''
Philosophy of writting Astract Neurones Models:
Pseudo-Memory
self.tensor:
	top : variables
	bottom : weights and consts
'''

class Neu(Model):	#Classic Neurone with f(ax+b), but f is composed of f0 and f1 : f = (f1 . f0)
	def __init__(self, function0=None, function1='identity', label=None):
		if function0 == None:
			self._functions = None
			#
			self.tensors = [Tensor([1],[0],[0],1,0, label=str(label)+'0')]	#input & output		#(Link will make sum with (i+))
			self.inputs = [self.tensors[0]]
			self.outputs = [self.tensors[-1]]	#same as self.tensors[0]
			#
		else:
			self._functions = function0, function1
			#
			self.tensors = tensors = [
				Tensor((1,),*[None]*4, label=str(label)+'.inp'),	#input 				#(Link will make sum with (i+))
				Tensor((1,),*[None]*4, label=str(label)+'.w'),		#y0=input*w0
				Tensor((1,),*[None]*4, label=str(label)+'.b'),		#y1=y0+w1
				Tensor((1,),*[None]*4, label=str(label)+'.f0'),		#y2=f0(y1)
				Tensor((1,),*[None]*4, label=str(label)+'.f1'),		#y3=f1(y2)	#output
				#
				Tensor((1,),[0],None,1,TYPES['weight'], label='W:'+str(label)),	#w0
				Tensor((1,),[0],None,1,TYPES['weight'], label='B:'+str(label)),	#w1
			]
			self.inputs = [tensors[0]]
			self.outputs = [tensors[-3]]
			#
			self.commands = [
				[etc.getcmd('*'),	tensors[1],0,  2, tensors[0],0,  tensors[-2],0],			#weight
				[etc.getcmd('+'),	tensors[2],0,  2, tensors[1],0,  tensors[-1],0],			#bias
				[etc.getcmd(')'),	tensors[3],0,  etc.getfunc(function0), 1, tensors[2],0],	#activate
				[etc.getcmd(')'),	tensors[4],0,  etc.getfunc(function1), 1, tensors[3],0],	#activate
			]