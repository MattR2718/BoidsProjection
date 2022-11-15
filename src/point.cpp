#include "../include/point.h"

Point::Point(int x_, int y_, int z_, int rad_, int r_, int g_, int b_) : Drawable{x_, y_, z_, r_, g_, b_}{
    this->setRadius(rad_);
}

void Point::setRadius(int rad_){
    if(rad_ >= 0){ this->rad = rad_; }
    else{ std::cout<<"ERROR POINT RADIUS IS TRYING TO BE SET TO NEGATIVE\n"; }
}

void Point::setFill(bool b){
    this->fill = b;
}

//Plot all 8 points, one in each octant of the circle
void Point::plotCircle(sf::Uint8 *pixels, const int width, const int height, int x, int y){
    //Lambda finction to plot a single pixel in the pixel array
    auto plot = [&](const int j, const int i){
        if(i < 0 || i > width || j < 0 || j > height){return;}
        pixels[(i * width + j) * 4] = this->r;
        pixels[(i * width + j) * 4 + 1] = this->g;
        pixels[(i * width + j) * 4 + 2] = this->b;
    };
    //Plots points accordint to bresenhams algorithm
    plot(this->x + x, this->y + y);
    plot(this->x - x, this->y + y);
    plot(this->x + x, this->y - y);
    plot(this->x - x, this->y - y);
    plot(this->x - y, this->y + x);
    plot(this->x + y, this->y - x);
    plot(this->x - y, this->y - x);
    plot(this->x + y, this->y + x);

    //Lambda finction to fill the pixel array with a line
    auto plotLine = [&](const int x1, const int x2, const int y){
        int px1 = x1, px2 = x2;
        if(px1 > px2){ px1 = x2; px2 = x1; }
        for(int c = px1; c < px2; c++){
            plot(c, y);
        }
    };

    //If fill is set to true then draws lines across each point to fill in the circle
    if(this->fill){
        plotLine(this->x - y, this->x + y, this->y + x);
        plotLine(this->x - x, this->x + x, this->y + y);
        plotLine(this->x - x, this->x + x, this->y - y);
        plotLine(this->x - y, this->x + y, this->y - x);
    }

}

void Point::draw(sf::Uint8 *pixels, const int width, const int height){
    //Bresenhams line drawing algorithm
    int x = 0;
    int y = this->rad;
    int decPar = 3 - 2 * this->rad;
    plotCircle(pixels, width, height, x, y);
    while( y >= x){
        x++;
        if(decPar > 0){
            y--;
            decPar = decPar + 4 * (x - y) + 10;
        }else{
            decPar = decPar + 4 * x + 6;
        }
        plotCircle(pixels, width, height, x, y);
    }
}