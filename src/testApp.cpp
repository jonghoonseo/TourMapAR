#include "testApp.h"

#define IsGenuine	((indexTrain==indexExperimentClass) ? true : false)



//--------------------------------------------------------------
void testApp::setup(){
	char path[100];
	for(int i=0; i<NUMBER_OF_DB_IMAGES; ++i)
	{
		sprintf(path, "train/train_%d.jpg", i);
		imagesTrain[i].loadImage(path);

		sprintf(path, "experiment/image_%d/", i);
		dir.listDir(path);
		dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

		//allocate the vector to have as many ofImages as files
		if( dir.size() )
			imagesExperiment[i].assign(dir.size(), ofImage());
		
		// you can now iterate through the files and load them into the ofImage vector
		for(int j = 0; j < (int)dir.size(); j++){
			imagesExperiment[i][j].loadImage(dir.getPath(j));
		}
	}

	// 매칭 초기화
	//------------------------------
	brisk = new cv::BRISK(40,5);
	freak = new cv::FREAK(40,5);


	// 인덱스 초기화
	//------------------------------
	indexExperimentClass = 0;
	indexExperimentNo = 0;
	indexTrain = 0;

	imageCurrentTrain		= &( imagesTrain[0] );
	imageCurrentExperiment	= &( imagesExperiment[0][0] );

	computeDescriptors(imagesTrain, &keyPointsTrainByBRISK, &keyPointsTrainByFREAK, &descriptorsTrainByBRISK, &descriptorsTrainByFREAK);
	
	processDone = false;
}

//--------------------------------------------------------------
void testApp::update(){
	for(int i=0; i<100; ++i)
	{
		histogram.accumulateAValue(10);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(200, 0);
	//ofSetColor(ofColor::yellow);
	//histogram.draw();

	if(!processDone)
	{
		imageCurrentTrain->draw(0,0);
		imageCurrentExperiment->draw(imageCurrentTrain->width, 0);

		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(5);
		if(IsGenuine)
			ofSetColor(ofColor::green);
		else
			ofSetColor(ofColor::red);
		ofRect(imageCurrentTrain->width, 0, imageCurrentExperiment->width, imageCurrentExperiment->height);
		ofPopStyle();

		// Keypoints 그리기
		//------------------------------
		ofPushStyle();
		ofFill();
		ofSetColor(ofColor::blue);
		for(int i=0; i<keyPointsTrainByBRISK.size(); ++i)
			ofCircle(ofPoint(keyPointsTrainByBRISK[i].pt.x, keyPointsTrainByBRISK[i].pt.y), 3);
		
		ofTranslate(imageCurrentTrain->getWidth(), 0, 0);
		ofSetColor(ofColor::yellow);
		for(int i=0; i<keyPointsExperimentByBRISK.size(); ++i)
			ofCircle(ofPoint(keyPointsExperimentByBRISK[i].pt.x, keyPointsExperimentByBRISK[i].pt.y), 3);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' ')
	{
		if(!processDone)
		{
			indexExperimentNo++;
			if(indexExperimentNo >= imagesExperiment[indexExperimentClass].size())
			{
				indexExperimentClass++;
				indexExperimentNo = 0;

				if(indexExperimentClass >= NUMBER_OF_DB_IMAGES)
				{
					indexTrain++;
					indexExperimentClass = 0;

					if(indexTrain >= NUMBER_OF_DB_IMAGES)
						processDone = true;
					else
					{
						// Train 영상에서 Keypoint/Descriptor 생성
						ofLogVerbose("testApp", "Train 영상 Keypoint/Descriptor 생성");
						computeDescriptors(&(imagesTrain[indexTrain]), &keyPointsTrainByBRISK, &keyPointsTrainByFREAK, &descriptorsTrainByBRISK, &descriptorsTrainByFREAK);
					}
				}
			}
			// 현재 영상 수정
			imageCurrentTrain		= &( imagesTrain[indexTrain] );
			imageCurrentExperiment	= &( imagesExperiment[indexExperimentClass][indexExperimentNo] );

			ofLogVerbose("keyPressed", "실험 영상 KeyPoint / Descriptor 생성");
			computeDescriptors( imageCurrentExperiment, &keyPointsExperimentByBRISK, &keyPointsExperimentByFREAK, &descriptorsExperimentByBRISK, &descriptorsExperimentByFREAK );

			match();
		}
	}
	if(key == 'f')
		ofToggleFullscreen();
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::computeDescriptors(ofImage * image, vector<cv::KeyPoint> * keyPointsByBRISK, vector<cv::KeyPoint> * keyPointsByFREAK
												, cv::Mat * descriptorsByBRISK, cv::Mat * descriptorsByFREAK)
{
	// ofImage -> OpenCV Mat (BGR)
	//------------------------------
	cv::Mat mat(image->getHeight(), image->getWidth(), CV_8UC3, image->getPixels());
	cv::cvtColor(mat, mat, CV_RGB2BGR);

	// OpenCV Mat(BGR) -> OpenCV Mat(Gray)
	cv::Mat mat_gray(mat.size(), CV_8U);
	cv::cvtColor(mat, mat_gray, CV_BGR2GRAY);

	//cv::imshow("test", mat_gray);
	//cv::waitKey(0);

	// 키포인트 찾고 디스크립터 계산
	//------------------------------
	keyPointsByBRISK->clear();
	keyPointsByFREAK->clear();
	vector<cv::KeyPoint>	kp, kpByBRISK, kpByFREAK;
	brisk->detect(mat_gray, kp);

	kpByBRISK = kp;
	brisk->compute(mat_gray, kpByBRISK, *descriptorsByBRISK);
	gcKeypointsRecover(kpByBRISK, *keyPointsByBRISK, kp);

	kpByFREAK = kp;
	freak->compute(mat_gray, kpByFREAK, *descriptorsByFREAK);
	gcKeypointsRecover(kpByFREAK, *keyPointsByFREAK, kp);

	ofLogNotice("computeDescriptors", "BRISK: " + ofToString(keyPointsByBRISK->size()) + "\tFREAK: " + ofToString(keyPointsByFREAK->size()));
}

void testApp::match()
{



}