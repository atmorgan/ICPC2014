#!/usr/bin/python

"""This is inspired by Tom Do's format_notebook.pl."""

import sys
import os
import re
import subprocess


_sections = {
	"Graphs": [
		("Articulation Points", 'cpp', "ArticulationPoint.cc"),
		("Bellman-Ford", 'cpp', "BellmanFord.cc"),
		("Floyd-Warshall", 'cpp', "FloydWarshall.cc"),
		("Maximum Flow (Dinic)", 'cpp', "MaximumFlowDinic.cc"),
		("Maximum Flow (Push-relabel)", 'cpp', "MaximumFlow_PushRelabel.cc"),
		("Minimum-cost Maximum Flow", 'cpp', "MinCostMaxFlow.cc"),
		("Strongly-connected Components", 'cpp', "SCC.cc"),
	],
	"Maths": [
		("Integer Algebra", 'cpp', "Algebra.cc"),
		("Linear Algebra", 'cpp', "LinearAlgebra.cc"),
		("Simplex", 'cpp', "Simplex.cc"),
	],
	"Geometry": [
		("Basic floating point comparison", 'cpp', "FloatCompare.cc"),
		("Basic Vector/Point", 'cpp', "Vector.cc"),
		("Plane Geometry", 'cpp', "PlaneGeometry.cc"),
		("Polygon", 'cpp', "Polygon.cc"),
	],
	"Strings": [
		("Knuth-Morris-Pratt Table/Algorithm", 'cpp', "KMP.cc"),
		("Suffix Array/LCP", 'cpp', "SuffixArray.cc"),
	]
}

_fmtnames = {'cpp' : 'C++', 'java' : 'Java'}

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

def make_html(fn, fmt):
	code = preprocess(fn)
	html_page = subprocess.Popen(
		('enscript',
			'-E{fmt}'.format(fmt=fmt),
			'-2', # number of columns
			'--tabsize=2',
			'--language=html',
			'-o-',
			'--color'),
		stdin=subprocess.PIPE,
		stdout=subprocess.PIPE,
		universal_newlines=True).communicate(code)[0]
	return re.search('<PRE>.*</PRE>', html_page, re.DOTALL).group(0)

def make_notebook(sections):
	print( """\
<!DOCTYPE html>
<HTML>
<HEAD>
<STYLE>
h2 { font-size: 12pt }
ol { font-size: 10pt }
pre {
    font-size: 11pt;
    -moz-tab-size:    2;
    -o-tab-size:      2;
    -webkit-tab-size: 2;
    -ms-tab-size:     2;
    tab-size:         2;
}
</STYLE>
<TITLE>UW-Madison ACM-ICPC Reference 2014</TITLE>
</HEAD>
<BODY>
<H1>UW-Madison ACM-ICPC Reference 2014</H1>\
""")

	os.chdir('.')

	fileno = 1
	for s in sorted(sections.keys()):
		print('<H2>{sec}</H2>'.format(sec=s))
		print('<OL>')
		for f in sections[s]:
			print('<LI><A HREF="#file{n}">{label} ({lang})</A>'
				.format( n=fileno, label=f[0], lang=_fmtnames[f[1]] )
				)
			fileno += 1
		print('</OL>')

	fileno = 1
	for s in sorted(sections.keys()):
		for f in sections[s]:
			print('<HR>')
			print('<H2><A NAME="file{n}">{label} ({lang})</A></H2>'
				.format( n=fileno, label=f[0], lang=_fmtnames[f[1]] )
				)
			fileno += 1
			print( make_html(f[2], f[1]) )

	print( '</BODY></HTML>' )


if __name__ == "__main__":
	make_notebook(_sections)

