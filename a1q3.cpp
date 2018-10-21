#include "pixmap.h"

void colour(PixMap& image, const Pixel& fillColour, int x, int y, Pixel& initialColour);

bool fill(PixMap& image, const Pixel& fillColour, int x, int y){
    //If pixel at x and y location equals to the colour to be filled, return since that specific spot is already that colour.
    if(image.getPixel(x, y) == fillColour){
        return false;
    }
    else{
        Pixel prev = image.getPixel(x, y); //Setting the initial colour to search for.
        colour(image, fillColour, x, y, prev);
        return true;
    }
}

/*
 This function is passed an image, the colour to fill in the image, x and y coordinates and the initial colour, which will be replaces with fillColour.
 */
void colour(PixMap& image, const Pixel& fillColour, int x, int y, Pixel& initialColour){
    //If x and y are less than 0 or equal and more than image's width and height, return will be executed.
    if (x < 0 || x >= image.width() || y < 0 || y >= image.height()){
        return;
    }
    //If the pixel at x and y is different than the initial colour (it found a different colour/border), return.
    else if(image.getPixel(x, y) != initialColour){
        return;
    }
    else{
        //It sets the pixel colour as fillColour at x and y location, then calls itself with the neighbour coordinates, until the base condition is met.
        image.setPixel(fillColour, x, y);
        colour(image, fillColour, x+1, y, initialColour); //Right
        colour(image, fillColour, x-1, y, initialColour); //Left
        colour(image, fillColour, x, y+1, initialColour); //Down
        colour(image, fillColour, x, y-1, initialColour); //Left
    }
}
