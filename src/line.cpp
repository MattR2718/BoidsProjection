#include "line.h"

Line::Line(Point p1_, Point p2_, int width, int height, bool antiAliased_, int r_, int g_, int b_) : Drawable{0, 0, 0, width, height, r_, g_, b_}{
    this->p1 = p1_;
    this->p2 = p2_;
    this->setColour(p1_.r, p1_.g, p1_.b);
    this->antiAliased = antiAliased_;
}

void Line::setPoints(Point p1_, Point p2_){
    this->p1 = p1_;
    this->p1 = p2_;
}

template <typename T>
int iPart(T x){
    return floor(x);
}

template <typename T>
int roundN(T x){
    return iPart(x + 0.5);
}

template <typename T>
float fPart(T x){
    return x - floor(x);
}

template <typename T>
float rfPart(T x){
    return 1 - fPart(x);
}

//Performs Xiaolin Wu's Line drawing algorithm to draw an anti-aliased line between the two points in line
void Line::XiaolinWu(sf::Uint8 *pixels, const int width, const int height){
    //Lambda function to plot a single point at position (xval, yval) on screen
    auto plot = [&](int xval, int yval, float bright){
        if(xval < -this->offsetx || xval >= this->offsetx || yval < -this->offsety || yval >= this->offsety){return;}
        int index = ((yval + this->offsety) * width + (xval + this->offsetx)) * 4;
        pixels[index] = this->r;
        pixels[index + 1] = this->g;
        pixels[index + 2] = this->b;
        pixels[index + 3] = 255 * bright;
    };
    
    int LENGTH = width * height * 4;
    
    bool steep = std::abs(this->p2.py - this->p1.py) > std::abs(this->p2.px - this->p1.px);

    if(steep){
        int t = this->p1.px;
        this->p1.px = this->p1.py;
        this->p1.py = t;

        t = this->p2.px;
        this->p2.px = this->p2.py;
        this->p2.py = t;
    }

    if(this->p1.px > this->p2.px){
        int t = this->p1.px;
        this->p1.px = this->p2.px;
        this->p2.px = t;

        t = this->p1.py;
        this->p1.py = this->p2.py;
        this->p2.py = t;
    }

    float dx = this->p2.px - this->p1.px;
    float dy = this->p2.py - this->p1.py;

    float gradient = 0;

    if (dx == 0){
        gradient = 1;
    } else{
        gradient = dy/dx;
    }

    float xend = roundN(this->p1.px);
    float yend = this->p1.py + gradient * (xend - this->p1.px);
    float xgap = rfPart(static_cast<float>(this->p1.px) + 0.5);
    float xpxl1 = xend;
    float ypxl1 = iPart(yend);

    if (steep){
        plot(ypxl1, xpxl1, rfPart(yend) * xgap);
        plot(ypxl1 + 1, xpxl1, fPart(yend) * xgap);
    } else{
        plot(xpxl1, ypxl1, rfPart(yend) * xgap);
        plot(xpxl1, ypxl1 + 1, fPart(yend) * xgap);
    }
    float intery = yend + gradient;

    xend = roundN(this->p2.px);
    yend = this->p2.py + gradient * (xend - this->p2.px);
    xgap = fPart(this->p2.px + 0.5);
    float xpxl2 = xend;
    float ypxl2 = iPart(yend);
    if(steep){
        plot(ypxl2, xpxl2, rfPart(yend) * xgap);
        plot(ypxl2 + 1, xpxl2, fPart(yend) * xgap);
    }else{
        plot(xpxl2, ypxl2, rfPart(yend) + xgap);
        plot(xpxl2, ypxl2 + 1, fPart(yend) * xgap);
    }

    if(steep){
        for (int x = xpxl1 + 1; x < xpxl2; x++){
            plot(iPart(intery), x, rfPart(intery));
            plot(iPart(intery) + 1, x, fPart(intery));
            intery += gradient;
        }
    }else{
        for(int x = xpxl1 + 1; x < xpxl2; x++){
            plot(x, iPart(intery), rfPart(intery));
            plot(x, iPart(intery) + 1, fPart(intery));
            intery += gradient;
        }
    }

}

