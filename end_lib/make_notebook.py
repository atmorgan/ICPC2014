#!/usr/bin/python

from __future__ import with_statement

"""This is inspired by Tom Do's format_notebook.pl."""

import sys
import os
import cStringIO
import re
import subprocess

_sections = [
    ('Graph',
        ('Maximum matching', 'cpp', 'MaximumMatching.cpp'),
        ('Maximum flow', 'cpp', 'MaximumFlowDinic.cpp'),
        ('Strongly connected components', 'cpp', 'SCC.cpp'),
        ('Minimum spanning tree', 'cpp', 'Kruskal.cpp'),
        ('Suurballe', 'cpp', 'Suurballe.cpp'),
        ('Dijkstra', 'cpp', 'Dijkstra.cpp'),
        ('Bellman ford', 'cpp', 'BellmanFord.cpp'),
        ('Articulation point', 'cpp', 'ArticulationPoint.cpp'),
        ('Floyd', 'cpp', 'Floyd.cpp'),
    ),
    ('Geometry and Num',
        ('Euclid', 'cpp', 'Euclid.cpp'),
        ('Geometry', 'cpp', 'Geometry.cpp'),
        ('Convex hull', 'cpp', 'ConvexHull.cpp'),
        ('Combination', 'cpp', 'Combination.cpp'),
        ('Gauss jordan', 'cpp', 'GaussJordan.cpp'),
    ),
    ('String manipulation',
        ('Knut morris pratt', 'cpp', 'KMP.cpp'),
        ('LIS', 'cpp', 'LongestIncreasingSubsequence.cpp'),
        ('Suffix array', 'cpp', 'SuffixArray.cpp'),
    ),
]

_fmtnames = {'cpp' : 'C++', 'java' : 'Java'}

_re_START = re.compile('\s*//\s*BEGIN\s*$')
_re_END = re.compile('\s*//\s*END\s*$')

def preprocess(fn):
    out = cStringIO.StringIO()
    state = 1
    with open(fn, 'r') as f:
        for line in f:
            if state != 2 and _re_START.match(line):
                state = 2
            elif state == 2 and _re_END.match(line):
                state = 3
            elif state == 2:
                out.write(line)
    if state != 3:
        raise ValueError, 'File %s lack delimiters' % fn
    return out.getvalue()

def make_html(fn, fmt):
    code = preprocess(fn)
    html_page = subprocess.Popen(
        ('enscript',
         '-E%s' % fmt, '--language=html', '-o-', '--color'),
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE).communicate(code)[0]
    return re.search('<PRE>.*</PRE>', html_page, re.DOTALL).group(0)

def make_notebook(sections):
    print """<!DOCTYPE html PUBLIC "-//IETF//DTD HTML 2.0//EN">
<HTML>
<HEAD>
<STYLE>
h2 { font-size: 12pt }
ol { font-size: 10pt }
pre { font-size: 11pt }
</STYLE>
<TITLE>UW-Madison ACM Reference 2014</TITLE>
</HEAD>
<BODY BGCOLOR="#FFFFFF" TEXT="#000000" LINK="#1F00FF" ALINK="#FF0000" VLINK="#9900DD">
<H1>UW-Madison ACM Reference 2014</H1>"""

    os.chdir('.')

    fileno = 1
    for s in sections:
        print '<H2>%s</H2>' % s[0]
        print '<OL>'
        for f in s[1:]:
            print '<LI><A HREF="#file%d">%s (%s)</A>' \
                % (fileno, f[0], _fmtnames[f[1]])
            fileno += 1
        print '</OL>'

    fileno = 1
    for s in sections:
        for f in s[1:]:
            print '<HR>'
            print '<H2><A NAME="file%d">%s (%s)</A></H2>' \
                % (fileno, f[0], _fmtnames[f[1]])
            fileno += 1
            print make_html(f[2], f[1])

    print '</BODY></HTML>'


if __name__ == "__main__":            
    make_notebook(_sections)
