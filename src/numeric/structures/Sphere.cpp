//
// Created by giacomo on 10/01/2020.
//

#include "numeric/structures/Sphere.h"

IntervalArithmetic Sphere::equation(const glm::tvec3<float> &point) {
    IntervalArithmetic x{point.x};
    IntervalArithmetic y{point.y};
    IntervalArithmetic z{point.z};

    return equation(x, y, z);
}

IntervalArithmetic
Sphere::equation(const IntervalArithmetic &x, const IntervalArithmetic &y, const IntervalArithmetic &z) {
    IntervalArithmetic cx{center.x};
    IntervalArithmetic cy{center.y};
    IntervalArithmetic cz{center.z};
    IntervalArithmetic r{radius};

    return (x - cx).pow(2) + (y - cy).pow(2) + (z -cz).pow(2) - r.pow(2);
}