//Plot lines with slopes between 0 and -1 using bresenham
void Line::PlotLineLow(sf::Uint8 *pixels, const int width, const int height, int x0, int y0, int x1, int y1){
    //Lambda function to plot a single point at position (xval, yval) on screen
    auto plot = [&](int xval, int yval, float bright){
        if(xval < -this->offsetx || xval >= this->offsetx || yval < -this->offsety || yval >= this->offsety){return;}
        int index = ((yval + this->offsety) * width + (xval + this->offsetx)) * 4;
        pixels[index] = this->r;
        pixels[index + 1] = this->g;
        pixels[index + 2] = this->b;
        pixels[index + 3] = 255 * bright;
    };

    float dx = x1 - x0;
    float dy = y1 - y0;
    int yi = 1;
    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    float D = (2 * dy) - dx;
    float y = y0;
    int xi = (x0 < x1) ? 1 : -1;
    for(int x = x0; x != x1; x += xi){
        plot(x, y, 1);
        if(D > 0){
            y += yi;
            D += (2 * (dy - dx));
        }else{
            D += 2 * dy;
        }
    }

}

//Plot lines with sloped between 0 and 1
void Line::PlotLineHigh(sf::Uint8 *pixels, const int width, const int height, int x0, int y0, int x1, int y1){
    //Lambda function to plot a single point at position (xval, yval) on screen
    auto plot = [&](int xval, int yval, float bright){
        if(xval < -this->offsetx || xval >= this->offsetx || yval < -this->offsety || yval >= this->offsety){return;}
        int index = ((yval + this->offsety) * width + (xval + this->offsetx)) * 4;
        pixels[index] = this->r;
        pixels[index + 1] = this->g;
        pixels[index + 2] = this->b;
        pixels[index + 3] = 255 * bright;
    };

    float dx = x1 - x0;
    float dy = y1 - y0;
    int xi = 1;
    if(dx < 0){
        xi = -1;
        dx = -dx;
    }
    float D = (2 * dx) - dy;
    float x = x0;
    int yi = (y0 < y1) ? 1 : -1;
    for(int y = y0; y != y1; y += yi){
        plot(x, y, 1);
        if(D > 0){
            x += xi;
            D += (2 * (dx - dy));
        }else{
            D += 2 * dx;
        }
    }
}

//Plot line between two points using bresenhams line drawing algorithm
//Check slope and call correct function
void Line::Bresenham(sf::Uint8 *pixels, const int width, const int height){
    if(abs(this->p2.py - this->p1.py) < abs(this->p2.px - this->p1.px)){
        if(this->p1.px > this->p2.px){
            this->PlotLineLow(pixels, width, height, this->p2.px, this->p2.py, this->p1.px, this->p1.py);
        }else{
            this->PlotLineLow(pixels, width, height, this->p1.px, this->p1.py, this->p2.px, this->p2.py);
        }
    }else{
        if(this->p1.py > this->p2.py){
            this->PlotLineHigh(pixels, width, height, this->p2.px, this->p2.py, this->p1.px, this->p1.py);
        }else{
            this->PlotLineHigh(pixels, width, height, this->p1.px, this->p1.py, this->p2.px, this->p2.py);
        }
    }

}


void Line::draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct){
    //Rotate points at each end of the line
    this->p1.rotAll(tx, ty, tz, trigfunct);
    this->p2.rotAll(tx, ty, tz, trigfunct);

    this->sortVal = (this->p1.sortVal < this->p2.sortVal) ? this->p2.sortVal : this->p1.sortVal ;

    //Draw line connecting two points using projected x and y
    if(this->antiAliased){
        XiaolinWu(pixels, width, height);
    }else{
        Bresenham(pixels, width, height);
    }

    //If boolean is true then draws points at each end of line using point draw function
    if(this->drawPoints){
        this->p1.rotAll(tx, ty, tz, trigfunct);
        this->p2.rotAll(tx, ty, tz, trigfunct);
        this->p1.draw(pixels, width, height, tx, ty, tz, trigfunct);
        this->p2.draw(pixels, width, height, tx, ty, tz, trigfunct);
    }
}

void Line::quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const bool drawLinePoints){
    this->drawPoints = drawLinePoints;
    this->draw(pixels, width, height, tx, ty, tz, trigfunct);
}