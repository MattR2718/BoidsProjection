#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <cmath>
#include <variant>
#include <ranges>
#include <thread>
#include <chrono>
#include <future>
#include <semaphore>
#include <mutex>
#include <functional>
#include <random>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "drawableData.h"
#include "camera.h"
#include "window.h"

#include "drawable.h"
#include "point.h"
#include "line.h"
#include "box.h"
#include "vect.h"
#include "boid.h"

#define PI 3.14159

std::binary_semaphore
    smphSignalMainToThread{1},
    smphSignalThreadToMain{0};

std::binary_semaphore sem{1};

std::mutex pixelMutex;

bool runThread = true;

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector, Boid>>;

void initPixels(sf::Uint8 *arr, const int length, float* colour){
    for (int i = 0; i < length; i += 4){
        arr[i] = colour[0] * 255;
        arr[i + 1] = colour[1] * 255;
        arr[i + 2] = colour[2] * 255;
        arr[i + 3] = 255;
    }
}

void drawObjectsToPixels(Window& window, Camera& camera, sf::Uint8 *pixels, DrawableData& drawData, DrawVariantVector& drawObjects){
    //Rotate camera if any auto rotate is set
    camera.autoRotate();

    //sort drawables based on sortval
    std::ranges::sort(drawObjects, std::greater(), [](auto const& x){
        return std::visit([](auto const& e){ return e.sortVal; }, x);
    });

    //variables to store number of objects to keep correct number on screen
    int pointCount = 0;
    int boxCount = 0;
    int boidCount = 0;

    //Start by clearing pixels
    initPixels(pixels, window.WIDTH * window.HEIGHT * 4, window.backgroundColour);

    drawData.drawAllObjectsToScreen(drawObjects, pixels, window, camera, pointCount, boxCount, boidCount);

    //populate drawobjects with the correct number of each drawable
    drawData.populateDrawPoints(drawObjects, pointCount, drawData.numPoints, window.WIDTH, window.HEIGHT);
    drawData.populateDrawBox(drawObjects, boxCount, drawData.numBoxes, window.WIDTH, window.HEIGHT);
    drawData.populateBoids(drawObjects, boidCount, drawData.numBoids, window.WIDTH, window.HEIGHT);

}


int main(){

    Window window;
    DrawableData drawData;
    Camera camera;

    //Create a pixel array which will contain the pixels drawn to the screen
    sf::Uint8* pixels  = new sf::Uint8[window.WIDTH * window.HEIGHT * 4];
    initPixels(pixels, window.WIDTH * window.HEIGHT * 4, window.backgroundColour);
    sf::Uint8* drawPixels  = new sf::Uint8[window.WIDTH * window.HEIGHT * 4];
    initPixels(drawPixels, window.WIDTH * window.HEIGHT * 4, window.backgroundColour);

    //Create vector and fill with objects to test with
    Point O(0, 0, 0, window.WIDTH, window.HEIGHT, 20);
    Point X(200, 0, 0, window.WIDTH, window.HEIGHT,  20, 255, 0, 0);
    Point Y(0, 200, 0, window.WIDTH, window.HEIGHT,  20, 0, 255, 0);
    Point Z(0, 0, 200, window.WIDTH, window.HEIGHT,  20, 0, 0, 255);

    //Create lines for all 3 axis
    Line l(Point(0, 0, 0, window.WIDTH, window.HEIGHT, 10), Point(282, 282, 282, window.WIDTH, window.HEIGHT, 10), window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 255, 255, 255);
    Line xAxis(X, O, window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 255, 0, 0);
    xAxis.del = false;
    Line yAxis(Y, O, window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 0, 255, 0);
    yAxis.del = false;
    Line zAxis(Z, O, window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 0, 0, 255);
    zAxis.del = false;
    std::vector<Line> lines { l, xAxis, yAxis, zAxis };
    
    std::vector<Line> axis{xAxis, yAxis, zAxis};

    //Create bounding box for boids to move around in
    Box boundingBox(0, 0, 0, drawData.boundingBoxSize, window.WIDTH, window.HEIGHT, drawData.boxAntiAliasing, false, 255, 255, 255);
    boundingBox.del = false;

    //Create vector to store all opbjects that will be drawn to screen
    DrawVariantVector drawObjects = {O, /* X, Y, Z, */ xAxis, yAxis, zAxis, l, boundingBox};

    //create random generator device
    std::random_device dev;
    //create a seed for the random number generator based on time
    std::mt19937::result_type seed = dev() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );
    //seed the random number generator
    std::mt19937 gen(seed);
    //create a distribution of random numbers
    std::uniform_int_distribution<std::mt19937::result_type> rngposdist(-(drawData.boundingBoxSize - 20)/2, (drawData.boundingBoxSize - 20)/2);
    std::uniform_int_distribution<std::mt19937::result_type> rngdirdist(-10, 10);


    //Create all boids
    std::vector<Boid> boids;
    for(int i = 0; i < 50; i++){
#if 1
        boids.emplace_back(Boid(
            rngposdist(gen),
            rngposdist(gen),
            rngposdist(gen),
            rngdirdist(gen),
            rngdirdist(gen),
            rngdirdist(gen),
            window.WIDTH, window.HEIGHT
        ));
#else
        boids.emplace_back(Boid(0, 0, 0,
            window.WIDTH, window.HEIGHT
        ));
#endif
        boids[boids.size() - 1].del = false;
        //boids.emplace_back(temp);
    }

    bool randomise = true;

    std::stop_token drawThreadStopToken;

    drawObjects.insert(drawObjects.end(), boids.begin(), boids.end());

    //Run program while window is open
    while (window.running())
    {
        drawObjectsToPixels(window, camera, pixels, drawData, drawObjects);

        if(!drawData.showDemoObjects){
            const auto [first, last] = std::ranges::remove_if(drawObjects, [](auto const& x){
                        return std::visit([](auto const& e){ return e.del; }, x);
                    });
            drawObjects.erase(first, last);
        }

        drawData.updateBoids(drawObjects, window);

        window.pollEvents(camera);
        window.updateImGui();
        
        window.drawImGui(drawData, drawObjects, camera);

        window.drawPixelArrayToScreen(pixels);
        window.render();
    }

    delete[] pixels;
    
}