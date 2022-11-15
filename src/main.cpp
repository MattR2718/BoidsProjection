#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "../include/drawable.h"
#include "../include/point.h"

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
    constexpr int WIDTH = 800;
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

    //Create vector and fill with objects to test with
    std::vector<Point> points;
    Point d(100, 100, 100, 20);
    for(int i = 0; i < 1000; i++){
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        d.setPosition(x, y, 0);
        d.setRadius(rand() % 20);
        points.push_back(d);
    }

    //Colour for drawable object
    float colour[3] = { 1, 1, 1 };
    //Boolean to store whether to fill test points
    bool fill = false;

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
            }
        }
        //Update imgui window
        ImGui::SFML::Update(window, deltaClock.restart());

        //Start by clearing pixels
        initPixels(pixels, WIDTH * HEIGHT * 4);
        
        //Create imgui window to allow colour picking
        ImGui::Begin("Colours");
        ImGui::ColorEdit3("Dot", (float*)&colour);
        if(ImGui::Button("Randomise")){
            for(auto& p : points){
                int x = rand() % WIDTH;
                int y = rand() % HEIGHT;
                p.setPosition(x, y, 0);
            }
        }
        ImGui::Checkbox("Fill", &fill);
        ImGui::End();

        //Loop through all points and draw them
        //Update colours in case user has changed the colour
        for(auto& point : points){
            //Set colour to the colour picked from colour picker
            point.setColour(round(colour[0] * 255), round(colour[1] * 255), round(colour[2] * 255));
            point.setFill(fill);
            point.draw(pixels, WIDTH, HEIGHT);
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