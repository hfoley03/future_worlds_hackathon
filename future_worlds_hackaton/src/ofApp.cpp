#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Variables Setup
    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    offsetX = centreW/2;
    offsetY = centreH/2;

    orbitAng2 = 0.1;
    orbitAng1 = 0.1;
    orbit1 = centreH*2;
    orbit2 = centreH*2;

    pollutionIncreasing = false;
    populationIncreasing = true;
    extremePollution = false;
    startYear = 1900;
    endYear = 2100;
    
//    myfont.load("LEDLIGHT.otf", 60);
//    myfont.load("nasalization-rg.otf", 32);
//    colorSpace.setHsb(0, 255, 0);
//    colorIce.setHsb(0, 0, 255);
//    colorCity.setHsb(255, 255, 255);
//    colorLand.setHsb(100,200,255);
//    colorOcean.setHsb(180,200,255);
    
    ofBackground(0);
    ofSetCircleResolution(100);
    oscOut.setup("localhost", 8000);    //OSC
    oscIn.setup(8000);                  //OSC

    createLandmasses(); // only calling this in setup to save processing power, should be called everytime window is resized, not important for now
    createEarthVector();
    
    std::cout << pollutionData[10] << std::endl;
    
    lastMinute = ofGetElapsedTimeMillis();

    maxPopYear = distance(populationData, max_element(populationData, populationData + 200));
    maxPulYear = distance(pollutionData, max_element(pollutionData, pollutionData + 200));
    maxIndYear = distance(industryData, max_element(industryData, industryData + 200));
//    maxPopYear = distance(populationData, max_element(populationData, populationData + 200));


    std::cout << maxPopYear << std::endl;
    
}

