#include "../include/drawable.h"

Drawable::Drawable(int x_, int y_, int z_, int width, int height, int r_, int g_, int b_){
    this->x = x_;
    this->y = -y_; /* Y is negative to make towards the top of the screen positive rather than negative*/
    this->z = z_;
    this->px = x_;
    this->py = y_;
    this->pz = z_;
    this->r = r_;
    this->g = g_;
    this->b = b_;
    this->offsetx = width / 2;
    this->offsety = height / 2;
}

template<typename T, typename U>
void Drawable::draw(sf::Uint8 *arr, const int width, T& tx, T& ty, T& tz, std::map<std::string, float> & trigfunct){
    this->rotAll(tx, ty, tz, trigfunct)
    int index = (this->py * width + this->px) * 4;
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

template<typename T, typename U>
void Drawable::rotX(T tx, T ty, T tz, U trigfunct){
    //x * cosy * cosz - y * cosy * sinz + z * siny
    this->px = this->x * trigfunct.at("cy") * trigfunct.at("cz") - this->y * trigfunct.at("cy")* trigfunct.at("sz") + this->z * trigfunct.at("sy");
}

template<typename T, typename U>
void Drawable::rotY(T tx, T ty, T tz, U trigfunct){
    //x * (sinx * siny * cosx + cosx * sinz) - y * (sinx * siny * sinz - cosx * cosz) - z * sinx * cosy
    this->py = this->x * (trigfunct.at("sx") * trigfunct.at("sy") * trigfunct.at("cx") + trigfunct.at("cx") * trigfunct.at("sx")) - this->y * (trigfunct.at("sx") * trigfunct.at("sy") * trigfunct.at("sz") - trigfunct.at("cx") * trigfunct.at("cz")) - this->z * trigfunct.at("sx") * trigfunct.at("cy");
}

template<typename T, typename U>
void Drawable::rotZ(T tx, T ty, T tz, U trigfunct){
    //x * (sinx * siny - cosx * cosz * siny) + y * (cosx * siny * sinz + sinx * cosz) + z * cosx * cosy
    this->pz = this->x * (trigfunct.at("sx") * trigfunct.at("sy") - trigfunct.at("cx") * trigfunct.at("cz") * trigfunct.at("sy")) + this->y * (trigfunct.at("cx") * trigfunct.at("sy") * trigfunct.at("sz") + trigfunct.at("sx") * trigfunct.at("cx")) + this->z * trigfunct.at("cx") * trigfunct.at("cy");
}

/* template<typename T, typename U> */
void Drawable::rotAll(float tx, float ty, float tz, std::map<std::string, float>  trigfunct){
    this->rotX(tx, ty, tz, trigfunct);
    this->rotY(tx, ty, tz, trigfunct);
    this->rotZ(tx, ty, tz, trigfunct);
}