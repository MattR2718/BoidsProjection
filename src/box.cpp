
#include "box.h"

Box::Box(int x_, int y_, int z_, int size_, int width, int height, bool antiAliased_, bool fixedSize_, int r_, int g_, int b_) : Drawable{x_, y_, z_, width, height, r_, g_, b_}{
    this->antiAliased = antiAliased_;
    this->size = size_;
    this->centre.setPosition(x_, y_, z_);
    this->sortVal = y_;
    this->fixedSize = fixedSize_;
    this->setColour(r_, g_, b_);
}

std::vector<std::tuple<int, int, int>> Box::generateVerticies(){
    std::vector<std::tuple<int, int, int>> verticies;
    int hSize = this->size / 2;
    for(int i = 0; i < 8; i++){
        verticies.push_back({
            ((i & 4) != 0 ? hSize : -hSize) + this->x,
            ((i & 2) != 0 ? hSize : -hSize) + this->y,
            ((i & 1) != 0 ? hSize : -hSize) + this->z
        });
    }
    return verticies;
}

void Box::generateLines(const int width, const int height){
    std::vector<std::tuple<int, int, int>> verticies = this->generateVerticies();

    auto addLine = [&](int a, int b){
        return Line(
                    Point(std::get<0>(verticies[a]), std::get<1>(verticies[a]), std::get<2>(verticies[a]), width, height, this->size * 0.05, this->r, this->g, this->b),
                    Point(std::get<0>(verticies[b]), std::get<1>(verticies[b]), std::get<2>(verticies[b]), width, height, this->size * 0.05),
                    width, height, this->antiAliased, this->r, this->g, this->b
                );
    };

    lines.push_back(addLine(0, 1));
    lines.push_back(addLine(1, 3));
    lines.push_back(addLine(3, 2));
    lines.push_back(addLine(0, 2));
    
    lines.push_back(addLine(4, 5));
    lines.push_back(addLine(5, 7));
    lines.push_back(addLine(7, 6));
    lines.push_back(addLine(4, 6));

    lines.push_back(addLine(0, 4));
    lines.push_back(addLine(1, 5));
    lines.push_back(addLine(2, 6));
    lines.push_back(addLine(3, 7));
}

void Box::draw(sf::Uint8 *pixels, const int width, const int height, const float tx, const float ty, const float tz, const std::map<std::string, float>& trigfunct, const bool drawLinePoints, const int boxSize, const int camDist){
    this->setSize(boxSize);

    this->centre.rotAll(tx, ty, tz, trigfunct, camDist);
    
    this->sortVal = this->centre.sortVal;
    this->lines.clear();
    this->generateLines(width, height);
    for(auto& l : this->lines){
        l.quickDraw(pixels, width, height, tx, ty, tz, trigfunct, drawLinePoints, camDist);
    }
}


void Box::setSize(int s){
    if(!fixedSize){
        this->size = s;
    }
}

bool Box::atOrigin(){
    return this->x == 0 && this->y == 0 && this->y == 0;
}