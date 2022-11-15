#include "../include/drawable.h"

Drawable::Drawable(int x_, int y_, int z_, int r_, int g_, int b_){
    this->x = x_;
    this->y = y_;
    this->z = z_;
    this->r = r_;
    this->g = g_;
    this->b = b_;
}

void Drawable::draw(sf::Uint8 *arr, const int width){
    int index = (this->y * width + this->x) * 4;
    arr[index] = this->r;
    arr[index + 1] = this->g;
    arr[index + 2] = this->b;
}

void Drawable::setColour(int r_, int g_, int b_){
    this->r = r_;
    this->g = g_;
    this->b = b_;
    //std::cout<<this->r<<' '<<this->g<<' '<<this->b<<'\n';
}

void Drawable::setPosition(int x_, int y_, int z_){
    this->x = x_;
    this->y = y_;
    this->z = z_;
}