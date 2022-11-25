#ifndef BOX_H
#define BOX_H

#include <iostream>
#include <vector>
#include <tuple>
#include<SFML/Graphics.hpp>

#include "drawable.h"
#include "point.h"
#include "line.h"

class Box : public Drawable{

public:
    Box(int x_, int y_, int z_, int size_, int width, int height, int r_ = 255, int g_ = 255, int b_ = 255);
    std::vector<std::tuple<int, int, int>> generateVerticies();
    void draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const bool drawLinePoints, const int boxSize);
    void setSize(int s);

private:
    int size;
    Point centre{0, 0, 0, 0, 0, 0};

    std::vector<Line> lines;

    void generateLines(const int width, const int height);


protected:



};


#endif