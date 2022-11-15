#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "drawable.h"

class Point: public Drawable{

public:
    //Variables
    int fill = true;

    //Methods
    Point(int x_, int y_, int z_, int rad_ = 20, int r_ = 255, int g_ = 255, int b_ = 255);
    void setRadius(int rad_);
    void setFill(bool b);
    void draw(sf::Uint8 *pixels, const int width, const int height);

private:
    //Variables
    int rad;

    //Methods
    void plotCircle(sf::Uint8 *pixels, const int width, const int height, int x, int y);


};



#endif