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

	//Colors for my shapes
	Color8u circleColor;
	Color8u rectColor;

	// increments as the program runs and keeps track of time
	float time_;
	int circleTime_;

	static const int kAppWidth = 800;
	static const int kAppHeight = 600;
	static const int kTextureSize = 1024;
	
	void drawRectangle(uint8_t* pixels, int x, int y, int rectWidth, int rectHeight, Color8u fillColor);
	void drawCircle(uint8_t* pixels, int centerX, int centerY, int radius, Color8u fillColor);
	void copyRectangularSection(uint8_t* pixels, int startX, int startY, int width, int height, int endX, int endY);
	void drawCircleTint(Color8u firstColor, Color8u tintColor);
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
			float doubleY = y+0.0;
			float doubleX = x+0.0;

			if(((doubleX-centerX)*(doubleX-centerX) + (doubleY-centerY)*(doubleY-centerY) <= radius*radius)){
				pixels[3*(x + y*kTextureSize)] = fillColor.r;
				pixels[3*(x + y*kTextureSize)+1] = fillColor.g;
				pixels[3*(x + y*kTextureSize)+2] = fillColor.b;
			}
		}
	}
}

void CatPictureApp::drawCircleTint(Color8u firstColor, Color8u tintColor){
	for(int i = 0; i < 1024*1024; i++){
		firstColor.r = (firstColor.r + tintColor.r)%255;
		firstColor.g = (firstColor.g + tintColor.g)%255;
		firstColor.b = (firstColor.b + tintColor.b)%255;
	}
	circleColor = firstColor;
}

void CatPictureApp::copyRectangularSection(uint8_t* pixels, int startX, int startY, int width, int height, int endX, int endY){
	for(int i = startY; i<startY+height; i++){
		for(int j = startX; j<startX+width; j++){
			pixels[3*(j+height + (i+width)*kTextureSize)] = pixels[3*(j+i*kTextureSize)];
			pixels[3*(j+height + (i+width)*kTextureSize)+1] = pixels[3*(j+i*kTextureSize)+1];
			pixels[3*(j+height + (i+width)*kTextureSize)+2] = pixels[3*(j+i*kTextureSize)+2];
		}
	}
}

void CatPictureApp::setup()
{
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	circleTime_ = 0;
	time_ = 0.0;
	rectColor = Color8u(0, 139, 57);
	circleColor = Color8u(255, 255, 255);
}

void CatPictureApp::mouseDown( MouseEvent event ){
	
	drawCircleTint(circleColor, Color8u(13, 27, 31));

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

	circleTime_ += 5;

	drawCircle(dataArray, circleTime_%kAppWidth, circleTime_%kAppHeight, 100, circleColor);
	
	drawRectangle(dataArray, 50, 100, 200, 100, rectColor);

	copyRectangularSection(dataArray, 200, 150, 150, 150, 400, 300);
}

void CatPictureApp::draw()
{

	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
