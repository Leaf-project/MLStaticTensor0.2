def writecsv(file, content):
	with open(file, 'w') as co:
		co.write(content.strip('\n'))

def loadcsv(file):
	with open(file, 'r') as co:
		autorized = '0123456789,;\n'
		text = co.read()
		if any(not (unauto:=x) in autorized  for x in text):
			print(f"Unautorized character {unauto} in {file}")
			exit()
		return [list(map(
			lambda sline: list(map(float, sline.split(','))), 
			line.split(';'))) for line in text.strip('\n').split('\n')]

def plumcsv(table):
	for line in table:
		print(line[0], '||', line[1])