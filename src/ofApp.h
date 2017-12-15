#pragma once

#include "ofMain.h"
#include "ofxOrbbecAstra.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    
    ofParameter<float> near;
    ofParameter<float> far;
    ofxPanel gui;
    
    ofxOrbbecAstra astra;
    ofxCvContourFinder contourFinder;
    
    ofxCvGrayscaleImage     grayImage;
    ofVboMesh mesh;
    ofTrueTypeFont myFont;
    
    void reset();
    int startTime;
    int nowTime;
    int preTime;
    
    ofImage cursor;
    
    bool bDrawPointCloud;
    bool bPointCloudUseColor;
    bool bUseRegistration;
    
    int stage = 0;
    float timer = 0;
    float stageLength = 5;
    
    //ofVec3f p[64][48];
    //int count[64][48];
    
    ofVec3f p[144][90];
    int count[144][90];
    ofEasyCam cam;
    
    ofImage background;
    
   
    
};
