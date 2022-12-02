#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <cmath>
#include <variant>
#include <ranges>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "drawableData.h"
#include "camera.h"

#include "drawable.h"
#include "point.h"
#include "line.h"
#include "box.h"
#include "vect.h"

#define PI 3.14159

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector>>;

void initPixels(sf::Uint8 *arr, const int length){
    for (int i = 0; i < length; i += 4){
        arr[i] = 0;
        arr[i + 1] = 0;
        arr[i + 2] = 0;
        arr[i + 3] = 255;
    }
}

void displayFPS(sf::RenderWindow& window, const float& fps, const sf::Font& font){
    sf::Text text;
	// select the font
	text.setFont(font);
	// set the string to display
	text.setString(std::to_string(fps));
	// set the character size
	text.setCharacterSize(30);
	// set the color
	text.setFillColor(sf::Color::Red);
	//Set position
	text.setPosition(sf::Vector2f(0, 0));
	window.draw(text);
}

int main()
{

    //Limit the windows frame rate to 30
    window.setFramerateLimit(30);
    //Init imgui
    if(!ImGui::SFML::Init(window)){ std::cout<<"ERROR INITIALISING IMGUI WINDOW\n"; throw std::invalid_argument("IMGUI WINDOW FAILED TO INITIALISE\n");}

    //Create a pixel array which will contain the pixels drawn to the screen
    sf::Uint8* pixels  = new sf::Uint8[WIDTH * HEIGHT * 4];
    initPixels(pixels, WIDTH * HEIGHT * 4);

    DrawableData drawData;
    Camera camera;

    //Create vector and fill with objects to test with
    Point O(0, 0, 0, WIDTH, HEIGHT, 20);
    Point X(200, 0, 0, WIDTH, HEIGHT,  20, 255, 0, 0);
    Point Y(0, 200, 0, WIDTH, HEIGHT,  20, 0, 255, 0);
    Point Z(0, 0, 200, WIDTH, HEIGHT,  20, 0, 0, 255);
    //std::vector<Point> points = { O, X, Y, Z };
    //populatePoints(points, drawData.numPoints, WIDTH, HEIGHT);

    Line l(Point(0, 0, 0, WIDTH, HEIGHT, 10), Point(200, 200, 200, WIDTH, HEIGHT, 10), WIDTH, HEIGHT);
    Line xAxis(X, O, WIDTH, HEIGHT);
    Line yAxis(Y, O, WIDTH, HEIGHT);
    Line zAxis(Z, O, WIDTH, HEIGHT);
    std::vector<Line> lines { l, xAxis, yAxis, zAxis };

    Box b(0, 0, 0, drawData.boxSize, WIDTH, HEIGHT);

    DrawVariantVector drawObjects = {O, /* X, Y, Z, */ xAxis, yAxis, zAxis, l, b};

    for(int i = 0; i < 100; i++){
        drawObjects.push_back(Vector(
            rand() % (WIDTH - 400) - WIDTH / 2 + 200,
            rand() % (HEIGHT - 400) - HEIGHT / 2 + 200,
            rand() % (WIDTH - 400) - WIDTH / 2 + 200,
            rand() % 50 - 25,
            rand() % 50 - 25,
            rand() % 50 - 25,
            WIDTH, HEIGHT
        ));
    }
    drawObjects.push_back(Vector(0, 0, 0, 100, 100, 100, WIDTH, HEIGHT));


    //Create variable to store fps, clock to calculate fps and times to store change in time
    float fps;
    sf::Clock fps_clock = sf::Clock();
    sf::Time previous_time = fps_clock.getElapsedTime();
    sf::Time current_time;
    //Load font from file and throw exeption if not found
    sf::Font font;
    if(!font.loadFromFile("../../fonts/arial.ttf")){
        throw std::invalid_argument("FONT NOT FOUND");
    }

    bool randomise = true;

    sf::Clock deltaClock;
    //Run program while window is open
    while (window.isOpen())
    {
        //Loop over all events since last loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Process imgui events
            ImGui::SFML::ProcessEvent(event);
            //If close requested then close window
            if (event.type == sf::Event::Closed){
                window.close();
            }else if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case(sf::Keyboard::Down): {
                        camera.tx += 1;
                        camera.setTrigValues();
                    }
                    break;
                    case(sf::Keyboard::Up): {
                        camera.tx -= 1;
                        camera.setTrigValues();
                    }
                    break;
                    case(sf::Keyboard::Left): {
                        camera.ty += 1;
                        camera.setTrigValues();
                    }
                    break;
                    case(sf::Keyboard::Right): {
                        camera.ty -= 1;
                        camera.setTrigValues();
                    }
                    break;
                    case(sf::Keyboard::Comma):{
                        camera.tz -= 1;
                        camera.setTrigValues();
                    }
                    break;
                    case(sf::Keyboard::Period):{
                        camera.tz += 1;
                        camera.setTrigValues();
                    }
                    break;
                    case(sf::Keyboard::Space): {
                        camera.tx = 0;
                        camera.ty = 0;
                        camera.tz = 0;
                        camera.setTrigValues();
                    }
                    break;
                    default:{
                        std::cout<<"Key Code Pressed: "<<event.key.code<<'\n';
                    }
                    break;
                }
            }
        }
        //Update imgui window
        ImGui::SFML::Update(window, deltaClock.restart());

        //Start by clearing pixels
        initPixels(pixels, WIDTH * HEIGHT * 4);
        
        //Create imgui window to allow colour picking
        ImGui::Begin("Points");
        ImGui::Checkbox("Show Points", &drawData.showPoints);
        ImGui::ColorEdit3("Fill", (float*)&drawData.pointFillColour);
        ImGui::ColorEdit3("Outline", (float*)&drawData.pointOutlineColour);
        if(ImGui::Button("Randomise")){
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Point>(obj)){
                    int x = rand() % (WIDTH - 400) - WIDTH / 2 + 200;
                    int y = rand() % (HEIGHT - 400) - HEIGHT / 2 + 200;
                    int z = rand() % (WIDTH - 400) - WIDTH/ 2 + 200;
                    std::get<Point>(obj).setPosition(x, y, z);
                }
            }
        }
        ImGui::Checkbox("Fill", &drawData.fill);
        ImGui::SliderInt("Num Points", &drawData.numPoints, 0, 5000);
        ImGui::Checkbox("Auto Rotate X", &camera.autoRotatex);
        ImGui::Checkbox("Auto Rotate Y", &camera.autoRotatey);
        ImGui::Checkbox("Auto Rotate Z", &camera.autoRotatez);
        ImGui::End();

        ImGui::Begin("Lines");
        ImGui::Checkbox("Show Lines", &drawData.showLines);
        ImGui::Checkbox("Draw Line Points", &drawData.drawLinePoints);
        ImGui::End();

        ImGui::Begin("Boxes");
        ImGui::Checkbox("Show Boxes", &drawData.showBoxes);
        ImGui::SliderInt("Num Boxes", &drawData.numBoxes, 0, 1000);
        if(ImGui::Button("Randomise")){
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Box>(obj) && !std::get<Box>(obj).atOrigin()){
                    int x = rand() % (WIDTH - 400) - WIDTH / 2 + 200;
                    int y = rand() % (HEIGHT - 400) - HEIGHT / 2 + 200;
                    int z = rand() % (WIDTH - 400) - WIDTH/ 2 + 200;
                    std::get<Box>(obj).setPosition(x, y, z);
                }
            }
        }
        ImGui::SliderInt("Main Box Size", &drawData.boxSize, 0, 800);
        ImGui::End();

        ImGui::Begin("Vectors");
        ImGui::Checkbox("Show Vectors", &drawData.showVectors);
        if(ImGui::Button("Randomise Direction")){
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Vector>(obj)){
                    int x = rand() % 50 - 25;
                    int y = rand() % 50 - 25;
                    int z = rand() % 50 - 25;
                    std::get<Vector>(obj).setDir(x, y, z, WIDTH, HEIGHT);
                }
            }
        }
        if(ImGui::Button("Randomise Position")){
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Vector>(obj)){
                    int x = rand() % (WIDTH - 400) - WIDTH / 2 + 200;
                    int y = rand() % (HEIGHT - 400) - HEIGHT / 2 + 200;
                    int z = rand() % (WIDTH - 400) - WIDTH / 2 + 200;
                    std::get<Vector>(obj).setPos(x, y, z, WIDTH, HEIGHT);
                }
            }
        }
        ImGui::End();

        //populatePoints(points, drawData.numPoints, WIDTH, HEIGHT);

        if(camera.autoRotatex){
            camera.tx += 1;
        }
        if(camera.autoRotatey){
            camera.ty += 1;
        }
        if(camera.autoRotatez){
            camera.tz += 1;
        }
        camera.setTrigValues();

        std::ranges::sort(drawObjects, std::greater(), [](auto const& x){
            return std::visit([](auto const& e){ return e.sortVal; }, x);
        });

        int pointCount = 0;
        int boxCount = 0;

        //Draw all objects to screen
        for(auto& obj : drawObjects){
            switch(obj.index()){
                case 0:{ //Drawable
                    break;
                }
                case 1:{ //Point
                    pointCount++;
                    if(drawData.showPoints){
                        std::get<Point>(obj).quickDraw(pixels, WIDTH, HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, drawData.pointFillColour, drawData.pointOutlineColour, drawData.fill);
                    }
                    break;
                }
                case 2:{ //Line
                    if(drawData.showLines){
                        std::get<Line>(obj).quickDraw(pixels, WIDTH, HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, drawData.drawLinePoints);
                    }
                    break;
                };
                case 3:{ //Box
                    boxCount++;
                    if(drawData.showBoxes){
                        std::get<Box>(obj).draw(pixels, WIDTH, HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, drawData.drawLinePoints, drawData.boxSize);
                    }
                    break;
                }
                case 4:{ //Vector
                    std::get<Vector>(obj).move(WIDTH, HEIGHT);
                    if(drawData.showVectors){
                        std::get<Vector>(obj).draw(pixels, WIDTH, HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions);
                    }
                    break;
                }
                default:{
                    std::cout<<"UNKNOWN camera.tyPE IN VARIANT, INDEX = "<<obj.index()<<'\n';
                    break;
                }
            }
        }

        drawData.populateDrawPoints(drawObjects, pointCount, drawData.numPoints, WIDTH, HEIGHT);
        drawData.populateDrawBox(drawObjects, boxCount, drawData.numBoxes, WIDTH, HEIGHT);
        

        //Create an sf::image which will be load the pixels
        sf::Image image;
        image.create(WIDTH, HEIGHT, pixels);
        //Create an sf::texture and load the image to the texture
        sf::Texture texture;
        texture.loadFromImage(image);
        //Create an sf::sprite and draw the sprite to the screen
        sf::Sprite sprite(texture);
        window.draw(sprite);

        //Calculate fps from change in time and draw to screen
        current_time = fps_clock.getElapsedTime();
        fps = 1.f / (current_time.asSeconds() - previous_time.asSeconds());
        displayFPS(window, fps, font);
        previous_time = current_time;

        //Render imgui windows
        ImGui::SFML::Render(window);
        //End the current frame
        window.display();
    }
}