#include "boid.h"


Boid::Boid(int x_, int y_, int z_, int width, int height, int r_, int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->point = Point(x_, y_, z_, width, height, 10, r_, g_, b_);
    this->dir = Vector(x_, y_, z_, 
                    rand() % 20 - 10,
                    rand() % 20 - 10,
                    rand() % 20 - 10,
                    width, height, false, 100, true, r_, g_, b_);
    this->del = false;
}


void Boid::quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* boidFillColour, float* boidOutlineColour, const bool fill){
    this->dir.setColour(boidFillColour[0] * 255, boidFillColour[1] * 255, boidFillColour[2] * 255);
    //this->dir.setPos(this->x, this->y, this->z, width, height);
    this->dir.move(width, height);
    this->dir.draw(pixels, width, height, tx, ty, tz, trigFunct);
    auto[dirx, diry, dirz]{this->dir.getXYZ()};
    this->point.setPosition(dirx, diry, dirz);
    this->point.quickDraw(pixels, width, height, tx, ty, tz, trigFunct, boidFillColour, boidOutlineColour, fill);
    this->sortVal = this->point.sortVal;
}

void Boid::boundCheck(const int boundingBoxSize){
    int max = boundingBoxSize / 2;
    auto[x, y, z]{this->point.getXYZ()};
    this->setPosition(x, y, z);
    if(this->x >= max){
        this->x = max - 1;
        this->dir.sdx *= -1;
    }else if(this->x <= -max){
        this->x = 1 - max;
        this->dir.sdx *= -1;
    }

    if(this->y >= max){
        this->y = max - 1;
        this->dir.sdy *= -1;
    }else if(this->y <= -max){
        this->y = 1 - max;
        this->dir.sdy *= -1;
    }

    if(this->z >= max){
        this->z = max - 1;
        this->dir.sdz *= -1;
    }else if(this->z <= -max){
        this->z = 1 - max;
        this->dir.sdz *= -1;
    }
    this->point.setPosition(this->x, this->y, this->z);
    this->dir.setPosition(this->x, this->y, this->z);
}

void Boid::updateSpeedMult(const float boidSpeedMult){
    this->dir.dx = this->dir.sdx * boidSpeedMult;
    this->dir.dy = this->dir.sdy * boidSpeedMult;
    this->dir.dz = this->dir.sdz * boidSpeedMult;
}

void Boid::setRadius(const int rad){
    this->point.setRadius(rad);
}
