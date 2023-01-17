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

#define PI 3.14159

std::binary_semaphore
    smphSignalMainToThread{1},
    smphSignalThreadToMain{0};

std::binary_semaphore sem{1};

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector>>;

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

    drawData.drawAllObjectsToScreen(drawObjects, pixels, window, camera, pointCount, boxCount);

    drawData.populateDrawPoints(drawObjects, pointCount, drawData.numPoints, window.WIDTH, window.HEIGHT);
    drawData.populateDrawBox(drawObjects, boxCount, drawData.numBoxes, window.WIDTH, window.HEIGHT);

}

void drawingThread(std::stop_token stop_token, Window& window, Camera& camera, sf::Uint8 *pixels, DrawableData& drawData, DrawVariantVector& drawObjects){
    while (!stop_token.stop_requested()){
        smphSignalMainToThread.acquire();
        //sem.acquire();
        
        //Start by clearing pixels
        initPixels(pixels, window.WIDTH * window.HEIGHT * 4);
        
        camera.autoRotate();

        std::ranges::sort(drawObjects, std::greater(), [](auto const& x){
            return std::visit([](auto const& e){ return e.sortVal; }, x);
        });

        int pointCount = 0;
        int boxCount = 0;

        drawData.drawAllObjectsToScreen(drawObjects, pixels, window, camera, pointCount, boxCount);

        drawData.populateDrawPoints(drawObjects, pointCount, drawData.numPoints, window.WIDTH, window.HEIGHT);
        drawData.populateDrawBox(drawObjects, boxCount, drawData.numBoxes, window.WIDTH, window.HEIGHT);
        smphSignalThreadToMain.release();
        //sem.release();
    }
}

int main(){

    Window window;

    //Create a pixel array which will contain the pixels drawn to the screen
    sf::Uint8* pixels  = new sf::Uint8[window.WIDTH * window.HEIGHT * 4];
    initPixels(pixels, window.WIDTH * window.HEIGHT * 4);

    DrawableData drawData;
    Camera camera;

    //Create vector and fill with objects to test with
    Point O(0, 0, 0, window.WIDTH, window.HEIGHT, 20);
    Point X(200, 0, 0, window.WIDTH, window.HEIGHT,  20, 255, 0, 0);
    Point Y(0, 200, 0, window.WIDTH, window.HEIGHT,  20, 0, 255, 0);
    Point Z(0, 0, 200, window.WIDTH, window.HEIGHT,  20, 0, 0, 255);
    //std::vector<Point> points = { O, X, Y, Z };
    //populatePoints(points, drawData.numPoints, window.WIDTH, window.HEIGHT);

    Line l(Point(0, 0, 0, window.WIDTH, window.HEIGHT, 10), Point(200, 200, 200, window.WIDTH, window.HEIGHT, 10), window.WIDTH, window.HEIGHT);
    Line xAxis(X, O, window.WIDTH, window.HEIGHT);
    Line yAxis(Y, O, window.WIDTH, window.HEIGHT);
    Line zAxis(Z, O, window.WIDTH, window.HEIGHT);
    std::vector<Line> lines { l, xAxis, yAxis, zAxis };

    Box b(0, 0, 0, drawData.boxSize, window.WIDTH, window.HEIGHT);

    DrawVariantVector drawObjects = {O, /* X, Y, Z, */ xAxis, yAxis, zAxis, l, b};

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

    bool randomise = true;

    std::stop_token drawThreadStopToken;

    std::jthread drawThread([&]{drawingThread(drawThreadStopToken, window, camera, pixels, drawData, drawObjects);});

    //Run program while window is open
    while (window.running())
    {
        //smphSignalMainToThread.release();
        window.pollEvents(camera);
        window.updateImGui();
        
        window.drawImGui(drawData, drawObjects, camera);

        bool threadRunning = true;
        //drawingThreadOriginal(window, camera, pixels, drawData, drawObjects);
        //std::jthread drawThread{drawingThread, window, camera, pixels, drawData, drawObjects};
        //TODO When changing number of boxes and points after making thread, doesnt add boxes until press randomise
        
        //drawThread = std::jthread{[&]{drawingThread(window, camera, pixels, drawData, drawObjects);}};
        //std::jthread testThread{[]{}};
        //drawThread.join();
        //testThread.join();

        //auto ret = std::async(std::launch::async, [&]{drawingThread(window, camera, pixels, drawData, drawObjects);});
        //auto tret = std::async(std::launch::async, [&]{std::this_thread::sleep_for(std::chrono::seconds(1)); std::cout<<"SLEPT FOR 1 SECOND\n"; });
        //ret.get();

        //auto now = std::chrono::system_clock::now();
        smphSignalThreadToMain.acquire();
        //sem.acquire();
        window.drawPixelArrayToScreen(pixels);
        //sem.release();
        smphSignalMainToThread.release();
        //auto end = std::chrono::system_clock::now();
        //float currentTime = float(std::chrono::duration_cast < std::chrono::milliseconds> (end - now).count());
        //std::cout << "Elapsed Time: " << currentTime << " ms \n";
        window.render();
        //tret.get();
    }

    drawThread.request_stop();

    delete[] pixels;
    
}