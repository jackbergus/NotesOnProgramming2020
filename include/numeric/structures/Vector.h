//
// Created by giacomo on 10/01/2020.
//

#ifndef TUTORIALS_VECTOR_H
#define TUTORIALS_VECTOR_H

#include <glm.hpp>
#include <vec3.hpp>
#include <utility>
#include <ostream>

struct Vector {
    bool hasPrecomputedResult;
    /**
     * The distinct representation from source and destination will be required from the
     */
    glm::tvec3<float> source;
    glm::tvec3<float> destination;
    glm::tvec3<float> casted_difference;

    Vector(const Vector& rhs) = default;
    Vector& operator=(const Vector& rhs);

    /**
     * Defining a vector as the difference from point y to x.
     *
     * @param x         Source Point
     * @param y         Destination point
     */
    Vector(const glm::tvec3<float> &x, const glm::tvec3<float> &y);

    /**
     * Defining a vector as a direction.
     *
     * I.e., the destination point is exactly the direction vector, and the source point is the origin of the axes.
     *
     * @param direction         Vector as a direction
     */
    Vector(const glm::tvec3<float> &direction);


    /**
     * Defining a vector as a component-wise direction.
     *
     * @param x
     * @param y
     * @param z
     */
    Vector(float x, float y, float z) : Vector{{x, y, z}} {};

    /**
     * Default constructor: zero vector.
     */
    Vector();



    /**
     * Returning the vector as the difference between two points. (Implicit casting)
     *
     * @return  (destination point)-(source point)
     */
    explicit operator glm::tvec3<float>();

    /**
     * Explicit casting into a vector as the difference between two points
     * @return
     */
    glm::tvec3<float> asVector();

    /**
     * Returning the b-th component of the vector of its "actual" representation (as a point difference)
     * @param b     Vector position, counting from 0
     * @return      Value of the  b-th component (if 0<=b<3).
     */
    float operator[](int b);
};

/**
 * Approximates the fact that the vector is a zero vector with the fact that each component is less than epsilon
 */
#define isZeroVector(v)         (glm::all(glm::lessThan(glm::abs(v), glm::vec3(std::numeric_limits<float>::epsilon()))))

/**
 * This function normalizes the vector if and only if the provided vector is not a zero vector.
 *
 * @param x         Vector to be normalized
 * @return          Normalized vector
 */
glm::tvec3<float> normalizeNotZeroVector(const glm::tvec3<float>& x);

/**
 * Checks whether two vectors are parallel or not. If they are parallel, they have an intersection axis (second argument true)
 * that is provided by the first argument. Otherwise, there is no separating axis.
 *
 * @param ab                First vector
 * @param cd                Second vector
 * @param doCrossNormalize  Whether we need to normalize the cross product in the case that the vectors have a large magnitude
 * @return          Returns a pair, where the second element returns whether the two vectors have a separating axis.
 *                  If that element exists, then it is returned in the first argument of the pair. Otherwise, the
 *                  first argument will be an arbitrary vector
 */
std::pair<glm::tvec3<float>, bool> SepAxisCrossProd(Vector &ab, Vector &cd, bool doRobust = true);

std::ostream &operator<<(std::ostream &os, const glm::tvec3<float> &cp);


/**
 * Printing the result of the moreRobustLineTest function.
 *
 * @param os        Output stream
 * @param cp        Result
 * @return          Updated output stream
 */
std::ostream &operator<<(std::ostream &os, const std::pair<glm::tvec3<float>, bool>& cp);

/**
 * Test for the current section
 */
void vector_test_intersection();

#endif //TUTORIALS_VECTOR_H
