#include "vect.h"

Vector::Vector(int x_, int y_, int z_, int dx_, int dy_, int dz_, int width, int height, bool antiAliased_, int max_, bool moveVector_, int r_ , int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->antiAliased = antiAliased_;
    this->x = x_;
    this->y = y_;
    this->z = z_;
    this->dx = dx_;
    this->dy = dy_;
    this->dz = dz_;
    this->sdx = dx_;
    this->sdy = dy_;
    this->sdz = dz_;
    this->max = max_;
    this->moveVector = moveVector_;
    //std::cout<<this->r<< ' '<<this->g<<' '<<this->b<<'\n';
    this->setColour(r_, g_, b_);
    Point p1_(this->x, this->y, this->z, width, height, 20, r_, g_, b_);
    this->p1 = p1_;
    this->p1.setColour(this->r, this->g, this->b);
    //std::cout<<this->r<< ' '<<this->g<<' '<<this->b<<'\n';
    Point p2_(this->x + this->dx * 5, this->y + this->dy * 5, this->z + this->dz * 5, width, height, 20, 255, 0, 255);
    this->p2 = p2_;
    Line dir_(this->p1, this->p2, width, height, this->antiAliased, r_, g_, b_);
    this->dir = dir_;
}

//TODO vector colours are not working for the lines of the boids

void Vector::setPos(int x_, int y_, int z_, const int width, const int height){
    this->x = x_;
    this->y = y_;
    this->z = z_;
    this->p1.setPosition(x_, y_, z_);
    this->p2.setPosition(this->x + this->dx * 5, this->y + this->dy * 5, this->z + this->dz * 5);
    Line dir_(this->p1, this->p2, width, height, this->antiAliased);
    this->dir = dir_;
}

void Vector::setDir(int x_, int y_, int z_){
    this->dx = x_;
    this->dy = y_;
    this->dz = z_;
    this->p2.setPosition(this->x + this->dx * 5, this->y + this->dy * 5, this->z + this->dz * 5);
    //Line dir_(this->p1, this->p2, width, height, this->antiAliased);
    //this->dir = dir_;
    this->dir.setPoints(this->p1, this->p2);
}

void Vector::move(const int width, const int height){
    this->x += this->dx;
    this->y += this->dy;
    this->z += this->dz;
    this->updateVector(width, height);
}

void Vector::updateVector(const int width, const int height){
    //Check for max vector size
    auto mag = [&](){
        return (this->sdx * this->sdx + this->sdy * this->sdy + this->sdz * this->sdz);
    };

    //if magnitude greater than max then limit velocity
    auto vecmag = mag();
    if(vecmag > (this->max * this->max) && vecmag != 0){
        //std::cout<<"TOO BIG\n";
        //this->sdx = (this->sdx / vecmag) * this->max;
        //this->sdy = (this->sdy / vecmag) * this->max;
        //this->sdz = (this->sdz / vecmag) * this->max;
        
        this->dx = (this->dx / vecmag) * this->max;
        this->dy = (this->dy / vecmag) * this->max;
        this->dz = (this->dz / vecmag) * this->max;
    }
    
    this->p1.setPosition(this->x, this->y, this->z);
    this->p2.setPosition(this->x + this->dx * 5, this->y + this->dy * 5, this->z + this->dz * 5);
    Line dir_(this->p1, this->p2, width, height, this->antiAliased, this->r, this->g, this->b);
    this->dir = dir_;
}


void Vector::draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const int camDist){
    this->sortVal = this->p1.sortVal;
    this->p1.setColour(this->r, this->g, this->b);
    this->p2.setColour(this->r, this->g, this->b);
    this->dir.setColour(this->r, this->g, this->b);
    //this->p1.draw(pixels, width, height, tx, ty, tz, trigfunct);
    //this->p2.draw(pixels, width, height, tx, ty, tz, trigfunct);
    dir.quickDraw(pixels, width, height, tx, ty, tz, trigfunct, false, camDist);
}

void Vector::add(const Vector& v){
    this->sdx += v.dx;
    this->sdy += v.dy;
    this->sdz += v.dz;
    //this->setDir(this->sdx, this->sdy, this->sdz);
}

Vector operator+ (Vector a, Vector b){
    a.dx += b.dx;
    a.dy += b.dy;
    a.dz += b.dz;
    a.setDir(a.dx, a.dy, a.dz);
    return a;
}

Vector operator* (Vector a, float b){
    a.dx *= b;
    a.dy *= b;
    a.dz *= b;
    a.setDir(a.dx, a.dy, a.dz);
    return a;
}

Vector operator/ (Vector a, int b){
    a.dx /= b;
    a.dy /= b;
    a.dz /= b;
    a.setDir(a.dx, a.dy, a.dz);
    return a;
}

std::ostream& operator<<(std::ostream& os, const Vector& v){
    os<<"dx: "<<v.dx<<"  dy: "<<v.dy<<"  dz: "<<v.dz;
    return os;
}