#include "drawableData.h"


void DrawableData::populateDrawPoints(DrawVariantVector& drawObjects, int pointCount, const int numPoints, const int WIDTH, const int HEIGHT){
    if(pointCount < numPoints){
        for(int i = pointCount; i <= numPoints; i++){
            drawObjects.push_back(Point(
                rand() % (WIDTH - 400) - WIDTH / 2 + 200,
                rand() % (HEIGHT - 400) - HEIGHT / 2 + 200,
                rand() % (WIDTH - 400) - WIDTH / 2 + 200,
                WIDTH, HEIGHT,
                rand() % 30

            ));
        }
    }else if (pointCount > numPoints){
        int ind = 0;
        while((ind < drawObjects.size()) && (pointCount > numPoints)){
            if(std::holds_alternative<Point>(drawObjects[ind])){
                drawObjects.erase(drawObjects.begin() + ind);
                pointCount--;
            }else{ ind++; }
        }
    }
}


void DrawableData::populateDrawBox(DrawVariantVector& drawObjects, int boxCount, const int numBoxes, const int WIDTH, const int HEIGHT){
    if(boxCount < numBoxes){
        for(int i = boxCount; i <= numBoxes; i++){
            drawObjects.push_back(Box(
                rand() % (WIDTH - 400) - WIDTH / 2 + 200,
                rand() % (HEIGHT - 400) - HEIGHT / 2 + 200,
                rand() % (WIDTH - 400) - WIDTH / 2 + 200,
                rand() % 60,
                WIDTH, HEIGHT,
                this->boxAntiAliasing,
                true,
                rand() % 255, rand() % 255, rand() % 255
            ));
        }
    }else if (boxCount > numBoxes){
        int ind = 0;
        while((ind < drawObjects.size()) && (boxCount > numBoxes)){
            if(std::holds_alternative<Box>(drawObjects[ind]) && !std::get<3>(drawObjects[ind]).atOrigin()){
                drawObjects.erase(drawObjects.begin() + ind);
                boxCount--;
            }else{ ind++; }
        }
    }
}


void DrawableData::populateBoids(DrawVariantVector& drawObjects, int boidCount, const int numBoids, const int WIDTH, const int HEIGHT){
    if(boidCount < numBoids){
        for(int i = boidCount; i <= numBoids; i++){
            drawObjects.push_back(Boid(0, 0, 0,
                WIDTH, HEIGHT
            ));
        }
    }else if (boidCount > numBoids){
        int ind = 0;
        while((ind < drawObjects.size()) && (boidCount > numBoids)){
            if(std::holds_alternative<Boid>(drawObjects[ind])){
                drawObjects.erase(drawObjects.begin() + ind);
                boidCount--;
            }else{ ind++; }
        }
    }
}






void DrawableData::drawAllObjectsToScreen(DrawVariantVector& drawObjects, sf::Uint8* pixels, Window& window, const Camera& camera, int& pointCount, int& boxCount, int& boidCount){
    
    //Draw all objects to screen
    for(auto& obj : drawObjects){
        switch(obj.index()){
            case 0:{ //Drawable
                break;
            }
            case 1:{ //Point
                pointCount++;
                if(this->showPoints){
                    std::get<Point>(obj).quickDraw(pixels, window.WIDTH, window.HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, this->pointFillColour, this->pointOutlineColour, this->fill);
                }
                break;
            }
            case 2:{ //Line
                if(this->showLines){
                    std::get<Line>(obj).quickDraw(pixels, window.WIDTH, window.HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, this->drawLinePoints);
                }
                break;
            };
            case 3:{ //Box
                boxCount++;
                if(this->showBoxes){
                    std::get<Box>(obj).draw(pixels, window.WIDTH, window.HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, this->drawLinePoints, this->boundingBoxSize);
                }
                break;
            }
            case 4:{ //Vector
                if(std::get<Vector>(obj).moveVector){
                    std::get<Vector>(obj).move(window.WIDTH, window.HEIGHT);
                }
                if(this->showVectors){
                    std::get<Vector>(obj).draw(pixels, window.WIDTH, window.HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions);
                }
                break;
            }
            case 5:{ //Boid
                boidCount++;
                if(this->showBoids){
                    std::get<Boid>(obj).quickDraw(pixels, window.WIDTH, window.HEIGHT, camera.tx, camera.ty, camera.tz, camera.trigFunctions, this->boidFillColour, this->boidOutlineColour, this->fillBoids, this->drawCohesion, this->drawAlignment, this->drawSeparation, this->drawDirection);
                }
                break;
            }
            default:{
                std::cout<<"UNKNOWN TYPE IN VARIANT, INDEX = "<<obj.index()<<'\n';
                break;
            }
        }
    }
}

void DrawableData::updateBoids(DrawVariantVector& drawObjects){
    for(auto& obj : drawObjects){
        if(obj.index() == 5){ //Boid
            auto& boid = std::get<Boid>(obj);
            boid.boundCheck(this->boundingBoxSize);
            boid.updateSpeedMult(this->boidSpeedMult);
            boid.setRadius(this->boidSize);
        }
    }
}