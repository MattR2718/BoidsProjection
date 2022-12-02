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