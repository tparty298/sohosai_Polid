#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPubSubOsc.h"
#include "ofxLiveShader.h"

#include "ofxPubSubOsc.h"
#include "ofxQuadWarp.h"
#include "ofxSyphon.h"



#define PORT 9030


#define WIDTH  1000
#define HEIGHT 1800



//本番用です〜〜よろしくお願いします〜〜

using namespace glm;

class ofApp : public ofBaseApp{
    
    void setup();
public:
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
    


    ofxPanel gui;
    ofEasyCam cam;
    
    ofVideoPlayer video;
    
    
    //utils
    void setupQuadWarper();
    void allocateFbos();
    bool bDebug = false;
    //--------
    
    //warper
    ofxQuadWarp warper;
    ofFbo fbo;
    ofPoint points[10];
    //-----
    //OSC
    void setupOsc();

    
    bool isStart = false;
    
    float current_time = -1.0;
    bool isUpdate = false;
    
    
    
    
    

    
};
