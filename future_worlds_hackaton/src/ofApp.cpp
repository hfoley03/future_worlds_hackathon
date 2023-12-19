#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    startYear = 1900;
    endYear = 2100;
    orbitAng1 = 0.1;
    orbitAng2 = 0.1;

    orbit1 = centreH*2;
    orbit2 = centreH*2;

    pollutionIncreasing = false;
    populationIncreasing = true;
    extremePollution = false;
    
//    colorSpace.setHsb(0, 255, 0);
//    colorIce.setHsb(0, 0, 255);
//    colorCity.setHsb(255, 255, 255);
//    colorLand.setHsb(100,200,255);
//    colorOcean.setHsb(180,200,255);
    
    ofBackground(0);
    ofSetCircleResolution(100);
    oscOut.setup("localhost", 8000);    //OSC
    oscIn.setup(8000);                  //OSC
    offsetX = centreW/2;
    offsetY = centreH/2;
    createLandmasses(); // only calling this in setup to save processing power, should be called everytime window is resized, not important for now
    createEarthVector();
    
    std::cout << pollutionData[10] << std::endl;
    
    lastMinute = ofGetElapsedTimeMillis();

    
}

//--------------------------------------------------------------
void ofApp::update(){
    sendOsc();
    receiveOsc();
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    orbit1 = centreH*2;
    orbit2 = centreH*2;
    
    if (currentYear < 200){
        now = ofGetElapsedTimeMillis();
        if (now - lastMinute >= 500) { // 60000 milliseconds = 1 minute
            //           cout << "time interval" << endl;
            cellure();
            checkDataDirection();
            currentYear += 1;
            lastMinute = now;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    //PLANETS
    orbitAng1 = ofGetElapsedTimef()*0.2;
    orbitAng2 = ofGetElapsedTimef()*0.1;

    //Mars
    ofSetColor(231,125,17, 200);
    ofDrawEllipse(orbit1*cos(orbitAng1) + centreW, orbit1*sin(orbitAng1) + centreH*2.5, 50, 50);
    //Some other planet
    ofSetColor(79,76,176, 200);
    ofDrawEllipse(orbit2*cos(orbitAng2) + centreW, orbit2*sin(orbitAng2) + centreH*2.75, 30, 30);
    
    ofSetRectMode(OF_RECTMODE_CORNER);

    ofDrawBitmapStringHighlight("point " + ofToString(mouseX) + " " + ofToString(mouseY) , 600, 600);
    ofSetColor(200, 200, 200);
//    ofDrawCircle(imgWidth/2 + offsetX , imgHeight/2 + offsetY, imgWidth/2 - 10);
    
    ofSetColor(0, 0, 0, 255);
    // EARTH
    drawEarthFromVCellTypesVector();
    
    // Draw cities names
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 0, 0, 255);
    ofDrawBitmapStringHighlight("LA", 330, 395);
    ofDrawBitmapStringHighlight("NYC", 505, 385);
    ofDrawBitmapStringHighlight("CDMX", 385, 479);
    ofDrawBitmapStringHighlight("BG", 480, 570);

    

    ofDrawBitmapStringHighlight("Year:       " + ofToString(currentYear + startYear) , 50, 30);

    ofDrawBitmapStringHighlight("Population: " + ofToString(populationData[currentYear]) , 50, 50);
    ofDrawBitmapStringHighlight("Resources:  " + ofToString(resourceData[currentYear]) , 50, 70);
    ofDrawBitmapStringHighlight("Pollution:  " + ofToString(pollutionData[currentYear]) , 50, 90);
    ofDrawBitmapStringHighlight("Industry:   " + ofToString(industryData[currentYear]) , 50, 110);
    ofDrawBitmapStringHighlight("Food:       " + ofToString(foodData[currentYear]) , 50, 130);

    //for debugging only
    ofDrawBitmapStringHighlight("Population Increasing: " + ofToString(populationIncreasing) , 700, 30);
    ofDrawBitmapStringHighlight("Resources increasing:  " + ofToString(resourcesIncreasing) , 700, 50);
    ofDrawBitmapStringHighlight("Pollution increasing:  " + ofToString(pollutionIncreasing) , 700, 70);
    ofDrawBitmapStringHighlight("Industry increasing:   " + ofToString(industryIncreasing) , 700, 90);
    ofDrawBitmapStringHighlight("Food incsreasing:       " + ofToString(foodIncreasing) ,700, 110);
    
    ofDrawBitmapStringHighlight("dick rec:       " + ofToString(timeReceived) ,700, 220);
    ofDrawBitmapStringHighlight("shit betwen:       " + ofToString(timeReceived1) ,700, 250);
    ofDrawBitmapStringHighlight("city counter:       " + ofToString(cityCounter) ,700, 280);


    


    



}

void ofApp::checkDataDirection(){
    int nextYear = currentYear+1;
    if(pollutionData[currentYear] < pollutionData[nextYear] && pollutionData[currentYear] > 0.1){pollutionIncreasing = true;}
    else {pollutionIncreasing = false;}
    
    if(populationData[currentYear] < populationData[nextYear]  && populationData[currentYear] > 0.1){populationIncreasing = true;}
    else {populationIncreasing = false;}
    
    if(foodData[currentYear] < foodData[nextYear]){foodIncreasing = true;}
    else {foodIncreasing = false;}
    
    if(industryData[currentYear] < industryData[nextYear]){industryIncreasing = true;}
    else {industryIncreasing = false;}
    
    if(resourceData[currentYear] < resourceData[nextYear]){resourcesIncreasing = true;}
    else {resourcesIncreasing = false;}
    
    if(pollutionData[currentYear]>0.5){extremePollution = true;}
    else {extremePollution = false;}

}

void ofApp::drawEarthFromVCellTypesVector(){
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255, 0, 0, 255);

    for(int i = 0; i < 90; i = i + 1)
    {
        for(int j = 0; j< 90; j = j + 1)
        {
            int thisCellType = cellTypes[j*90 + i];
//            if (thisCellType == 0){ofSetColor(0, 0, 0);}        //if space
            if (thisCellType == 1){ofSetColor(98,188,47, 200);}      //if land
            if (thisCellType == 2)
            {
                if(i%2 ==0){ofSetColor(0, 120, 255, 150);}
                else {ofSetColor(0, 131, 255, 150);}
                
            }      //if ocean
            if (thisCellType == 3){ofSetColor(255, 255, 255, 150);}  //if ice
            if (thisCellType == 4){ofSetColor(105,103,97, 150);}  //if city

            //fix later
//            if (thisCellType == 0){ofSetColor(colorSpace);}        //if space
//            if (thisCellType == 1){ofSetColor(colorLand);}      //if land
//            if (thisCellType == 2){ofSetColor(colorOcean);}      //if ocean
//            if (thisCellType == 3){ofSetColor(colorIce);}  //if ice
//            if (thisCellType == 4){ofSetColor(colorCity);}  //if city
           
            if (thisCellType != 0){
                ofDrawRectangle( (j*5) + (250), (i*5) + (180), 10, 10); //looks cool with 10,10 and 150 alpha
    //             ofDrawRectangle( (j*5) + (250), (i*5) + (180), 4, 4); //looks cool with 10,10 and 150 alpha
            }        //if space

        }
    }
}

