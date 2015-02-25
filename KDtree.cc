#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>

#define EP 1e-10
#define INF 1000000000
#define ALPHA .5

using namespace std;

// BEGIN
// Fully dynamic n-dimensional sledgehammer kd-tree. 
// Constructs 100,000 point kd tree in about 2 seconds
// Handles 100,000 2D NN searches on 100,000 points in about 2 seconds
// Handles 1,000 2D range queries of ranges ~O(sqrt(n)) points
// on 50,000 points in about 2 seconds
// May degenerate when points are not uniformly distributed
// I hope you don't implement the whole thing.

// use to change data structure of pts in kdtree
typedef int T;
typedef vector<T> vt;
typedef vector<vt*> vvt;
typedef vector<vvt> vvvt;

struct kdnode {
    size_t d;
    kdnode* left;
    kdnode* right;

    // number of alive nodes in subtree rooted at this node, 
    // including this node if alive
    size_t nAlive;

    // number of flagged dead nodes in subtree rooted at this node, 
    // including this node if dead
    size_t nDead;

    // is flagged or not
    bool isAlive;
    vt *pt;

    // computes distance in n-dimensional space
    double dist(vt &pt1, vt &pt2) {
        double retVal = 0;

        for (size_t i = 0; i < pt1.size(); ++i) {
            retVal += (pt1[i]-pt2[i]) * (pt1[i]-pt2[i]);
        }

        return sqrt(retVal);
    }

    // returns closer point to qpt
    vt * minPt(vt &qpt, vt *pt1, vt *pt2) {
        if (pt1 == NULL) return pt2;
        if (pt2 == NULL) return pt1;
        return dist(*pt1, qpt) < dist(*pt2, qpt) ? pt1 : pt2;
    }

    // find median based on chosen algorithm
    T findMed(vvt &pts, size_t d) {
        vt arr(pts.size());

        for (size_t i = 0; i < pts.size(); ++i)
            arr[i] = (*pts[i])[d];

        sort(arr.begin(), arr.end());
        return arr[arr.size()/2];
    }

    void printPt(vt &pt) {
        for (size_t i = 0; i < pt.size(); ++i)
            cout << pt[i] << " ";
    }

    // intersects orthogonal region with left or right 
    // of orthogonal halfspace on dth dimension
    vt region_intersect(vt region, T line, size_t d, bool goLeft) {
        if (goLeft) {
            region[d*2+1] = line;
        }
        else {
            region[d*2] = line;
        }

        return region;
    }

    // returns true iff the entire region is contained in the given range
    bool region_contained(vt &region, vt &range) {
        for (size_t i = 0; i < region.size(); ++i) {
            if (i % 2 == 0) {
                if (region[i] < range[i])
                    return false;
            }
            else {
                if (region[i] > range[i])
                    return false;
            }
        }

        return true;
    }

    // returns true if point is in range
    bool pt_contained(vt* pt, vt &range) {
        for (size_t i = 0; i < pt->size(); ++i) {
            if ((*pt)[i] < range[i*2] || (*pt)[i] > range[i*2+1])
                return false;
        }

        return true;
    }

    // creates "infinite" region, unbounded on all dimensions
    vt infRegion(size_t d) {
        vt region(d*2);

        for (size_t i = 0; i < region.size(); ++i) {
            if (i % 2 == 0)
                region[i] = -INF;
            else
                region[i] = INF;
        }

        return region;
    }

    void build_tree(vvt &pts, size_t d, size_t num_d) {
        pt = NULL;
        this->d = d;
        nAlive = pts.size();
        nDead = 0;
        isAlive = true;

        vvt leftV, rightV;
        T med = findMed(pts, d);

        for (size_t i = 0; i < pts.size(); ++i) {
            if ((*pts[i])[d] == med && pt == NULL)
                pt = pts[i];
            else if ((*pts[i])[d] <= med)
                leftV.push_back(pts[i]);
            else
                rightV.push_back(pts[i]);
        }

        left = leftV.empty() ? NULL : new kdnode(leftV, (d+1)%num_d, num_d);
        right = rightV.empty() ? NULL : new kdnode(rightV, (d+1)%num_d, num_d);
    }

    // constructs kd tree
    kdnode(vvt &pts, size_t d, size_t num_d) {
        build_tree(pts, d, num_d);
    }

    // adds pt to tree
    void addPt(vt* newPt) {
        ++nAlive;

        bool goLeft = (*newPt)[d] <= (*pt)[d];
        kdnode* child = goLeft ? left : right;
        size_t childCt = (child == NULL ? 0 : child->nAlive) + 1;

        // rebuild
        if (childCt > (1+ALPHA)/2 * nAlive) {
            vvt allPts;
            addPtToResult(allPts);
            allPts.push_back(newPt);

            delete left; delete right;

            build_tree(allPts, d, pt->size());
        }
        else if (child == NULL) {
            // add node
            vvt ptV(1, newPt);

            if (goLeft)
                left = new kdnode(ptV, (d+1)%pt->size(), pt->size());
            else
                right = new kdnode(ptV, (d+1)%pt->size(), pt->size());
        }
        else {
            // recurse
            child->addPt(newPt);
        }
    }

