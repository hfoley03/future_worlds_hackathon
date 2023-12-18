#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    ofBackground(0);
    ofSetCircleResolution(100);
//    oscOut.setup("localhost", 7331);
//    oscIn.setup(7331);
    
    createLandmasses();
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

//    ofNoFill();
    ofSetColor(200, 200, 200);
    ofDrawCircle(imgWidth/2 + 100 , imgHeight/2 + 100, imgWidth/2 - 10);



    
    
    ofSetColor(0, 0, 0, 255);
    
    for (int i = 0; i < landmasses.size(); i++){
        landmasses[i].draw();
        
//        ofMesh theMesh;
//        ofTessellator tess;
//        // **** convert poly to mesh ****
//        tess.tessellateToMesh(landmasses[i], ofPolyWindingMode::OF_POLY_WINDING_ODD, theMesh, true);
//        theMesh.draw();
        ofDrawBitmapStringHighlight(ofToString(i), landmasses[i].getCentroid2D().x, landmasses[i].getCentroid2D().y);
    }
    
    
    ofSetColor(255, 0, 0);
//    ofDrawCircle(300,300, 10);
//    for (int k = 0; k < landmasses.size(); k++)
//    {
//        bool checkIfInside = landmasses[k].inside(300, 300);
//        if(checkIfInside){
//            std::cout<<"HIT: "<< k <<std::endl;
//        }
//    }
    ofPolyline worldCircle;
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0, 255, 0, 20);
    for(int i = 0; i < ofGetWidth(); i = i + 2)
    {
        for(int j = 0; j< ofGetHeight(); j = j +2)
        {
            ofSetColor(0, 0, 0, 255);
            if( ofDist(i,j,imgWidth/2 + 100 , imgHeight/2 + 100) <= (imgWidth/2 - 10)){
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
                    ofSetColor(0, 255, 0, 50);
                }
                if(foundInLandmass==1){
                    ofSetColor(255, 255, 255);
                }
            }
            ofDrawRectangle(i, j, 2, 2);
        }
    }
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
                polyline.addVertex(point.x + 100, point.y + 100);
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
