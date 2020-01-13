//
// Created by giacomo on 10/01/2020.
//

#ifndef TUTORIALS_AABB_H
#define TUTORIALS_AABB_H

#include <vec3.hpp>
#include "Sphere.h"

#ifndef GENERATE_ENUM_STRINGS
#define DECL_ENUM_ELEMENT( element ) element
#define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME {
#define END_ENUM( ENUM_NAME ) } ENUM_NAME; \
            char* GetString##ENUM_NAME(enum tag##ENUM_NAME index);
#else
#define DECL_ENUM_ELEMENT( element ) #element
    #define BEGIN_ENUM( ENUM_NAME ) char* gs_##ENUM_NAME [] =
    #define END_ENUM( ENUM_NAME ) ; char* GetString##ENUM_NAME(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }
#endif

BEGIN_ENUM(IntersectionTest)
    DECL_ENUM_ELEMENT(liesOutside),
    DECL_ENUM_ELEMENT(intersectsBoundary),
    DECL_ENUM_ELEMENT(liesFullyInside)
END_ENUM(IntersectionTest)

class AABB {
    glm::tvec3<float> p1, p2;

public:
    AABB(const glm::tvec3<float> &p1, const glm::tvec3<float> &p2);
    tagIntersectionTest testWithSphere(Sphere& s);
};


void sphere_AABB_intersection();

#endif //TUTORIALS_AABB_H
