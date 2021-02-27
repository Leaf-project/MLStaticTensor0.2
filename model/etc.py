import struct
import random

class SetList:
	def __init__(self, iterable):
		self.iterable = iterable

	def add(self, iterable):
		for i in iterable:
			if not i in self.iterable:
				self.iterable += [i]

	def del_(self, obj):
		for i in range(len(self.iterable)):
			if obj == self.iterable[i]:
				del self.iterable[i]
				return

	def __getitem__(self, index):
		return self.iterable[index]

	def index(self, obj):
		return self.iterable.index(obj)

	def __str__(self):
		return str(self.iterable)

	def __contains__(self, elm):
		return elm in self.iterable

	def __len__(self):
		return len(self.iterable)

'''
Exemple:
mdl = [
	{'shape':(2,2,1),'arr':(1,2,1,2),'ptrs':0,'importance':1,'lable':"Ok",'type':0},
	{'shape':(1,3),'arr':(2,1,2),'ptrs':0,'importance':1,'lable':"lbl1",'type':0},
	{'shape':(1),'arr':(0,),'ptrs':0,'importance':1,'lable':"a",'type':0}
]
insts = {
	'insts' : [
		[0, 1,0, 0,0, 2,0],
		...
	],
	'inputs' : [0],
	'outputs' : [2]
}
'''

funcs = {
	'identity' : 1,		#f(x) = x
	'bin_step' : 1,		#f(x) = 1 if x >=0 else x
	'logistic' : 1,		#f(x) = 1 / (1 + e**(-x))
	'tanh' : 1,			#f(x) = (1 - e**(-2x)) / (1 + e**(-2x))
	'relu' : 1,			#f(x) = max(x,0) = Seuil(x,0)
	'gelu' : 1, 			#f(x) = 0.5*x*(1 + tanh(sqrt(2/pi) * (x + 0.044715*x**3)))
	'softplus' : 1,		#f(x) = ln(1 + e**x)
	'elu' : 1,				#
	'selu' : 1,			#
	'leaky_relu' : 1,		#
	'softsign' : 1,		#
	'sqnl' : 1,			#
	'bent_Ind' : 1,		#
	'gauss' : 1,			#f(x) = e**(-x**2)
	'sq_rbf' : 1,			#
	'softmax' : 1,			#
	'maxout' : 1,			#
	'seuil' : 2,			#f(x,a) = max(x,a)
	'condition_like' : 3	#f(x,a,k) = a + |a|**(-(x - a)**(2*k))
}

getfunc = lambda str: list(funcs.keys()).index(str)

size_t = lambda *l: struct.pack('N'*len(l), l)

cmds = {	#Grammar: x's <- array of x, n <- lenght of next array
	'+' :  {
		'text' : 'Sum',
		'lambda' : 'y = a0+a1+...',
		'binform' : "id,y,yt,n,(a,at)'s",
		'bin' : lambda y,*args: size_t(0, *y, len(args), *args)	#y:(y,yt), arg's: (a,at)'s
	},
	'-' :  {
		'text' : 'Sub',
		'lambda' : 'y = a0-a1-...',
		'binform' : "id,y,yt,n,(a,at)'s",
		'bin' : lambda y,*args: size_t(1, *y, len(args), *args)	#y:(y,yt), arg's: (a,at)'s
	},
	'*' :  {
		'text' : 'Mul',
		'lambda' : 'y = a0*a1*...',
		'binform' : "id,y,yt,n,(a,at)'s",
		'bin' : lambda y,*args: size_t(2, *y, len(args), *args)	#y:(y,yt), arg's: (a,at)'s
	},
	'/' :  {
		'text' : 'Div',
		'lambda' : 'y = a0/a1/...',
		'binform' : "id,y,yt,n,(a,at)'s",
		'bin' : lambda y,*args: size_t(3, *y, len(args), *args)	#y:(y,yt), arg's: (a,at)'s
	},
	'^' :  {
		'text' : 'Pow',
		'lambda' : 'y = a0^a1^...',
		'binform' : "id,y,yt,n,(a,at)'s",
		'bin' : lambda y,*args: size_t(4, *y, len(args), *args)	#y:(y,yt), arg's: (a,at)'s
	},
	'@' :  {
		'text' : 'Dot product',
		'lambda' : 'y = a @ b',
		'binform' : "id,y,yt,a,at,b,bt",
		'bin' : lambda y,yt,a,at,b,bt: size_t(5, y,yt,a,at,b,bt)	#y:(y,yt), arg's: (a,at)'s
	},
	'i+' :  {
		'text' : 'Iter sum',
		'lambda' : 'y = i+ a',
		'binform' : 'id,y,yt,a,at'
	},
	'i-' :  {
		'text' : 'Iter sub',
		'lambda' : 'y = i- a',
		'binform' : 'id,y,yt,a,at'
	},
	'i*' :  {
		'text' : 'Iter mul',
		'lambda' : 'y = i* a',
		'binform' : 'id,y,yt,a,at'
	},
	'i/' :  {
		'text' : 'Iter div',
		'lambda' : 'y = i/ a',
		'binform' : 'id,y,yt,a,at'
	},
	'i^' :  {
		'text' : 'Iter pow',
		'lambda' : 'y = i^ a',
		'binform' : 'id,y,yt,a,at'
	},
	')' :  {
		'text' : 'Function',
		'lambda' : 'y = f(a,...)',
		'binform' : "id,y,yt,f,n,(a,at)'s"
	},
	'T' :  {
		'text' : 'Transpose',
		'lambda' : 'y = a.T',
		'binform' : 'id,y,yt,a,at'
	},
	'|' :  {
		'text' : 'Kernel convultion',
		'lambda' : 'y = a | K mod',
		'binform' : 'id,y,yt,a,at,k,kt,modid  (mod-ids'
	},
	'#x' :  {
		'text' : 'Convolution (x-size)',
		'lambda' : 'y = a #x b',
		'bin' : "id,y,yt,a,at,b,bt,x's (x - block sizes)"
	},
	'p)' :  {
		'text' : 'Pooling',
		'lambda' : 'y = pool(a,x)',
		'bin' : 'id,y,yt,a,at,f,x (x- block size)'
	},
	'%' :  {
		'text' : 'Random',
		'lambda' : 'y = %s',
		'bin' : 'id,y,yt,s	(srand will be'
	},
	's' :  {
		'text' : 'Split',
		'lambda' : 'y0,y1... s a',
		'binform' : "id,n,(y,yt)'s,a,at"
	},
	'P' :  {
		'text' : 'Pyramidals',
		'lambda' : 'y=P(a,w)mod|p',
		'binform' : "id,y,yt,a,at,w,wt,modid,param's"
	}
}

