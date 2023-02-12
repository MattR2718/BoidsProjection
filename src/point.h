#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <map>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "drawable.h"

class Point: public Drawable{

public:
    //Variables
    int fill = true;

    //Methods
    Point(int x_, int y_, int z_, int width, int height, int rad_ = 20, int r_ = 255, int g_ = 255, int b_ = 255);
    void setRadius(int rad_);
    void setFill(bool b);
    void setOutlineColour(int r_, int g_, int b_);
    void draw(sf::Uint8 *pixels, const int width, const int height, float tx, float ty, float tz, std::map<std::string, float> trigfunct, const int camDist);

    void quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* pointFillColour, float* pointOutlineColour, const bool fill, const int camDist);

private:
    //Variables
    int rad;
    int outr, outg, outb;

    //Methods
    void plotCircle(sf::Uint8 *pixels, const int width, const int height, int x, int y);


};



#endif