#include "point.h"

Point::Point(int x_, int y_, int z_, int width, int height, int rad_, int r_, int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->setRadius(rad_);
    this->outr = this->r;
    this->outg = this->g;
    this->outb = this->b;
}

void Point::setRadius(int rad_){
    if(rad_ >= 0){ this->rad = rad_; }
    else{ std::cout<<"ERROR POINT RADIUS IS TRYING TO BE SET TO NEGATIVE\n"; }
}

void Point::setFill(bool b){
    this->fill = b;
}

void Point::setOutlineColour(int r_, int g_, int b_){
    this->outr = r_;
    this->outg = g_;
    this->outb = b_;
}

//Plot all 8 points, one in each octant of the circle
void Point::plotCircle(sf::Uint8 *pixels, const int width, const int height, int x, int y){
    //Lambda finction to plot a single pixel in the pixel array
    auto plot = [&](const int i, const int j, bool outline = false){
        if(i < -this->offsetx || i >= this->offsetx || j < -this->offsety || j >= this->offsety){return;}
        int r, g, b;
        if(!outline){ r = this->outr; g = this->outg; b = this->outb; }
        else{ r = this->r; g = this->g; b = this->b; }
        pixels[((j + this->offsety) * width + (i + this->offsetx)) * 4] = r;
        pixels[((j + this->offsety) * width + (i + this->offsetx)) * 4 + 1] = g;
        pixels[((j + this->offsety) * width + (i + this->offsetx)) * 4 + 2] = b;
    };
    //Plots points accordint to bresenhams algorithm
    plot(this->px + x, this->py + y);
    plot(this->px - x, this->py + y);
    plot(this->px + x, this->py - y);
    plot(this->px - x, this->py - y);
    plot(this->px - y, this->py + x);
    plot(this->px + y, this->py - x);
    plot(this->px - y, this->py - x);
    plot(this->px + y, this->py + x);

    //Lambda finction to fill the pixel array with a line
    auto plotLine = [&](const int x1, const int x2, const int y){
        int px1 = x1, px2 = x2;
        if(px1 > px2){ px1 = x2; px2 = x1; }
        for(int c = px1 + 1; c < px2; c++){
            plot(c, y, true);
        }
    };

    //If fill is set to true then draws lines across each point to fill in the circle
    if(this->fill){
        plotLine(this->px - y, this->px + y, this->py + x);
        plotLine(this->px - x, this->px + x, this->py + y);
        plotLine(this->px - x, this->px + x, this->py - y);
        plotLine(this->px - y, this->px + y, this->py - x);
    }

}

/* template<typename T, typename U> */
void Point::draw(sf::Uint8 *pixels, const int width, const int height, float tx, float ty, float tz, std::map<std::string, float> trigfunct){
    //this->rotAll(tx, ty, tz, trigfunct);
    //Bresenhams circle drawing algorithm
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

void Point::quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* pointFillColour, float* pointOutlineColour, const bool fill){
    this->setColour(round(pointFillColour[0] * 255), round(pointFillColour[1] * 255), round(pointFillColour[2] * 255));
    this->setOutlineColour(round(pointOutlineColour[0] * 255), round(pointOutlineColour[1] * 255), round(pointOutlineColour[2] * 255));
    this->setFill(fill);
    this->rotAll(tx, ty, tz, trigFunct);
    this->draw(pixels, width, height, tx, ty, tz, trigFunct);
}