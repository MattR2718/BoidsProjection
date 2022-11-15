#include "../include/point.h"

Point::Point(int x_, int y_, int z_, int rad_, int r_, int g_, int b_) : Drawable{x_, y_, z_, r_, g_, b_}{
    this->setRadius(rad_);
}

void Point::setRadius(int rad_){
    if(rad_ >= 0){ this->rad = rad_; }
    else{ std::cout<<"ERROR POINT RADIUS IS TRYING TO BE SET TO NEGATIVE\n"; }
}

//Plot all 8 points, one in each octant of the circle
void Point::plotCircle(sf::Uint8 *pixels, const int width, int x, int y){
    auto plot = [&](const int j, const int i){
        if(i < 0 || i > width || j < 0 || j > 800){return;}
        pixels[(i * width + j) * 4] = this->r;
        pixels[(i * width + j) * 4 + 1] = this->g;
        pixels[(i * width + j) * 4 + 2] = this->b;
    };
    plot(this->x + x, this->y + y);
    plot(this->x - x, this->y + y);
    plot(this->x + x, this->y - y);
    plot(this->x - x, this->y - y);
    plot(this->x - y, this->y + x);
    plot(this->x + y, this->y - x);
    plot(this->x - y, this->y - x);
    plot(this->x + y, this->y + x);
}

void Point::draw(sf::Uint8 *pixels, const int width){
    int x = 0;
    int y = this->rad;
    int decPar = 3 - 2 * this->rad;
    plotCircle(pixels, width, x, y);
    while( y >= x){
        x++;
        if(decPar > 0){
            y--;
            decPar = decPar + 4 * (x - y) + 10;
        }else{
            decPar = decPar + 4 * x + 6;
        }
        plotCircle(pixels, width, x, y);
    }
}