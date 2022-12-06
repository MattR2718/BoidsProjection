#include "window.h"

Window::Window(){
    //Create a window that the program will draw to
    this->window = new sf::RenderWindow(sf::VideoMode(this->WIDTH, this->HEIGHT), "Boids Projection");
    //Limit the windows frame rate to 30
    //this->window->setFramerateLimit(30);
    //Init imgui
    if(!ImGui::SFML::Init(*this->window)){ std::cout<<"ERROR INITIALISING IMGUI WINDOW\n"; throw std::invalid_argument("IMGUI WINDOW FAILED TO INITIALISE\n");}

    if(!this->font.loadFromFile("../../fonts/arial.ttf")){
        throw std::invalid_argument("FONT NOT FOUND");
    }
}

Window::~Window(){
    delete this->window;
}

const bool Window::running() const{
	return this->window->isOpen();
}

void Window::pollEvents(Camera& camera){
    //Loop over all events since last loop
    sf::Event event;
    while (this->window->pollEvent(event))
    {
        //Process imgui events
        ImGui::SFML::ProcessEvent(event);
        //If close requested then close window
        if (event.type == sf::Event::Closed){
            this->window->close();
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
}

void Window::updateImGui(){
    //Update imgui window
    ImGui::SFML::Update(*this->window, this->deltaClock.restart());
}

void Window::drawImGui(DrawableData& drawData, DrawVariantVector& drawObjects, Camera& camera){
    //Create imgui window to allow colour picking
    ImGui::Begin("Points");
    ImGui::Checkbox("Show Points", &drawData.showPoints);
    ImGui::ColorEdit3("Fill", (float*)&drawData.pointFillColour);
    ImGui::ColorEdit3("Outline", (float*)&drawData.pointOutlineColour);
    if(ImGui::Button("Randomise")){
        for(auto& obj : drawObjects){
            if(std::holds_alternative<Point>(obj)){
                int x = rand() % (this->WIDTH - 400) - this->WIDTH / 2 + 200;
                int y = rand() % (this->HEIGHT - 400) - this->HEIGHT / 2 + 200;
                int z = rand() % (this->WIDTH - 400) - this->WIDTH/ 2 + 200;
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
                int x = rand() % (this->WIDTH - 400) - this->WIDTH / 2 + 200;
                int y = rand() % (this->HEIGHT - 400) - this->HEIGHT / 2 + 200;
                int z = rand() % (this->WIDTH - 400) - this->WIDTH/ 2 + 200;
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
                std::get<Vector>(obj).setDir(x, y, z, this->WIDTH, this->HEIGHT);
            }
        }
    }
    if(ImGui::Button("Randomise Position")){
        for(auto& obj : drawObjects){
            if(std::holds_alternative<Vector>(obj)){
                int x = rand() % (this->WIDTH - 400) - this->WIDTH / 2 + 200;
                int y = rand() % (this->HEIGHT - 400) - this->HEIGHT / 2 + 200;
                int z = rand() % (this->WIDTH - 400) - this->WIDTH / 2 + 200;
                std::get<Vector>(obj).setPos(x, y, z, this->WIDTH, this->HEIGHT);
            }
        }
    }
    ImGui::End();
}


void Window::drawPixelArrayToScreen(sf::Uint8 *pixels){
    //Create an sf::image which will be load the pixels
    sf::Image image;
    image.create(this->WIDTH, this->HEIGHT, pixels);
    //Create an sf::texture and load the image to the texture
    sf::Texture texture;
    texture.loadFromImage(image);
    //Create an sf::sprite and draw the sprite to the screen
    sf::Sprite sprite(texture);
    this->window->draw(sprite);
}

void Window::drawFPS(){
    //Calculate fps from change in time and draw to screen
    this->current_time = this->fps_clock.getElapsedTime();
    this->fps = 1.f / (this->current_time.asSeconds() - this->previous_time.asSeconds());
    sf::Text text;
	// select the font
	text.setFont(this->font);
	// set the string to display
	text.setString(std::to_string(this->fps));
	// set the character size
	text.setCharacterSize(30);
	// set the color
	text.setFillColor(sf::Color::Red);
	//Set position
	text.setPosition(sf::Vector2f(0, 0));
	this->window->draw(text);
    this->previous_time = this->current_time;
}

void Window::render(){
    //Render imgui windows
    ImGui::SFML::Render(*this->window);
    //End the current frame
    this->window->display();
}