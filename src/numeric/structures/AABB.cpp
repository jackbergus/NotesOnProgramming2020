//
// Created by giacomo on 10/01/2020.
//

#include <iostream>
#include "numeric/structures/AABB.h"

AABB::AABB(const glm::tvec3<float> &m, const glm::tvec3<float> &M) : min(m), max(M) {}

tagIntersectionTest AABB::testWithSphere(Sphere &s) {
    const IntervalArithmetic &result = s.equation({min.x, max.x}, {min.y, max.y}, {min.z, max.z});
    if (result.x > 0)
        return liesOutside;
    else if ((result.x <= 0) && (result.y <= 0)) {
        return  intersectsBoundary;
    } else if (result.y > 0)
        return  liesFullyInside;
    throw std::runtime_error("ERROR: no case was matched");
}

void sphere_AABB_intersection() {
    Sphere S{{0,0,0}, 2};
    AABB   B{{2,0,0}, {2,2,2}};
    switch (B.testWithSphere(S)) {
        case liesOutside:
            std::cout << "liesOutside" << std::endl;
            break;
        case intersectsBoundary:
            std::cout << "intersectsBoundary" << std::endl;
            break;
        case liesFullyInside:
            std::cout << "liesFullyInside" << std::endl;
            break;
    }
}
