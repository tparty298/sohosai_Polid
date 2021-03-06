#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bDebug = false;
    ofBackground(0);
   // ofEnableDepthTest();
        ofBuffer buffer = ofBufferFromFile("settings.txt");
    
    if(buffer.size()) {
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            
            string line = *it;
            
            // copy the line to draw later
            // make sure its not a empty line
            if(line.empty() == false) {
                id = atoi(line.c_str());
            }
            
            // print out the line
            //cout << line << endl;
            
            
        }
        
    }
    
    idImg.load(ofToString(id)+".png");
    
    
    
    
    //-------
    
    
    
    //ofToggleFullscreen();
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

//--------------------------------------------------------------
void ofApp::update(){
    alpha += (0.0-alpha)/4.0;
    
    if(isStart == false&&current_time>=0.0){
        //始まってない状態で何秒って情報を受ける
        //フレーム数に変換　具体的には60掛け
        //setFrameしてからあとはかってに走り出してもらう
        int frame = current_time*60.0;
        video.setFrame(frame);
        video.play();
        isStart = true;
        isUpdate = true;
        
    }
    if(isUpdate){
        video.update();
    }
    
    
    
    video.update();
    
    
    
    fbo.begin();
    ofClear(0,255);
    ofSetColor(255);
    video.draw(0.0,0.0,WIDTH,HEIGHT);
    if(bDebug){
    idImg.draw(0.0,0.0,WIDTH,HEIGHT);
    }
    
    fbo.end();
    
    
    
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofMatrix4x4 mat = warper.getMatrix();
    
    //======================== use the matrix to transform our fbo.
    
    ofPushMatrix();
    ofMultMatrix(mat);
    //fbo.draw(0, 0,WIDTH,HEIGHT);
    fbo.draw(0.0,0.0,WIDTH,HEIGHT);
    
    ofPopMatrix();
    
    ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();
    ofSetColor(255);
    
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
