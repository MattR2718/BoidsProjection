#include "vect.h"

Vector::Vector(int x_, int y_, int z_, int dx_, int dy_, int dz_, int width, int height, int max_, int r_ , int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->dx = dx_;
    this->dy = dy_;
    this->dz = dz_;
    this->max = max_;
}

void Vector::draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct){
    Point p1(this->x, this->y, this->z, width, height, 0);
    Point p2(this->x + this->dx * 5, this->y + this->dy * 5, this->z + this->dz * 5, width, height, 0);
    Line dir(p1, p2, width, height);
    dir.quickDraw(pixels, width, height, tx, ty, tz, trigfunct, false);
}