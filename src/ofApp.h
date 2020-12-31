#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPubSubOsc.h"
#include "ofxLiveShader.h"

#include "ofxPubSubOsc.h"
#include "ofxQuadWarp.h"
#include "ofxSyphon.h"
#include "ofxNDIReceiver.h"
#include "ofxNDIRecvStream.h"



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
    void drawBitmapStrings();
    void drawWarper();
    void drawTexture();
    void setupOsc();
    void setupNDI();
    void updateTexture();
private:
    ofxPanel gui;
    ofVideoPlayer video;
    
    
    //utils
    void setupQuadWarper();
    void allocateFbos();
    bool bDebug = true;
    //--------
    
    //warper
    ofxQuadWarp warper;
    ofFbo fbo;
    ofPoint points[10];
    //-----
    
    bool isStart = false;
    float current_time = -1.0;
    bool isUpdate = false;
    void debugDraw();
    ofImage idImg;
    int id = 0;
    string log = "";
    float alpha = 0.0;
    
    ofxNDIReceiver receiver_;
    ofxNDIRecvVideoFrameSync video_;
    ofPixels pixels_;

};
