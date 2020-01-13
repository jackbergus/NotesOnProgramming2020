//
// Created by giacomo on 13/01/2020.
//

#include <iostream>
#include "numeric/structures/OBB.h"

OBB::OBB(float halfX, float halfY, float halfZ) : center_point{0, 0, 0}, halfwidth_sizes{halfX, halfY, halfZ} {
    local_axes[0] = {1, 0, 0}; // Using the default copy constructor
    local_axes[1] = {0, 1, 0};
    local_axes[2] = {0, 0, 1};
}

OBB::OBB(const glm::tvec3<float> &c, const glm::tvec3<float> &e) : center_point{c}, halfwidth_sizes{e} {
    local_axes[0] = {1, 0, 0}; // Using the default copy constructor
    local_axes[1] = {0, 1, 0};
    local_axes[2] = {0, 0, 1};
}

OBB::OBB(const glm::tvec3<float> &c, const std::vector<Vector> &x, const glm::tvec3<float> &e) : center_point{c}, halfwidth_sizes{e} {
    local_axes[0] = x[0];
    local_axes[1] = x[1];
    local_axes[2] = x[2];
}

/**
 * Checks whether the OBBs provided as parameters overlap over a given axis
 *
 * @param a  First OBB
 * @param b  Second OBB
 * @param n  Axis over which we want to test the overlap
 * @return   Whether the overlap happens or not
 */
static inline bool chunkNotOverlapTest(OBB& a, OBB& b, const glm::tvec3<float>& n) {
    auto tDiff = a.center_point.asVector() - b.center_point.asVector();
#ifdef DEBUG
    std::cout << n << "\t\t: ";
    std::cout << std::fabs(glm::dot(tDiff, n)) << " > " <<  (a.e[0] * std::fabs(glm::dot(a.uM[0].asVector(), n)) + a.e[1] * std::fabs(glm::dot(a.uM[1].asVector(), n)) + a.e[2] * std::fabs(glm::dot(a.uM[2].asVector(), n)))
    << " + "<< (b.e[0] * std::fabs(glm::dot(b.uM[0].asVector(), n)) + b.e[1] * std::fabs(glm::dot(b.uM[1].asVector(), n)) + b.e[2] * std::fabs(glm::dot(b.uM[2].asVector(), n))) << std::endl;
#endif
    return std::fabs(glm::dot(tDiff, n)) > (a.halfwidth_sizes[0] * std::fabs(glm::dot(a.local_axes[0].asVector(), n)) + a.halfwidth_sizes[1] * std::fabs(glm::dot(a.local_axes[1].asVector(), n)) + a.halfwidth_sizes[2] * std::fabs(glm::dot(a.local_axes[2].asVector(), n)))
                                           + (b.halfwidth_sizes[0] * std::fabs(glm::dot(b.local_axes[0].asVector(), n)) + b.halfwidth_sizes[1] * std::fabs(glm::dot(b.local_axes[1].asVector(), n)) + b.halfwidth_sizes[2] * std::fabs(glm::dot(b.local_axes[2].asVector(), n)));
}

bool NaifOverlap(OBB &a, OBB &b, bool doRobust) {
    // Do return the first time that I see a separation: if the elements overlap, they shall overlap along all the axes
    if (chunkNotOverlapTest(a, b, a.local_axes[0].asVector())) return false;
    if (chunkNotOverlapTest(a, b, a.local_axes[1].asVector())) return false;
    if (chunkNotOverlapTest(a, b, a.local_axes[2].asVector())) return false;

    if (chunkNotOverlapTest(a, b, b.local_axes[0].asVector())) return false;
    if (chunkNotOverlapTest(a, b, b.local_axes[1].asVector())) return false;
    if (chunkNotOverlapTest(a, b, b.local_axes[2].asVector())) return false;

    if (!doRobust) {
        // Using the naive version, with no robust cross product computation
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[0].asVector(), b.local_axes[0].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[0].asVector(), b.local_axes[1].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[0].asVector(), b.local_axes[2].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[1].asVector(), b.local_axes[0].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[1].asVector(), b.local_axes[1].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[1].asVector(), b.local_axes[2].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[2].asVector(), b.local_axes[0].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[2].asVector(), b.local_axes[1].asVector()))) return false;
        if (chunkNotOverlapTest(a, b, glm::cross(a.local_axes[2].asVector(), b.local_axes[2].asVector()))) return false;
    } else {
        // Using the cross product enhanced for this specific problem, i.e. for computing a separating axis
        std::function<bool(OBB&, OBB&, int, int)> testLambda = [doRobust](OBB& a, OBB& b, int i, int j) {
            std::pair<glm::tvec3<float>, bool> result;
            // If there is a separating axis
            if ((result = moreRobustCrossProductForSeparatingAxis(a.local_axes[i], b.local_axes[j], doRobust)).second) {
                // Check whether the elements do overlap or not
                return (chunkNotOverlapTest(a, b, result.first));
            } else {
                // There is no separating axis because the cross product provides a nearly-zero vector: continue
                //std::cout << "Skipped" << std::endl;
                return false;
            }
        };

        if (testLambda(a, b, 0, 0)) return false;
        if (testLambda(a, b, 0, 1)) return false;
        if (testLambda(a, b, 0, 2)) return false;
        if (testLambda(a, b, 1, 0)) return false;
        if (testLambda(a, b, 1, 1)) return false;
        if (testLambda(a, b, 1, 2)) return false;
        if (testLambda(a, b, 2, 0)) return false;
        if (testLambda(a, b, 2, 1)) return false;
        if (testLambda(a, b, 2, 2)) return false;
    }

    // If there is no separating axis, then the elements do overlap
    return true;
}