void ofApp::cellure(){ //we'll call this once a year
    
    cityCounter = 0;
    //behaves similar to game of life
    //for each world cell
    //check if the surrounding neighbours meet some condintions then change cell type
    for(int i = 0; i < 90; i = i + 1)
    {
        for(int j = 0; j< 90; j = j + 1)
        {
            if(cellTypes[j*90 + i] > 0){ //if its not space we care about it
                int neighboursIce = 0;
                int neighboursCity = 0;
                int neighboursOcean = 0;
                int neighboursLand = 0;
                
                if (cellTypes[j*90 + i]==4){cityCounter++;}
                
                // iterate around the surrounding cells
                for(int xx = -1; xx <= 1; xx = xx + 1){
                    for(int yy = -90; yy <= 90; yy = yy + 90){

                        int thisType = cellTypes[j*90 + i + xx + yy]; // get the type of the neighbour cell
//                        std::cout << j*90 + i + xx + yy << std::endl;
                        
                        //types of cells
                        //0 = space
                        //1 = land
                        //2 = ocean
                        //3 = ice cap
                        //4 = city

                        if (thisType == 1){ //land
                            if ((j*90 + i + xx + yy) != (j*90 + i)){neighboursLand += 1;}
                        }
                        if (thisType == 2){ //ocean
                            if ((j*90 + i + xx + yy) != (j*90 + i)){neighboursOcean += 1;}
                        }
                        if (thisType == 3){ //ice
                            if ((j*90 + i + xx + yy) != (j*90 + i)){neighboursIce += 1;}
                        }
                        if (thisType == 4){ //city
                            if ((j*90 + i + xx + yy) != (j*90 + i)){neighboursCity += 1;}
                        }
                    }
                }
                if(neighboursIce == 3 && ofRandom(1.0) > 0.5){ //if ive exactly 3 neighbours of ice and then some probability thing
                    if(pollutionIncreasing){cellTypes[j*90 + i] = 2;}
                    else {cellTypes[j*90 + i] = 3;}
                }
                
                if(neighboursCity == 1 && ofRandom(1.0) > 0.95 && cellTypes[j*90 + i] != 2){ //if ive exactly 1 neighbours of city and then some probability thing
                    if(populationIncreasing){cellTypes[j*90 + i] = 4;}
                    else {cellTypes[j*90 + i] = 1;}
                }
                
                if(neighboursOcean == 3 && ofRandom(1.0) > 0.95 && cellTypes[j*90 + i] == 1){ //if ive exactly 1 neighbours of city and then some probability thing
                    if(extremePollution){cellTypes[j*90 + i] = 2;}
                    else {cellTypes[j*90 + i] = 1;}
                }
                
            }
        }
    }
    //To stop cities being able to completely disapeer
    cellTypes[14*90 + 43] = 4;     //LA = 320, 395
    cellTypes[49*90 + 41] = 4;     //NYC = 495 385
    cellTypes[25*90 + 60] = 4;     // MEX = 375, 479
    cellTypes[44*90 + 76] = 4;     // bog = 470, 650

    
    
}