getcmd = lambda str: list(cmds.keys()).index(str)

def mul(lst):
	a = 1
	for i in lst: a *= i
	return a

def mkts(shape, arr, type):
	return struct.pack(
		'N'*(1 + len(shape))+'d'*len(arr)+'B', 
		len(shape), *shape, *arr, type)

def readvar(bins):
	var = {}
	#Deep
	deep = struct.unpack('N', bytes(bins[:8]))
	bins = bins[8:]
	var['shape'] = struct.unpack('N'*deep, bytes(bins[:8*deep]))
	bins = bins[8*len_:]
	#Arr
	len_ = mul(var['shape'])
	var['arr'] = struct.unpack('d'*len_, bytes(bins[:8*len_]))
	bins = bins[8*len_:]
	#Pointers
	var['ptrs'] = struct.unpack('N'*len_, bytes(bins[:8*len_]))
	bins = bins[8*len_:]
	#Importance
	var['importance'] = struct.unpack('f', bytes(bins[:4]))
	bins = bins[4:]
	#Str Label
	len_ = struct.unpack('N', bytes(bins[:8]))	#sizeof str
	bins = bins[8:]
	var['label'] = ''.join(list(map(chr, struct.unpack('c'*len_, bytes(bins[:len_])))))
	bins = bins[len_:]
	#Type
	var['type'] = struct.unpack('c', bytes(bins[:1]))
	bins = bins[1:]
	#
	return var, bins

def writevar(var):
	#Deep
	out = struct.pack(f"{len(var['shape'])}N", len(var['shape']))
	#Shape
	out += struct.pack(f"{len(var['shape'])}N", *var['shape'])
	#Arr
	out += struct.pack(f"{len(var['arr'])}d", *var['arr'])
	#Pointers
	out += struct.pack(f"{len(var['ptrs'])}N", *var['ptrs'])
	#Importance
	out += struct.pack('f', var['importance'])
	#Str
	out += struct.pack('N', len(var['label']))
	out += struct.pack(f"{len(var['label'])}b", *list(map(ord, var['label'])))	#'c' = char = 1
	#Type
	out += struct.pack('c', bytes([var['type']]))
	#
	return out

def readmdl(filename):
	with open(filename, 'rb') as co: bins = co.read()
	#
	model = [0 for _ in struct.unpack('N', bytes(bins[:8]))]
	bins = bins[:8]
	#
	for i in range(len(model)):
		model[i], bins = readvar(bins)
	#
	return model

def readinsts(filename):
	insts = {'insts':[], 'inputs':[], 'outputs':[]}
	#
	with open(filename, 'rb') as co: bins = co.read()
	#
	insts['insts'] = [0 for i in struct.unpack('N', bytes(bins[:8]))]
	bins = bins[8:]
	insts['inputs'] = [0 for i in struct.unpack('N', bytes(bins[:8]))]
	bins = bins[8:]
	insts['outputs'] = [0 for i in struct.unpack('N', bytes(bins[:8]))]
	bins = bins[8:]
	#
	for name in ('inputs', 'outputs'):
		inputs[ARR] = struct.unpack('N'*len(insts[ARR]), bytes(bins[:8*len(insts[ARR])]))
		bins = bins[8*len(insts[ARR]):]
	#
	for i in range(len(insts['insts'])):
		len_ = struct.unpack('N', bytes(bins[:8]))
		bins = bins[8:]
		insts['insts'][i] = struct.unpack('N'*len_, bytes(bins[:8*len_]))
		bins = bins[8*len_:]
	#
	return insts

def writemdl(model, buff):
	#print(model)
	buff.write(struct.pack('N', len(model)))
	#=========Vars=========
	for tensor in model:
		buff.write(writevar(tensor))

def writeinsts(insts, buff):
	buff.write(struct.pack('NNN'+'N'*(len(insts['inputs'])+len(insts['outputs'])), len(insts['insts']), len(insts['inputs']), len(insts['outputs']), *insts['inputs'], *insts['outputs']))
	#
	for inst in insts['insts']:
		buff.write(a:=struct.pack('N'+'N'*len(inst), len(inst), *inst))
		#print(a)
