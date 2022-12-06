#include "camera.h"

Camera::Camera(){
    this->setTrigValues();
}

template <typename T>
float Camera::degToRad(T angle){
    return float(angle) * PI / 180.f;
}

void Camera::setTrigValues(){

    this->trigFunctions.at("sx") = std::sin(this->degToRad(this->tx));
    this->trigFunctions.at("sy") = std::sin(this->degToRad(this->ty));
    this->trigFunctions.at("sz") = std::sin(this->degToRad(this->tz));

    this->trigFunctions.at("cx") = std::cos(this->degToRad(this->tx));
    this->trigFunctions.at("cy") = std::cos(this->degToRad(this->ty));
    this->trigFunctions.at("cz") = std::cos(this->degToRad(this->tz));
}

void Camera::autoRotate(){
    if(this->autoRotatex){
        this->tx += 1;
    }
    if(this->autoRotatey){
        this->ty += 1;
    }
    if(this->autoRotatez){
        this->tz += 1;
    }
    this->setTrigValues();
}