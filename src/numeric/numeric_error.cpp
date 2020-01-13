//
// Created by giacomo on 12/12/2019.
//
#include "numeric/structures/Vector.h"
#include "numeric/structures/OBB.h"

#include <numeric/FibonacciTotalError.h>
#include <numeric/structures/IntervalArithmetic.h>
#include <numeric/structures/AABB.h>


int main(void) {
    /// Introduction to the float representation and to the total error
    printing_float_representation();
    total_error_plot();

    /// Cross product error considerations: vector cross product and obb overlaps
    vector_test_intersection();
    obb_overlap_testing();

    /// Last Topic: Interval Arithmetic (bounding the error with an interval)
    examples_on_interval_arithmetic();
    sphere_AABB_intersection();
    return 0;
}