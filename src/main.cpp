#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "drawable.h"
#include "point.h"
#include "line.h"

#define PI 3.14159

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

auto populatePoints(std::vector<Point>& points, const int numPoints, const int width, const int height){
    while(points.size() < numPoints){
        Point p(100, 100, 100, width, height, 100);
        int x = rand() % (width - 400) - width / 2 + 200;
        int y = rand() % (height - 400) - height / 2 + 200;
        int z = rand() % (width - 400) - width / 2 + 200;
        p.setPosition(x, y, z);
        p.setRadius(rand() % 30);
        points.push_back(p);
    }

    if(points.size() > numPoints){
        points.erase(points.begin() + numPoints, points.end());
    }
}

template <typename T>
auto degToRad(T angle){
        return float(angle) * PI / 180.f;
}

template <typename T, typename U>
void setTrigValues(const T tx, const T ty, const T tz, U& tfunct){
    tfunct.at("sx") = sin(degToRad(tx));
    tfunct.at("sy") = sin(degToRad(ty));
    tfunct.at("sz") = sin(degToRad(tz));

    tfunct.at("cx") = cos(degToRad(tx));
    tfunct.at("cy") = cos(degToRad(ty));
    tfunct.at("cz") = cos(degToRad(tz));
}

int main()
{
    constexpr int WIDTH = 1000;
    constexpr int HEIGHT = 800;

    //Create a window that the program will draw to
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Boids Projection");
    //Limit the windows frame rate to 30
    window.setFramerateLimit(30);
    //Init imgui
    ImGui::SFML::Init(window);

    //Create a pixel array which will contain the pixels drawn to the screen
    sf::Uint8* pixels  = new sf::Uint8[WIDTH * HEIGHT * 4];
    initPixels(pixels, WIDTH * HEIGHT * 4);

    //Colour for drawable object
    float pointFillColour[3] = { 1, 1, 1 };
    float pointOutlineColour[3] = { 0.5, 0.5, 0.5 };
    //Boolean to store whether to fill test points
    bool fill = false;
    //Integer to store number of points to plot
    int numPoints = 100;
    bool drawLinePoints = false;
    //Floats to store the rotation angle of the cameras
    float tx = 30, ty = -30, tz = 0;
    //Map to store values for trig finctions
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};

    //Create vector and fill with objects to test with
    Point O(0, 0, 0, WIDTH, HEIGHT, 20);
    Point X(200, 0, 0, WIDTH, HEIGHT,  20, 255, 0, 0);
    Point Y(0, 200, 0, WIDTH, HEIGHT,  20, 0, 255, 0);
    Point Z(0, 0, 200, WIDTH, HEIGHT,  20, 0, 0, 255);
    std::vector<Point> points = { O, X, Y, Z };
    populatePoints(points, numPoints, WIDTH, HEIGHT);

    Line l(Point(0, 0, 0, WIDTH, HEIGHT, 10), Point(200, 200, 200, WIDTH, HEIGHT, 10), WIDTH, HEIGHT);
    Line xAxis(X, O, WIDTH, HEIGHT);
    Line yAxis(Y, O, WIDTH, HEIGHT);
    Line zAxis(Z, O, WIDTH, HEIGHT);
    std::vector<Line> lines { l, xAxis, yAxis, zAxis };


    bool autoRotatex = false, autoRotatey = true, autoRotatez = false;

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
                        tx -= 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Up): {
                        tx += 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Left): {
                        ty -= 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Right): {
                        ty += 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Comma):{
                        tz -= 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Period):{
                        tz += 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Space): {
                        tx = 0;
                        ty = 0;
                        tz = 0;
                        setTrigValues(tx, ty, tz, trigFunctions);
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
        ImGui::ColorEdit3("Fill", (float*)&pointFillColour);
        ImGui::ColorEdit3("Outline", (float*)&pointOutlineColour);
        if(ImGui::Button("Randomise")){
            for(auto& p : points){
                int x = rand() % (WIDTH - 400) - WIDTH / 2 + 200;
            int y = rand() % (HEIGHT - 400) - HEIGHT / 2 + 200;
            int z = rand() % (WIDTH - 400) - WIDTH/ 2 + 200;
                p.setPosition(x, y, z);
            }
        }
        ImGui::Checkbox("Fill", &fill);
        ImGui::SliderInt("Num Points", &numPoints, 0, 5000);
        ImGui::Checkbox("Auto Rotate X", &autoRotatex);
        ImGui::Checkbox("Auto Rotate Y", &autoRotatey);
        ImGui::Checkbox("Auto Rotate Z", &autoRotatez);
        ImGui::End();

        ImGui::Begin("Lines");
        ImGui::Checkbox("Draw Line Points", &drawLinePoints);
        ImGui::End();

        populatePoints(points, numPoints, WIDTH, HEIGHT);

        if(autoRotatex){
            tx += 1;
        }
        if(autoRotatey){
            ty += 1;
        }
        if(autoRotatez){
            tz += 1;
        }
        setTrigValues(tx, ty, tz, trigFunctions);

        //Loop through all points and draw them
        //Update colours in case user has changed the colour
        for(auto& point : points){
            //Set colour to the colour picked from colour picker
            point.setColour(round(pointFillColour[0] * 255), round(pointFillColour[1] * 255), round(pointFillColour[2] * 255));
            point.setOutlineColour(round(pointOutlineColour[0] * 255), round(pointOutlineColour[1] * 255), round(pointOutlineColour[2] * 255));
            point.setFill(fill);
            point.rotAll(tx, ty, tz, trigFunctions);
            //point.draw(pixels, WIDTH, HEIGHT, tx, ty, tz, trigFunctions);
        }

        std::sort(points.begin(), points.end(), [&](Point a, Point b){ return a.pz<b.pz; });
        for(auto& p : points){
            p.draw(pixels, WIDTH, HEIGHT, tx, ty, tz, trigFunctions);
        }

        for(auto& line : lines){
            line.drawPoints = drawLinePoints;
            line.draw(pixels, WIDTH, HEIGHT, tx, ty, tz, trigFunctions);
        }

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