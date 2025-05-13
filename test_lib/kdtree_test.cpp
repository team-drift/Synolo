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

    std::vector<Point*> points = {
        new Point(2.0, 3.0, 3.0),   // Index 0
        new Point(5.0, 4.0, 2.0),   // Index 1
        new Point(9.0, 6.0, 7.0),   // Index 2
        new Point(4.0, 7.0, 9.0),   // Index 3
        new Point(8.0, 1.0, 5.0),   // Index 4
        new Point(7.0, 2.0, 6.0),   // Index 5 (Median at first split)
        new Point(9.0, 4.0, 1.0),   // Index 6
        new Point(8.0, 4.0, 2.0),   // Index 7
        new Point(9.0, 7.0, 8.0),   // Index 8
        new Point(6.0, 3.0, 1.0),   // Index 9
        new Point(3.0, 4.0, 5.0),   // Index 10
        new Point(1.0, 6.0, 8.0),   // Index 11
        new Point(9.0, 5.0, 3.0),   // Index 12
        new Point(2.0, 1.0, 3.0),   // Index 13
        new Point(8.0, 7.0, 6.0)    // Index 14
    };

    KdTree tree(3);
    Node* root = tree.createkdtree(points, 3);
    std::cout<<root->point.x<<std::endl;

    return 0;
}
