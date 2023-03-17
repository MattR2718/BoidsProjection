#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <variant>
#include <vector>
#include <future>
#include <semaphore>
#include <functional>


#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <implot.h>

#include "scrollingBuffer.h"

#include "drawableData.h"
#include "camera.h"
#include "window.h"

#include "point.h"
#include "line.h"
#include "box.h"
#include "vect.h"
#include "boid.h"

#include "sanitisation.h"
#include "validation.h"

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector, Boid>>;

class DrawableData;

class Window{
    public:
        const int WIDTH = sf::VideoMode::getDesktopMode().width * 0.95;
        const int HEIGHT = sf::VideoMode::getDesktopMode().height * 0.95;

        float backgroundColour[3] = {0.0, 0.0, 0.0};

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
        ScrollingBuffer frameData;
        sf::Clock fpsClock;
        float time = 0;
        float plott = 0;
        const int fpsWidgetWidth = sf::VideoMode::getDesktopMode().width * 0.3;
        const int fpsWidgetHeight = sf::VideoMode::getDesktopMode().height * 0.15;
        const int fpsWidgetMaxFPS = 60;
        float imguiWindowFontScale = 2.5;
        bool showFPS = true;
        int frameRate = 30;


    protected:


};



#endif