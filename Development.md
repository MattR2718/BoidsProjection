# Development of program

## 14/11/22
---
###  **Basic Program**
Create basic program to check build is working

```cpp
#include <iostream>
 
int main() {
    std::cout << "Hello, world!\n";
}
```
---
### **Shape Test**
Create a basic test program which draws a circle to the screen

```cpp
#include <iostream>
 
#include <SFML/Graphics.hpp>
 
int main()
{
    //Create a window that the program will draw to
    sf::RenderWindow window(sf::VideoMode(600, 600), "Boids Projection");
    //Limit the windows frame rate to 30
    window.setFramerateLimit(30);
 
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
 
        //Clear window with black colour
        window.clear(sf::Color::Black);
 
        //Create a circle to draw to screen
        sf::CircleShape shape(300);
        //Set shape colour
        shape.setFillColor(sf::Color::Green);
        //Draw shape to screen
        window.draw(shape);
        //End the current frame
        window.display();
    }
}

```
![Test Circle](imgs/testCircle.JPG)

-----

## **Pixel Array**

Since my program will be drawing to the screen using a pixel array I will now change the basic program to that

```cpp
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

```

![Test Pixel Array Drawing](imgs/testPixelArray.JPG)

---
### **FPS**
Add fps counter in order to track efficiency and make sure program runs at acceptible speeds

```cpp
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
```

```cpp
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
```
```cpp
//Calculate fps from change in time and draw to screen
current_time = fps_clock.getElapsedTime();
fps = 1.f / (current_time.asSeconds() - previous_time.asSeconds());
displayFPS(window, fps, font);
previous_time = current_time;
```

![FPS Counter Image](imgs/testFps.JPG)

---
### **Basic Drawable**

Header file for initial drawable object
Object just draws one pixel to the screen at the x, y, coordinate

_drawable.h_
```cpp
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics.hpp>

class Drawable{

public:
    //Variables
    int x, y, z;

    //Methods
    Drawable(int x_, int y_, int z_, int r_ = 255, int g_ = 255, int b_ = 255);
    void draw(sf::Uint8 *arr, const int width);
    void setColour(int r, int g, int b);
    void setPosition(int x_, int y_, int z_);

private:
    //Variables
    int r, g, b;

    //Methods

};

#endif
```

_drawable.cpp_
```cpp
#include "../include/drawable.h"

Drawable::Drawable(int x_, int y_, int z_, int r_, int g_, int b_){
    this->x = x_;
    this->y = y_;
    this->z = z_;
    this->r = r_;
    this->g = g_;
    this->b = b_;
}

void Drawable::draw(sf::Uint8 *arr, const int width){
    int index = (this->y * width + this->x) * 4;
    arr[index] = this->r;
    arr[index + 1] = this->g;
    arr[index + 2] = this->b;
}

void Drawable::setColour(int r_, int g_, int b_){
    this->r = r_;
    this->g = g_;
    this->b = b_;
}

void Drawable::setPosition(int x_, int y_, int z_){
    this->x = x_;
    this->y = y_;
    this->z = z_;
}
```

---
## 15/11/22
---
### **ImGui Setup**
Update CMakeLists.txt to link to imgui
```cmake
cmake_minimum_required(VERSION 3.0.0)
project(BoidsProjection VERSION 0.1.0)

list(APPEND CMAKE_PREFIX_PATH "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\sfml")
find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
include_directories(${SFML_INCLUDE_DIR})

list(APPEND CMAKE_PREFIX_PATH "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\imgui")
list(APPEND CMAKE_PREFIX_PATH "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\ImGui-SFML")
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable(BoidsProjection src/main.cpp src/drawable.cpp)

target_link_libraries(${PROJECT_NAME}
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
)
```

Add imgui code to _main.cpp_ in correct places to initialise, create and get input from gui

