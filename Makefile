tests: test_algebra

test_algebra: Algebra.cc
	g++ -o test_algebra Algebra.cc -pedantic -Wall
