#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <map>
#include <string>
#include <cmath>

#define PI 3.14159

class Camera{
    public:

        Camera();
    
        //Floats to store the rotation angle of the cameras
        float tx = 0, ty = 30, tz = 0;
        //Default rotating each axis
        bool autoRotatex = false, autoRotatey = true, autoRotatez = false;
        //Map to store values for trig finctions
        std::map<std::string, float> trigFunctions = {{"sx", 0.0f},
                                                        {"sy", 0.0f},
                                                        {"sz", 0.0f},
                                                        {"cx", 0.0f},
                                                        {"cy", 0.0f},
                                                        {"cz", 0.0f}};
        void setTrigValues();
        void autoRotate();

    private:
        template <typename T>
        float degToRad(T angle);

    protected:

};

#endif