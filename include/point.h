#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "drawable.h"

class Point: public Drawable{

public:
    //Vaariables


    //Methods
    Point(int x_, int y_, int z_, int rad_, int r_ = 255, int g_ = 255, int b_ = 255);
    void draw(sf::Uint8 *arr, const int width);

private:
    //Variables
    int rad;

    //Methods



};



#endif