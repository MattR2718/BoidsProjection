#ifndef LINE_H
#define LINE_H

#include <iostream>
#include<SFML/Graphics.hpp>

#include "drawable.h"
#include "point.h"

class Line : public Drawable{

public:

    bool drawPoints = false;

    Line(Point p1_, Point p2_, int width, int height, int r_ = 255, int g_ = 255, int b_ = 255);
    void draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct);
    void quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const bool drawLinePoints);
    void setPoints(Point p1_, Point p2_);

private:

    Point p1{0, 0, 0, 0, 0};
    Point p2{0, 0, 0, 0, 0};

    void drawLine(sf::Uint8 *pixels, const int width, const int height);


protected:


};

#endif