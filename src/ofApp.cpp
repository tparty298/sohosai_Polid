#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bDebug = false;
    ofBackground(0);
    ofBuffer buffer = ofBufferFromFile("settings.txt");
    
    if(buffer.size()) {
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            string line = *it;
            if(line.empty() == false) {
                id = atoi(line.c_str());
            }
        }
    }
    setupNDI();
    
    idImg.load(ofToString(id)+".png");
    
    //-------
    
    setupQuadWarper();
    
    video.load("dance_ex.mov");
    video.setVolume(0.0);
    video.play();
    isUpdate = false;
    
    video.setLoopState(OF_LOOP_NORMAL);
    fbo.allocate(WIDTH,HEIGHT);
    
    ofxSubscribeOsc(PORT,"/3/current_time",current_time);
    
    ofxSubscribeOsc(PORT,"/3/stop",[=](){
        log = "stop";
        isUpdate = false;
        video.stop();
    });
    
    ofxSubscribeOsc(PORT,"/3/reset",[=](){
        log = "reset";
        isUpdate = false;
        isStart = false;
        current_time = -1.0;
        video.setFrame(0);
        video.stop();
        
    });
    ofxSubscribeOsc(PORT,"/3/seek",[=](float val){
        log = "seek"+ofToString(val);
        int frame = val*60.0;
        video.setFrame(frame);
        video.play();
    });
    ofxSubscribeOsc(PORT,"/3/debug",bDebug);
    
    ofxSubscribeOsc(PORT, "/3/hey", [=](){
        log = "hey";
        alpha = 1.0;
    });
    
}

void ofApp::setupNDI(){
    NDIlib_initialize();
    auto findSource = [](const string &name_or_url) {
        auto sources = ofxNDI::listSources();
        if(name_or_url == "") {
            return make_pair(ofxNDI::Source(), false);
        }
        auto found = find_if(std::begin(sources), std::end(sources), [name_or_url](const ofxNDI::Source &s) {
            return ofIsStringInString(s.p_ndi_name, name_or_url) || ofIsStringInString(s.p_url_address, name_or_url);
        });
        if(found == std::end(sources)) {
            ofLogWarning("ofxNDI") << "no NDI source found by string:" << name_or_url;
            return make_pair(ofxNDI::Source(), false);
        }
        return make_pair(*found, true);
    };
    string name_or_url = "";    // Specify name or address of expected NDI source. In case of blank or not found, receiver will grab default(which is found first) source.
    auto result = findSource(name_or_url);
    if(result.second ? receiver_.setup(result.first) : receiver_.setup()) {
        video_.setup(receiver_);
    }
}
void ofApp::updateTexture(){
    if(receiver_.isConnected()) {
        video_.update();
        if(video_.isFrameNew()) {
            video_.decodeTo(pixels_);
            fbo.begin();
            ofClear(0,255);
            ofSetColor(255);
            if(pixels_.isAllocated()) {
                ofImage(pixels_).draw(0,0);
            }
            fbo.end();        }
        
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    updateTexture();
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawTexture();
    drawWarper();
    ofSetColor(255);
    drawBitmapStrings();
}

void ofApp::drawTexture(){
    ofMatrix4x4 mat = warper.getMatrix();
    ofPushMatrix();
    ofMultMatrix(mat);
    fbo.draw(0.0,0.0,WIDTH,HEIGHT);
    ofPopMatrix();
}

void ofApp::drawWarper(){
    ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();
}

void ofApp::drawBitmapStrings(){
    
    ofDrawBitmapString("PolidMappingSystem by TParty on sohosaiZenya", 10, 10);
    
    ofDrawBitmapString(ofToString(video.getCurrentFrame())+"/"+ofToString(video.getTotalNumFrames()), 10, 30);
    if(isStart){
        ofDrawBitmapString("starting", 10, 20);
    }else{
        ofDrawBitmapString("waiting", 10, 20);
    }
    
    
    ofDrawBitmapString("s show Handle", 10, 40);
    ofDrawBitmapString("h save Handle", 10, 50);
    if(bDebug){
        ofDrawBitmapString("debug:true", 10, 60);
        
    }else{
        ofDrawBitmapString("debug:false", 10, 60);
    }
    ofDrawBitmapString("1-4 bind handle to cursor", 10, 70);
    
    ofDrawBitmapString("id:"+ofToString(id), 10, 80);
    ofDrawBitmapString("yoroshikune", 10, 90);
    
    
    ofDrawBitmapString("latest received current_time:"+ofToString(current_time), 10,  100);
    ofDrawBitmapString("latest received signal:"+log, 10,  110);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'a'){
        //  bDebug = !bDebug;
    }
    if(key == 's' || key == 'S') {
        warper.toggleShow();
    }
    
    if(key == 'l' || key == 'L') {
        warper.load();
    }
    
    if(key == 'h' || key == 'H') {
        warper.save();
    }
    
    if(key== 'd'){
        bDebug = !bDebug;
    }
    
    if(key == 't'){
        ofToggleFullscreen();
    }
    
}

void ofApp::setupQuadWarper(){
    
    
    int x = (ofGetWidth()*1.0 - WIDTH) * 0.5;       // center on screen.
    int y =  10;     // center on screen.
    int w = WIDTH;
    int h = HEIGHT;
    // fbo.allocate(w, h);
    //  cout<<vec2(fbo.getWidth(),fbo.getHeight())<<endl;
    warper.setSourceRect(ofRectangle(0, 0, w, h));              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warper.setTopLeftCornerPosition(ofPoint(x, y));             // this is position of the quad warp corners, centering the image on the screen.
    warper.setTopRightCornerPosition(ofPoint(x + w, y));        // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomLeftCornerPosition(ofPoint(x, y + h));      // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomRightCornerPosition(ofPoint(x + w, y + h)); // this is position of the quad warp corners, centering the image on the screen.
    warper.setup();
    warper.load(); // reload last saved changes.
}

