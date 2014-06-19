tests: test_algebra test_articulation_point test_bellmanford test_vector test_plane_geometry test_polygon test_floydwarshall test_KMP test_SCC test_suffix_array test_float_compare

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

notebook.html: make_notebook.py Algebra.cc ArticulationPoint.cc BellmanFord.cc ConvexHull.cc FloydWarshall.cc Geometry.cc KMP.cc LinearAlgebra.cc Makefile make_notebook.py MaximumFlowDinic.cc SCC.cc Simplex.cc SuffixArray.cc
	python make_notebook.py > notebook.html
