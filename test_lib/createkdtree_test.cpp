/**
 * @file createkdtree_test.cpp
 * @brief Tests for createkdtree function
 * @version 0.1
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <memory>
#include "../include/synolo/kdtree.h"

TEST_CASE("Handles single point", "[createkdtree]") {
    std::vector<Point*> points = {new Point(1.0, 2.0, 3.0)};
    KdTree tree(3);
    Node* root = tree.createkdtree(points, 3);

    REQUIRE(root != nullptr);
    REQUIRE(root->point.x == 1.0);
    REQUIRE(root->point.y == 2.0);
    REQUIRE(root->point.z == 3.0);
    REQUIRE(root->left == nullptr);
    REQUIRE(root->right == nullptr);

}

TEST_CASE("Handles two points correctly", "[createkdtree]") {
    std::vector<Point*> points = {new Point(5.0, 4.0, 2.0), new Point(7.0, 2.0, 6.0)};
    KdTree tree(3);
    Node* root = tree.createkdtree(points, 3);

    // Root should be first sorted element (x=5)
    REQUIRE(root->point.x == 5.0);
    REQUIRE(root->point.y == 4.0);
    REQUIRE(root->point.z == 2.0);
    
    // Child should be in left branch (axis=0, x=7 > 5 should be in right)
    REQUIRE(root->right != nullptr); // Paper's logic uses superkey comparison
    REQUIRE(root->right->point.x == 7.0);
    REQUIRE(root->right->point.y == 2.0);
    REQUIRE(root->right->point.z == 6.0);

}

TEST_CASE("Handles three points with median root", "[createkdtree]") {
    std::vector<Point*> points = {
        new Point(1.0, 2.0, 3.0),
        new Point(5.0, 4.0, 2.0), // Expected median
        new Point(9.0, 6.0, 7.0)
    };
    
    KdTree tree(3);
    Node* root = tree.createkdtree(points, 3);

    REQUIRE(root->point.x == 5.0);
    REQUIRE(root->left->point.x == 1.0);
    REQUIRE(root->right->point.x == 9.0);

}

TEST_CASE("Removes duplicates during construction", "[createkdtree]") {
    std::vector<Point*> points = {
        new Point(5.0, 4.0, 2.0),
        new Point(5.0, 4.0, 2.0) // Duplicate
    };

    KdTree tree(3);
    Node* root = tree.createkdtree(points, 3);

    REQUIRE(root != nullptr);
    REQUIRE(root->left == nullptr); // Single node after deduplication
    REQUIRE(tree.size() == 1);


}

TEST_CASE("KdTree construction matches research paper example", "[createkdtree]") {
    // Replicate the 15 (x,y,z) tuples from the research paper (Figure 2)
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

    // Build tree for 3D data
    KdTree tree(3);
    Node* root = tree.createkdtree(points, 3);

    // Verify root node matches paper's first split median (7,2,6)
    REQUIRE(root->point.x == 7.0);
    REQUIRE(root->point.y == 2.0);
    REQUIRE(root->point.z == 6.0);

    // Verify left subtree root (5,4,2)
    REQUIRE(root->left->point.x == 5.0);
    REQUIRE(root->left->point.y == 4.0);
    REQUIRE(root->left->point.z == 2.0);

    // Verify left subtree of depth 2
    REQUIRE(root->left->left->point.x == 2.0);
    REQUIRE(root->left->left->point.y == 1.0);
    REQUIRE(root->left->left->point.z == 3.0);

    // Verify left subtree left leaf node 
    REQUIRE(root->left->left->left->point.x == 6.0);
    REQUIRE(root->left->left->left->point.y == 3.0);
    REQUIRE(root->left->left->left->point.z == 1.0);

    // Verify left subtree right leaf node 
    REQUIRE(root->left->left->right->point.x == 2.0);
    REQUIRE(root->left->left->right->point.y == 3.0);
    REQUIRE(root->left->left->right->point.z == 3.0);

    // Verify left right subtree of depth 2
    REQUIRE(root->left->right->point.x == 1.0);
    REQUIRE(root->left->right->point.y == 6.0);
    REQUIRE(root->left->right->point.z == 8.0);

    // Verify left right left leaf node
    REQUIRE(root->left->right->left->point.x == 3.0);
    REQUIRE(root->left->right->left->point.y == 4.0);
    REQUIRE(root->left->right->left->point.z == 5.0);

    // Verify left right right leaf node
    REQUIRE(root->left->right->right->point.x == 4.0);
    REQUIRE(root->left->right->right->point.y == 7.0);
    REQUIRE(root->left->right->right->point.z == 9.0);

    // Verify right subtree root (9,5,3)
    REQUIRE(root->right->point.x == 9.0);
    REQUIRE(root->right->point.y == 5.0);
    REQUIRE(root->right->point.z == 3.0);

    // Verify right left subtree
    REQUIRE(root->right->left->point.x == 8.0);
    REQUIRE(root->right->left->point.y == 4.0);
    REQUIRE(root->right->left->point.z == 2.0);

    // Verify right left left leaf
    REQUIRE(root->right->left->left->point.x == 9.0);
    REQUIRE(root->right->left->left->point.y == 4.0);
    REQUIRE(root->right->left->left->point.z == 1.0); 
    
    // Verify right left right leaf
    REQUIRE(root->right->left->right->point.x == 8.0);
    REQUIRE(root->right->left->right->point.y == 1.0);
    REQUIRE(root->right->left->right->point.z == 5.0);     

    // Verify right right subtree
    REQUIRE(root->right->right->point.x == 9.0);
    REQUIRE(root->right->right->point.y == 6.0);
    REQUIRE(root->right->right->point.z == 7.0);    

    // Verify right right left leaf
    REQUIRE(root->right->right->left->point.x == 8.0);
    REQUIRE(root->right->right->left->point.y == 7.0);
    REQUIRE(root->right->right->left->point.z == 6.0);
    
    // Verify right right left leaf
    REQUIRE(root->right->right->right->point.x == 9.0);
    REQUIRE(root->right->right->right->point.y == 7.0);
    REQUIRE(root->right->right->right->point.z == 8.0);     

}

