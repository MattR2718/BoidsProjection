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
    int sdx, sdy, sdz;

    bool antiAliased = false;
    bool moveVector = false;

    Vector(int x_, int y_, int z_, int dx_, int dy_, int dz_, int width, int height, bool antiAliased_ = false, int max_ = 10, bool moveVector_ = true, int r_ = 255, int g_ = 255, int b_ = 255);
    
    void setPos(int x_, int y_, int z_, const int width, const int height);
    void setDir(int x_, int y_, int z_);
    void move(const int width, const int height);
    void draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct);
    void add(const Vector& v);


private:
    Point p1{0, 0, 0, 0, 0};
    Point p2{0, 0, 0, 0, 0};
    Line dir{p1, p2, 0, 0};

    void updateVector(const int width, const int height);
    
protected:


friend Vector operator+ (Vector a, Vector b);
};


#endif