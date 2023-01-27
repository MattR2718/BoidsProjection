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

void initPixels(sf::Uint8 *arr, const int length){
    for (int i = 0; i < length; i += 4){
        arr[i] = 0;
        arr[i + 1] = 0;
        arr[i + 2] = 0;
        arr[i + 3] = 255;
    }
}

void drawingThreadOriginal(Window& window, Camera& camera, sf::Uint8 *pixels, DrawableData& drawData, DrawVariantVector& drawObjects){
    camera.autoRotate();

    std::ranges::sort(drawObjects, std::greater(), [](auto const& x){
        return std::visit([](auto const& e){ return e.sortVal; }, x);
    });

    int pointCount = 0;
    int boxCount = 0;
    int boidCount = 0;

    //Start by clearing pixels
    initPixels(pixels, window.WIDTH * window.HEIGHT * 4);
    drawData.drawAllObjectsToScreen(drawObjects, pixels, window, camera, pointCount, boxCount, boidCount);

    drawData.populateDrawPoints(drawObjects, pointCount, drawData.numPoints, window.WIDTH, window.HEIGHT);
    drawData.populateDrawBox(drawObjects, boxCount, drawData.numBoxes, window.WIDTH, window.HEIGHT);
    drawData.populateBoids(drawObjects, boidCount, drawData.numBoids, window.WIDTH, window.HEIGHT);

}

void drawingThread(std::stop_token stop_token, Window& window, Camera& camera, sf::Uint8 *pixels, DrawableData& drawData, DrawVariantVector& drawObjects){
    window.window->setActive(true);
    
    while (!stop_token.stop_requested()){
        //window.window->setActive(true);
        //window.pollEvents(camera);
        window.updateImGui();
        
        window.drawImGui(drawData, drawObjects, camera);
            //Start by clearing pixels
            //initPixels(pixels, window.WIDTH * window.HEIGHT * 4);
            
            camera.autoRotate();

            std::ranges::sort(drawObjects, std::greater(), [](auto const& x){
                return std::visit([](auto const& e){ return e.sortVal; }, x);
            });

            int pointCount = 0;
            int boxCount = 0;
            int boidCount = 0;
            
            //drawData.drawAllObjectsToScreen(drawObjects, pixels, window, camera, pointCount, boxCount);

            {
                //smphSignalMainToThread.acquire();
                //std::lock_guard guard(pixelMutex);
                //Start by clearing pixels
                initPixels(pixels, window.WIDTH * window.HEIGHT * 4);
                drawData.drawAllObjectsToScreen(drawObjects, pixels, window, camera, pointCount, boxCount, boidCount);
                //smphSignalThreadToMain.release();
            }

            drawData.populateDrawPoints(drawObjects, pointCount, drawData.numPoints, window.WIDTH, window.HEIGHT);
            drawData.populateDrawBox(drawObjects, boxCount, drawData.numBoxes, window.WIDTH, window.HEIGHT);
            //drawData.populateBoids(drawObjects, boidCount, drawData.numBoids, window.WIDTH, window.HEIGHT)


            window.drawPixelArrayToScreen(pixels);
            window.render();

            //pixelMutex.unlock();
            //smphSignalThreadToMain.release();
            //sem.release();
        //}
        //window.window->setActive(false);
    }
    window.window->close();
}



