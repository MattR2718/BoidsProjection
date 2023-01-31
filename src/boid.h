#ifndef BOID_H
#define BOID_H
#include <iostream>
#include <vector>
#include <variant>
#include <cmath>

#include "drawable.h"
#include "point.h"
#include "line.h"
#include "box.h"
#include "vect.h"
#include "boid.h"

class Boid;

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector, Boid>>;

class Boid: public Drawable{

public:

    Boid(int x_, int y_, int z_, int width, int height, int r_ = 255, int g_ = 255, int b_ = 255);

    void quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* boidFillColour, float* boidOutlineColour, const bool fill, bool drawCohesion, bool drawAlignment, bool drawSeparation, bool drawDirection);

    void boundCheck(const int boundingBoxSize, bool wrapAround);
    void updateSpeedMult(const float boidSpeedMult);
    void setRadius(const int rad);
    //void setPosition(int x_, int y_, int z_);
    void resetPos();
    void behaviours(DrawVariantVector& drawObjects, const float& cohesionMult, const float& alignmentMult, const float& separationMult);
private:
    Point point{0, 0, 0, 0, 0};
    Vector dir{0, 0, 0, 0, 0, 0, 0, 0};
    
    Vector separation{0, 0, 0, 0, 0, 0, 0, 0};
    Vector cohesion{0, 0, 0, 0, 0, 0, 0, 0};
    Vector alignment{0, 0, 0, 0, 0, 0, 0, 0};

    //Angle is the distance from the normal around the boid
    //An angle of 180 degrees is a full circle as it is 180 degrees either direction
    float fov = 180;
    float maxDist = 100;
    float maxDistSqrd = maxDist * maxDist;

protected:


};

#endif