/**
 * @file bench.cpp
 * @brief benchmark for creating and building a kdtree
 * @version 0.1
 * 
 * This file describes a benchmark for measuring the time it 
 * takes to perform the createkdtree and insert functions
 * TODO: bench generating with pcl, and for both implementations,
 * bench the filtering/search algorithms.
 * Add rebalancing logic to insert and bench
 */

 #include "../include/synolo/kdtree.h"

 #include <chrono>
 #include <iostream>

/**
 * @brief generates a vector of Point* with random values
 * Uses 3-D points
 * @param count number of points to generate
 * @returns vector of random points
 */
std::vector<Point*> generate_points(int count) {

    std::vector<Point*> points;

    for(int i = 0; i < count; i++) {
        points.push_back(
        new Point(
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX), 
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX),
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX)
            )
        );
    }

    return points;
}

int main() {

    // Number of points to test on
    const int N = 100; //2^20

    // Number of times to test
    const int iterations = 1000;

    std::cout<<"------------Benchmarking started------------"<<std::endl;

    // Determine if our clock is steady:

    if (!std::chrono::high_resolution_clock::is_steady) {

        std::cout << "Warning: high_resolution_clock is not steady!" << std::endl;

    }

    std::cout <<"------Testing createkdtree perfomance------" << std::endl;

    // Total time for N tests
    long double create_performance = 0;

    for(int i = 0; i < iterations; i++) {

        // Generate dataset of N points
        std::vector<Point*> points = generate_points(N);

        // Start the clock
        auto start = std::chrono::high_resolution_clock::now();

        // Construct empty 3D tree
        KdTree kdtree = KdTree(3);
    
        // Build the tree with points
        kdtree.createkdtree(points, 3);
    
        // Stop the clock
        auto stop = std::chrono::high_resolution_clock::now();
    
        // Calculate the time
        auto diff = stop - start;

        // Add to total
        create_performance += std::chrono::duration<double, std::milli>(diff).count();

        // Clean allocated points
        for (Point* p : points) {
            delete p;
        }
        
    }


    // Should the clock start before the for loop and end after? 
    std::cout <<"---------Testing insert perfomance---------" << std::endl;

    // Total time for N tests
    long double insert_performance = 0;

    for(int i = 0; i < iterations; i++) {

        // Generate dataset of N points
        std::vector<Point*> points = generate_points(N);

        // Create kdtree
        KdTree kdtree = KdTree(3);

        // Iterate over the entire list of points
        for(int j = 0; j < N; j++) {

            // Start the clock
            auto start = std::chrono::high_resolution_clock::now();

            // Insert point into the tree
            kdtree.insert(*points.at(j));

            // Stop the clock
            auto stop = std::chrono::high_resolution_clock::now();
    
            // Calculate the time
            auto diff = stop - start;

            // Add to the total 
            insert_performance += std::chrono::duration<double, std::milli>(diff).count();
        }

        // Clean allocated points
        for (Point* p : points) {
            delete p;
        }
    }

    std::cout <<"------------------Results------------------" << std::endl;
    std::cout << "createkdtree time: " << create_performance / iterations << "ms" << std::endl;
    std::cout << "Avg insert per tree: " << insert_performance / iterations << " ms" << std::endl;
    //std::cout << "Avg insert per point: " << insert_performance / (iterations * N) << " ms" << std::endl;

    return 0;
}           