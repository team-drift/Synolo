/**
 * @file kdtree_test.cpp
 * @brief Simple test for functionality of KD-Tree 
 * Mainly used for debugging 
 * @version 0.1
 */

#include "../include/synolo/kdtree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

std::vector<Point> GetPoints(){
    std::ifstream LogFile("log.txt");
    std::string data;
    std::vector<Point> points;
    while (getline(LogFile, data)) {
        std::istringstream iss(data);
        double angle, distance, strength = 0.0; // Default to 0
        iss >> angle >> distance >> strength; 
        points.emplace_back(angle, distance, strength);
    }
    return points;
}

// Helper to print a list of points
void printPoints(const std::vector<Point>& pts, const std::string& label) {
    std::cout << label;
    if (pts.empty()) {
        std::cout << " <none>\n";
        return;
    }
    for (const auto& p : pts) {
        std::cout << " (" 
                  << p.x << "," << p.y << "," << p.z << ")";
    }
    std::cout << "\n";
}

int main() {

    // Construct 3-D KdTree
    KdTree kdtree = KdTree(3);
    // Construct point object to add
    Point p = Point(7.0, 2.0, 6.0);
    Point p2 = Point(7.0, 2.0, 6.0);

    kdtree.insert(p);
    kdtree.insert(p2);
    Node* root = kdtree.get_root();

    std::cout<<kdtree.size()<<std::endl;
    if ((root != nullptr) && (root->left == nullptr) && (root->right == nullptr)) {
        std::cout << "all good" << std::endl;
    } else {
        std::cout << "oh oh" << std::endl;
    }

    return 0;
}
