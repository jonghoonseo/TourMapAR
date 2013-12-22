#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxHistogram.hpp"

#include "parameters.h"

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

		vector<ofImage> imagesExperiment[NUMBER_OF_DB_IMAGES];
		ofImage	imagesTrain[NUMBER_OF_DB_IMAGES];

		// 현재 영상 포인터
		ofImage * imageCurrentExperiment;
		ofImage * imageCurrentTrain;

		// 이미지 인덱스
		int	indexTrain;
		int indexExperimentClass;
		int indexExperimentNo;

		bool processDone;

		// 최종 결과 저장할 히스토그램
		ofxHistogram histogram;


		// Matching
		//------------------------------
		cv::BRISK	*brisk;
		cv::FREAK	*freak;

		vector<cv::KeyPoint> 	keyPointsTrainByBRISK;
		vector<cv::KeyPoint> 	keyPointsTrainByFREAK;
		cv::Mat					descriptorsTrainByBRISK;
		cv::Mat					descriptorsTrainByFREAK;

		vector<cv::KeyPoint>	keyPointsExperimentByBRISK;
		vector<cv::KeyPoint>	keyPointsExperimentByFREAK;
		cv::Mat					descriptorsExperimentByBRISK;
		cv::Mat					descriptorsExperimentByFREAK;

		void computeDescriptors(ofImage * image, vector<cv::KeyPoint> * keyPointsByBRISK, vector<cv::KeyPoint> * keyPointsByFREAK, cv::Mat * descriptorsByBRISK, cv::Mat * descriptorsByFREAK);
		void match();
};
