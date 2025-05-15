/**
 * @file insert_test.cpp
 * @brief Tests for insert function
 * @version 0.1
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../include/synolo/kdtree.h"

TEST_CASE("Insert test", "[insert]") {

    // Construct 3-D KdTree
    KdTree kdtree = KdTree(3);

    SECTION("Empty", "Ensures we can insert a point into en empty tree") {

        // Construct point object to add
        Point p = Point(7.0, 2.0, 6.0);

        kdtree.insert(p);
        Node* root = kdtree.get_root();
        
        REQUIRE(root->point.x == 7.0);
        REQUIRE(root->point.y == 2.0);
        REQUIRE(root->point.z == 6.0);
        REQUIRE(root->left == nullptr);
        REQUIRE(root->right == nullptr);
    }

    SECTION("Duplicates", "Ensures insert function ignores duplicates") {

        // Construct point object to add
        Point p = Point(7.0, 2.0, 6.0);
        Point p2 = Point(7.0, 2.0, 6.0);

        kdtree.insert(p);
        kdtree.insert(p2);
        Node* root = kdtree.get_root();

        REQUIRE(root->left == nullptr);
        REQUIRE(root->right == nullptr);
        REQUIRE(kdtree.size() == 1);
    }
    
    SECTION("Order", "Ensures points are inserted in the correct order") {
        // Root insertion
        Point rootPoint = Point(7.0, 2.0, 6.0);
        kdtree.insert(rootPoint);
        Node* root = kdtree.get_root();
        REQUIRE(root != nullptr);
        REQUIRE(root->point.x == 7.0);
        REQUIRE(root->point.y == 2.0);
        REQUIRE(root->point.z == 6.0);
    
        // Insert first-level children
        {
            Point p1 = Point(5.0, 4.0, 2.0);
            Point p2 = Point(9.0, 5.0, 3.0);
            kdtree.insert(p1);
            kdtree.insert(p2);
    
            REQUIRE(root->left != nullptr);
            REQUIRE(root->left->point.x == 5.0);
            REQUIRE(root->right != nullptr);
            REQUIRE(root->right->point.x == 9.0);
        }
    
        // Insert second-level left subtree
        {
            Point p3 = Point(2.0, 1.0, 3.0);
            Point p4 = Point(1.0, 6.0, 8.0);
            kdtree.insert(p3);
            kdtree.insert(p4);
    
            REQUIRE(root->left->left != nullptr);
            REQUIRE(root->left->left->point.x == 2.0);
            REQUIRE(root->left->right != nullptr);
            REQUIRE(root->left->right->point.x == 1.0);
        }
    
        // Insert second-level right subtree
        {
            Point p5 = Point(8.0, 4.0, 2.0);
            Point p6 = Point(9.0, 6.0, 7.0);
            kdtree.insert(p5);
            kdtree.insert(p6);
    
            REQUIRE(root->right->left != nullptr);
            REQUIRE(root->right->left->point.x == 8.0);
            REQUIRE(root->right->right != nullptr);
            REQUIRE(root->right->right->point.x == 9.0);
        }
    
        // Insert deeper left subtree
        {
            Point p7 = Point(6.0, 3.0, 1.0);
            Point p8 = Point(2.0, 3.0, 3.0);
            Point p9 = Point(3.0, 4.0, 5.0);
            Point p10 = Point(4.0, 7.0, 9.0);
    
            kdtree.insert(p7);
            kdtree.insert(p8);
            kdtree.insert(p9);
            kdtree.insert(p10);
    
            REQUIRE(root->left->left->left != nullptr);
            REQUIRE(root->left->left->left->point.x == 6.0);
            REQUIRE(root->left->left->right != nullptr);
            REQUIRE(root->left->left->right->point.x == 2.0);
    
            REQUIRE(root->left->right->left != nullptr);
            REQUIRE(root->left->right->left->point.x == 3.0);
            REQUIRE(root->left->right->right != nullptr);
            REQUIRE(root->left->right->right->point.x == 4.0);
        }
    
        // Insert deeper right subtree
        {
            Point p11 = Point(9.0, 4.0, 1.0);
            Point p12 = Point(8.0, 1.0, 5.0);
            Point p13 = Point(8.0, 7.0, 6.0);
            Point p14 = Point(9.0, 7.0, 8.0);
    
            kdtree.insert(p11);
            kdtree.insert(p12);
            kdtree.insert(p13);
            kdtree.insert(p14);
    
            REQUIRE(root->right->left->left != nullptr);
            REQUIRE(root->right->left->left->point.x == 9.0);
            REQUIRE(root->right->left->right != nullptr);
            REQUIRE(root->right->left->right->point.x == 8.0);
    
            REQUIRE(root->right->right->left != nullptr);
            REQUIRE(root->right->right->left->point.x == 8.0);
            REQUIRE(root->right->right->right != nullptr);
            REQUIRE(root->right->right->right->point.x == 9.0);
        }
    }
    
}