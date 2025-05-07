/**
 * @file kdtree.h
 * @brief Definition of KD-tree, Node, and point
 * @version 0.1
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
    Point(float angle_deg, float dist, float str = 0);
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

        KdTree();
        /**
         * @brief Constructor that builds a KdTree from a vector of points.
         * Internally calls build() to construct a balanced tree.
         */
        KdTree(const std::vector<Point>& points);

        //destructor
        ~KdTree();

        /**
         * @brief Builds a balanced KdTree from a vector of points.
         * Any existing tree is cleared before building.
         */
        void build(const std::vector<Point>& points);

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
    private:

        Node* root;
        const int k;
        // Helper functions:
        // Handle intenral details like recursion, depth, or pointer management when insert is called
        // Allow to pass down additional data with each recursive call

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

        Node* buildKdTree(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, int depth);

        double getCoordinate(const Point& point, int axis) const;        

        /**
         * @brief Helper recursive function for insertion.
         * Pointer to pointer is used to modify pointer directly within the helper function
         * If a pointer is NULL, we can check for NULL and assign a new node right there
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