//
// Created by giacomo on 10/01/2020.
//

#include <iostream>
#include <utility>
#include <ostream>

#include "numeric/structures/Vector.h"

glm::tvec3<float> Vector::asVector() {
    if (!hasPrecomputedResult) { // Check whether we have a precomputed GLM representation
        casted_difference = destination - source; // Represent the vector as a GLM vector
        hasPrecomputedResult = true;
    }
    return casted_difference;
}

Vector::Vector(const glm::tvec3<float> &x, const glm::tvec3<float> &y) : source(x), destination(y), hasPrecomputedResult{false} {}

Vector::Vector(const glm::tvec3<float> &direction) :  destination{direction} { source.x = 0.0f; source.y = 0.0f; source.z = 0.0f; hasPrecomputedResult = false; }

Vector::Vector() : Vector{0.0f, 0.0f, 0.0f} {}

Vector::operator glm::tvec3<float>() {
    return asVector();
}

float Vector::operator[](int b) {
    return asVector()[b];
}

Vector &Vector::operator=(const Vector &rhs) {
    hasPrecomputedResult = rhs.hasPrecomputedResult;
    source = rhs.source;
    destination = rhs.destination;
    casted_difference = rhs.casted_difference;
    return *this;
}


glm::tvec3<float> normalizeNotZeroVector(const glm::tvec3<float> &x) {
    return (isZeroVector(x)) ? x : glm::normalize(x);
}

std::pair<glm::tvec3<float>, bool>
SepAxisCrossProd(Vector &ab, Vector &cd, bool doRobust) {
    // Determining the (numerical) vectors from the source and destination points;
    const glm::tvec3<float> vectorAB = ab.asVector(), vectorCD = cd.asVector();

    /*
     * When the two vectors are nearly parallel, the result of the cross product might be a zero vector. As a
     * consequence, all the projections over such axis will have a zero distance...
     */

    // NOTE:
    // =====
    //
    // Why are we normalizing the vectors for the cross product?
    // 1) Vectors having a large magnitude could also add significant errors while computing the cross product. If no
    // bounding value for the magnitude of the vector is known, then it is a good idea to normalize the vector4
    // 2) Given that vectors in our scenario simply represent a direction, then we can ignore the original magnitude value.
    //
    // How to deal with the maximum expected magnitude will vary a lot depending on the scenario of choice and on the
    // reason why we're computing the cross product.
    glm::tvec3<float> m = glm::cross(normalizeNotZeroVector(vectorAB), normalizeNotZeroVector(vectorCD));
    if (!isZeroVector(m)) {
        /*
         * Vectors (ab) and (cd) are not parallel: m is the potential separating axis. So, the second argument is true.
         */
        return std::make_pair(m, true);
    } else if (doRobust) {
        /*
         * ... unless we do an additional safety check: we need to test if the resulting cross product vector is
         * also a (near) zero vector, and so either we're going to produce another separating axis perpendicular to
         * the two vectors, or by finally determining that no separation can occur.
         *
         */
        glm::tvec3<float> ca = cd.source - ab.source;
        glm::tvec3<float> n = glm::cross(vectorAB, ca);
        m = glm::cross(normalizeNotZeroVector(vectorAB),  normalizeNotZeroVector(n));

        // The computed cross product will be a valid separating axis if and only if the resulting vector is not a zero
        // vector
        return std::make_pair(m, !isZeroVector(m));
    } else {
        return std::make_pair(m, false);
    }
}


std::ostream &operator<<(std::ostream &os, const std::pair<glm::tvec3<float>, bool> &cp) {
    os << "<[" <<  cp.first.x << "," << cp.first.y << "," << cp.first.z  << "], " << (cp.second ? "true>" : "false>");
    return os;
}

std::ostream &operator<<(std::ostream &os, const glm::tvec3<float> &cp) {
    os << '[' <<  cp.x << "," << cp.y << "," << cp.z  << ']';
    return os;
}


void vector_test_intersection() {
    // Getting the smallest number representable as float
    float e = std::numeric_limits<float>::epsilon();

    Vector ab{{1, 2, 3}, {0, 0, 0}};

    // Adding a minimum quantity that will be indistinguishable for non-robust algorithms
    Vector abDelta{{1 + e, 2 - e, 3 + e}, {0, 0, 0}};

    // false is the expected result: there should not be a separating axis (the two vectors are the same, and therefore
    // they are parallel).
    std::cout << SepAxisCrossProd(ab, ab) << std::endl;

    // true is the expected result: there should be a separating axis given that the two vectors are indeed not the same,
    // because they differ only by a small quantity (e)
    std::cout << SepAxisCrossProd(ab, abDelta) << std::endl;

    // false is the expected result: the algorithm is less robust, and then the test might fail.
    std::cout << SepAxisCrossProd(ab, abDelta, false) << std::endl;
}

