tests: test_algebra test_articulation_point test_bellmanford test_vector test_plane_geometry test_polygon test_floydwarshall test_KMP test_SCC test_suffix_array test_float_compare test_mincostmaxflow test_pushrelabel test_segment_tree test_maxcard_bm test_mincost_bm test_kdtree

test_algebra: Algebra.cc
	g++ -o test_algebra Algebra.cc -pedantic -Wall -O2

test_articulation_point: ArticulationPoint.cc
	g++ -o test_articulation_point ArticulationPoint.cc -pedantic -Wall -O2

test_bellmanford: BellmanFord.cc
	g++ -o test_bellmanford BellmanFord.cc -pedantic -Wall -O2

test_vector: Vector.cc
	g++ -o test_vector -DBUILD_TEST_VECTOR Vector.cc -pedantic -Wall -O2

test_plane_geometry: PlaneGeometry.cc
	g++ -o test_plane_geometry -DBUILD_TEST_PLANE_GEOMETRY PlaneGeometry.cc -pedantic -Wall -O2

test_polygon: Polygon.cc
	g++ -o test_polygon -DBUILD_TEST_POLYGON Polygon.cc -pedantic -Wall -O2

test_floydwarshall: FloydWarshall.cc
	g++ -o test_floydwarshall FloydWarshall.cc -pedantic -Wall -O2

test_KMP: KMP.cc
	g++ -o test_KMP KMP.cc -pedantic -Wall -O2

test_SCC: SCC.cc
	g++ -o test_SCC SCC.cc -pedantic -Wall -O2

test_suffix_array: SuffixArray.cc
	g++ -o test_suffix_array SuffixArray.cc -pedantic -Wall -O2

test_float_compare: FloatCompare.cc
	g++ -o test_float_compare -DTEST_FLOAT_COMPARE FloatCompare.cc -O2 -pedantic -Wall

test_mincostmaxflow: MinCostMaxFlow.cc
	g++ -o test_mincostmaxflow MinCostMaxFlow.cc -O2 -pedantic -Wall

test_pushrelabel: MaximumFlow-PushRelabel.cc
	g++ -o test_pushrelabel MaximumFlow-PushRelabel.cc -O2 -pedantic -Wall

test_segment_tree: SegmentTree.cc
	g++ -o test_segment_tree SegmentTree.cc -O2 -pedantic -Wall

test_maxcard_bm: MaxCardBipartiteMatching.cc
	g++ -o test_maxcard_bm MaxCardBipartiteMatching.cc -O2 -pedantic -Wall

test_mincost_bm: MinCostBipartiteMatching.cc
	g++ -o test_mincost_bm MinCostBipartiteMatching.cc -O2 -pedantic -Wall

test_kdtree: KDtree.cc
	g++ -o test_kdtree KDtree.cc -O2 -pedantic -Wall

SOURCES = \
	ArticulationPoint.cc BellmanFord.cc FloydWarshall.cc MaximumFlowDinic.cc MaximumFlow-PushRelabel.cc MinCostMaxFlow.cc SCC.cc \
	Algebra.cc LinearAlgebra.cc Simplex.cc FFT.cc \
	FloatCompare.cc Vector.cc PlaneGeometry.cc Polygon.cc \
	KMP.cc SuffixArray.cc SegmentTree.cc MaxCardBipartiteMatching.cc MinCostBipartiteMatching.cc KDtree.cc

COMPILED = $(SOURCES:%.cc=%.cc.compiled)

LIBRARY = vimrc $(COMPILED)

%.cc.compiled: %.cc
	python process_file.py $< $@

notebook.ps: notebook.dvi
	dvips notebook

notebook.dvi: notebook.tex $(LIBRARY)
	latex -shell-escape notebook
	latex -shell-escape notebook

notebook.pdf: notebook.tex $(LIBRARY)
	pdflatex --shell-escape notebook
	pdflatex --shell-escape notebook

