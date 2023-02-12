#include "boid.h"


Boid::Boid(int x_, int y_, int z_, int dx, int dy, int dz, int width, int height, int r_, int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->point = Point(x_, y_, z_, width, height, 3, r_, g_, b_);
        
    
    this->dir = Vector(x_, y_, z_, 
                    dx, dy, dz,
                    width, height, false, 20, true, r_, g_, b_);
    this->cohesion = this->dir;
    this->cohesion.setColour(255, 0, 0);
    this->separation = this->dir;
    this->separation.setColour(0, 255, 0);
    this->alignment = this->dir;
    this->alignment.setColour(0, 0, 255);

    this->del = false;
}


void Boid::quickDraw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigFunct, float* boidFillColour, float* boidOutlineColour, const bool fill, bool drawCohesion, bool drawAlignment, bool drawSeparation, bool drawDirection, const int camDist){
    this->dir.setColour(boidFillColour[0] * 255, boidFillColour[1] * 255, boidFillColour[2] * 255);
    //this->dir.setPos(this->x, this->y, this->z, width, height);
    this->dir.move(width, height);

    if(drawDirection){
        this->dir.draw(pixels, width, height, tx, ty, tz, trigFunct, camDist);
    }

    auto[dirx, diry, dirz]{this->dir.getXYZ()};
    this->point.setPosition(dirx, diry, dirz);
    
    //Draw behaviour vectors
    if(drawCohesion){
        this->cohesion.setPos(dirx, diry, dirz, width, height);
        this->cohesion.draw(pixels, width, height, tx, ty, tz, trigFunct, camDist);
    }
    if(drawSeparation){
        this->separation.setPos(dirx, diry, dirz, width, height);
        this->separation.draw(pixels, width, height, tx, ty, tz, trigFunct, camDist);
    }
    if(drawAlignment){
        this->alignment.setPos(dirx, diry, dirz, width, height);
        this->alignment.draw(pixels, width, height, tx, ty, tz, trigFunct, camDist);
    }

    this->point.quickDraw(pixels, width, height, tx, ty, tz, trigFunct, boidFillColour, boidOutlineColour, fill, camDist);
    this->sortVal = this->point.sortVal;
}

void Boid::boundCheck(const int boundingBoxSize, bool wrapAround){
    int max = boundingBoxSize / 2;
    auto[x, y, z]{this->point.getXYZ()};
    this->setPosition(x, y, z);
    
    auto bound = [](int& pos, const int& max, int& dir){
        if(pos >= max){
            pos = max - 10;
            dir *= -1;
        }else if(pos <= -max){
            pos = 20 - max;
            dir *= -1;
        }
    };

    auto wrap = [](int& pos, const int& max){
        //if(std::abs(pos) >= max){ pos = -pos; }
        if(pos >= max){ pos = 20 - max; }
        else if(pos <= -max){ pos = max - 20; }
    };

    if(!wrapAround){
        bound(this->x, max, this->dir.sdx);
        bound(this->y, max, this->dir.sdy);
        bound(this->z, max, this->dir.sdz);
    }else{
        wrap(this->x, max);
        wrap(this->y, max);
        wrap(this->z, max);
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

void Boid::behaviours(DrawVariantVector& drawObjects, const float& cohesionMult, const float& alignmentMult, const float& separationMult, const int& WIDTH, const int& HEIGHT){

    //Lambda function to calculate distance beterrn this boid and a given boid
    //Returns distance squared
    auto dist = [&](auto& b){
        auto[tx, ty, tz]{this->getXYZ()};
        auto[bx, by, bz]{b.getXYZ()};
        return ((tx - bx) * (tx - bx) + (ty - by) * (ty - by) + (tz - bz) * (tz - bz));
    };

    //Lambda function to calculate the angle between this boid and another boid
    auto angle = [&](auto& b){        
        auto[tx, ty, tz]{this->getXYZ()};
        auto[bx, by, bz]{b.getXYZ()};
        return std::acos((tx * bx + ty * by + tz * bz)/(std::sqrt(tx*tx + ty*ty + tz*tz) * std::sqrt(bx*bx + by*by + bz*bz)));
    };

    auto[tx, ty, tz]{this->getXYZ()};
    int numNeighbours = 1;
    this->cohesion.setDir(0, 0, 0);
    this->alignment.setDir(0, 0, 0);
    this->separation.setDir(0, 0, 0);
    //Loop over all drawable objects and check if they are boids
    //if so, calculate the respective behaviours
    for(auto& obj : drawObjects){
        if(obj.index() == 5){ //Boid
            auto& b = std::get<Boid>(obj);
            //Check is boid is local
            if(dist(b) <= this->maxDistSqrd && angle(b) <= this->fov){
                numNeighbours++;
                //Cohesion
                cohesion.dx += b.x;
                cohesion.dy += b.y;
                cohesion.dz += b.z;

                //Separation
                separation.dx += (tx - b.x);
                separation.dy += (ty - b.y);
                separation.dz += (tz - b.z);

                //Alignment
                alignment.dx += b.dir.dx;
                alignment.dy += b.dir.dy;
                alignment.dz += b.dir.dz;

            }
        }
    }
    //Calculate values for cohesion
    cohesion = cohesion / numNeighbours;
    cohesion.dx -= tx;
    cohesion.dy -= ty;
    cohesion.dz -= tz;
    cohesion = cohesion * cohesionMult;
    
    //Add cohesion to direction vector>
    this->dir.sdx = this->dir.dx + cohesion.dx;
    this->dir.sdy = this->dir.dy + cohesion.dy;
    this->dir.sdz = this->dir.dz + cohesion.dz;


    //Calculate vector for separation
    separation = separation * separationMult;

    //Add separation to direction vector
    this->dir.sdx = this->dir.sdx + separation.dx;
    this->dir.sdy = this->dir.sdy + separation.dy;
    this->dir.sdz = this->dir.sdz + separation.dz;


    //Calculate values for alignment
    alignment = alignment / numNeighbours;
    alignment = alignment * alignmentMult;

    //Add alignment to direction vector
    this->dir.sdx = this->dir.sdx + alignment.dx;
    this->dir.sdy = this->dir.sdy + alignment.dy;
    this->dir.sdz = this->dir.sdz + alignment.dz;



    this->dir.sdx = this->dir.sdx + this->dir.dx;
    this->dir.sdy = this->dir.sdy + this->dir.dy;
    this->dir.sdz = this->dir.sdz + this->dir.dz;
    this->dir.sdx = this->dir.sdx + this->dir.dx;
    this->dir.sdy = this->dir.sdy + this->dir.dy;
    this->dir.sdz = this->dir.sdz + this->dir.dz;


    /* std::cout<<"--------------------------------\n";
    std::cout<<"Direction: "<<this->dir<<'\n';
    std::cout<<"Cohesion: "<<this->cohesion<<'\n';
    std::cout<<"Alignment: "<<this->alignment<<'\n';
    std::cout<<"Separation: "<<this->separation<<'\n';
    std::cout<<"--------------------------------\n"; */


}