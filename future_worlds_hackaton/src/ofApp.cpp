#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    ofBackground(0);
    ofSetCircleResolution(100);
//    oscOut.setup("localhost", 7331);
//    oscIn.setup(7331);
    
    worldIcon.load("images/worldGraphic4.png");
    worldIcon.resize(445, 438);
    worldIcon.setImageType(OF_IMAGE_GRAYSCALE);
    imgWidth = worldIcon.getWidth();
    imgHeight = worldIcon.getHeight();
    std::cout<< imgWidth << std::endl;
    std::cout<< imgHeight << std::endl;

    colorImg.allocate(imgWidth,imgHeight);
    grayImage.allocate(imgWidth,imgHeight);
    grayBg.allocate(imgWidth,imgHeight);
    grayDiff.allocate(imgWidth,imgHeight);

    thresholdValue = 168;
    thresh = 0.0f;
    
    grayImage.setFromPixels(worldIcon.getPixels());
    grayImage.threshold(168);
    contourFinder.findContours(grayImage, 100, 45000, 10, true);
}

//--------------------------------------------------------------
void ofApp::update(){
    //    sendOsc();
    //    receiveOsc();
    
//    thresholdValue = (int)(255* (mouseX/ofGetWidth()));
    
//    thresh =  255*(float)mouseX / (float)ofGetWidth();
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
//    ofDrawEllipse(centreW, centreH, 200, 200);
//    ofDrawBitmapStringHighlight("timeSent " + ofToString(timeSent, 3) , 50, 50);
//    ofDrawBitmapStringHighlight("timeRec " + ofToString(timeReceived, 3) , 50, 90);
//    ofDrawBitmapStringHighlight("Between " + ofToString(timeSent - timeReceived, 3) , 50, 120);
    
//    ofDrawBitmapStringHighlight("threshold " + ofToString(thresh, 3) , 50, 50);
    ofNoFill();
    ofDrawCircle(imgWidth/2 + 100 , imgHeight/2 + 100, imgWidth/2 - 5);
    int numBlobs = contourFinder.nBlobs;
    for (int i=0; i<numBlobs; i++){
        
        if(i!=1 && i !=3){
            
            //contourFinder.blobs[i].draw(0,0);
            int num = contourFinder.blobs[i].nPts;
            ofSetColor(255, 100);

            ofDrawBitmapStringHighlight(ofToString(i), contourFinder.blobs[i].centroid.x + 100, contourFinder.blobs[i].centroid.y + 100);
            
            ofPolyline polyline;
            ofSetColor(255, 255, 0);

            // Add points to the polyline
            for(auto & point : contourFinder.blobs[i].pts) {
                polyline.addVertex(point.x + 100, point.y + 100);
//                ofDrawCircle(point, 1);
            }
            ofSetColor(255, 255, 255);
            polyline.close();
            polyline.draw();
            
            
        }
    }
    
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
