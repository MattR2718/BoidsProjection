#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "drawable.h"

class Point: public Drawable{

public:
    //Vaariables


    //Methods
    Point(int x_, int y_, int z_, int rad_ = 20, int r_ = 255, int g_ = 255, int b_ = 255);
    void setRadius(int rad_);
    void draw(sf::Uint8 *pixels, const int width);

private:
    //Variables
    int rad;

    //Methods
    void plotCircle(sf::Uint8 *pixels, const int width, int x, int y);


};



#endif