int RobustOBBOverlap(OBB &a, OBB &b, const float epsilon) {
    float ra, rb;
    glm::tmat3x3<float> R, AbsR;
    glm::tvec3<float> t;                        // Translation vector

    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            float tmp = glm::dot(a.local_axes[i].asVector(), b.local_axes[j].asVector());
            R[i][j] = tmp;
            AbsR[i][j] = std::abs(tmp) + epsilon;
        }
    }

    // Computing the translation from b to a
    t = b.center_point.asVector() - a.center_point.asVector();
    // Bringing translation into a's coordinate frame
    t.x = glm::dot(t, a.local_axes[0].asVector());
    t.y = glm::dot(t, a.local_axes[1].asVector());
    t.z = glm::dot(t, a.local_axes[2].asVector());


    for (int i = 0; i<3; i++) {
        ra = a.halfwidth_sizes[i];
        rb = b.halfwidth_sizes[0] * AbsR[i][0] + b.halfwidth_sizes[1] * AbsR[i][1] + b.halfwidth_sizes[2] * AbsR[i][2];
#ifdef DEBUG
        std::cout << std::fabs(t[i]) << " > " << ra << " + " <<  rb<< std::endl;
#endif
        if (std::fabs(t[i]) > ra + rb) return 0;

        ra = a.halfwidth_sizes[0] * AbsR[0][i] + a.halfwidth_sizes[1] * AbsR[1][i] + a.halfwidth_sizes[2] * AbsR[2][i];
        rb = b.halfwidth_sizes[i];
#ifdef DEBUG
        std::cout << std::fabs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) << " > " << ra << " + " <<  rb<< std::endl;
#endif
        if (std::fabs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
    }


    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            ra = a.halfwidth_sizes[(i + 1) % 3] * AbsR[(i + 2) % 3][j] + a.halfwidth_sizes[(i + 2) % 3] * AbsR[(i + 1) % 3][j];
            rb = b.halfwidth_sizes[(j + 1) % 3] * AbsR[i][(j + 2) % 3] + b.halfwidth_sizes[(j + 2) % 3] * AbsR[i][(j + 1) % 3];
#ifdef DEBUG
            std::cout << (std::fabs(t[(i+2)%3] * R[(i+1)%3][j] - t[(i+1)%3] * R[(i+2)%3][j])) << " > " << ra << " + " <<  rb<< std::endl;
#endif
            if (std::fabs(t[(i+2)%3] * R[(i+1)%3][j] - t[(i+1)%3] * R[(i+2)%3][j]) > ra + rb) return 0;
        }
    }

    return 1;
}

std::ostream &operator<<(std::ostream &os, glm::tvec3<float> &classe) {
    return os << "(" << classe.x << "," << classe.y << "," << classe.z << ")";
}

