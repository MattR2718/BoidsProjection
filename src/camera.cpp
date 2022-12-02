#include "camera.h"

/* template <typename T>
float Camera::degToRad(T angle){
    return float(angle) * PI / 180.f;
} */

float Camera::degToRad(float angle){
    return float(angle) * PI / 180.f;
}

void Camera::setTrigValues(){

    this->trigFunctions.at("sx") = std::sin(this->degToRad(this->tx));
    this->trigFunctions.at("sy") = std::sin(this->degToRad(this->ty));
    this->trigFunctions.at("sz") = std::sin(this->degToRad(this->tz));

    this->trigFunctions.at("cx") = std::cos(this->degToRad(this->tx));
    this->trigFunctions.at("cy") = std::cos(this->degToRad(this->ty));
    this->trigFunctions.at("cz") = std::cos(this->degToRad(this->tz));

    /* this->trigFunctions = {{"sx", std::sin(tx* PI / 180.f)},
                                                        {"sy", std::sin(this->ty * PI / 180.f)},
                                                        {"sz", std::sin(this->tz * PI / 180.f)},
                                                        {"cx", std::cos(this->tx * PI / 180.f)},
                                                        {"cy", std::cos(this->ty * PI / 180.f)},
                                                        {"cz", std::cos(this->tz * PI / 180.f)}}; */
}

void Camera::print(){
    std::cout<<this->tx<<'\n';
}