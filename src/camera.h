#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <map>
#include <string>
#include <cmath>

#define PI 3.14159

class Camera{
    public:
        //Floats to store the rotation angle of the cameras
        float tx = 0, ty = 30, tz = 0;
        //Default rotating each axis
        bool autoRotatex = false, autoRotatey = true, autoRotatez = false;
        //Map to store values for trig finctions
        /* std::map<std::string, float> trigFunctions = {{"sx", std::sin(this->degToRad(tx))},
                                                        {"sy", std::sin(this->degToRad(ty))},
                                                        {"sz", std::sin(this->degToRad(tz))},
                                                        {"cx", std::cos(this->degToRad(tx))},
                                                        {"cy", std::cos(this->degToRad(ty))},
                                                        {"cz", std::cos(this->degToRad(tz))}}; */
        std::map<std::string, float> trigFunctions = {{"sx", std::sin(tx* PI / 180.f)},
                                                        {"sy", std::sin(ty * PI / 180.f)},
                                                        {"sz", std::sin(tz * PI / 180.f)},
                                                        {"cx", std::cos(tx * PI / 180.f)},
                                                        {"cy", std::cos(ty * PI / 180.f)},
                                                        {"cz", std::cos(tz * PI / 180.f)}};
        void setTrigValues();
        void print()/* {std::cout<<this->tx<<'\n';} */;

    private:
        /* template <typename T>
        float degToRad(T angle); */
        float degToRad(float angle);

    protected:

};

#endif