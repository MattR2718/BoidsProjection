#include "../include/point.h"

Point::Point(int x_, int y_, int z_, int rad_, int r_, int g_, int b_) : Drawable{x_, y_, z_, r_, g_, b_}{
    this->rad = rad_;
}

void Point::draw(sf::Uint8 *arr, const int width){
    for(int i = -5; i < 6; i++){
        for(int j = -5; j < 6; j++){
            int index = ((this->y + j) * width + (this->x + i));

            if(index > -1 && index < 800){
                arr[index] = this->r;
                arr[index + 1] = this->g;
                arr[index + 2] = this->b;
            }
        }
    }
}