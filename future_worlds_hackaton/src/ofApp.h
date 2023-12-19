#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"


#define PORT 7331

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
    
    void sendOsc();
    void receiveOsc();
    
    int centreH;
    int centreW;
    
    ofxOscReceiver oscIn;
    ofxOscSender oscOut;
    
    ofxOscMessage oscMessage;
    
    float timeSent, timeReceived;
    
    ofImage worldIcon;
    
    ofxCvColorImage            colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvGrayscaleImage     grayBg;
    ofxCvGrayscaleImage     grayDiff;
    ofxCvContourFinder        contourFinder;

    int                        thresholdValue;
    float thresh;
    bool                    bLearnBackground;
    
    int imgWidth;
    int imgHeight;
    
    void squares(ofPolyline polyline);
    
    vector<ofPolyline> landmasses;
    
    void createLandmasses();
    
    int startYear;
    int endYear;
    int currentYear;
    
    int offsetX; //used to reposition
    int offsetY;
    
    vector<int> cellTypes;
    
    void createEarthVector();
    
    void drawEarthFromVCellTypesVector();

    void oldDrawEarth();
    
    int iceCell(int i, int j);
    
    void cellure();
    
    bool pollutionIncreasing;
    
		
};