```cpp
//Init imgui
ImGui::SFML::Init(window);

//Process imgui events
ImGui::SFML::ProcessEvent(event);

//Update imgui window
ImGui::SFML::Update(window, deltaClock.restart());

//Create imgui window to allow colour picking
ImGui::Begin("Colours");
ImGui::ColorEdit3("Dot", (float*)&colour);
ImGui::End();
//Set colour to the colour picked from colour picker
d.setColour(round(colour[0] * 255), round(colour[1] * 255), round(colour[2] * 255));

//Draw d at 1000 random positions
for(int i = 0; i < 1000; i++){
    int x = rand() % WIDTH;
    int y = rand() % HEIGHT;
    d.setPosition(x, y, 0);
    d.draw(pixels, WIDTH);
}

//Render imgui windows
ImGui::SFML::Render(window);
```

![Imgui Random Dots](imgs/testColourPicker.JPG)

---
### **Create Point Class**
_point.h_
```cpp
#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "drawable.h"

class Point: public Drawable{

public:
    //Vaariables


    //Methods
    Point(int x_, int y_, int z_, int rad_, int r_ = 255, int g_ = 255, int b_ = 255);
    void draw(sf::Uint8 *arr, const int width);

private:
    //Variables
    int rad;

    //Methods



};



#endif
```

_point.cpp_
```cpp
#include "../include/point.h"

Point::Point(int x_, int y_, int z_, int rad_, int r_, int g_, int b_) : Drawable{x_, y_, z_, r_, g_, b_}{
    this->rad = rad_;
}

void Point::draw(sf::Uint8 *arr, const int width){
    for(int i = -5; i < 6; i++){
        for(int j = -5; j < 6; j++){
            int index = ((this->y + j) * width + (this->x + i));
            if(index > -1 && index < 800){
                arr[index] = this->r;
                arr[index + 1] = this->g;
                arr[index + 2] = this->b;
            }
        }
    }
}
```
The points are stuck at the very top left

![Broken Point](imgs/brokenPoint.JPG)

Loop was too small so not all indexes were being allowed.  
Change to width*width for the for loop to allow all points on screen  
Should change to width * height to allow for window size to be defined at a single point in program and still work

```cpp
if(index > -1 && index < width*width){
    arr[index * 4] = this->r;
    arr[index * 4 + 1] = this->g;
    arr[index * 4+ 2] = this->b;
}
```

![Fixed Simple Point](imgs/fixedSimplePoint.JPG)

---
### **Circular Points**

Currently points are just squares with sized of 10  
The points would look much better if they were circular  
Circular points can be implemented using Bresenhams circle drawing algorithm
```cpp
//Plot all 8 points, one in each octant of the circle
void Point::plotCircle(sf::Uint8 *pixels, const int width, int x, int y){
    auto plot = [&](const int j, const int i){
        if(i < 0 || i > width || j < 0 || j > 800){return;}
        pixels[(i * width + j) * 4] = this->r;
        pixels[(i * width + j) * 4 + 1] = this->g;
        pixels[(i * width + j) * 4 + 2] = this->b;
    };
    plot(this->x + x, this->y + y);
    plot(this->x - x, this->y + y);
    plot(this->x + x, this->y - y);
    plot(this->x - x, this->y - y);
    plot(this->x - y, this->y + x);
    plot(this->x + y, this->y - x);
    plot(this->x - y, this->y - x);
    plot(this->x + y, this->y + x);
}

void Point::draw(sf::Uint8 *pixels, const int width){
    int x = 0;
    int y = this->rad;
    int decPar = 3 - 2 * this->rad;
    plotCircle(pixels, width, x, y);
    while( y >= x){
        x++;
        if(decPar > 0){
            y--;
            decPar = decPar + 4 * (x - y) + 10;
        }else{
            decPar = decPar + 4 * x + 6;
        }
        plotCircle(pixels, width, x, y);
    }
}
```

![Point Drawn As Empty Circles](imgs/pointCircles.JPG)

---
### **Fill Points**

To fill thr points we can draw lines across thw circle every time we calculate new x and y values for the edge of the circle


```cpp
auto plotLine = [&](const int x1, const int x2, const int y){
    int px1 = x1, px2 = x2;
    if(px1 > px2){ px1 = x2; px2 = x1; }
    for(int c = px1; c < px2; c++){
        plot(c, y);
    }
};

if(this->fill){
    plotLine(this->x - y, this->x + y, this->y + x);
    plotLine(this->x - x, this->x + x, this->y + y);
    plotLine(this->x - x, this->x + x, this->y - y);
    plotLine(this->x - y, this->x + y, this->y - y);
}
```

