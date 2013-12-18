#pragma once

#include "ofMain.h"

#include "ofxHistogram.hpp"

#define NUMBER_OF_DB_IMAGES	3

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// we will have a dynamic number of images, based on the content of a directory:
		ofDirectory dir;

		vector<ofImage> experimental_images[NUMBER_OF_DB_IMAGES];
		ofImage	train_images[NUMBER_OF_DB_IMAGES];

		ofImage *currentImage;
		int currentImageIndex;

		ofxHistogram histogram;

};