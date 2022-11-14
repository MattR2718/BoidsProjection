#include <iostream>

#include <SFML/Graphics.hpp>


void initPixels(sf::Uint8 *arr, const int length){
    for (int i = 0; i < length; i += 4){
        arr[i] = 0;
        arr[i + 1] = i % (255 * 4);
        arr[i + 2] = 0;
        arr[i + 3] = 255;
    }
}

int main()
{
    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 800;

    //Create a window that the program will draw to
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Boids Projection");
    //Limit the windows frame rate to 30
    window.setFramerateLimit(30);

    sf::Uint8* pixels  = new sf::Uint8[WIDTH * HEIGHT * 4];
    initPixels(pixels, WIDTH * HEIGHT * 4);

    //Run program while window is open
    while (window.isOpen())
    {
        //Loop over all events since last loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //If close requested then close window
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

        sf::Image image;
        image.create(WIDTH, HEIGHT, pixels);
        sf::Texture texture;
        texture.loadFromImage(image);
        sf::Sprite sprite(texture);
        //sprite.setTexture(texture);
        window.draw(sprite);

        //End the current frame
        window.display();
    }
}