    // deletes existing point from kd-tree, rebalancing if necessary
    // returns the number of dead nodes removed from this subtree, 
    // and bool for if pt found both are necessary in this implementation
    // to retain proper balancing invariants
    pair<size_t, bool> deletePt(vt* oldPt) {
        ++nDead;
        --nAlive;

        // need to reconstruct - last part is to avoid
        // an empty tree construction. Will get picked up by parent later
        if (nAlive < (1.0-ALPHA) * (nAlive + nDead) && nAlive > 0) {
            vvt allPts;
            addPtToResult(allPts);

            bool found = false;
            for (size_t i = 0; i < allPts.size(); ++i) {
                if (*allPts[i] == *oldPt) {
                    found = true;
                    allPts.erase(allPts.begin() + i);
                    break;
                }
            }

            delete left; delete right;

            size_t deadRemoved = nDead;

            build_tree(allPts, d, pt->size());

            return make_pair(deadRemoved, found);
        }
        else if (*pt == *oldPt) { // base case, point found
            isAlive = false;

            return make_pair(0, true);
        }
        else {
            bool goLeft = (*oldPt)[d] <= (*pt)[d];
            kdnode* child = goLeft ? left : right;
            
            size_t deadRemoved = 0;
            bool found = false;
            if (child != NULL) {
                // recurse
                pair<size_t, bool> result = child->deletePt(oldPt);
                deadRemoved = result.first;
                found = result.second;
            }

            // point may not have been found
            if (!found)
                ++nAlive;

            nDead -= deadRemoved;

            return make_pair(deadRemoved, found);
        }
    }

    // returns points in orthogonal range in O(n^((d-1)/d) + k) 
    // where k is the number of points returned
    vvt range_query(vt &range) {
        vvt result;
        int dummy = 0;
        vt region = infRegion(pt->size());
        
        range_query(range, region, result, dummy, false);

        return result;   
    }

    // counts number of queries in range, runs in O(n^((d-1)/d))
    int count_query(vt &range) {
        int numPts = 0;
        vvt dummy;
        vt region = infRegion(pt->size());
        
        range_query(range, region, dummy, numPts, true);

        return numPts;
    }

    void range_query(vt &range, vt &region, vvt &result, int &numPts, bool count) {
        if (region_contained(region, range)) {
            if (count)
                //by only adding size, we get rid of parameter
                //k in output sensitive O(n^((d-1)/d) + k) analysis
                numPts += nAlive;      
            else
                addPtToResult(result);
            return;
        }
        else if (isAlive && pt_contained(pt, range)) {
            if (count)
                ++numPts;
            else
                result.push_back(pt);
        }

        // are parts of the range to the right of splitting line?
        if ((*pt)[d] <= range[d*2+1] && right != NULL) {
            vt newRegion = region_intersect(region, (*pt)[d], d, false);
            right->range_query(range, newRegion, result, numPts, count);
        }

        // are parts of the range to the left of splitting line?
        if ((*pt)[d] >= range[d*2] && left != NULL) {
            vt newRegion = region_intersect(region, (*pt)[d], d, true);
            left->range_query(range, newRegion, result, numPts, count);
        }
    }

    // adds point to vector result and recursively calls addPt on children
    void addPtToResult(vvt &result) {
        if (isAlive)
            result.push_back(pt);

        if (left != NULL)
            left->addPtToResult(result);
        if (right != NULL)
            right->addPtToResult(result);
    }

    // overloaded for first call with no current best
    vt * NN(vt &qpt) {
        vt *result = NN(qpt, NULL);
        return result;
    }

    // performs NN query
    vt * NN(vt &qpt, vt* curBest) {
        bool goLeft = qpt[d] <= (*pt)[d];
        kdnode* child = goLeft ? left : right;

        if (isAlive)
            curBest = minPt(qpt, pt, curBest);

        if (child != NULL)
            curBest = child->NN(qpt, curBest);

        double curDist = curBest == NULL ? INF : dist(*curBest, qpt);
        
        // need to check other subtree
        if (curDist + EP > abs((*pt)[d] - qpt[d])) {
            kdnode* oppChild = goLeft ? right : left;

            if (oppChild != NULL) {
                curBest = oppChild->NN(qpt, curBest);
            }
        }

        return curBest;
    }

    // prints tree somewhat nicely
    void print_tree() {
        printf("((%c%d", isAlive ? 'A' : 'D', (*pt)[0]);

        for (size_t i = 1; i < pt->size(); ++i) {
            printf(" %d", (*pt)[i]);
        }

        cout << endl;
        
        if (left != NULL) left->print_tree();
        printf(", ");
        if (right != NULL) right->print_tree();

        printf(")");
    }

    ~kdnode() {
        delete left;
        delete right;
    }
};
// END

int main() {
    //TODO: implmement tests here.
    //NOTE: this has been tested; but requires verifier and test files
    return 0;
}
