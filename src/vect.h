#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "drawable.h"
#include "point.h"
#include "line.h"

class Vector : public Drawable{

public:
    int max;
    int dx, dy, dz;
    Vector(int x_, int y_, int z_, int dx_, int dy_, int dz_, int width, int height, int max_ = 10, int r_ = 255, int g_ = 255, int b_ = 255);
    
    void setPos(int x_, int y_, int z_, const int width, const int height);
    void setDir(int x_, int y_, int z_, const int width, const int height);
    
    void draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct);

private:
    Point p1{0, 0, 0, 0, 0};
    Point p2{0, 0, 0, 0, 0};
    Line dir{p1, p2, 0, 0};

    
protected:


};

#endif