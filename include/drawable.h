#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Drawable{

public:
    //Variables

    //Methods
    Drawable(int x_, int y_, int z_, int r_ = 255, int g_ = 255, int b_ = 255);
    void draw(sf::Uint8 *arr, const int width);
    void setColour(int r, int g, int b);
    void setPosition(int x_, int y_, int z_);

private:

protected:
    //Variables
    int x, y, z;
    int r, g, b;

    //Methods

};

#endif