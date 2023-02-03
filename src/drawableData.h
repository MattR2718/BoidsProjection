#ifndef DRAWABLEDATA_H
#define DRAWABLEDATA_H

#include <iostream>
#include <vector>
#include <variant>
#include <functional>


#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "drawableData.h"
#include "camera.h"
#include "window.h"

#include "drawable.h"
#include "point.h"
#include "line.h"
#include "box.h"
#include "vect.h"
#include "boid.h"

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector, Boid>>;

class Window;

class DrawableData{
    public:
        //Colours for drawable objects
        float pointFillColour[3] = { 1, 1, 1 };
        float boidFillColour[3] = {0, 1, 0};
        float pointOutlineColour[3] = { 0.5, 0.5, 0.5 };
        float boidOutlineColour[3] = {0.5, 0.5, 0.5};
        //Store whether to fill points
        bool fill = false;
        bool fillBoids = true;

        //Default drawing each drawable
        bool showPoints = true, showLines = true, showBoxes = true, showVectors = true;
        bool showBoids = true;

        //Draw random points and boxes
        bool showDemoObjects = false;

        //Number of each object to randomly plot
        int numPoints = 100;
        int numBoxes = 20;
        int numBoids = 250;

        //Size of bounding box
        int boundingBoxSize = 750;

        //Draw points at end of lines which mark the start and end
        bool drawLinePoints = false;

        //Antialiasing option
        bool lineAntiAliasing = false;
        bool boxAntiAliasing = false;
        bool vectorAntiAliasing = false;

        //Whether to draw behaviour vectors
        bool drawDirection = true;
        bool drawCohesion = true;
        bool drawAlignment = false;
        bool drawSeparation = true;

        float boidSpeedMult = 1.0;
        int boidSize = 10;

        float cohesionMult = 0.1;
        float alignmentMult = 0.75;
        float separationMult = 0.005;

        bool wrapAround = false;

        void populateDrawPoints(DrawVariantVector& drawObjects, int pointCount, const int numPoints, const int WIDTH, const int HEIGHT);
        void populateDrawBox(DrawVariantVector& drawObjects, int boxCount, const int numBoxes, const int WIDTH, const int HEIGHT);
        void populateBoids(DrawVariantVector& drawObjects, int boidCount, const int numBoids, const int WIDTH, const int HEIGHT);

        void drawAllObjectsToScreen(DrawVariantVector& drawObjects, sf::Uint8* pixels, Window& window, const Camera& camera, int& pointCount, int& boxCount, int& boidCount);

        void updateBoids(DrawVariantVector& drawObjects, Window& window);

    private:

    protected:

};

#endif