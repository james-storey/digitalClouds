#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//ofSetFullscreen(true);
	ofBackground(100, 127, 220);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofSetOrientation(OF_ORIENTATION_DEFAULT, true);
	ofSetupScreenPerspective(ofGetWindowWidth(), ofGetWindowHeight(), 60.0, 5.0, 1000.0f);
	ofSetDepthTest(true);
	for(int i = 0; i < 5; i++)
	{
		plane[i] = ofPlanePrimitive(ofGetWidth()*10, ofGetHeight()*15, 100, 150);
		plane[i].setPosition(0.0f, i*-100.0f, -100.0);
		plane[i].rotate(-45.0, 1, 0, 0);
	}

	mesh.setup();
	mesh.setParent(plane[0]);

}

//--------------------------------------------------------------
void testApp::update(){
	mesh.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
	ofSetColor(240);
	//glPointSize(4.0);

	// point to sphere point:  x = s + r*(p-s)/(norm(p-s))

	for(int i = 0; i < 5; i++)
	{
		plane[i].move(0, 0.5, -0.5);
		//plane[i].rotate(-0.1, 1, 0, 0);
		glPointSize(3.0f-(3*i/5.0f));
		//plane[i].drawVertices();
	}
	mesh.draw();
	ofSetColor(255);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
