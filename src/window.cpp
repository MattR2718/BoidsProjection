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
                case(sf::Keyboard::BackSlash):{
                    camera.tz -= 1;
                    camera.setTrigValues();
                }
                break;
                case(sf::Keyboard::Slash):{
                    camera.tz += 1;
                    camera.setTrigValues();
                }
                break;
                case(sf::Keyboard::Space): {
                    camera.tx = 15;
                    camera.ty = 30;
                    camera.tz = 0;
                    camera.setTrigValues();
                }
                break;
                case(sf::Keyboard::X): {
                    camera.tx = 0;
                    camera.ty = 90;
                    camera.tz = 0;
                    camera.setTrigValues();
                }
                break;
                case(sf::Keyboard::Z): {
                    camera.tx = 0;
                    camera.ty = 0;
                    camera.tz = -90;
                    camera.setTrigValues();
                }
                break;
                case(sf::Keyboard::Y): {
                    camera.tx = 90;
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
    
    //ImGui::ShowDemoWindow();

    //Create ImGui window to contain options
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
    ImGui::Begin("Options", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowPos(ImVec2(0, 0));
    
    ImGui::Checkbox("Show Demo Objects", &drawData.showDemoObjects);
    ImGui::Checkbox("Show FPS Graph", &this->showFPS);

    ImGui::ColorEdit3("Background Colour", (float*)&this->backgroundColour);

    if(ImGui::CollapsingHeader("Camera")){
        ImGui::Checkbox("Auto Rotate X", &camera.autoRotatex);
        ImGui::Checkbox("Auto Rotate Y", &camera.autoRotatey);
        ImGui::Checkbox("Auto Rotate Z", &camera.autoRotatez);

        ImGui::Checkbox("Perspective", &drawData.perspective);
        if(drawData.perspective){
            drawData.camDistance = drawData.camDistanceCache;
            ImGui::SliderInt("Camera Distance", &drawData.camDistance, 0, 5000);
            drawData.camDistanceCache = drawData.camDistance;
        }else{
            drawData.camDistance = 0;
        }
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
    if(ImGui::CollapsingHeader("Boxes")){
        ImGui::Checkbox("Show Boxes", &drawData.showBoxes);
        ImGui::SliderInt("Bounding Box Size", &drawData.boundingBoxSize, 0, 1000);
        ImGui::ColorEdit3("Bounding Box Colour", (float*)&drawData.boundingBoxColour);
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

    if(ImGui::CollapsingHeader("Boids")){
        ImGui::ColorEdit3("Fill##Boids", (float*)&drawData.boidFillColour);
        drawData.boidOutlineColour[0] = drawData.boidFillColour[0];
        drawData.boidOutlineColour[1] = drawData.boidFillColour[1];
        drawData.boidOutlineColour[2] = drawData.boidFillColour[2];
        ImGui::SliderInt("Num Boids##Boids", &drawData.numBoids, 0, 1500);
        ImGui::SliderFloat("Speed Multiplier##Boids", &drawData.boidSpeedMult, 0.0, 1.0);
        ImGui::SliderInt("Point Size##Boid", &drawData.boidSize, 0, 20);
        
        ImGui::Checkbox("Wrap Around at Edge##Boids", &drawData.wrapAround);

        ImGui::Checkbox("Show Direction Vector", &drawData.drawDirection);
        
        //Behaviour Rules Data
        //Alignment
        ImGui::TextColored(ImVec4(0.0, 0.0, 1.0, 1.0), "Alignment");
        ImGui::PushItemWidth(100);
        //Checkbox for whether to show the alignment vector
        ImGui::Checkbox("Show Vector##Alignment", &drawData.drawAlignment);
        //SLider to allow input of multiplier
        ImGui::SliderFloat("##AlignmentMultiplierSlider", &drawData.alignmentMult, 0.0, 1.0);
        //Buffer to store text input
        static char alignBuff[32] = "";
        ImGui::SameLine();
        //Runs whenever text in text box is changed
        if(ImGui::InputText("Multiplier##Alignment Text Box", alignBuff, 32)){
            std::string alignString = alignBuff;
            //Sanitise input
            sanitise(alignString);
            //Check whether valid floating point number and if so converts
            if(floatingPointConversionCheck(alignString)){
                float alignFloat = std::stof(alignString);
                //Checks whether input is within the valid ranges,
                //if so then it sets the multiplier
                if(rangeCheck(alignFloat)){
                    drawData.alignmentMult = alignFloat;
                }
            }
        }
        ImGui::PopItemWidth();

        //Cohesion
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Cohesion");
        ImGui::PushItemWidth(100);
        ImGui::Checkbox("Show Vector##Cohesion", &drawData.drawCohesion);
        ImGui::SliderFloat("##Cohesion Multiplier Slider", &drawData.cohesionMult, 0.0, 1.0);
        static char cohesionBuff[32] = "";
        ImGui::SameLine();
        if(ImGui::InputText("Multiplier##Cohesion Text Box", cohesionBuff, 32)){
            std::string cohesionString = cohesionBuff;
            sanitise(cohesionString);
            if(floatingPointConversionCheck(cohesionString)){
                float cohesionFloat = std::stof(cohesionString);
                if(rangeCheck(cohesionFloat)){
                    drawData.cohesionMult = cohesionFloat;
                }
            }
        }
        ImGui::PopItemWidth();

        //Separation
        ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "Separation");
        ImGui::PushItemWidth(100);
        ImGui::Checkbox("Show Vector##Separation", &drawData.drawSeparation);
        ImGui::SliderFloat("##MultiplierSeparation", &drawData.separationMult, 0.0, 0.05);
        static char separationBuff[32] = "";
        ImGui::SameLine();
        if(ImGui::InputText("Multiplier##Separation Text Box", separationBuff, 32)){
            std::string separationString = separationBuff;
            sanitise(separationString);
            if(floatingPointConversionCheck(separationString)){
                float separationFloat = std::stof(separationString);
                if(rangeCheck(separationFloat)){
                    drawData.separationMult = separationFloat;
                }
            }
        }
        ImGui::PopItemWidth();
        
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