//--------------------------------------------------------------
void ofApp::update(){

    centreH = ofGetHeight()/2;
    centreW = ofGetWidth()/2;
    orbit1 = centreH*2;
    orbit2 = centreH*2;
    
    if (currentYear < 199 && start)
    {
        now = ofGetElapsedTimeMillis();
        if (now - lastMinute >= 50) { // 60000 milliseconds = 1 minute
            cellular();
            checkDataDirection();
            currentYear += 1;
            lastMinute = now;
            sendOsc();
            receiveOsc();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawPlanets();
    drawGUI();
    drawEarthFromVCellTypesVector();
    //currentMessyDraw();
    drawLandmassesPolylines();
}

//--------------------------------------------------------------
// check if the value of a dataset will be higher or lower next year
//--------------------------------------------------------------
void ofApp::checkDataDirection() {
    
    int nextYear = currentYear + 1;
    pollutionIncreasing = (pollutionData[currentYear] < pollutionData[nextYear] && pollutionData[currentYear] > 0.1);
    populationIncreasing = (populationData[currentYear] < populationData[nextYear] && populationData[currentYear] > 0.1);
    foodIncreasing = (foodData[currentYear] < foodData[nextYear]);
    industryIncreasing = (industryData[currentYear] < industryData[nextYear]);
    resourcesIncreasing = (resourceData[currentYear] < resourceData[nextYear]);
    extremePollution = (pollutionData[currentYear] > 0.5);
}

//--------------------------------------------------------------
// draw earth from the cell types 
//--------------------------------------------------------------
void ofApp::drawEarthFromVCellTypesVector(){
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255, 0, 0, 255);

    for(int i = 0; i < 90; i = i + 1)
    {
        for(int j = 0; j< 90; j = j + 1)
        {
            int thisCellType = cellTypes[j * 90 + i];
            
            switch (thisCellType) {
                case 1:
                    ofSetColor(98, 188, 47, 150); // Land
                    break;
                case 2:
                    if (i % 2 == 0)
                        ofSetColor(0, 120, 255, 150); // Ocean, even column
                    else
                        ofSetColor(0, 131, 255, 150); // Ocean, odd column
                    break;
                case 3:
                    ofSetColor(255, 255, 255, 150); // Ice
                    break;
                case 4:
                    ofSetColor(105, 103, 97, 255); // City
                    break;
                default:
                    break;
            }

            if (thisCellType != 0){ // if earth

                if(animate){
  //                ofDrawRectangle( (j*5) + (250), (i*5) + (180), 10, 10); //looks cool with 10,10 and 150 alpha
 //                 ofDrawRectangle( (j*5) + (250), (i*5) + (180), 5, 5); //looks cool with 10,10 and 150 alpha
                    ofDrawEllipse( (j*5)*cos(orbitAng1) + (250) +centreW/2, (i*5) + (180), 10, 10); //looks cool with 10,10 and 150 alpha
                }
                else
                {
//                  ofDrawRectangle( (j*5) + (250), (i*5) + (180), 10, 10); //looks cool with 10,10 and 150 alpha
//                  ofDrawRectangle( (j*5) + (250), (i*5) + (180), 5, 5); //looks cool with 10,10 and 150 alpha
                  ofDrawEllipse( (j*5) + (250), (i*5) + (180), 10, 10); //looks cool with 10,10 and 150 alpha
                }
            }
        }
    }
    
    if(!animate){
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetColor(0, 0, 0, 255);
        ofDrawBitmapStringHighlight("LA", 330, 395);
        ofDrawBitmapStringHighlight("NYC", 505, 385);
        ofDrawBitmapStringHighlight("CDMX", 385, 479);
        ofDrawBitmapStringHighlight("BG", 480, 570);
    }
}



//--------------------------------------------------------------
// cellular automata
//--------------------------------------------------------------
void ofApp::cellular(){ //we'll call this once a year
    
    cityCounter = 0;
    int iceCounterTemp = 0;
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
                if (cellTypes[j*90 + i]==3){iceCounterTemp++;}

                // iterate around the surrounding cells
                for(int xx = -1; xx <= 1; xx = xx + 1)
                {
                    for(int yy = -90; yy <= 90; yy = yy + 90)
                    {
                        if ((j*90 + i + xx + yy) != (j*90 + i)){
                            int thisType = cellTypes[j*90 + i + xx + yy]; // get the type of the neighbour cell
                            //                        std::cout << j*90 + i + xx + yy << std::endl;
                            
                            //types of cells
                            //0 = space
                            //1 = land
                            //2 = ocean
                            //3 = ice cap
                            //4 = city
                            
                            switch(thisType){
                                case 1:
                                    neighboursLand += 1;
                                    break;
                                case 2:
                                    neighboursOcean += 1;
                                    break;
                                case 3:
                                    neighboursIce += 1;
                                    break;
                                case 4:
                                    neighboursCity += 1;
                                    break;
                            }
                        }
                    }
                }
                
                if(neighboursIce == 3 && ofRandom(1.0) > 0.5  && (iceCounter < 200 || currentYear > 90) ){ //if ive exactly 3 neighbours of ice and then some probability thing
                    if(pollutionIncreasing){cellTypes[j*90 + i] = 2;}
                    else {cellTypes[j*90 + i] = 3;}
                }
                
                if(neighboursCity == 1 && ofRandom(1.0) > 0.95 && cellTypes[j*90 + i] != 2){ //if ive exactly 1 neighbours of city and then some probability thing
                    if(populationIncreasing){cellTypes[j*90 + i] = 4;}
                    else {cellTypes[j*90 + i] = 1;}
                }
                
                if(neighboursOcean == 3 && ofRandom(1.0) > 0.85 && cellTypes[j*90 + i] == 1){ //if ive exactly 1 neighbours of city and then some probability thing
                    if(extremePollution){cellTypes[j*90 + i] = 2;}
                    else {cellTypes[j*90 + i] = 1;}
                }
                
                if(neighboursOcean == 3 && ofRandom(1.0) > 0.90 && cellTypes[j*90 + i] == 4){ //if ive exactly 1 neighbours of city and then some probability thing
                    if(extremePollution){cellTypes[j*90 + i] = 2;}
//                    else {cellTypes[j*90 + i] = 1;}
                }
                
            }
        }
    }
    //To stop cities being able to completely disapeer
    cellTypes[14*90 + 43] = 4;     //LA = 320, 395
    cellTypes[49*90 + 41] = 4;     //NYC = 495 385
    cellTypes[25*90 + 60] = 4;     // MEX = 375, 479
    cellTypes[44*90 + 76] = 4;     // bog = 470, 650

    iceCounter = iceCounterTemp;
    
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



//--------------------------------------------------------------
// takes png of drawing of earth and creates polylines of the landmasses
//--------------------------------------------------------------

void ofApp::createLandmasses(){
    
    //Load PNG of World Icon Drawing
    worldIcon.load("images/worldGraphic4.png");
    worldIcon.resize(445, 438);
    worldIcon.setImageType(OF_IMAGE_GRAYSCALE);
    imgWidth = worldIcon.getWidth();
    imgHeight = worldIcon.getHeight();
    std::cout<<"worldIcon width: " + to_string(imgWidth) + " height: " + to_string(imgHeight)<< std::endl;

    //Create CV image in grayscale & find contours
    grayImage.allocate(imgWidth,imgHeight);
    thresholdValue = 168;
    thresh = 0.0f;
    grayImage.setFromPixels(worldIcon.getPixels());
    grayImage.threshold(168);
    contourFinder.findContours(grayImage, 100, 45000, 10, true);
    
    //Create a polyline for each land mass and add to vector landmasses
    int numBlobs = contourFinder.nBlobs;
    for (int i=0; i<numBlobs; i++){
        
        if(i!=1 && i !=3){
            ofPolyline polyline;
            for(auto & point : contourFinder.blobs[i].pts) {
                polyline.addVertex(point.x + offsetX, point.y + offsetY);
                //polyline.addVertex(point.x, point.y);

            }
            polyline.close();
            polyline.simplify();
            landmasses.push_back(polyline);
        }
    }
}
//--------------------------------------------------------------
// draw polylines of the landmasses
//--------------------------------------------------------------
void ofApp::drawLandmassesPolylines(){
    ofSetColor(255);
    for (auto & element : landmasses)
    {
        element.draw();
    }
}

