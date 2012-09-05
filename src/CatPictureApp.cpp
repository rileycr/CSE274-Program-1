#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
private:
	Surface* mySurface_;

	// Variables for the brightness of the pixels
	float red_;
	float green_;
	float blue_;

	// increments as the program runs and keeps track of time
	float time_;

	static const int kAppWidth = 800;
	static const int kAppHeight = 600;
	static const int kTextureSize = 1024;
	
	void drawRectangle(uint8_t* pixels, int x, int y, int rectWidth, int rectHeight, Color8u fillColor);
	void drawCircle(uint8_t* pixels, int centerX, int centerY, int radius, Color8u fillColor);
};

void CatPictureApp::drawRectangle(uint8_t* pixels, int x1, int y1, int rectWidth, int rectHeight, Color8u fillColor){
	int startx = x1;
	int endx = x1 + rectWidth;
	int starty = y1;
	int endy = y1 + rectHeight;

	for(int y = starty; y < endy; y++){
		for(int x = startx; x < endx; x++){
			
			pixels[3*(x + y*kTextureSize)] = fillColor.r;
			pixels[3*(x + y*kTextureSize)+1] = fillColor.g;
			pixels[3*(x + y*kTextureSize)+2] = fillColor.b;
		}
	}
}

void CatPictureApp::drawCircle(uint8_t* pixels, int centerX, int centerY , int radius, Color8u fillColor){
	for(int y=0; y<kAppHeight; y++){
		for(int x=0; x<kAppWidth; x++){
			double doubleY = y+0.0;
			double doubleX = x+0.0;

			if(((doubleX-centerX)*(doubleX-centerX) + (doubleY-centerY)*(doubleY-centerY) <= radius*radius)){
				pixels[3*(x + y*kTextureSize)] = fillColor.r;
				pixels[3*(x + y*kTextureSize)+1] = fillColor.g;
				pixels[3*(x + y*kTextureSize)+2] = fillColor.b;
			}
		}
	}
}


void CatPictureApp::setup()
{
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	time_ = 0.0;
}

void CatPictureApp::mouseDown( MouseEvent event )
{
}

void CatPictureApp::update(){

	time_ = time_ + 0.05;
	red_ = .5*cos(time_/2) + 0.5;
	green_ = .5*cos(time_) + 0.5;
	blue_ = .5*cos(time_*2) + 0.5;	


	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	//Colors the surface's background in changing colors
	for(int i = 0; i < 1024*1024; i++){
			dataArray[3*(i)] = 255*red_;
			dataArray[3*(i)+1] = 255*green_;
			dataArray[3*(i)+2] = 255*blue_;
	}


	Color8u rectColor = Color8u(0, 139, 57);
	Color8u circleColor = Color8u(255, 255, 255);

	drawCircle(dataArray, 300, 300, 100, circleColor);
	drawRectangle(dataArray, 100, 100, 200, 100, rectColor);
}

void CatPictureApp::draw()
{

	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
