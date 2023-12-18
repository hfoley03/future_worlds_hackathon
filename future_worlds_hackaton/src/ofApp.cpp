#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    startYear = 1900;
    endYear = 2100;
    ofBackground(0);
    ofSetCircleResolution(100);
    oscOut.setup("localhost", 7331);    //OSC
    oscIn.setup(7331);                  //OSC
    offsetX = centreW/2;
    offsetY = centreH/2;
    createLandmasses(); // only calling this in setup to save processing power, should be called everytime window is resized, not important for now
}

//--------------------------------------------------------------
void ofApp::update(){
    //    sendOsc();
    //    receiveOsc();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetRectMode(OF_RECTMODE_CORNER);

//    centreH = ofGetHeight()/2;
//    centreW = ofGetWidth()/2;
//    ofDrawBitmapStringHighlight("timeSent " + ofToString(timeSent, 3) , 50, 50);
      ofDrawBitmapStringHighlight("point " + ofToString(mouseX) + " " + ofToString(mouseY) , 50, 50);


//    ofNoFill();
    ofSetColor(200, 200, 200);
//    ofDrawCircle(imgWidth/2 + offsetX , imgHeight/2 + offsetY, imgWidth/2 - 10);



    
    
    ofSetColor(0, 0, 0, 255);
    
//    for (int i = 0; i < landmasses.size(); i++){
//        landmasses[i].draw(); // Draws outline of landmasses
////        ofMesh theMesh;
////        ofTessellator tess;
////        // **** convert poly to mesh ****
////        tess.tessellateToMesh(landmasses[i], ofPolyWindingMode::OF_POLY_WINDING_ODD, theMesh, true);
////        theMesh.draw();
//        ofDrawBitmapStringHighlight(ofToString(i), landmasses[i].getCentroid2D().x, landmasses[i].getCentroid2D().y);
//    }
    
        ofPolyline worldCircle;
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255, 0, 0, 255);

    // i and j start add weird values becasue we only care about pixels around earth, this can be optimised
    for(int i = 250; i < 700; i = i + 5)
    {
        for(int j = 180; j< 630; j = j + 5)
        {
            if( ofDist(i,j,imgWidth/2 + offsetX , imgHeight/2 + offsetY) <= (imgWidth/2 - 10)){
                ofSetColor(0, 0, 255, 200);
                bool checkIfInside = false;
                bool notFound = false;
                int foundInLandmass = -1;
                while(checkIfInside == false && notFound == false){
                    for (int k = 0; k < landmasses.size(); k++)
                    {
                        checkIfInside = landmasses[k].inside(i, j);
                        if(checkIfInside){
                            foundInLandmass = k;
                            break;
                        }
                    }
                    notFound = true;
                }
                
                
                if(notFound){
                    //ofSetColor(0, 0, 0, 50);
                }
                if(checkIfInside){
                    ofSetColor(0, 255, 0, 200);
                }
                if(foundInLandmass==1){ //icecap
                    ofSetColor(255, 255, 255);
                }
                ofDrawRectangle(i, j, 4, 4);

            }
//            else {
//                if(ofRandom(1.0)>0.999){
//                    ofSetColor( 255, 235, 209);
//                    ofDrawRectangle(i, j, 2, 2);
//                }
//            }
//            ofDrawRectangle(i, j, 2, 2);
        }
    }
    
    ofSetColor(255, 0, 0, 255);
    ofDrawRectangle(320, 395, 6, 6); //LA
    ofDrawRectangle(495, 385, 6, 6); //NYC
    ofDrawRectangle(375, 479, 6, 6); //Mexico;



}

void ofApp::squares(ofPolyline polyline){
    
}

void ofApp::createLandmasses(){
    worldIcon.load("images/worldGraphic4.png");
    worldIcon.resize(445, 438);
    worldIcon.setImageType(OF_IMAGE_GRAYSCALE);
    imgWidth = worldIcon.getWidth();
    imgHeight = worldIcon.getHeight();
    std::cout<< imgWidth << std::endl;
    std::cout<< imgHeight << std::endl;

    grayImage.allocate(imgWidth,imgHeight);


    thresholdValue = 168;
    thresh = 0.0f;
    
    grayImage.setFromPixels(worldIcon.getPixels());
    grayImage.threshold(168);
    contourFinder.findContours(grayImage, 100, 45000, 10, true);
    
    int numBlobs = contourFinder.nBlobs;
    for (int i=0; i<numBlobs; i++){
        
        if(i!=1 && i !=3){
            ofPolyline polyline;
            for(auto & point : contourFinder.blobs[i].pts) {
                polyline.addVertex(point.x + offsetX, point.y + offsetY);
            }
            polyline.close();
            polyline.simplify();
            landmasses.push_back(polyline);
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
