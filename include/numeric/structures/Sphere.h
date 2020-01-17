//
// Created by giacomo on 10/01/2020.
//

#ifndef TUTORIALS_SPHERE_H
#define TUTORIALS_SPHERE_H


#include <vec3.hpp>
#include "IntervalArithmetic.h"

class Sphere {
    glm::tvec3<float> center;
    float radius;
public:
    Sphere(const glm::tvec3<float> &center, float radius) : center(center), radius(radius) {}
    IntervalArithmetic equation(const glm::tvec3<float>& point);
    IntervalArithmetic equation(const IntervalArithmetic& cx, const IntervalArithmetic& cy, const IntervalArithmetic& cz);
};


#endif //TUTORIALS_SPHERE_H
