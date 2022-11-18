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
    int px, py, pz;
    int r, g, b;

    //Methods
    template<typename T, typename U>
    void rotX(T tx, T ty, T tz, U trigfunct);
    template<typename T, typename U>
    void rotY(T tx, T ty, T tz, U trigfunct);
    template<typename T, typename U>
    void rotZ(T tx, T ty, T tz, U trigfunct);
    template<typename T, typename U>
    void rotAll(T tx, T ty, T tz, U trigfunct);

};

#endif