This code produces incorrect filling of the points, the top half of the circle is drawn upwards rather than down to fill the circle

![Broken Point Fill](imgs/brokenPointFill.JPG)

The incorrect line of code was drawing the line at this->y - y, since y was negative this meant the code was drawing above the centre position  
The correct code subtracts x from the y position to fill the correct rows of the circle
```cpp
//plotLine(this->x - y, this->x + y, this->y - y);
plotLine(this->x - y, this->x + y, this->y - x);
```
The lines are drawn to fill according to this diagram of how bresenhams circle algorithm draws a circle  
Lines connect all points on the same y value  
![Bresenham Circle Algorithm Diagram](imgs/bresenhamOctants.jpg)

The correct code produces circles which have all been filled
![Filled Points](imgs/fixedPointFill.JPG)

---
### **Randomise and Fill**
Added the ability to randomise the positions of all test points as well as toggle fill to imgui window
```cpp
if(ImGui::Button("Randomise")){
    for(auto& p : points){
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        p.setPosition(x, y, 0);
    }
}
ImGui::Checkbox("Fill", &fill);
```

![Randomise and Fill ImGui Window](imgs/randomiseAndFillImGuiWindow.JPG)

---
## 17/11/22
### **Change Point Fill Colour**

Currently all points have the same fill colour and outline colour, this makes them difficult to differentiate when they are stacked over eachother which can be solved by adding an outline to them  
Adding an outline is the same as changing the colour of the points that are plotted in the lines which are being drawn across the circles to fill them

```cpp
auto plot = [&](const int j, const int i, bool outline = false){
    if(i < 0 || i > width || j < 0 || j > height){return;}
    int r, g, b;
    if(!outline){ r = this->outr; g = this->outg; b = this->outb; }
    else{ r = this->r; g = this->g; b = this->b; }
    pixels[(i * width + j) * 4] = r;
    pixels[(i * width + j) * 4 + 1] = g;
    pixels[(i * width + j) * 4 + 2] = b;
};
```

![Broken Outline Points](imgs/brokenOutlineColour.JPG)

Currently the lines are being drawn from the leftmost x value to one before the rightmost which is causing the left hand side not to be drawn  
```cpp
auto plotLine = [&](const int x1, const int x2, const int y){
    int px1 = x1, px2 = x2;
    if(px1 > px2){ px1 = x2; px2 = x1; }
    for(int c = px1; c < px2; c++){
        plot(c, y, true);
    }
};
```

Changing the starting of the loop to int c = px1 + 1 fixes the issue  

```cpp
auto plotLine = [&](const int x1, const int x2, const int y){
    int px1 = x1, px2 = x2;
    if(px1 > px2){ px1 = x2; px2 = x1; }
    for(int c = px1 + 1; c < px2; c++){
        plot(c, y, true);
    }
};
```

![Fixed Point Outline](imgs/fixedPointOutline.JPG)

This solution is not the best solution as it leaves the top and botton of the circle quite bare and also doesn't allow for variable outline width.  This solution works for now and can easily be changed at a later point if that becomes a necessity.

### **Fixing Window Size Crash**
When the window size is changed in the source code the program crashes if the width gets bigger than 800 and if it is less than 800, the points are only drawn in a width x width square on the window

![Broken Width Window](imgs/brokenWidthWindow.JPG)

Runing the program under the debugger yields this error in the _point.cpp_ file.  
The error means I am trying to access memory which hasn't been allocated for this array and so the index (i * width + j) * 4 is greater than then length of the array or is negative

![Resize Window Error Message](imgs/resizeWindowErrorMessage.JPG)

By logging the relevant variable I can work out the values when the program crashes  
>i: 812 j: 193 index: 3248772 length: 3200000

The error is with the equation for calculating the index of the point in the array. The correct equation for the index if (j * width + i) * 4 whereas I have mistyped (i * width + j) * 4.

After this fix the program still crashed when running. Continuing to log the output I discovered that the program attempts to access outside of the pixel array multiple times.

