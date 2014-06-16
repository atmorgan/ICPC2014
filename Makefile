tests: test_algebra test_articulation_point test_bellmanford test_vector test_plane_geometry test_polygon

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

notebook.html: make_notebook.py Algebra.cc ArticulationPoint.cc BellmanFord.cc ConvexHull.cc FloydWarshall.cc Geometry.cc KMP.cc LinearAlgebra.cc Makefile make_notebook.py MaximumFlowDinic.cc SCC.cc Simplex.cc SuffixArray.cc
	python make_notebook.py > notebook.html
