/**
 * @file kdtree.h
 * @brief Definition of KD-tree, Node, and point
 * @version 0.2
 * 
 * This file allows us to use a KD-tree for k nearest neighbor algorithms
 * The main class is the KdTree class, which uses the Point structure to store values inside Nodes
 */

#pragma once
#include <vector>
#include <algorithm>
#include <limits>

struct Point{
    double x, y, z;
    double strength;
     // Constructor for LiDAR polar coordinates (angle, distance, strength)
    Point(float angle_deg, float dist, float str = 0);

    // Constructor for custom Cartesian coordinates (x, y, z, strength)
    Point(double x = 0, double y = 0, double z = 0, double strength = 0);

    // Defualt
    Point();
};

struct Node{
    Point point;
    Node* left;
    Node* right;
    int depth;
    Node(const Point& pt, int depth);
};

class KdTree{
    public:

        KdTree(int k);

        //destructor
        ~KdTree();

        /**
         * @brief perform the neccesary operations and build a kdtree
         * pointers allow for efficiency by preventing copying of objects
         * Adapted from Robert Sedgewick's "Algorithms in C++" 
         * p. 166. Addison-Wesley, Reading, MA, 1992 and Russell A. Brown's
         * "Building a Balanced k-d Tree in O(kn log n) Time", 2015
         * @param coordinates a vector of pointers, where each Point* points to a Point object
         * @param dim N dimention of the KdTree
         */
        Node *createkdtree(std::vector<Point*>& coordinates, const int dim);


        /**
         * @brief Inserts a new point into the tree.
         * @param point The point to insert.
         */
        void insert(const Point& point);

                
        /**
         * @brief Removes a point from the tree.
         * @return True if the point was successfully removed, false otherwise.
         */
        bool remove(const Point& point);


        /**
         * @brief Searches for all points within a given tolerance of a target.
         * @param target The center point.
         * @param tol Distance tolerance (search radius).
         * @return A vector of point ids found within the radius.
         */
        std::vector<Point> search(const Point& target, double tol);

        /**
         * @brief Recursively searches for the nearest neighbor to the target point.
         * Uses backtracking and bounding box pruning for efficiency.
         */
        void nearestNeighbor(Node* node, const Point& target, Point& best, double& bestDist, int depth) const;

        /**
         * @brief Searches entire tree for a single point
         * Used for testing purposes
         * @param point point to search for in tree
         * @return True if point is found, False otherwise
         */
        bool FindPoint(const Point& point);

        // Return size of Kdtree
        size_t size() const;

        /**
         * @brief public method to access root for testing
         * @return the root node of the tree
         */
        Node* get_root();
    private:

        Node* root;
        const int k;

        /**
         * @brief The superKeyCompare method compares two long arrays, representing coordinates, in all k dimensions,
         * and uses the sorting or partition coordinate as the most significant dimension.
         * Adapted from Russell A. Brown's "Building a Balanced k-d Tree in O(kn log n) Time", 2015
         * @param p1 point object to compare
         * @param p2 point object to compare
         * @param k the dimention where the comparison will start, signifying which superkey to use
         * @param dim dimention of the kdtree
         * @return 1 if p1 > p2, -1 otherwise, and 0 if they are equal 
         */
        int superKeyCompare(const Point& p1, const Point &p2, const int k, const int dim);

        /**
         * @brief recursivly sort an array of points using a superkey comparison
         * Adapted from Robert Sedgewick's "Algorithms in C++" 
         * p. 166. Addison-Wesley, Reading, MA, 1992 and Russell A. Brown's
         * "Building a Balanced k-d Tree in O(kn log n) Time", 2015
         * @param points the array of Point* pointers to Point objects, which will be sorted
         * @param temporary a buffer of the same size used during the merge phase
         * @param left start index of the left region when sorting 
         * @param right start index of the right region when sorting
         * @param k the dimention where the comparison will start, signifying which superkey to use
         * @param dim dimention of the kdtree
         */
        void mergesort(std::vector<Point*> &points,std::vector<Point*> &temporary ,const int left, const int right,
                    const int k, const int dim);

        // Helper functions:
        // Handle intenral details like recursion, depth, or pointer management when insert is called
        // Allow to pass down additional data with each recursive call
        
        /**
         * @brief recurisvly builds the kdtree using the median of the respective array of coordinates
         * each array represents Point* sorted by the superkey (x:y:z, y:z:x, z:x:y)
         * the function assumes sorted coordinates and no duplicates
         * Adapted from Robert Sedgewick's "Algorithms in C++" 
         * p. 166. Addison-Wesley, Reading, MA, 1992 and Russell A. Brown's
         * "Building a Balanced k-d Tree in O(kn log n) Time", 2015
         * @param references a vector of vector<Point*> where each vector is a sorted arrangment of Point* objects
         * @param temporary a buffer of the same size as a Point* vector used for partioning when splitting 
         * @param left left starting element of the reference arrays
         * @param rigth right starting element of the reference arrays
         * @param dim number of dimentions for the KdTree
         * @param depth depth of the tree
         * @returns pointer to the root Node of the tree
         */
        Node *buildkdtree(std::vector<std::vector<Point*>>& references, std::vector<Point*>& temporary, const int left, 
            const int right, const int dim, const int depth);

        /** 
         * @brief Recursively removes a point from the tree.
         * Handles subtree rebalancing by replacing with a min node when necessary.
         */
        Node* remove(Node* node, const Point& point, int depth);

        /**
         * @brief Finds the node with the minimum coordinate on a given axis.
         * Used when replacing a node during deletion.
         */
        Node* findMin(Node* node, int axis, int depth);

        // Recursively deletes all nodes
        void freeTree(Node* node);

        // Used in remove() to confirm successful deletion
        size_t countNodes(Node* node) const;

        // Euclidiean Distance. Used in nearestNeighbor, searchHelper
        double distance(const Point& a, const Point& b) const;

        double getCoordinate(const Point& point, int axis) const;        

        /**
         * @brief Helper recursive function for insertion.
         * Pointer to pointer is used to modify pointer directly within the helper function
         * If a pointer is NULL, we can check for NULL and assign a new node right there
         * @param node current node being compared
         * @param depth depth of the node in the tree
         * @param point point we want to insert
         *  */ 
        void insertHelper(Node** node, int depth, const Point& point);

        /** 
         * @brief Helper recursive function for searching points within a tolerance.
         * Tolerence search is a radius search. This will be used for DROR filter.
         * When you perform a search, we want to find all points within a certain distance
         *  */ 
        void searchHelper(const Point& target, Node* node, int depth, double tol, std::vector<Point>& results);

        bool findPointHelper(const Point& point, Node* node, int depth);
};