>i: 492 j: 800 index: 3201968 length: 3200000 width: 1000 height: 800  
i: 484 j: 800 index: 3201936 length: 3200000 width: 1000 height: 800  
i: 493 j: 800 index: 3201972 length: 3200000 width: 1000 height: 800  
i: 483 j: 800 index: 3201932 length: 3200000 width: 1000 height: 800  
i: 740 j: 800 index: 3202960 length: 3200000 width: 1000 height: 800  
i: 720 j: 800 index: 3202880 length: 3200000 width: 1000 height: 800  
i: 741 j: 800 index: 3202964 length: 3200000 width: 1000 height: 800  
i: 719 j: 800 index: 3202876 length: 3200000 width: 1000 height: 800  
i: 96 j: 800 index: 3200384 length: 3200000 width: 1000 height: 800  
i: 72 j: 800 index: 3200288 length: 3200000 width: 1000 height: 800  
i: 104 j: 800 index: 3200416 length: 3200000 width: 1000 height: 800  
i: 92 j: 800 index: 3200368 length: 3200000 width: 1000 height: 800  
i: 104 j: 800 index: 3200416 length: 3200000 width: 1000 height: 800  
i: 92 j: 800 index: 3200368 length: 3200000 width: 1000 height: 800  
i: 760 j: 800 index: 3203040 length: 3200000 width: 1000 height: 800  

This always happend when j = 800 which should never happen. The reason it is happening is because my check for whether a coordinate is valid is incorrect and allowing invalid coordinates to get through. 

The original line of code was:
```cpp
if(i < 0 || i > width || j < 0 || j > height){return;}
```
Whereas the correct line should be
```cpp
if(i < 0 || i >= width || j < 0 || j >= height){return;}
```
The reason for this is because width and height are the numbers of rows and columns however, the indexes for accessing the array start at zero, this means that a coordinate such as (width, height) is invalid and the bottom right coordinate is actually (width - 1, height -1)

After this fix the program runs but is still not drawing over the whole screen.

![Fixed Resize, Broken Draw Area](imgs/fixedResizeButBrokenDrawCoverage.JPG)

---
## 18/11/22
### **Fixing Not Drawing To Whole Screen**

Start by filling points with a single point which is meant to be drawn where points are currently not being drawn

```cpp
constexpr int WIDTH = 600;
constexpr int HEIGHT = 800;

std::vector<Point> points = {Point(500, 600, 0, 100)};
```

With the above window size and point coordinates the point is drawn here

![Half Circle](imgs/halfOffCircle.JPG)

With the same window size but a differenc coordinate position the circle is drawn here

![Full Circle](imgs/fullCircle.JPG)

Here what should be the y coordinate has decreased but the x potition on screen has changed

The problem is with my plot function parameters. The parameters are in toe order j(y), i(x) but when called the function is given them in order x(i), y(j)

```cpp 
auto plot = [&](const int j, const int i, bool outline = false){
```

This is easily fixed by switching the order of the parameters to

```cpp
auto plot = [&](const int i, const int j, bool outline = false)
```

Now the whole screen is covered by points when they are plotted in random positions

![Full Coverage Points](imgs/fixedPointsNotCoveringScreen.JPG)

---
### **Variable Number Of Points**

Adding a slider to allow the number of points being drawn for the user allows me to test the programs efficiency.  
Running in a debug build, the program is pretty smooth with a couple thousand points however, increasing the number up to 10000 does cause the fps to drop by quite a lot.

![Variable Number Of Points Debug](imgs/variableNumPointsDebugTest.JPG)

Running the program in a Release build produces much better performance as it is much more optimised. This means that at 10000 points being drawn, the program runs perfectly fine.

![Variable Number Of Points Release](imgs/variableNumPointsReleaseTest.JPG)

It is not until 100,000 points being drawn that the Release build drops to similar levels to debug mode at 10,000 points

![Variable Number Of Points Release 100000](imgs/variableNumPointsReleaseTest100000.JPG)

## **Implementing Rotation**

First I need 3 variables to store the angles of rotation of the camera as well as a map to store the values of the trig functions since they do not need to be calculated for every object, just when the camera moves.  
The values in the map have to be initialised since the _.at()_ function used in the set trig values does not allow initialisation and will throw an error if attempting to ccess an invalid position. This is better than using the _[ ]_ accessor since this can create new records in the map which could lead to errors doing arithmetic on empty map values.

