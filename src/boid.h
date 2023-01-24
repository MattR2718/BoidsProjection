#ifndef BOID_H
#define BOID_H


#include "drawable.h"
#include "point.h"
#include "vect.h"

class Boid: public Drawable{

public:

    Boid(int x_, int y_, int z_, int width, int height, int r_ = 255, int g_ = 255, int b_ = 255);

    void quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* pointFillColour, float* pointOutlineColour, const bool fill);

private:
    Point point{0, 0, 0, 0, 0};
    Vector dir{0, 0, 0, 0, 0, 0, 0, 0};

protected:


};

#endif