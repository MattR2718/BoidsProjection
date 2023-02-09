#include "window.h"

Window::Window(){
    //Create a window that the program will draw to
    this->window = new sf::RenderWindow(sf::VideoMode(this->WIDTH, this->HEIGHT), "Boids Projection");
    this->window->setFramerateLimit(30);
    //Init imgui
    this->window->setPosition(sf::Vector2i(0, 0));
    if(!ImGui::SFML::Init(*this->window)){ std::cout<<"ERROR INITIALISING IMGUI WINDOW\n"; throw std::invalid_argument("IMGUI WINDOW FAILED TO INITIALISE\n");}
    
    ImGui::CreateContext();
    ImPlot::CreateContext();

    //if(!this->font.loadFromFile("../../fonts/arial.ttf")){
    //    throw std::invalid_argument("FONT NOT FOUND");
    //}
}

Window::~Window(){
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    ImGui::SFML::Shutdown();
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
            //stop = true;
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
                    //std::cout<<"Key Code Pressed: "<<event.key.code<<'\n';
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
    
    ImGui::ShowDemoWindow();

    //Create ImGui window to contain options
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
    ImGui::Begin("Options", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowPos(ImVec2(0, 0));
    
    ImGui::Checkbox("Show Demo Objects", &drawData.showDemoObjects);
    ImGui::Checkbox("Show FPS Graph", &this->showFPS);

    if(ImGui::CollapsingHeader("Camera")){
        ImGui::Checkbox("Auto Rotate X", &camera.autoRotatex);
        ImGui::Checkbox("Auto Rotate Y", &camera.autoRotatey);
        ImGui::Checkbox("Auto Rotate Z", &camera.autoRotatez);
    }

    //Create imgui section for points
    if(ImGui::CollapsingHeader("Points")){
        ImGui::Checkbox("Show Points", &drawData.showPoints);
        ImGui::ColorEdit3("Fill##Points", (float*)&drawData.pointFillColour);
        ImGui::ColorEdit3("Outline##Points", (float*)&drawData.pointOutlineColour);
        ImGui::Checkbox("Fill", &drawData.fill);
        if(drawData.showDemoObjects){
            ImGui::SliderInt("Num Points", &drawData.numPoints, 0, 1000);
            if(ImGui::Button("Randomise##Points")){
                for(auto& obj : drawObjects){
                    if(std::holds_alternative<Point>(obj)){
                        int x = rand() % (this->WIDTH - 400) - this->WIDTH / 2 + 200;
                        int y = rand() % (this->HEIGHT - 400) - this->HEIGHT / 2 + 200;
                        int z = rand() % (this->WIDTH - 400) - this->WIDTH/ 2 + 200;
                        std::get<Point>(obj).setPosition(x, y, z);
                    }
                }
            }
        }
    }

    //Create imgui section for lines
    if(ImGui::CollapsingHeader("Lines")){
        ImGui::Checkbox("Show Lines", &drawData.showLines);
        ImGui::Checkbox("Draw Line Points", &drawData.drawLinePoints);
        std::string curraa = "Line Antialiasing: ";
        curraa += (drawData.lineAntiAliasing) ? "On" : "Off";
        ImGui::Text("%s", curraa.c_str());
        if(ImGui::Button("Toggle Antialiasing##Lines")){
            drawData.lineAntiAliasing = !drawData.lineAntiAliasing;
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Line>(obj)){
                    std::get<Line>(obj).antiAliased = drawData.lineAntiAliasing;
                }
            }
        }
    }

    //Create imgui section for boxes
    if(ImGui::CollapsingHeader("Boxes", ImGuiTreeNodeFlags_DefaultOpen)){
        ImGui::Checkbox("Show Boxes", &drawData.showBoxes);
        ImGui::SliderInt("Main Box Size", &drawData.boundingBoxSize, 0, 1000);
        std::string curraa = "Box Antialiasing: ";
        curraa += (drawData.boxAntiAliasing) ? "On" : "Off";
        ImGui::Text("%s", curraa.c_str());
        if(ImGui::Button("Toggle Antialiasing##Boxes")){
            drawData.boxAntiAliasing = !drawData.boxAntiAliasing;
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Box>(obj)){
                    std::get<Box>(obj).antiAliased = drawData.boxAntiAliasing;
                }
            }
        }
        if(drawData.showDemoObjects){
            ImGui::SliderInt("Num Boxes", &drawData.numBoxes, 0, 100);
            if(ImGui::Button("Randomise##Boxes")){
                for(auto& obj : drawObjects){
                    if(std::holds_alternative<Box>(obj) && !std::get<Box>(obj).atOrigin()){
                        int x = rand() % (this->WIDTH - 400) - this->WIDTH / 2 + 200;
                        int y = rand() % (this->HEIGHT - 400) - this->HEIGHT / 2 + 200;
                        int z = rand() % (this->WIDTH - 400) - this->WIDTH/ 2 + 200;
                        std::get<Box>(obj).setPosition(x, y, z);
                    }
                }
            }
        }
    }

    //Create imgui section for vectors
    if(ImGui::CollapsingHeader("Vectors")){
        ImGui::Checkbox("Show Vectors", &drawData.showVectors);
        std::string curraa = "Vector Antialiasing: ";
        curraa += (drawData.vectorAntiAliasing) ? "On" : "Off";
        ImGui::Text("%s", curraa.c_str());
        if(ImGui::Button("Toggle Antialiasing##Vector")){
            drawData.vectorAntiAliasing = !drawData.vectorAntiAliasing;
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Vector>(obj)){
                    std::get<Vector>(obj).antiAliased = drawData.vectorAntiAliasing;
                }
            }
        }
        if(drawData.showDemoObjects){
            if(ImGui::Button("Randomise Direction")){
                for(auto& obj : drawObjects){
                    if(std::holds_alternative<Vector>(obj)){
                        int x = rand() % 50 - 25;
                        int y = rand() % 50 - 25;
                        int z = rand() % 50 - 25;
                        //std::get<Vector>(obj).setDir(x, y, z, this->WIDTH, this->HEIGHT);
                        std::get<Vector>(obj).setDir(x, y, z);
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
        }
    }

    if(ImGui::CollapsingHeader("Boids", ImGuiTreeNodeFlags_DefaultOpen)){
        ImGui::ColorEdit3("Fill##Boids", (float*)&drawData.boidFillColour);
        ImGui::ColorEdit3("Outline##Boids", (float*)&drawData.boidOutlineColour);
        ImGui::SliderInt("Num Boids##Boids", &drawData.numBoids, 0, 1500);
        ImGui::SliderFloat("Speed Multiplier##Boids", &drawData.boidSpeedMult, 0.0, 1.0);
        ImGui::SliderInt("Point Size##Boid", &drawData.boidSize, 0, 20);
        
        ImGui::Checkbox("Wrap Around at Edge##Boids", &drawData.wrapAround);

        ImGui::Checkbox("Show Direction Vector", &drawData.drawDirection);
        
        ImGui::Checkbox("Show Alignment Vector", &drawData.drawAlignment);
        ImGui::SliderFloat("Alignment Multiplier##Slider", &drawData.alignmentMult, 0.0, 1.0);
        static char alignBuff[32] = ""; 
        ImGui::InputText("Alignment Multiplier##Text Box", alignBuff, 32);
        std::string test = alignBuff;
        std::cout<<test<<'\n';
        
        ImGui::Checkbox("Show Cohesion Vector", &drawData.drawCohesion);
        ImGui::SliderFloat("Cohesion Multiplier", &drawData.cohesionMult, 0.0, 1.0);
        
        ImGui::Checkbox("Show Separation Vector", &drawData.drawSeparation);
        ImGui::SliderFloat("Separation Multiplier", &drawData.separationMult, 0.0, 0.05);

        
        if(ImGui::Button("Explode##Boids")){
            for(auto& obj : drawObjects){
                if(std::holds_alternative<Boid>(obj)){
                    std::get<Boid>(obj).resetPos();
                }
            }
        }
    }

    ImGui::End();


    //ImPlot::ShowDemoWindow();
    
    if(this->showFPS){
        ImGui::Begin("Frame Rate", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            ImGui::SetWindowPos(ImVec2(WIDTH - fpsWidgetWidth, 0));
            ImGui::SetWindowSize(ImVec2(fpsWidgetWidth, fpsWidgetHeight));
            time = fpsClock.getElapsedTime().asSeconds();
            time = 1.0 / time;
            fpsClock.restart();
            plott += ImGui::GetIO().DeltaTime;
            frameData.AddPoint(plott, time);
            static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
            if(ImPlot::BeginPlot("##", ImVec2(-1, 150))){
                ImPlot::SetupAxes(NULL, NULL, flags, flags);
                ImPlot::SetupAxisLimits(ImAxis_X1, plott - 10, plott, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1,0,fpsWidgetMaxFPS);
                ImPlot::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 1, 1));
                ImPlot::PlotLine(std::to_string(time).c_str(), &frameData.Data[0].x, &frameData.Data[0].y, frameData.Data.size(), 0, frameData.Offset, 2*sizeof(float));
                ImPlot::PopStyleColor();
                ImPlot::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                frameData.getAvg();
                ImPlot::PlotInfLines(std::to_string(frameData.avg[0]).c_str(), frameData.avg, 1, ImPlotInfLinesFlags_Horizontal);
                ImPlot::PopStyleColor();
                ImPlot::EndPlot();
            }
        ImGui::End();
    }
    ImGui::PopStyleVar(3);
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