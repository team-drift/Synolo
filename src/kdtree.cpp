#include "../include/synolo/kdtree.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

Point::Point(float angle_deg, float dist, float str) {
    double angle_rad = angle_deg * M_PI / 180.0;
    x = dist * std::cos(angle_rad);
    y = dist * std::sin(angle_rad);
    z = 0; 
    strength = str;
}

Point::Point(double x, double y, double z, double strength)
    : x(x), y(y), z(z), strength(strength){};

Point::Point() : x(0.0), y(0.0), z(0.0), strength(0.0){};

Node::Node(const Point& pt, int depth)
    : point(pt), left(nullptr), right(nullptr), depth(depth) {}

KdTree::KdTree(int k) : root(nullptr), k(k) {}


KdTree::~KdTree() {
    freeTree(root);
}

Node* KdTree::createkdtree(std::vector<Point*>& coordinates, const int dim){
    // Initialize a vector of k=dim pointers, where each pointer points to vector of Point*
    // Each nested vector represents a different sorting of an array of pointers (xyz, yzx, zxy)
    std::vector<std::vector<Point*>> reference(dim, std::vector<Point*>(coordinates.size()));
    std::vector<Point*> temporary(coordinates.size());
    // Copying pointers from coordinates to references
    for(int i = 0; i < reference.size(); ++i) {
        for(int j = 0; j < coordinates.size(); ++j){
            reference.at(i).at(j) = coordinates.at(j);
        }
        // Sort based on superkey
        mergesort(reference.at(i), temporary, 0, reference.at(i).size()-1, i, dim);
    }

    // Remove duplicates and check merge sort
    int right;  // end index of reference.at(x)
    for(int i = 0; i < reference.size() ; i++) {
        right = 0;
        for( int j = 1; j < reference.at(i).size(); j++) {
            int compare = superKeyCompare(*reference.at(i).at(j), *reference.at(i).at(j-1), i, dim);
            if (compare < 0) {
                //illegal condition: throw error
                std::cout << "merge sort failure: superKeyCompare(ref[" << j << "], ref["
                << j-1 << "], (" << i << ") = " << compare  << std::endl;
                exit(1);
            }
            else if (compare > 0) {
                reference.at(i).at(++right) = reference.at(i).at(j);
            }
         }
    }

    root = buildkdtree(reference, temporary, 0, right, dim, 0);
 
    return root;
}



void KdTree::insert(const Point& point){
    KdTree::insertHelper(&root, 0, point);
}

bool KdTree::remove(const Point& point) {
    size_t oldCount = countNodes(root);
    root = remove(root, point, 0);
    size_t newCount = countNodes(root);
    return newCount < oldCount;
}

std::vector<Point> KdTree::search(const Point& target, double tol) {
    std::vector<Point> results;
    searchHelper(target, root, 0, tol, results);
    return results;
}

bool KdTree::FindPoint(const Point &point) {
    return findPointHelper(point, root, 0);
}

size_t KdTree::size() const {
    return countNodes(root);
}

Node* KdTree::get_root() {
    return root;
}
//___________________________________________________________________________________
//private methods

int KdTree::superKeyCompare(const Point& p1, const Point &p2, const int k, const int dim) {
    for(int i = 0; i < dim; ++i){
        // r determines which dimension to compare on
        // we can compare each dimension to compare superkeys
        int r = i + k;
        r = (r<dim) ? r: r-dim;  // faster than r % dim
        double coord1, coord2;
        switch (r) {
            case 0: coord1 = p1.x; coord2 = p2.x; break;
            case 1: coord1 = p1.y; coord2 = p2.y; break;
            default: coord1 = p1.z; coord2 = p2.z; break;
        }

        if (coord1 < coord2) return -1;
        if (coord1 > coord2) return 1;
        // If equal, continue to next dimension
    }
    return 0; // All dimensions equal}
}

void KdTree::mergesort(std::vector<Point*> &points, std::vector<Point*> &temporary, const int left, const int right,
    const int k, const int dim) {
        if (right > left) {  // if there is more than one element, continue 
            // equivalent to (left + right)/2, but avoids integer overflow
            const int mid = left + ((right - left) >> 1); 

            // Recursively subdivide the left and right halves of the array
            mergesort(points, temporary, left, mid, k, dim);
            mergesort(points, temporary, mid+1, right, k, dim);

            // Prepare temporary for merging
            int i, j, l;
            // copy the left half of points into temporary in order
            for (i = mid+1; i > left; i--) {
                temporary.at(i-1) = points.at(i-1);
            }
            //copy the right half into temporary in reverse order
            for (j = mid; j < right; j++) {
                temporary.at(mid + (right-j)) = points.at(j+1);
            }
            // temporary now contains the left half forward and the right half backward
            // merge step
            for (l = left; l <= right; l++){
                points.at(l) = superKeyCompare(*temporary.at(i), *temporary.at(j), k, dim) < 0 ? temporary.at(i++) : temporary.at(j--);
            }
        }
}

