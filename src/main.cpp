#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "../include/drawable.h"
#include "../include/point.h"

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
        int x = rand() % width;
        int y = rand() % height;
        p.setRadius(rand() % 30);
        p.setPosition(x, y, 0);
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
    float fillColour[3] = { 1, 1, 1 };
    float outlineColour[3] = { 1, 1, 1 };
    //Boolean to store whether to fill test points
    bool fill = false;
    //Integer to store number of points to plot
    int numPoints = 1000;
    //Floats to store the rotation angle of the cameras
    float tx = 0, ty = 0, tz = 0;
    //Map to store values for trig finctions
    std::map<std::string, float> trigFunctions = {{"sx", sin(degToRad(tx))},
                                                    {"sy", sin(degToRad(ty))},
                                                    {"sz", sin(degToRad(tz))},
                                                    {"cx", cos(degToRad(tx))},
                                                    {"cy", cos(degToRad(ty))},
                                                    {"cz", cos(degToRad(tz))}};

    //Create vector and fill with objects to test with
    Point O(0, 0, 0, WIDTH, HEIGHT, 20);
    Point X(100, 0, 0, WIDTH, HEIGHT,  20);
    Point Y(0, 100, 0, WIDTH, HEIGHT,  20);
    Point Z(0, 0, 100, WIDTH, HEIGHT,  20);
    std::vector<Point> points = { O, X, Y, Z };
    /* Point d(100, 100, 100, 20);
    for(int i = 0; i < numPoints; i++){
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        d.setPosition(x, y, 0);
        d.setRadius(rand() % 30);
        points.push_back(d);
    } */

    //Create variable to store fps, clock to calculate fps and times to store change in time
    float fps;
    sf::Clock fps_clock = sf::Clock();
    sf::Time previous_time = fps_clock.getElapsedTime();
    sf::Time current_time;
    //Load font from file and throw exeption if not found
    sf::Font font;
    if(!font.loadFromFile("../fonts/arial.ttf")){
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
                        tx += 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Up): {
                        tx -= 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Left): {
                        ty += 1;
                        setTrigValues(tx, ty, tz, trigFunctions);
                    }
                    break;
                    case(sf::Keyboard::Right): {
                        ty -= 1;
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
        ImGui::Begin("Colours");
        ImGui::ColorEdit3("Fill", (float*)&fillColour);
        ImGui::ColorEdit3("Outline", (float*)&outlineColour);
        if(ImGui::Button("Randomise")){
            for(auto& p : points){
                int x = rand() % WIDTH;
                int y = rand() % HEIGHT;
                p.setPosition(x, y, 0);
            }
        }
        ImGui::Checkbox("Fill", &fill);
        ImGui::SliderInt("Num Points", &numPoints, 0, 5000);
        ImGui::End();

        //populatePoints(points, numPoints, WIDTH, HEIGHT);

        //Loop through all points and draw them
        //Update colours in case user has changed the colour
        for(auto& point : points){
            //Set colour to the colour picked from colour picker
            point.setColour(round(fillColour[0] * 255), round(fillColour[1] * 255), round(fillColour[2] * 255));
            point.setOutlineColour(round(outlineColour[0] * 255), round(outlineColour[1] * 255), round(outlineColour[2] * 255));
            point.setFill(fill);
            //TODO IN DRAW USE PX< PY AND PZ VALUES FOR DRAWING AND FINISH ROTATIONS
            point.draw(pixels, WIDTH, HEIGHT, tx, ty, tz, trigFunctions);
        }

        //std::cout<<"tx: "<<tx<<" ty: "<<ty<<" tz: "<<tz<<'\n';

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