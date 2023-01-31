#include "boid.h"


Boid::Boid(int x_, int y_, int z_, int width, int height, int r_, int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->point = Point(x_, y_, z_, width, height, 10, r_, g_, b_);
    this->dir = Vector(x_, y_, z_, 
                    rand() % 20 - 10,
                    rand() % 20 - 10,
                    rand() % 20 - 10,
                    width, height, false, 100, true, r_, g_, b_);
    this->cohesion = this->dir;
    this->cohesion.setColour(255, 0, 0);
    this->separation = this->dir;
    this->separation.setColour(0, 255, 0);
    this->alignment = this->dir;
    this->alignment.setColour(0, 0, 255);

    this->del = false;
}


void Boid::quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* boidFillColour, float* boidOutlineColour, const bool fill, bool drawCohesion, bool drawAlignment, bool drawSeparation, bool drawDirection){
    this->dir.setColour(boidFillColour[0] * 255, boidFillColour[1] * 255, boidFillColour[2] * 255);
    //this->dir.setPos(this->x, this->y, this->z, width, height);
    this->dir.move(width, height);

    if(drawDirection){
        this->dir.draw(pixels, width, height, tx, ty, tz, trigFunct);
    }

    auto[dirx, diry, dirz]{this->dir.getXYZ()};
    this->point.setPosition(dirx, diry, dirz);
    
    //Draw behaviour vectors
    if(drawCohesion){
        this->cohesion.setPos(dirx, diry, dirz, width, height);
        this->cohesion.draw(pixels, width, height, tx, ty, tz, trigFunct);
    }
    if(drawSeparation){
        this->separation.setPos(dirx, diry, dirz, width, height);
        this->separation.draw(pixels, width, height, tx, ty, tz, trigFunct);
    }
    if(drawAlignment){
        this->alignment.setPos(dirx, diry, dirz, width, height);
        this->alignment.draw(pixels, width, height, tx, ty, tz, trigFunct);
    }

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

/* void Boid::setPosition(int x_, int y_, int z_){
    //this->setPosition(x_, y_, z_);
    //this->point.setPosition(this->x, this->y, this->z);
    //this->dir.setPosition(this->x, this->y, this->z);
} */

void Boid::resetPos(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->dir.setPosition(0, 0, 0);
    this->point.setPosition(0, 0, 0);
}

void Boid::behaviours(DrawVariantVector& drawObjects, const int& width, const int& height){

    auto dist = [&](auto& b){
        auto[tx, ty, tz]{this->getXYZ()};
        auto[bx, by, bz]{b.getXYZ()};
        return ((tx - bx) * (tx - bx) + (ty - by) * (ty - by) + (tz - bz) * (tz - bz));
    };

    auto angle = [&](auto& b){        
        auto[tx, ty, tz]{this->getXYZ()};
        auto[bx, by, bz]{b.getXYZ()};
        return std::acos((tx * bx + ty * by + tz * bz)/(std::sqrt(tx*tx + ty*ty + tz*tz) * std::sqrt(bx*bx + by*by + bz*bz)));
    };

    int numNeighbours = 0;
    this->cohesion.setDir(0, 0, 0);
    this->alignment.setDir(0, 0, 0);
    this->separation.setDir(0, 0, 0);
    /* for(auto& obj : drawObjects){
        if(obj.index() == 5){ //Boid
            auto& b = std::get<Boid>(obj);
            //Check is boid is local
            if(dist(b) <= this->maxDistSqrd && angle(b) <= this->fov){
                numNeighbours++;
                //Cohesion


            }
        }
    } */
    std::cout<<numNeighbours<<'\n';
}