//                 LOGIC OF NEIGHBOURING CELLS
//                cellTypes[j*90 +1 -90 - 1]; // a
//                cellTypes[j*90 +1 -90 + 0]; // b
//                cellTypes[j*90 +1 -90 + 1]; // c
//
//                cellTypes[j*90 +1 - 1]; // d
//                cellTypes[j*90 +1 + 1]; // f
//
//                cellTypes[j*90 +1 +90 - 1]; // g
//                cellTypes[j*90 +1 +90 + 0]; // h
                
//                cellTypes[j*90 +1 +90 + 1]; // i


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

void ofApp::createEarthVector(){
    //0 = space
    //1 = land
    //2 = ocean
    //3 = ice cap
    //4 = city
    
    int cellType = -1;
    
    for(int i = 0; i < 450; i = i + 5)
    {
        for(int j = 0; j< 450; j = j + 5)
        {
            cellType = -1;

            if( ofDist(i+250,j+180,imgWidth/2 + offsetX , imgHeight/2 + offsetY) <= (imgWidth/2 - 10)){
                bool checkIfInside = false;
                bool notFound = false;
                int foundInLandmass = -1;
                while(checkIfInside == false && notFound == false){
                    for (int k = 0; k < landmasses.size(); k++)
                    {
                        checkIfInside = landmasses[k].inside(i+250, j+180);
                        if(checkIfInside){
                            foundInLandmass = k;
                            break;
                        }
                    }
                    notFound = true;
                }
                
                
                if(notFound){ //ocean
                    cellType = 2;

                }
                if(checkIfInside){ //land
                    cellType = 1;
                }
                if(foundInLandmass==1){ //icecap
                    cellType = 3;

                }
            }
            else
            {
                // equals space
                cellType = 0;
            }
            cellTypes.push_back(cellType);
        }
    }
    
    //insert cities here and set to 4
    cellTypes[14*90 + 43] = 4;     //LA = 320, 395         x = 320-250=70 /5 = 14   395-180=215 /5  43
    cellTypes[49*90 + 41] = 4;     //NYC = 495 385
    cellTypes[25*90 + 60] = 4;     // MEX = 375, 479,

}