Node* KdTree::buildkdtree(std::vector<std::vector<Point*>>& references, std::vector<Point*>& temporary, const int left, const int right, const int dim, const int depth){
    int axis = depth % dim;
    // Base case 1: array of one
    if (left == right) {
        Point* p = references.at(0).at(right);
        Node* root = new Node(*p, depth);
        return root;
    }
    // Base case 2: array of two
    else if (right == left + 1) {
        Point* p1 = references.at(0).at(left);
        Point* p2 = references.at(0).at(right);
        Node* root = new Node(*p1, depth);
        Node* child = new Node(*p2, depth);
        root->right = child;
        return root;
    }
    // Base case 3: array of three
    else if (right == left + 2) {
        // Root will be the middle element
        Point* p1 = references.at(0).at(left+1);  
        Node* root = new Node(*p1, depth);
        // Determine which points will be left and right children
        Point* p2 = references.at(0).at(left);
        Point* p3 = references.at(0).at(right);
        if(superKeyCompare(*p2, *p3, depth, dim) == 1) {
            Node* rightchild = new Node(*p2, depth + 1);
            Node* leftchild = new Node(*p3, depth + 1);
            root->right = rightchild;
            root->left = leftchild;
            return root;
        }
        else{
            Node* rightchild = new Node(*p3, depth + 1);
            Node* leftchild = new Node(*p2, depth + 1);
            root->right = rightchild;
            root->left = leftchild;
            return root;
        }
    }

    // Find median of current array
    const int mid = left + ((right - left) / 2);

    // Index the median at the first array, which will initially be xyz,
    // but will eventually rotate with every recursive call, due to the
    // way partitioning works
    Point* p1 = references.at(0).at(mid);
    Node* root = new Node(*p1, depth);

    // Copy the xyz or references[0] to the temporary array
    // The following method is optimized such that only one temporary array is required
    for (int i = left; i <= right; i++) {
        temporary.at(i) = references.at(0).at(i);
    }

    // Reorder the rest of the superkey-based-sorted arrays with respect to the current superkey
    // The current array requires no partitioning because it is already sorted by its superkey
    int lower, upper;
    for (int i = 1; i < dim; i++) { 
        lower = left - 1;
        upper = mid;
        // the elements of the array are retrieved in order of increasing address
        for(int j = left; j <= right; j++) {
            // Each element is compared to the median
            // The current index array is partitioned and the result is stored in references[dim-1]
            // references[dim-1] index array was either stored in the array before it, or the temporary array
            int compare = superKeyCompare(*references.at(i).at(j), *p1, axis, dim);
            if(compare < 0) {
                // Place in the lower half if the result is less
                references.at(i-1).at(++lower) = references.at(i).at(j);                
            }
            else if (compare > 0) {
                // Place in the upper half if the result is greater
                references.at(i-1).at(++upper) = references.at(i).at(j);
            }
        }
    }
    // Copy the temporary array to references[dim-1] to finish permutation.
    for (int i = 0; i <= right; i++) {
        references.at(dim - 1).at(i) = temporary.at(i);
    }

    root->left = buildkdtree(references, temporary, left, mid, dim, depth+1);
    root->right = buildkdtree(references, temporary, mid+1, right, dim, depth+1);

    return root;
}

Node* KdTree::remove(Node* node, const Point& point, int depth) {
    if (node == nullptr) return nullptr;

    int axis = depth % k;
    if (node->point.x == point.x && node->point.y == point.y &&
        node->point.z == point.z && node->point.strength == point.strength) {
        if (node->right) {
            Node* minNode = findMin(node->right, axis, depth + 1);
            node->point = minNode->point;
            node->right = remove(node->right, minNode->point, depth + 1);
        } else if (node->left) {
            Node* minNode = findMin(node->left, axis, depth + 1);
            node->point = minNode->point;
            node->right = remove(node->left, minNode->point, depth + 1);
            node->left = nullptr;
        } else {
            delete node;
            return nullptr;
        }
        return node;
    }
    if (getCoordinate(point, axis) < getCoordinate(node->point, axis))
    node->left = remove(node->left, point, depth + 1);
    else
        node->right = remove(node->right, point, depth + 1);
    return node;
}

