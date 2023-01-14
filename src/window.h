#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <variant>
#include <vector>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "drawableData.h"
#include "camera.h"
#include "window.h"

// #include "drawable.h"
#include "point.h"
#include "line.h"
#include "box.h"
#include "vect.h"

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector>>;
class DrawableData;

class Window{
    public:
        int WIDTH = 1200;
        int HEIGHT = 1000;

        sf::RenderWindow* window;
        sf::Clock deltaClock;

        //Create variable to store fps, clock to calculate fps and times to store change in time
        float fps;
        sf::Clock fps_clock = sf::Clock();
        sf::Time previous_time = fps_clock.getElapsedTime();
        sf::Time current_time;
        //Load font from file and throw exeption if not found
        sf::Font font;

        Window();
        virtual ~Window();
        const bool running() const;

        void pollEvents(Camera& camera);
        void updateImGui();
        void drawImGui(DrawableData& drawData, DrawVariantVector& drawObjects, Camera& camera);

        void drawPixelArrayToScreen(sf::Uint8 *pixels);
        void drawFPS();
        void render();


    private:


    protected:


};



#endif