void ofApp::sendOsc(){
    timeSent = ofGetElapsedTimef();
    ofxOscMessage m1;
    m1.setAddress("/dick/1");
    m1.addFloatArg(0.333);

    oscOut.sendMessage(m1);
    
    ofxOscMessage m2;
    m2.setAddress("/dick/2");
    m2.addFloatArg(0.444);

    oscOut.sendMessage(m2);

}

void ofApp::receiveOsc(){
    while(oscIn.hasWaitingMessages()){
        ofxOscMessage msg;
        oscIn.getNextMessage(msg);
        if (msg.getAddress() == "/dick") {
            timeReceived = msg.getArgAsFloat(ofGetElapsedTimef());
        }
//        if (msg.getAddress() == "/dick") {
//            timeReceived1 = msg.getArgAsFloat(0);
//        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            cellure();
            std::cout << "run cellure automata to update state of cells" << std::endl;
            break;
        case 'p':
            pollutionIncreasing = !pollutionIncreasing;
            extremePollution = !extremePollution;

            std::cout << "pollution state flipped" << std::endl;
            break;
        case 'o':
            populationIncreasing = !populationIncreasing;
            std::cout << "population state flipped" << std::endl;
            break;
    }
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


void ofApp::oldDrawEarth(){
    //    for (int i = 0; i < landmasses.size(); i++){
    //        landmasses[i].draw(); // Draws outline of landmasses
    ////        ofMesh theMesh;
    ////        ofTessellator tess;
    ////        // **** convert poly to mesh ****
    ////        tess.tessellateToMesh(landmasses[i], ofPolyWindingMode::OF_POLY_WINDING_ODD, theMesh, true);
    ////        theMesh.draw();
    //        ofDrawBitmapStringHighlight(ofToString(i), landmasses[i].getCentroid2D().x, landmasses[i].getCentroid2D().y);
    //    }
        
    //        ofPolyline worldCircle;
    //    ofSetRectMode(OF_RECTMODE_CENTER);
    //    ofSetColor(255, 0, 0, 255);

        // i and j start add weird values becasue we only care about pixels around earth, this can be optimised
    //    for(int i = 0; i < 450; i = i + 5)
    //    {
    //        for(int j = 0; j< 450; j = j + 5)
    //        {
    //            if( ofDist(i+250,j+180,imgWidth/2 + offsetX , imgHeight/2 + offsetY) <= (imgWidth/2 - 10)){
    //                ofSetColor(0, 0, 255, 200);
    //                bool checkIfInside = false;
    //                bool notFound = false;
    //                int foundInLandmass = -1;
    //                while(checkIfInside == false && notFound == false){
    //                    for (int k = 0; k < landmasses.size(); k++)
    //                    {
    //                        checkIfInside = landmasses[k].inside(i+250, j+180);
    //                        if(checkIfInside){
    //                            foundInLandmass = k;
    //                            break;
    //                        }
    //                    }
    //                    notFound = true;
    //                }
    //
    //
    //                if(notFound){
    //                    //ofSetColor(0, 0, 0, 50);
    //                }
    //                if(checkIfInside){
    //                    ofSetColor(0, 255, 0, 200);
    //                }
    //                if(foundInLandmass==1){ //icecap
    //                    ofSetColor(255, 255, 255, 200);
    //
    //                }
    //                ofDrawRectangle(i+250, j+180, 4, 4); //looks cool with 10,10 and 150 alpha
    //
    //            }
    ////            else {
    //
    ////            }
    ////            ofDrawRectangle(i, j, 2, 2);
    //        }
    //    }
}