Node* KdTree::findMin(Node* node, int axis, int depth) {
    if (node == nullptr) return nullptr;

    int currentAxis = depth % k;
    if (currentAxis == axis) {
        if (node->left == nullptr)
            return node;
        return findMin(node->left, axis, depth + 1);
    }

    Node* leftMin = findMin(node->left, axis, depth + 1);
    Node* rightMin = findMin(node->right, axis, depth + 1);
    Node* minNode = node;

    if (leftMin && getCoordinate(leftMin->point, axis) < getCoordinate(minNode->point, axis))
        minNode = leftMin;
    if (rightMin && getCoordinate(rightMin->point, axis) < getCoordinate(minNode->point, axis))
        minNode = rightMin;

    return minNode;
}

void KdTree::freeTree(Node* node) {
    if (node == nullptr) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

size_t KdTree::countNodes(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

double KdTree::distance(const Point& a, const Point& b) const {
    return std::sqrt((a.x - b.x) * (a.x - b.x) +
                     (a.y - b.y) * (a.y - b.y) +
                     (a.z - b.z) * (a.z - b.z));
}

void KdTree::nearestNeighbor(Node* node, const Point& target, Point& best, double& bestDist, int depth) const {
    if (node == nullptr) return;

    double d = distance(target, node->point);
    if (d < bestDist) {
        bestDist = d;
        best = node->point;
    }

    int axis = depth % k;
    Node* nextBranch = (getCoordinate(target, axis) < getCoordinate(node->point, axis)) ? node->left : node->right;
    Node* otherBranch = (nextBranch == node->left) ? node->right : node->left;

    nearestNeighbor(nextBranch, target, best, bestDist, depth + 1);

    if (std::fabs(getCoordinate(target, axis) - getCoordinate(node->point, axis)) < bestDist)
        nearestNeighbor(otherBranch, target, best, bestDist, depth + 1);
}


double KdTree::getCoordinate(const Point& point, int axis) const {
    switch (axis) {
        case 0:  return point.x;
        case 1:  return point.y;
        default: return point.z;
    }
}

void KdTree::insertHelper(Node** node, int depth, const Point& point){
    //base case: empty spot found
    if (*node == nullptr){
        *node = new Node(point, depth);
        return;

    }
    else{
        //determine which axis to split the node into
        int axis = depth % k; 
        int compare = superKeyCompare(point, (*node)->point, axis, k);
        if (compare > 0) {
            insertHelper(&((*node)->right), depth + 1, point);
        }
        else if (compare < 0) {
            insertHelper(&((*node)->left), depth + 1, point);
        }
        else if (compare == 0) {
            return; //exit, no duplicates allowed
        }
    }
}


void KdTree::searchHelper(const Point& target, Node* node, int depth, double tol, std::vector<Point>& results) {
    if (node != nullptr) {
        if (node->point.x >= (target.x - tol) && node->point.x <= (target.x + tol) &&
            node->point.y >= (target.y - tol) && node->point.y <= (target.y + tol) &&
            node->point.z >= (target.z - tol) && node->point.z <= (target.z + tol)) {
            if (distance(node->point, target) <= tol)
                results.push_back(node->point);
        }

        int dim = depth % k;
        if ((dim == 0 && (target.x - tol) < node->point.x) ||
            (dim == 1 && (target.y - tol) < node->point.y) ||
            (dim == 2 && (target.z - tol) < node->point.z)) {
            searchHelper(target, node->left, depth + 1, tol, results);
        }
        if ((dim == 0 && (target.x + tol) > node->point.x) ||
            (dim == 1 && (target.y + tol) > node->point.y) ||
            (dim == 2 && (target.z + tol) > node->point.z)) {
            searchHelper(target, node->right, depth + 1, tol, results);
        }
    }
}

bool KdTree::findPointHelper(const Point& point, Node* node, int depth){
    if (node == nullptr) {
        return false;
    }

    if (node->point.x == point.x &&
        node->point.y == point.y &&
        node->point.z == point.z) {
        return true;
    }

    int dim = depth % k; 
    double targetCoord = getCoordinate(point, dim);
    double nodeCoord = getCoordinate(node->point, dim);

    if (targetCoord < nodeCoord) {
        return findPointHelper(point, node->left, depth + 1);
    } else {
        return findPointHelper(point, node->right, depth + 1);
    }
}

