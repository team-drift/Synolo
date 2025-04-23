// main.cpp

#include "../include/kdtree.h"
#include <iostream>
#include <vector>

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
    std::vector<Point> points = {
        {1.0, 2.0, 3.0, 0.5},
        {2.0, 3.0, 4.0, 0.6},
        {3.0, 1.0, 2.0, 0.7},
        {4.0, 4.0, 4.0, 0.8},
        {5.0, 2.0, 1.0, 0.9}
    };
    KdTree tree(points);
    std::cout << "Test 1: Built tree from 5 initial points.\n\n";

    // 2) Insert a new point
    Point p6{6.0, 2.5, 3.0, 1.0};
    tree.insert(p6);
    std::cout << "Test 2: Inserted point (6,2.5,3).\n\n";

    // 3) Search within radius 2.0 of (5,2,2)
    Point query{5.0, 2.0, 2.0, 0.0};
    auto results = tree.search(query, 2.0);
    printPoints(results, "Test 3: Search around (5,2,2) r=2.0:");

    // 4) Remove the inserted point
    bool removed = tree.remove(p6);
    std::cout << "Test 4: Removing (6,2.5,3) -> " 
              << (removed ? "Success" : "Fail") << "\n\n";

    // 5) Search again to confirm removal
    results = tree.search(query, 2.0);
    printPoints(results, "Test 5: Search after removal:");

    // 6) Try removing a non-existent point
    Point missing{100,100,100,0};
    bool removedMissing = tree.remove(missing);
    std::cout << "Test 6: Removing non-existent (100,100,100) -> " 
              << (removedMissing ? "Success (unexpected)" : "Fail (expected)") 
              << "\n\n";

    // 7) Rebuild with a new small dataset
    std::vector<Point> newPts = {
        {0,0,0,0}, {1,1,1,0}, {2,2,2,0}
    };
    tree.build(newPts);
    std::cout << "Test 7: Rebuilt tree with 3 points.\n";

    results = tree.search({1,1,1,0}, 1.5);
    printPoints(results, "Test 7: Search around (1,1,1) r=1.5:");

    std::cout << "\nAll Tests Complete\n";
    return 0;
}
