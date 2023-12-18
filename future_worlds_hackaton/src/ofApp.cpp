#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    ofBackground(0);
    ofSetCircleResolution(100);
    oscOut.setup("localhost", 7331);
    oscIn.setup(7331);
}

//--------------------------------------------------------------
void ofApp::update(){
    sendOsc();
    receiveOsc();
}

//--------------------------------------------------------------
void ofApp::draw(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    ofDrawEllipse(centreW, centreH, 200, 200);
    ofDrawBitmapStringHighlight("timeSent " + ofToString(timeSent, 3) , 50, 50);
    ofDrawBitmapStringHighlight("timeRec " + ofToString(timeReceived, 3) , 50, 90);
    ofDrawBitmapStringHighlight("Between " + ofToString(timeSent - timeReceived, 3) , 50, 120);


    
}

void ofApp::sendOsc(){
    timeSent = ofGetElapsedTimef();
    ofxOscMessage m;
    m.setAddress("/time");
    m.addFloatArg(timeSent);
    oscOut.sendMessage(m);
}

void ofApp::receiveOsc(){
    while(oscIn.hasWaitingMessages()){
        ofxOscMessage msg;
        oscIn.getNextMessage(msg);
        if (msg.getAddress() == "/time") {
            timeReceived = msg.getArgAsFloat(0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
