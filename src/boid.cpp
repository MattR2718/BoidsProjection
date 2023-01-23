#include "boid.h"


Boid::Boid(int x_, int y_, int z_, int width, int height, int r_, int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    
    this->point = Point(x_, y_, z_, width, height, 20, r_, g_, b_);

}


void Boid::quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* pointFillColour, float* pointOutlineColour, const bool fill){
    this->point.quickDraw(pixels, width, height, tx, ty, tz, trigFunct, pointFillColour, pointOutlineColour, fill);
}