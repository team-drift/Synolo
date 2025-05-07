/**
 * @file kdtree_test.cpp
 * @brief Simple test for functionality of KD-Tree using data from a log file
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
    std::cout << "KD-Tree Simple Tests\n\n";

    // 1) Build from initial point set
    std::vector<Point> points = GetPoints();  // read points from log file
    KdTree tree(points);
    std::cout << "Test 1: Built tree from initial points.\n\n";

    // 2) Insert a new point
    Point p6{-232.70, 0.80};
    tree.insert(p6);
    if(tree.FindPoint(p6)){
        std::cout << "Test 2: Inserted point (-232.70, 0.80).\n\n";
    }

    // 3) Search within radius 2.0 of (5,2)
    Point query(-242.44, 0.61);
    std::vector<Point> results = tree.search(query, 2.0);
    printPoints(results, "Test 3: Search around (-0.233, 0.540) r=2.0:");

    // 4) Remove the inserted point
    bool removed = tree.remove(p6);
    std::cout << "Test 4: Removing (-232.70, 0.80) -> " 
              << (removed ? "Success" : "Fail") << "\n\n";

    // 5) Search again to confirm removal
    results = tree.search(query, 2.0);
    printPoints(results, "Test 5: Search after removal:");

    // 6) Try removing a non-existent point
    Point missing(160, 2);
    bool removedMissing = tree.remove(missing);
    std::cout << "Test 6: Removing non-existent (100,100,100) -> " 
              << (removedMissing ? "Success (unexpected)" : "Fail (expected)") 
              << "\n\n";

    // 7) Rebuild with a new small dataset
    std::vector<Point> newPts = {
        {0,0}, {313.92, 0.24}, {-214.35, 0.45}
    };
    tree.build(newPts);
    std::cout << "Test 7: Rebuilt tree with 3 points.\n";

    results = tree.search({313.92, 0.24}, 1.5);
    printPoints(results, "Test 7: Search around (1,1,1) r=1.5:");

    std::cout << "\nAll Tests Complete\n";
    return 0;
}
