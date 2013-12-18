#include "testApp.h"

//	영상 입력받기
//	

//--------------------------------------------------------------
void testApp::setup(){
	char path[100];
	for(int i=0; i<NUMBER_OF_DB_IMAGES; ++i)
	{
		sprintf(path, "train/train_%d.jpg", i);
		train_images[i].loadImage(path);

		sprintf(path, "experiment/image_%d/", i);
		dir.listDir(path);
		dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

		//allocate the vector to have as many ofImages as files
		if( dir.size() )
			experimental_images[i].assign(dir.size(), ofImage());
		
		// you can now iterate through the files and load them into the ofImage vector
		for(int j = 0; j < (int)dir.size(); j++){
			experimental_images[i][j].loadImage(dir.getPath(j));
		}
	}
	currentImageIndex = 0;


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
	//train_images[currentImageIndex].draw(0,0);
	ofSetColor(ofColor::yellow);
	histogram.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' ')
	{
		//currentImageIndex = (currentImageIndex+1) % NUMBER_OF_DB_IMAGES;
		currentImageIndex++;
	}
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

