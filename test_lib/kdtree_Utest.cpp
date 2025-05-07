#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "kdtree.h"
#include <cmath>

constexpr double EPSILON = 1e-6;
bool pointsEqual(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) < EPSILON &&
           std::abs(a.y - b.y) < EPSILON &&
           std::abs(a.z - b.z) < EPSILON &&
           std::abs(a.strength - b.strength) < EPSILON;
}

TEST_CASE("KdTree Basic Operations", "[kdtree]") {
    KdTree tree;
    
    SECTION("Empty Tree Behavior") {
        REQUIRE(tree.size() == 0);
        REQUIRE(tree.search({0,0}, 1.0).empty());
        REQUIRE_FALSE(tree.remove({0,0}));
    }

    SECTION("Single Point Operations") {
        Point p(45.0, sqrt(2.0));
        tree.insert(p);
        
        REQUIRE(tree.size() == 1);
        REQUIRE(tree.FindPoint(p));
        
        auto results = tree.search(p, 0.1);
        REQUIRE(results.size() == 1);
        REQUIRE(pointsEqual(results[0], p));

        REQUIRE(tree.remove(p));
        REQUIRE(tree.size() == 0);
    }
}

TEST_CASE("Tree Construction and Balancing", "[kdtree]") {
    std::vector<Point> points = {
        {0.0, 2.0}, {90.0, 1.0}, {180.0, 3.0}, {270.0, 2.0}
    };
    
    KdTree tree(points);
    
    SECTION("Correct Construction") {
        REQUIRE(tree.size() == 4);
        REQUIRE(tree.FindPoint({90.0, 1.0}));
        REQUIRE(tree.FindPoint({270.0, 2.0}));
    }

    SECTION("Balanced Tree Properties") {
        // Verify through public interface only
        REQUIRE(tree.size() == 4);
        REQUIRE(tree.FindPoint({180.0, 3.0}));
    }
}

TEST_CASE("Insertion and Deletion", "[kdtree]") {
    KdTree tree;
    std::vector<Point> points = {
        {45.0, sqrt(2.0)}, {135.0, sqrt(2.0)}, 
        {225.0, sqrt(2.0)}, {315.0, sqrt(2.0)}
    };

    SECTION("Sequential Insertion") {
        for (const auto& p : points) {
            tree.insert(p);
        }
        REQUIRE(tree.size() == 4);
        REQUIRE(tree.FindPoint(points[2]));
    }

    SECTION("Duplicate Handling") {
        tree.insert(points[0]);
        tree.insert(points[0]);
        REQUIRE(tree.size() == 2);
    }

    SECTION("Deletion Scenarios") {
        tree.build(points);
        
        REQUIRE(tree.remove(points[3]));
        REQUIRE_FALSE(tree.FindPoint(points[3]));
        
        REQUIRE(tree.remove(points[0]));
        REQUIRE(tree.size() == 2);
    }
}

TEST_CASE("Search Operations", "[kdtree]") {
    std::vector<Point> points = {
        {0.0, 1.0}, {90.0, 1.0}, {180.0, 1.0}, {270.0, 1.0}
    };
    KdTree tree(points);

    SECTION("Exact Match Search") {
        auto results = tree.search(points[1], 0.0);
        REQUIRE(results.size() == 1);
        REQUIRE(pointsEqual(results[0], points[1]));
    }

    SECTION("Radius Search") {
        Point query(45.0, sqrt(2.0));
        auto results = tree.search(query, 1.5);
        REQUIRE(results.size() == 2);
    }

    SECTION("Empty Result Search") {
        Point query(45.0, 10.0);
        auto results = tree.search(query, 0.1);
        REQUIRE(results.empty());
    }
}

TEST_CASE("Nearest Neighbor", "[kdtree]") {
    std::vector<Point> points = {
        {0.0, 2.0}, {90.0, 1.0}, {180.0, 3.0}, {270.0, 2.0}
    };
    KdTree tree(points);

    SECTION("Exact Match") {
        Point target = points[1];
        Point best;
        double bestDist = INFINITY;
        tree.nearestNeighbor(target, best, bestDist);
        REQUIRE(pointsEqual(best, target));
    }

    SECTION("Close Neighbor") {
        Point target(80.0, 1.05);
        Point best;
        double bestDist = INFINITY;
        tree.nearestNeighbor(target, best, bestDist);
        REQUIRE(pointsEqual(best, points[1]));
    }
}

TEST_CASE("Edge Cases and Stress Tests", "[kdtree]") {
    SECTION("Large Dataset") {
        std::vector<Point> points;
        for (int i = 0; i < 1000; ++i) {
            points.emplace_back(i % 360, 1.0 + i/1000.0);
        }
        
        KdTree tree(points);
        REQUIRE(tree.size() == 1000);
        REQUIRE(tree.FindPoint(points.back()));
    }

    SECTION("Degenerate Cases") {
        std::vector<Point> colinear = {
            {0.0, 1.0}, {0.0, 2.0}, {0.0, 3.0}
        };
        
        KdTree tree(colinear);
        REQUIRE(tree.size() == 3);
        REQUIRE(tree.FindPoint({0.0, 2.0}));
    }
}