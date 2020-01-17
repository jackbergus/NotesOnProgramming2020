//
// Created by giacomo on 10/01/2020.
//

#ifndef TUTORIALS_AABB_H
#define TUTORIALS_AABB_H

#include <vec3.hpp>
#include "Sphere.h"

enum tagIntersectionTest {
    liesOutside,
    intersectsBoundary,
    liesFullyInside
};

class AABB {
    glm::tvec3<float> min, max;

public:
    AABB(const glm::tvec3<float> &m, const glm::tvec3<float> &M);
    tagIntersectionTest testWithSphere(Sphere& s);
};


void sphere_AABB_intersection();

#endif //TUTORIALS_AABB_H
