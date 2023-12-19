#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"


#define PORT 8000

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
    
    ofxCvColorImage         colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvGrayscaleImage     grayBg;
    ofxCvGrayscaleImage     grayDiff;
    ofxCvContourFinder      contourFinder;

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
    
    float lastMinute;
    unsigned long now;

    
    int offsetX; //used to reposition
    int offsetY;
    
    vector<int> cellTypes;
    
    void createEarthVector();
    
    void drawEarthFromVCellTypesVector();

    void oldDrawEarth();
    
    void checkDataDirection();
    
    int iceCell(int i, int j);
    
    void cellure();
    
    bool pollutionIncreasing;   //controls icecaps
    bool populationIncreasing;  //controls cities
    bool foodIncreasing;        //controls vegitation 
    bool extremePollution;      //controls sea lvl
    bool industryIncreasing;    //
    bool resourcesIncreasing;
    
    ofColor colorSpace;
    ofColor colorLand;
    ofColor colorOcean;
    ofColor colorIce;
    ofColor colorCity;
    
    float populationLvl =0.33;
    float pollutionLvl =0.33;
    float resourcesLvl =0.55;
    float foodLvl =0.45;
    float industryLvl =0.24;
    
    float orbit1;
    float orbit2;
    float orbitAng1;
    float orbitAng2;

    float pollutionData[200] = {
        0.16119, 0.20691, 0.19904, 0.19537, 0.19419, 0.19447, 0.19560, 0.19722, 0.19913, 0.20122,
        0.20344, 0.20573, 0.20806, 0.21045, 0.21288, 0.21538, 0.21794, 0.22056, 0.22324, 0.22599,
        0.22880, 0.23168, 0.23462, 0.23763, 0.24070, 0.24384, 0.24713, 0.25066, 0.25435, 0.25812,
        0.26196, 0.26583, 0.26974, 0.27368, 0.27762, 0.28157, 0.28555, 0.28957, 0.29363, 0.29774,
        0.30189, 0.30610, 0.31036, 0.31467, 0.31904, 0.32347, 0.32795, 0.33249, 0.33709, 0.34175,
        0.34648, 0.35126, 0.35611, 0.36102, 0.36599, 0.37103, 0.37614, 0.38131, 0.38655, 0.39185,
        0.39722, 0.40267, 0.40820, 0.41381, 0.41951, 0.42529, 0.43116, 0.43710, 0.44314, 0.44925,
        0.45545, 0.46174, 0.46812, 0.47458, 0.48112, 0.48776, 0.49448, 0.50130, 0.50820, 0.51519,
        0.52227, 0.52945, 0.53235, 0.53615, 0.54039, 0.54487, 0.54944, 0.55406, 0.55874, 0.56349,
        0.56830, 0.57318, 0.57812, 0.58312, 0.58817, 0.59325, 0.59837, 0.60353, 0.60872, 0.61396,
        0.61922, 0.62452, 0.62985, 0.63522, 0.64061, 0.64603, 0.65148, 0.65696, 0.66246, 0.66799,
        0.67370, 0.68000, 0.68645, 0.69294, 0.69943, 0.70589, 0.71234, 0.71876, 0.72518, 0.73158,
        0.73797, 0.74435, 0.75071, 0.75707, 0.76341, 0.76973, 0.77605, 0.78234, 0.78862, 0.79488,
        0.80081, 0.80662, 0.81237, 0.81809, 0.82186, 0.82121, 0.82192, 0.82333, 0.82510, 0.82705,
        0.82908, 0.83115, 0.83324, 0.83531, 0.83734, 0.83935, 0.84132, 0.84328, 0.84523, 0.84722,
        0.84924, 0.85128, 0.85333, 0.85546, 0.85771, 0.86001, 0.86232, 0.86462, 0.86692, 0.86920,
        0.87148, 0.87374, 0.87599, 0.87824, 0.88048, 0.88272, 0.88495, 0.88717, 0.88939, 0.89158,
        0.89374, 0.89587, 0.89798, 0.90007, 0.90215, 0.90423, 0.90633, 0.90843, 0.91054, 0.91265,
        0.91478, 0.91691, 0.91905, 0.92121, 0.92337, 0.92569, 0.92817, 0.93064, 0.93290, 0.93388,
        0.93593, 0.93848, 0.94124, 0.94408, 0.94693, 0.94977, 0.95257, 0.95535, 0.95810, 0.96077
    };
    
    float populationData[200] = {
        0.16119, 0.20691, 0.19904, 0.19537, 0.19419, 0.19447, 0.19560, 0.19722, 0.19913, 0.20122,
        0.20344, 0.20573, 0.20806, 0.21045, 0.21288, 0.21538, 0.21794, 0.22056, 0.22324, 0.22599,
        0.22880, 0.23168, 0.23462, 0.23763, 0.24070, 0.24384, 0.24713, 0.25066, 0.25435, 0.25812,
        0.26196, 0.26583, 0.26974, 0.27368, 0.27762, 0.28157, 0.28555, 0.28957, 0.29363, 0.29774,
        0.30189, 0.30610, 0.31036, 0.31467, 0.31904, 0.32347, 0.32795, 0.33249, 0.33709, 0.34175,
        0.34648, 0.35126, 0.35611, 0.36102, 0.36599, 0.37103, 0.37614, 0.38131, 0.38655, 0.39185,
        0.39722, 0.40267, 0.40820, 0.41381, 0.41951, 0.42529, 0.43116, 0.43710, 0.44314, 0.44925,
        0.45545, 0.46174, 0.46812, 0.47458, 0.48112, 0.48776, 0.49448, 0.50130, 0.50820, 0.51519,
        0.52227, 0.52945, 0.53235, 0.53615, 0.54039, 0.54487, 0.54944, 0.55406, 0.55874, 0.56349,
        0.56830, 0.57318, 0.57812, 0.58312, 0.58817, 0.59325, 0.59837, 0.60353, 0.60872, 0.61396,
        0.61922, 0.62452, 0.62985, 0.63522, 0.64061, 0.64603, 0.65148, 0.65696, 0.66246, 0.66799,
        0.67370, 0.68000, 0.68645, 0.69294, 0.69943, 0.70589, 0.71234, 0.71876, 0.72518, 0.73158,
        0.73797, 0.74435, 0.75071, 0.75707, 0.76341, 0.76973, 0.77605, 0.78234, 0.78862, 0.79488,
        0.80081, 0.80662, 0.81237, 0.81809, 0.82186, 0.82121, 0.82192, 0.82333, 0.82510, 0.82705,
        0.82908, 0.83115, 0.83324, 0.83531, 0.83734, 0.83935, 0.84132, 0.84328, 0.84523, 0.84722,
        0.84924, 0.85128, 0.85333, 0.85546, 0.85771, 0.86001, 0.86232, 0.86462, 0.86692, 0.86920,
        0.87148, 0.87374, 0.87599, 0.87824, 0.88048, 0.88272, 0.88495, 0.88717, 0.88939, 0.89158,
        0.89374, 0.89587, 0.89798, 0.90007, 0.90215, 0.90423, 0.90633, 0.90843, 0.91054, 0.91265,
        0.91478, 0.91691, 0.91905, 0.92121, 0.92337, 0.92569, 0.92817, 0.93064, 0.93290, 0.93388,
        0.93593, 0.93848, 0.94124, 0.94408, 0.94693, 0.94977, 0.95257, 0.95535, 0.95810, 0.96077
    };
    
    float foodData[200] = {
        0.16119, 0.20691, 0.19904, 0.19537, 0.19419, 0.19447, 0.19560, 0.19722, 0.19913, 0.20122,
        0.20344, 0.20573, 0.20806, 0.21045, 0.21288, 0.21538, 0.21794, 0.22056, 0.22324, 0.22599,
        0.22880, 0.23168, 0.23462, 0.23763, 0.24070, 0.24384, 0.24713, 0.25066, 0.25435, 0.25812,
        0.26196, 0.26583, 0.26974, 0.27368, 0.27762, 0.28157, 0.28555, 0.28957, 0.29363, 0.29774,
        0.30189, 0.30610, 0.31036, 0.31467, 0.31904, 0.32347, 0.32795, 0.33249, 0.33709, 0.34175,
        0.34648, 0.35126, 0.35611, 0.36102, 0.36599, 0.37103, 0.37614, 0.38131, 0.38655, 0.39185,
        0.39722, 0.40267, 0.40820, 0.41381, 0.41951, 0.42529, 0.43116, 0.43710, 0.44314, 0.44925,
        0.45545, 0.46174, 0.46812, 0.47458, 0.48112, 0.48776, 0.49448, 0.50130, 0.50820, 0.51519,
        0.52227, 0.52945, 0.53235, 0.53615, 0.54039, 0.54487, 0.54944, 0.55406, 0.55874, 0.56349,
        0.56830, 0.57318, 0.57812, 0.58312, 0.58817, 0.59325, 0.59837, 0.60353, 0.60872, 0.61396,
        0.61922, 0.62452, 0.62985, 0.63522, 0.64061, 0.64603, 0.65148, 0.65696, 0.66246, 0.66799,
        0.67370, 0.68000, 0.68645, 0.69294, 0.69943, 0.70589, 0.71234, 0.71876, 0.72518, 0.73158,
        0.73797, 0.74435, 0.75071, 0.75707, 0.76341, 0.76973, 0.77605, 0.78234, 0.78862, 0.79488,
        0.80081, 0.80662, 0.81237, 0.81809, 0.82186, 0.82121, 0.82192, 0.82333, 0.82510, 0.82705,
        0.82908, 0.83115, 0.83324, 0.83531, 0.83734, 0.83935, 0.84132, 0.84328, 0.84523, 0.84722,
        0.84924, 0.85128, 0.85333, 0.85546, 0.85771, 0.86001, 0.86232, 0.86462, 0.86692, 0.86920,
        0.87148, 0.87374, 0.87599, 0.87824, 0.88048, 0.88272, 0.88495, 0.88717, 0.88939, 0.89158,
        0.89374, 0.89587, 0.89798, 0.90007, 0.90215, 0.90423, 0.90633, 0.90843, 0.91054, 0.91265,
        0.91478, 0.91691, 0.91905, 0.92121, 0.92337, 0.92569, 0.92817, 0.93064, 0.93290, 0.93388,
        0.93593, 0.93848, 0.94124, 0.94408, 0.94693, 0.94977, 0.95257, 0.95535, 0.95810, 0.96077
    };
    
    float industryData[200] = {
        0.16119, 0.20691, 0.19904, 0.19537, 0.19419, 0.19447, 0.19560, 0.19722, 0.19913, 0.20122,
        0.20344, 0.20573, 0.20806, 0.21045, 0.21288, 0.21538, 0.21794, 0.22056, 0.22324, 0.22599,
        0.22880, 0.23168, 0.23462, 0.23763, 0.24070, 0.24384, 0.24713, 0.25066, 0.25435, 0.25812,
        0.26196, 0.26583, 0.26974, 0.27368, 0.27762, 0.28157, 0.28555, 0.28957, 0.29363, 0.29774,
        0.30189, 0.30610, 0.31036, 0.31467, 0.31904, 0.32347, 0.32795, 0.33249, 0.33709, 0.34175,
        0.34648, 0.35126, 0.35611, 0.36102, 0.36599, 0.37103, 0.37614, 0.38131, 0.38655, 0.39185,
        0.39722, 0.40267, 0.40820, 0.41381, 0.41951, 0.42529, 0.43116, 0.43710, 0.44314, 0.44925,
        0.45545, 0.46174, 0.46812, 0.47458, 0.48112, 0.48776, 0.49448, 0.50130, 0.50820, 0.51519,
        0.52227, 0.52945, 0.53235, 0.53615, 0.54039, 0.54487, 0.54944, 0.55406, 0.55874, 0.56349,
        0.56830, 0.57318, 0.57812, 0.58312, 0.58817, 0.59325, 0.59837, 0.60353, 0.60872, 0.61396,
        0.61922, 0.62452, 0.62985, 0.63522, 0.64061, 0.64603, 0.65148, 0.65696, 0.66246, 0.66799,
        0.67370, 0.68000, 0.68645, 0.69294, 0.69943, 0.70589, 0.71234, 0.71876, 0.72518, 0.73158,
        0.73797, 0.74435, 0.75071, 0.75707, 0.76341, 0.76973, 0.77605, 0.78234, 0.78862, 0.79488,
        0.80081, 0.80662, 0.81237, 0.81809, 0.82186, 0.82121, 0.82192, 0.82333, 0.82510, 0.82705,
        0.82908, 0.83115, 0.83324, 0.83531, 0.83734, 0.83935, 0.84132, 0.84328, 0.84523, 0.84722,
        0.84924, 0.85128, 0.85333, 0.85546, 0.85771, 0.86001, 0.86232, 0.86462, 0.86692, 0.86920,
        0.87148, 0.87374, 0.87599, 0.87824, 0.88048, 0.88272, 0.88495, 0.88717, 0.88939, 0.89158,
        0.89374, 0.89587, 0.89798, 0.90007, 0.90215, 0.90423, 0.90633, 0.90843, 0.91054, 0.91265,
        0.91478, 0.91691, 0.91905, 0.92121, 0.92337, 0.92569, 0.92817, 0.93064, 0.93290, 0.93388,
        0.93593, 0.93848, 0.94124, 0.94408, 0.94693, 0.94977, 0.95257, 0.95535, 0.95810, 0.96077
    };

    float resourceData[200] = {
        0.16119, 0.20691, 0.19904, 0.19537, 0.19419, 0.19447, 0.19560, 0.19722, 0.19913, 0.20122,
        0.20344, 0.20573, 0.20806, 0.21045, 0.21288, 0.21538, 0.21794, 0.22056, 0.22324, 0.22599,
        0.22880, 0.23168, 0.23462, 0.23763, 0.24070, 0.24384, 0.24713, 0.25066, 0.25435, 0.25812,
        0.26196, 0.26583, 0.26974, 0.27368, 0.27762, 0.28157, 0.28555, 0.28957, 0.29363, 0.29774,
        0.30189, 0.30610, 0.31036, 0.31467, 0.31904, 0.32347, 0.32795, 0.33249, 0.33709, 0.34175,
        0.34648, 0.35126, 0.35611, 0.36102, 0.36599, 0.37103, 0.37614, 0.38131, 0.38655, 0.39185,
        0.39722, 0.40267, 0.40820, 0.41381, 0.41951, 0.42529, 0.43116, 0.43710, 0.44314, 0.44925,
        0.45545, 0.46174, 0.46812, 0.47458, 0.48112, 0.48776, 0.49448, 0.50130, 0.50820, 0.51519,
        0.52227, 0.52945, 0.53235, 0.53615, 0.54039, 0.54487, 0.54944, 0.55406, 0.55874, 0.56349,
        0.56830, 0.57318, 0.57812, 0.58312, 0.58817, 0.59325, 0.59837, 0.60353, 0.60872, 0.61396,
        0.61922, 0.62452, 0.62985, 0.63522, 0.64061, 0.64603, 0.65148, 0.65696, 0.66246, 0.66799,
        0.67370, 0.68000, 0.68645, 0.69294, 0.69943, 0.70589, 0.71234, 0.71876, 0.72518, 0.73158,
        0.73797, 0.74435, 0.75071, 0.75707, 0.76341, 0.76973, 0.77605, 0.78234, 0.78862, 0.79488,
        0.80081, 0.80662, 0.81237, 0.81809, 0.82186, 0.82121, 0.82192, 0.82333, 0.82510, 0.82705,
        0.82908, 0.83115, 0.83324, 0.83531, 0.83734, 0.83935, 0.84132, 0.84328, 0.84523, 0.84722,
        0.84924, 0.85128, 0.85333, 0.85546, 0.85771, 0.86001, 0.86232, 0.86462, 0.86692, 0.86920,
        0.87148, 0.87374, 0.87599, 0.87824, 0.88048, 0.88272, 0.88495, 0.88717, 0.88939, 0.89158,
        0.89374, 0.89587, 0.89798, 0.90007, 0.90215, 0.90423, 0.90633, 0.90843, 0.91054, 0.91265,
        0.91478, 0.91691, 0.91905, 0.92121, 0.92337, 0.92569, 0.92817, 0.93064, 0.93290, 0.93388,
        0.93593, 0.93848, 0.94124, 0.94408, 0.94693, 0.94977, 0.95257, 0.95535, 0.95810, 0.96077
    };

    
		
};