//--------------------------------------------------------------
// identify a cell type for each 5x5 square of the earth
//--------------------------------------------------------------
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

            if( ofDist( i+250, j+180, imgWidth/2 + offsetX , imgHeight/2 + offsetY) <= (imgWidth/2 - 10)) //True if inside earth
            {
                bool checkIfInside = false;
                bool notFound = false;
                int foundInLandmass = -1;
                
                while(checkIfInside == false && notFound == false)
                {
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

//--------------------------------------------------------------
// Send OSC Messages to Ableton
//--------------------------------------------------------------
void ofApp::sendOsc(){
    timeSent = ofGetElapsedTimef();
    ofxOscMessage m1;
    m1.setAddress("/pop");

    
    ofxOscMessage m2;
    m2.setAddress("/pul");

    
    ofxOscMessage m3;
    m3.setAddress("/ind");

    
    if(currentYear < 2098){
        m1.addFloatArg(populationData[currentYear]);
        m2.addFloatArg(pollutionData[currentYear]);
        m3.addFloatArg(industryData[currentYear]);
    }
    else {
        m1.addFloatArg(0.0);
        m2.addFloatArg(0.0);
        m3.addFloatArg(0.0);
    }
    
    oscOut.sendMessage(m3);
    oscOut.sendMessage(m1);
    oscOut.sendMessage(m2);

    
    
}

void ofApp::receiveOsc(){
    while(oscIn.hasWaitingMessages()){
        ofxOscMessage msg;
        oscIn.getNextMessage(msg);
        if (msg.getAddress() == "/pop") {
            timeReceived = msg.getArgAsFloat(0);
        }
//        if (msg.getAddress() == "/dick") {
//            timeReceived1 = msg.getArgAsFloat(0);
//        }
    }
}

//--------------------------------------------------------------
// Draw the other planets
//--------------------------------------------------------------
void ofApp::drawPlanets(){
    //PLANETS
    orbitAng1 = ofGetElapsedTimef()*0.2;
    orbitAng2 = ofGetElapsedTimef()*0.1;

    //Mars
    ofSetColor(231,125,17, 160);
    ofDrawEllipse(orbit1*cos(orbitAng1) + centreW, orbit1*sin(orbitAng1) + centreH*2.5, 50, 50);
//    ofSetColor(255,0,0, 200);
//    ofSetRectMode(OF_RECTMODE_CENTER);
//    ofDrawRectangle(orbit1*cos(orbitAng1) + centreW, orbit1*sin(orbitAng1) + centreH*2.5, 5, 5);
//    ofDrawRectangle(orbit1*cos(orbitAng1) + centreW + 5, orbit1*sin(orbitAng1) + centreH*2.5 + 5, 5, 5);
//    ofDrawRectangle(orbit1*cos(orbitAng1) + centreW + 5, orbit1*sin(orbitAng1) + centreH*2.5 - 5, 5, 5);
//    ofDrawRectangle(orbit1*cos(orbitAng1) + centreW - 5, orbit1*sin(orbitAng1) + centreH*2.5 + 5, 5, 5);
//    ofDrawRectangle(orbit1*cos(orbitAng1) + centreW - 5, orbit1*sin(orbitAng1) + centreH*2.5 - 5, 5, 5);

    //Some other planet
    ofSetColor(79,76,176, 200);
    ofDrawEllipse(orbit2*cos(orbitAng2) + centreW, orbit2*sin(orbitAng2) + centreH*2.75, 30, 30);
}

//--------------------------------------------------------------
// Draw GUI
//--------------------------------------------------------------
void ofApp::drawGUI(){
    //    myfont.drawString("Future Worlds", ofGetWidth()/2 - myfont.stringWidth("Future Wolrds")/2 - 25, 100);
    //    ofDrawBitmapStringHighlight("point " + ofToString(mouseX) + " " + ofToString(mouseY) , 600, 600);

    ofSetRectMode(OF_RECTMODE_CORNER);
    ofFill();
    ofSetColor(255, 255, 255);
//    ofDrawEllipse(ofGetWidth() - 300 , ofGetHeight() - 50 , 50, 50);
    
    // timeline
    ofDrawLine(centreW - 300 , ofGetHeight() - 50 , centreW + 300 , ofGetHeight() - 50);
    ofDrawLine(centreW - 300 , ofGetHeight() - 50 + 5 , centreW - 300 , ofGetHeight() - 50 - 5);
    ofDrawLine(centreW + 300 , ofGetHeight() - 50 + 5 , centreW + 300 , ofGetHeight() - 50 - 5);
    
    // timeline cursror
    ofDrawEllipse(centreW - 300 + (currentYear * 3), ofGetHeight() - 50, 10,10);
    
    // Year
    ofDrawBitmapStringHighlight(ofToString(currentYear + startYear) , centreW - 315 + (currentYear * 3), ofGetHeight() - 28);
    ofDrawBitmapStringHighlight("1900", centreW - 300 , ofGetHeight() - 70);
    ofDrawBitmapStringHighlight("2100", centreW + 270 , ofGetHeight() - 70);
    
    ofSetColor(255, 0, 0);
    ofDrawEllipse(centreW - 300 + (maxPopYear * 3), ofGetHeight() - 50, 5,5);
    ofSetColor(0, 255, 0);
    ofDrawEllipse(centreW - 300 + (maxPulYear * 3), ofGetHeight() - 50, 5,5);
    ofSetColor(255, 255, 0);
    ofDrawEllipse(centreW - 300 + (maxIndYear * 3), ofGetHeight() - 50, 5,5);
    
    ofSetColor(255, 0, 0, 255);

    
//    ofDrawBitmapStringHighlight("Year:       " + ofToString(currentYear + startYear) , 50, 30);

    // Data and Current Values
    ofDrawBitmapStringHighlight("Population: " + ofToString(populationData[currentYear], 3) , 50, 50);
    ofDrawBitmapStringHighlight("Resources:  " + ofToString(resourceData[currentYear], 3) , 50, 70);
    ofDrawBitmapStringHighlight("Pollution:  " + ofToString(pollutionData[currentYear], 3) , 50, 90);
    ofDrawBitmapStringHighlight("Industry:   " + ofToString(industryData[currentYear], 3) , 50, 110);
    ofDrawBitmapStringHighlight("Food:       " + ofToString(foodData[currentYear], 3) , 50, 130);
    
    // Data Type color indicator
    ofSetColor(255, 0, 0);
    ofDrawEllipse(40, 45, 5,5);
    ofSetColor(0, 255, 0);
    ofDrawEllipse(40, 85, 5,5);
    ofSetColor(255, 255, 0);
    ofDrawEllipse(40, 105, 5,5);
}

void ofApp::currentMessyDraw(){

        ofSetRectMode(OF_RECTMODE_CORNER);
        ofFill();

        ofSetColor(200, 200, 200);
    //    ofDrawCircle(imgWidth/2 + offsetX , imgHeight/2 + offsetY, imgWidth/2 - 10);
        
        ofSetColor(0, 0, 0, 255);
        // EARTH
        
        if(animate)
        {
            drawEarthFromVCellTypesVectorWobble();
        }
        else
        {
            drawEarthFromVCellTypesVector();
        }
        
        // Draw cities names
    //    ofSetRectMode(OF_RECTMODE_CORNER);
    //    ofSetColor(0, 0, 0, 255);
    //    ofDrawBitmapStringHighlight("LA", 330, 395);
    //    ofDrawBitmapStringHighlight("NYC", 505, 385);
    //    ofDrawBitmapStringHighlight("CDMX", 385, 479);
    //    ofDrawBitmapStringHighlight("BG", 480, 570);
        
        
    //    (j*5)*cos(orbitAng1) + (250) +centreW/2



        //for debugging only
    //    ofDrawBitmapStringHighlight("Population Increasing: " + ofToString(populationIncreasing) , 700, 30);
    //    ofDrawBitmapStringHighlight("Resources increasing:  " + ofToString(resourcesIncreasing) , 700, 50);
    //    ofDrawBitmapStringHighlight("Pollution increasing:  " + ofToString(pollutionIncreasing) , 700, 70);
    //    ofDrawBitmapStringHighlight("Industry increasing:   " + ofToString(industryIncreasing) , 700, 90);
    //    ofDrawBitmapStringHighlight("extremePollution :       " + ofToString(extremePollution) ,700, 110);
    //
    //
    //    ofDrawBitmapStringHighlight("city counter:       " + ofToString(cityCounter) ,700, 280);
    //    ofDrawBitmapStringHighlight("ice counter:       " + ofToString(iceCounter) ,700, 300);
    //    ofSetRectMode(OF_RECTMODE_CENTER);
    //
    //    ofSetColor(255, 255, 255);
    //    grayImage.draw(centreW,centreH);

    
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            cellular();
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
        case 's':
            start = !start;
            std::cout << "play pause" << std::endl;

            break;
            
        case 'a':
            animate = !animate;
            std::cout << "animate" << std::endl;

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
