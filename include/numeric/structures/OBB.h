//
// Created by giacomo on 13/01/2020.
//

#ifndef TUTORIALS_OBB_H
#define TUTORIALS_OBB_H

#include <ostream>
#include <functional>
#include <vector>

// glm
#include <vec3.hpp>

// Vector
#include <numeric/structures/Vector.h>




struct OBB {
    Vector center_point;            // OBB Center point
    Vector local_axes[3];         // Local x-, y-, and z- axes
    Vector halfwidth_sizes;            // Positive halfwidth extends of OBB along each axis

    /**
     * Setting up the OOB sitting at the origin, with the same axes as the cartesian ones, and specific half distances
     *
     * @param halfX
     * @param halfY
     * @param halfZ
     */
    OBB(float halfX, float halfY, float halfZ);

    /**
     * Setting up the OBB sitting at a given center, with the same axes as the cartesian ones, and specific half distances
     * @param c
     * @param e
     */
    OBB(const glm::tvec3<float>& c,  const glm::tvec3<float>&e);

    /**
     * Setting up a custom OBB
     * @param c
     * @param x
     * @param e
     */
    OBB(const glm::tvec3<float>& c,  const std::vector<Vector>& x, const glm::tvec3<float>&e);
};

/**
 * Using the naif overlap. This solution implements the mathematical constraint definition as it is, with no numerical
 * optimization (besides the cross product one)
 *
 * @param a             First OBB
 * @param b             Second OBB
 * @param doRobust      Whether we want to perform the robust cross product or not
 * @return              Returns whether the two OBBs overlap or not
 */
bool NaifOverlap(OBB& a, OBB& b, bool doRobust = true);


/**
 * More robust overlap, where the cross products for the OBB overlap test are completely removed
 *
 * @param a             First OBB
 * @param b             Second OBB
 * @param epsilon       The tolerance that we want to achieve for the
 * @return              Returns whether the two OBBs overlap or not
 */
int RobustOBBOverlap(OBB& a, OBB& b, const float epsilon = 1e-6);


/**
 * Printing a vector 3
 * @param os
 * @param classe
 * @return
 */
std::ostream &operator<<(std::ostream &os, glm::tvec3<float> &classe);


void obb_overlap_testing();


#endif //TUTORIALS_OBB_H
