#!/usr/bin/python

import sys
import re

_re_START  = re.compile('\s*//\s*BEGIN\s*$')
_re_END    = re.compile('\s*//\s*END\s*$')

def preprocess(fn):
	out = ""
	state = 1
	with open(fn, 'r') as f:
		for line in f:
			if state in [1,3] and _re_START.match(line):
				state = 2
			elif state == 2 and _re_END.match(line):
				state = 3
			elif state == 2:
				out += line
	if state != 3:
		raise ValueError('File {0} lack delimiters'.format(fn))
	return out

with open(sys.argv[2],'w') as outf:
    outf.write( preprocess(sys.argv[1]) )

