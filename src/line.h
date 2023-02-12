#ifndef LINE_H
#define LINE_H

#include <iostream>
#include<SFML/Graphics.hpp>

#include "drawable.h"
#include "point.h"

class Line : public Drawable{

public:

    bool drawPoints = false;
    bool antiAliased = false;

    Line(Point p1_, Point p2_, int width, int height, bool antiAliased_ = false, int r_ = 0, int g_ = 255, int b_ = 255);
    void draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const int camDist);
    void quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const bool drawLinePoints, const int camDist);
    void setPoints(Point p1_, Point p2_);
    std::tuple<int, int, int, int, int, int> getPointPXYZs();

private:

    Point p1{0, 0, 0, 0, 0};
    Point p2{0, 0, 0, 0, 0};

    void XiaolinWu(sf::Uint8 *pixels, const int width, const int height);
    void PlotLineLow(sf::Uint8 *pixels, const int width, const int height, int x0, int y0, int x1, int y1);
    void PlotLineHigh(sf::Uint8 *pixels, const int width, const int height, int x0, int y0, int x1, int y1);
    void Bresenham(sf::Uint8 *pixels, const int width, const int height);

protected:

};

#endif