void obb_overlap_testing() {

    // Getting the smallest number representable as float
    float e = std::numeric_limits<float>::epsilon();
    // A small change number to add some noise to the data
    float small_change = 0.000001;

    std::cout << "Testing OBB-s as AABB-s" << std::endl;
    // These tests are quite naive, and treat the OBBs as AABBs: the local axes are not rotated
    {
        // OOB with the same axes as the orthonormal basis, with half distance of 2 and center at the origin of the axes
        OBB a{2, 2, 2};
        // OOB with the same axes as the orthonormal basis, with half distance of 2 and centered in (10,10,10)
        OBB b{{10, 10, 10}, {2, 2, 2}};

        assert(NaifOverlap(a, a)); // Self containment == 1
        assert(NaifOverlap(b, b)); // Self containment == 1
        assert(!NaifOverlap(a, b)); // box b is centered in (10,10,10) and has a radius of 2, so it cannot overlap with b == 0
    }

    {
        // OOB with the same axes as the orthonormal basis, with half distance of 2 and center at the origin of the axes
        OBB a{2, 2, 2};
        // OOB with the same axes as the orthonormal basis, with half distance of 1 and center at the origin of the axes
        OBB b{1, 1, 1};

        assert(NaifOverlap(a, a)); // Self containment == 1
        assert(NaifOverlap(b, b)); // Self containment == 1
        assert(NaifOverlap(a, b)); // a contains b, because a is bigger than b == 1
    }

    // Making two OOB boxes overlap with only a small amount of change
    {
        // OOB with the same axes as the orthonormal basis, with half distance of 1 and center at the origin of the axes
        OBB a{1, 1, 1};
        // OOB with the same axes as the orthonormal basis, with half distance of 1 and center at (2,2,2) minus a small
        // distance. As a consequence, all the components shall overlap
        OBB b{{2 - small_change, 2 - small_change, 2 - small_change}, {1, 1, 1}};

        assert(NaifOverlap(a, a));        // Self containment == 1
        assert(NaifOverlap(b, b));        // Self containment == 1
        assert(NaifOverlap(a, b));        // Partial overlap == 1
        assert(NaifOverlap(a, b, false)); // Partial overlap == 1
        assert(RobustOBBOverlap(a, b));     // If we use an epsilon which is bigger than e, 1e-6, then the test won't fail as expected.
        assert(RobustOBBOverlap(a, b, e));  // Using a smaller epsilon for the test
    }

    // Adding a smaller quantity, on the other hand, might cause some problems for testOBBOverlap for bigger error thresholds
    {
        float small_change = 0.000001;
        OBB a{1, 1, 1};
        OBB b{{2 + small_change, 2 + small_change, 2 + small_change}, {1, 1, 1}};

        assert(NaifOverlap(a, a));        // Self containment == 1
        assert(NaifOverlap(b, b));        // Self containment == 1
        assert(!NaifOverlap(a, b));        // Partial overlap == 1
        assert(!NaifOverlap(a, b, false)); // Partial overlap == 1
        //assert(!testOBBOverlap(a, b));     // If we use an epsilon which is bigger than e, 1e-6, then the test fails
        assert(!RobustOBBOverlap(a, b, e));  // Using a smaller epsilon for the test will make it pass
    }


    std::cout << "Testing OBB-s" << std::endl;
    // Adding now a slight rotation to the axes: these two boxes shall overlap with all the algorithms
    {
        std::vector<Vector> vectorByCols;
        vectorByCols.emplace_back(-0.0641566f, 0, -0.99794f);
        vectorByCols.emplace_back(0.0f, 1.0f, 0.0f);
        vectorByCols.emplace_back(-0.99794f, 0, 0.0641566f);

        OBB a{3.53553f, 1.76777f, 0.0f};
        OBB b{{-0.147256f, 1.76777f, 1.80947f}, vectorByCols, {2.33155f, 0.565685f, 0.56452f}};

        assert(RobustOBBOverlap(a, b));
        assert(NaifOverlap(a, b));
        assert(NaifOverlap(a, b, false));
    }

    // Now, rotating the axes by a smaller quantity: the non-separating axes should now overlap
    {
        std::vector<Vector> vectorByCols;
        vectorByCols.emplace_back(1+e,-e,e);
        vectorByCols.emplace_back(e,1-e,e);
        vectorByCols.emplace_back(e,-e,1+e);

        OBB a{1, 1, 1};
        OBB b{{2 + e, 2 + e, 2 + e}, vectorByCols, {1, 1, 1}};

        assert(NaifOverlap(a, a));        // Self containment == 1
        assert(NaifOverlap(b, b));        // Self containment == 1

        //Now, the naif algorithm fails in all the scenarios, even though we're trying to use the robust cross product
        //implementation. As a consequence, it is better to always use more numerical stable algorithms for ad hoc
        //solution instead of making each single component of a non-robust algorithm robust...
        //assert(!NaifOverlap(a, b));
        //assert(!NaifOverlap(a, b, false));

        // Test passed for the robust version
        assert(RobustOBBOverlap(a, b, e));  // Using a smaller epsilon for the test will make it pass
    }
}
