#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <iostream>
#include <map>
#include <tuple>
#include <SFML/Graphics.hpp>

class Drawable{

public:
    //Variables
    int px, py, pz;
    int r, g, b;
    int sortVal = 100;
    //Methods
    Drawable(int x_, int y_, int z_, int width, int height, int r_ = 255, int g_ = 255, int b_ = 255);
    /* template<typename T, typename U>
    void draw(sf::Uint8 *arr, const int width, T& tx, T& ty, T& tz, std::map<std::string, float> & trigfunct); */
    void draw(sf::Uint8 *pixels, const int width, const int height, float tx, float ty, float tz, std::map<std::string, float> trigfunct);
    void setColour(int r, int g, int b);
    void setPosition(int x_, int y_, int z_);
    std::tuple<int, int, int> getPXYZ();
    std::tuple<int, int, int> getXYZ();

    //template<typename T, typename U>
    //void rotAll(T tx, T ty, T tz, U trigfunct);
    void rotAll(float tx, float ty, float tz, std::map<std::string, float>  trigfunct);
private:

protected:
    //Variables
    int x, y, z;
    /* int px, py, pz; */
    int offsetx, offsety;
    

    //Methods
    template<typename T, typename U>
    void rotX(T tx, T ty, T tz, U trigfunct);
    template<typename T, typename U>
    void rotY(T tx, T ty, T tz, U trigfunct);
    template<typename T, typename U>
    void rotZ(T tx, T ty, T tz, U trigfunct);
    /* template<typename T, typename U> */
    /* void rotAll(float tx, float ty, float tz, std::map<std::string, float>  trigfunct); */

};

#endif