int main(){

    Window window;

    //Create a pixel array which will contain the pixels drawn to the screen
    sf::Uint8* pixels  = new sf::Uint8[window.WIDTH * window.HEIGHT * 4];
    initPixels(pixels, window.WIDTH * window.HEIGHT * 4);
    sf::Uint8* drawPixels  = new sf::Uint8[window.WIDTH * window.HEIGHT * 4];
    initPixels(drawPixels, window.WIDTH * window.HEIGHT * 4);

    //std::cout<<"INITTED PIXELS\n";

    DrawableData drawData;
    Camera camera;

    //Create vector and fill with objects to test with
    Point O(0, 0, 0, window.WIDTH, window.HEIGHT, 20);
    Point X(200, 0, 0, window.WIDTH, window.HEIGHT,  20, 255, 0, 0);
    Point Y(0, 200, 0, window.WIDTH, window.HEIGHT,  20, 0, 255, 0);
    Point Z(0, 0, 200, window.WIDTH, window.HEIGHT,  20, 0, 0, 255);
    //std::vector<Point> points = { O, X, Y, Z };
    //populatePoints(points, drawData.numPoints, window.WIDTH, window.HEIGHT);

    //std::cout<<"MADE POINTS\n";

    Line l(Point(0, 0, 0, window.WIDTH, window.HEIGHT, 10), Point(282, 282, 282, window.WIDTH, window.HEIGHT, 10), window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 255, 255, 255);
    Line xAxis(X, O, window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 255, 0, 0);
    xAxis.del = false;
    Line yAxis(Y, O, window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 0, 255, 0);
    yAxis.del = false;
    Line zAxis(Z, O, window.WIDTH, window.HEIGHT, drawData.lineAntiAliasing, 0, 0, 255);
    zAxis.del = false;
    std::vector<Line> lines { l, xAxis, yAxis, zAxis };
    
    std::vector<Line> axis{xAxis, yAxis, zAxis};

    //std::cout<<"MADE LINES\n";

    Box boundingBox(0, 0, 0, drawData.boundingBoxSize, window.WIDTH, window.HEIGHT, drawData.boxAntiAliasing, false, 255, 255, 255);
    boundingBox.del = false;

    //std::cout<<"MADE BOUNDING BOX\n";

    DrawVariantVector drawObjects = {O, /* X, Y, Z, */ xAxis, yAxis, zAxis, l, boundingBox};

    //std::cout<<"MADE DRAW OBJECTS\n";

    //std::vector<std::reference_wrapper<Boid>> boids;
    std::vector<Boid> boids;
    for(int i = 0; i < 50; i++){
        /* boids.emplace_back(Boid(
            rand() % (window.WIDTH - 400) - window.WIDTH / 2 + 200,
            rand() % (window.HEIGHT - 400) - window.HEIGHT / 2 + 200,
            rand() % (window.WIDTH - 400) - window.WIDTH / 2 + 200,
            window.WIDTH, window.HEIGHT
        )); */
        boids.emplace_back(Boid(0, 0, 0,
            window.WIDTH, window.HEIGHT
        ));
        boids[boids.size() - 1].del = false;
        //boids.emplace_back(temp);
    }

    //std::cout<<"ADD BOIDS TO VECTOR\n";

    for(int i = 0; i < 100; i++){
        drawObjects.push_back(Vector(
            rand() % (window.WIDTH - 400) - window.WIDTH / 2 + 200,
            rand() % (window.HEIGHT - 400) - window.HEIGHT / 2 + 200,
            rand() % (window.WIDTH - 400) - window.WIDTH / 2 + 200,
            rand() % 50 - 25,
            rand() % 50 - 25,
            rand() % 50 - 25,
            window.WIDTH, window.HEIGHT
        ));
    }
    drawObjects.push_back(Vector(0, 0, 0, 100, 100, 100, window.WIDTH, window.HEIGHT));

    //std::cout<<"ADD VECTORS TO VECTOR\n";

    bool randomise = true;

    std::stop_token drawThreadStopToken;
    //bool stop = false;

    //std::jthread drawThread([&]{drawingThread(drawThreadStopToken, window, camera, drawPixels, drawData, drawObjects);});

    //window.window->setActive(false);

    drawObjects.insert(drawObjects.end(), boids.begin(), boids.end());

    //Run program while window is open
    while (window.running())
    {
        //char c;
        //std::cin>>c;
        drawingThreadOriginal(window, camera, pixels, drawData, drawObjects);

        /* if(!drawData.showDemoObjects){
            drawObjects.clear();
            drawObjects.emplace_back(boundingBox);
            drawObjects.insert(drawObjects.end(), axis.begin(), axis.end());
            drawObjects.insert(drawObjects.end(), boids.begin(), boids.end());
            drawObjects.emplace_back(
                Vector(0, 0, 0, 40, 40, 40, window.WIDTH, window.HEIGHT, false, 1000, false, 255, 0, 0)
            );
             drawObjects.emplace_back(Box(100, 100, 100,
                                        200, window.WIDTH, window.HEIGHT,
                                        drawData.boxAntiAliasing, true, 
                                        255, 255, 255)); 
            //std::get<Vector>(drawObjects[drawObjects.size() - 1]).updateVector(window.WIDTH, window.HEIGHT);
        } */

        if(!drawData.showDemoObjects){
            const auto [first, last] = std::ranges::remove_if(drawObjects, [](auto const& x){
                        return std::visit([](auto const& e){ return e.del; }, x);
                    });
            drawObjects.erase(first, last);
        }

        drawData.updateBoids(drawObjects);

        window.pollEvents(camera);
        window.updateImGui();
        
        window.drawImGui(drawData, drawObjects, camera);

        window.drawPixelArrayToScreen(pixels);
        window.render();
    }

    delete[] pixels;
    
}