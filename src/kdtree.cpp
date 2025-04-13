#include "kdtree.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

Node::Node(const Point& pt, int d) : point(pt), left(nullptr), right(nullptr), depth(d) {}

KdTree::KdTree() : root(nullptr), k(3) {};

KdTree::KdTree(const std::vector<Point>& points) : root(nullptr), k(3) {
    build(points);
}

KdTree::~KdTree() {
    freeTree(root);
}

void KdTree::build(const std::vector<Point>& points) {
    // Make a copy as buildKdTree uses iterators and reorders the points.
    std::vector<Point> pts = points;
    freeTree(root);
    root = buildKdTree(pts.begin(), pts.end(), 0);
}

void KdTree::insert(const Point& point, int id){
    KdTree::insertHelper(&root, 0, point, id);
}

bool KdTree::remove(const Point& point) {
    size_t oldCount = countNodes(root);
    root = remove(root, point, 0);
    size_t newCount = countNodes(root);
    return newCount < oldCount;
}

std::vector<int> KdTree::search(const Point& target, double tol){
    std::vector<int> ids;
    KdTree::searchHelper(target, root, 0, tol, ids);
    return ids;
}

//___________________________________________________________________________________
//private methods

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
    if (node == nullptr) return 0;
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

Node* KdTree::buildKdTree(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, int depth) {
    if (begin >= end)
        return nullptr;
    int axis = depth % k;

    // Find the median using nth_element so that the splitting point is found efficiently.
    auto mid = begin + std::distance(begin, end) / 2;
    std::nth_element(begin, mid, end, [this, axis](const Point& a, const Point& b) {
        return getCoordinate(a, axis) < getCoordinate(b, axis);
    });

    // Create the current node using the median element.
    Node* node = new Node(*mid, depth);
    node->left = buildKdTree(begin, mid, depth + 1);
    node->right = buildKdTree(mid + 1, end, depth + 1);
    return node;
}

double KdTree::getCoordinate(const Point& point, int axis) const {
    switch (axis) {
        case 0:  return point.x;
        case 1:  return point.y;
        default: return point.z;
    }
}

void KdTree::insertHelper(Node** node, int depth, const Point& point, int id){
    //base case: empty spot found
    if (*node == nullptr){
        *node = new Node(point, id);
    }
    else{
        //determine which axis to split the node into
        int dim = depth % 3;
        if((dim == 0 && point.x < (*node)->point.x) ||
           (dim == 1 && point.y < (*node)->point.y) ||
           (dim == 2 && point.z < (*node)->point.z)){
            //help modify child pointer directly by passing address of node 
            insertHelper(&((*node)->left), depth+1, point, id);
           }
           else{
            insertHelper(&((*node)->right), depth + 1, point, id);
           }
    }
}


void KdTree::searchHelper(const Point &target, Node *node, int depth, double tol, std::vector<int> &ids){
    if (node != nullptr) {
        // Check if current node is within the bounding box around target.
        if (node->point.x >= (target.x - tol) && node->point.x <= (target.x + tol) &&
            node->point.y >= (target.y - tol) && node->point.y <= (target.y + tol) &&
            node->point.z >= (target.z - tol) && node->point.z <= (target.z + tol)) {
            double dx = node->point.x - target.x;
            double dy = node->point.y - target.y;
            double dz = node->point.z - target.z;
            double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (distance <= tol)
                ids.push_back(node->id);
        }
        
        // Decide which branches to search based on current splitting dimension.
        int dim = depth % 3;
        if (dim == 0) {
            if ((target.x - tol) < node->point.x)
                searchHelper(target, node->left, depth + 1, tol, ids);
            if ((target.x + tol) > node->point.x)
                searchHelper(target, node->right, depth + 1, tol, ids);
        }
        else if (dim == 1) {
            if ((target.y - tol) < node->point.y)
                searchHelper(target, node->left, depth + 1, tol, ids);
            if ((target.y + tol) > node->point.y)
                searchHelper(target, node->right, depth + 1, tol, ids);
        }
        else {  // dim == 2
            if ((target.z - tol) < node->point.z)
                searchHelper(target, node->left, depth + 1, tol, ids);
            if ((target.z + tol) > node->point.z)
                searchHelper(target, node->right, depth + 1, tol, ids);
        }
    }
}