```cpp
//Floats to store the rotation angle of the cameras
float tx = 0, ty = 0, tz = 0;
//Map to store values for trig finctions
std::map<std::string, float> trigFunctions = {{"sx", sin(degToRad(tx))},
                                                {"sy", sin(degToRad(ty))},
                                                {"sz", sin(degToRad(tz))},
                                                {"cx", cos(degToRad(tx))},
                                                {"cy", cos(degToRad(ty))},
                                                {"cz", cos(degToRad(tz))}};
```

Functions to convert between degrees and radians, and to calculate the values of the trig function

```cpp
template <typename T>
auto degToRad(T angle){
        return float(angle) * PI / 180.f;
}

template <typename T, typename U>
void setTrigValues(const T tx, const T ty, const T tz, U tfunct){
    tfunct.at("sx") = sin(degToRad(tx));
    tfunct.at("sy") = sin(degToRad(ty));
    tfunct.at("sz") = sin(degToRad(tz));

    tfunct.at("cx") = cos(degToRad(tx));
    tfunct.at("cy") = cos(degToRad(ty));
    tfunct.at("cz") = cos(degToRad(tz));
}
```

Then I need to implement keyboard input to allow for the camera to be rotated.

```cpp
if (event.type == sf::Event::Closed){
    window.close();
}else if (event.type == sf::Event::KeyPressed) {
    switch(event.key.code) {
        case(sf::Keyboard::Down): {
            tx += 1;
        }
        break;
        case(sf::Keyboard::Up): {
            tx -= 1;
        }
        break;
        case(sf::Keyboard::Left): {
            ty += 1;
        }
        break;
        case(sf::Keyboard::Right): {
            ty -= 1;
        }
        break;
        case(sf::Keyboard::Comma):{
            tz += 1;
        }
        break;
        case(sf::Keyboard::Period):{
            tz -= 1;
        }
        break;
        case(sf::Keyboard::Space): {
            tx = 26;
            ty = 40;
            tz = 0;
        }
        break;
        default:{
            std::cout<<"Key Code Pressed: "<<event.key.code<<'\n';
        }
        break;
    }
}
```

All rotations are multiplied out to produce a single calculation that can be completed for each component of the coordinate


```cpp
template<typename T, typename U>
void Drawable::rotX(T tx, T ty, T tz, U trigfunct){
    //x * cosy * cosz - y * cosy * sinz + z * siny
    this->px = this->x * trigfunct.at("cy") * trigfunct.at("cz") - this->y * trigfunct.at("cy")* trigfunct.at("sz") + this->z * trigfunct.at("sy");
}

template<typename T, typename U>
void Drawable::rotY(T tx, T ty, T tz, U trigfunct){
    //x * (sinx * siny * cosx + cosx * sinz) - y * (sinx * siny * sinz - cosx * cosz) - z * sinx * cosy
    this->py = this->x * (trigfunct.at("sx") * trigfunct.at("sy") * trigfunct.at("cx") + trigfunct.at("cx") * trigfunct.at("sx")) - this->y * (trigfunct.at("sx") * trigfunct.at("sy") * trigfunct.at("sz") - trigfunct.at("cx") * trigfunct.at("cz")) - this->z * trigfunct.at("sx") * trigfunct.at("cy");
}

template<typename T, typename U>
void Drawable::rotZ(T tx, T ty, T tz, U trigfunct){
    //x * (sinx * siny - cosx * cosz * siny) + y * (cosx * siny * sinz + sinx * cosz) + z * cosx * cosy
    this->pz = this->x * (trigfunct.at("sx") * trigfunct.at("sy") - trigfunct.at("cx") * trigfunct.at("cz") * trigfunct.at("sy")) + this->y * (trigfunct.at("cx") * trigfunct.at("sy") * trigfunct.at("sz") + trigfunct.at("sx") * trigfunct.at("cx")) + this->z * trigfunct.at("cx") * trigfunct.at("cy");
}

template<typename T, typename U>
void Drawable::rotAll(T tx, T ty, T tz, U trigfunct){
    this->rotX(tx, ty, tz, trigfunct);
    this->rotY(tx, ty, tz, trigfunct);
    this->rotZ(tx, ty, tz, trigfunct);
}
```

