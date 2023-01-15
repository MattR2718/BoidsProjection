#ifndef DRAWABLEDATA_H
#define DRAWABLEDATA_H

#include <iostream>
#include <vector>
#include <variant>

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

using DrawVariantVector = std::vector<std::variant<Drawable, Point, Line, Box, Vector>>;
class Window;

class DrawableData{
    public:
        //Colours for drawable objects
        float pointFillColour[3] = { 1, 1, 1 };
        float pointOutlineColour[3] = { 0.5, 0.5, 0.5 };
        //Store whether to fill points
        bool fill = false;

        //Default drawing each drawable
        bool showPoints = true, showLines = true, showBoxes = true, showVectors = true;

        //Number of each object to randomly plot
        int numPoints = 100;
        int numBoxes = 100;

        //Size of bounding box
        int boxSize = 300;

        //Draw points at end of lines which mark the start and end
        bool drawLinePoints = false;

        //Antialiasing option
        bool lineAntiAliasing = false;
        bool boxAntiAliasing = false;
        bool vectorAntiAliasing = false;

        void populateDrawPoints(DrawVariantVector& drawObjects, int pointCount, const int numPoints, const int WIDTH, const int HEIGHT);
        void populateDrawBox(DrawVariantVector& drawObjects, int boxCount, const int numBoxes, const int WIDTH, const int HEIGHT);

        void drawAllObjectsToScreen(DrawVariantVector& drawObjects, sf::Uint8* pixels, Window& window, const Camera& camera, int& pointCount, int& boxCount);

    private:

    protected:

};

#endif