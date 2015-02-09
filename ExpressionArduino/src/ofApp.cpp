#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);


    serial.setup("COM4", 9600);
	serial.startContinuousRead(false);
	ofAddListener(serial.NEW_MESSAGE,this,&testApp::onNewMessage);

	message = "";
	remember = false;
    oldMessage = "999";

	tracker.setup();
	tracker.setRescale(.5);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		if(tracker.update(toCv(cam))) {
			classifier.classify(tracker);
		}
	}

	if(message != "" && remember == false && message!=oldMessage)
	 {
		cout << "sending message: " << message << "\n";
		serial.writeString(message);
		oldMessage=message;
		message="";
	 }
}

void testApp::onNewMessage(string & message)
{
	cout << "onNewMessage, message: " << message << "\n";

}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	tracker.draw();

	int w = 100, h = 12;
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(5, 10);
	int n = classifier.size();
	int primary = classifier.getPrimaryExpression();

for(int i = 0; i<n; i++){
    if(i==primary){
        string test = classifier.getDescription(i);
        message += test;
    }
}



  for(int i = 0; i < n; i++){
		ofSetColor(i == primary ? ofColor::red : ofColor::black);
		ofRect(0, 0, w * classifier.getProbability(i) + .5, h);
		ofSetColor(255);
		ofDrawBitmapString(classifier.getDescription(i), 5, 9);
		ofTranslate(0, h + 5);
  }
	ofPopMatrix();
	ofPopStyle();

	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
	drawHighlightString(
		string() +
		"r - reset\n" +
		"e - add expression\n" +
		"a - add sample\n" +
		"s - save expressions\n"
		"l - load expressions",
		14, ofGetHeight() - 7 * 12);
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
		classifier.reset();
	}
	if(key == 'e') {
		classifier.addExpression();
	}
	if(key == 'a') {
		classifier.addSample(tracker);
	}
	if(key == 's') {
		classifier.save("expressions");
	}
	if(key == 'l') {
		classifier.load("expressions");
	}
}
