#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    //ofSetWindowShape(1280, 720);
    ofSetFullscreen(true);
    ofSetVerticalSync(true);
    
    astra.setup();
    //astra.enableRegistration(true);
    
    astra.initColorStream();
    astra.initDepthStream();
    astra.initPointStream();
    
    gui.setup();
    gui.add( near.set("near", 30, 0, 8000) );
    gui.add( far.set("far", 1800, 0, 8000) );
    
    
    grayImage.allocate(1280, 720);
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    myFont.load("OpenSans-Semibold.ttf",6);
    
    
    bDrawPointCloud = false;
    bPointCloudUseColor = false;
    bUseRegistration = true;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    astra.setup();
    astra.enableRegistration(bUseRegistration);
    
    astra.initColorStream();
    
    astra.initDepthStream();
    astra.initPointStream();
    astra.initHandStream();
    
    //cursor.load("cursor.png");
    background.load("website2.png");
    
    reset();
}

//--------------------------------------------------------------
void ofApp::reset() {
    startTime = (int)ofGetElapsedTimeMillis();
    preTime = (int)ofGetElapsedTimeMillis();
    nowTime = (int)ofGetElapsedTimeMillis();
    for (int i = 0; i < 144; i++) {
        for (int j = 0; j < 90; j++) {
            count[i][j] = j * 100 + ofRandom(0, 4000);
            p[i][j] = ofVec3f(i * 10, j * 10, 0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    astra.setDepthClipping(near.get(), far.get());
    astra.update();
    
    for (auto& hand : astra.getHandsDepth()) {
        auto& pos_real = hand.second;
        ofPoint pos = ofPoint((int)(pos_real.x*2.25),(int)(pos_real.y*1.875));              //timer += ofGetLastFrameTime();
        if(pos.x > 250 && pos.x < 350 && pos.y > 250 && pos.y < 350) {
            stage = 1;
        }else{
            stage=0;
        }
    }
    
    for (auto& hand : astra.getHandsWorld()) {
        auto& pos_real = hand.second;
        ofVec3f pos = ofVec3f((int)(pos_real.x*2.25),(int)(pos_real.y*1.875),pos_real.z);
        if(pos.x > 250 && pos.x < 350 && pos.y > 250 && pos.y < 350) {
            if (pos.z < 800){
                stage = 2;
            }else{
                stage=0;
            }
        }
    }
    
    
    
//    timer += ofGetLastFrameTime();
//    //if(timer >= stageLengths[stage]) {
//    if(timer >= 45) {
//        stage = 3;
//    }
    
    if (astra.isFrameNew() && bDrawPointCloud) {
        mesh.clear();
        
        int maxDepth = 1500;
        int w = astra.getDepthImage().getWidth();
        int h = astra.getDepthImage().getHeight();
        
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                ofVec3f p = astra.getWorldCoordinateAt(x, y);
                
                if (p.z == 0) continue;
                if (p.z > maxDepth) continue;
                
                
                mesh.addVertex(p);
                
                if (bPointCloudUseColor) {
                    mesh.addColor(astra.getColorImage().getColor(x, y));
                } else {
                    float hue  = ofMap(p.z, 0, maxDepth, 0, 255);
                    mesh.addColor(ofColor::fromHsb(hue, 255, 255));
                }
            }
        }
    }
  

    
    if(astra.isFrameNew()) {
        
        grayImage.setFromPixels(astra.getDepthImage());
        grayImage.threshold(80);
        mesh.clear();
        
        nowTime = (int)ofGetElapsedTimeMillis();
        for (int i = 0; i < 144; i++) {
            for (int j = 0; j < 90; j++) {
                if(count[i][j]<10000)
                    count[i][j] += (nowTime - preTime);
                
                ofVec3f p_astra = astra.getWorldCoordinateAt((int)(i*4.44), (int)(j*5.33));
                if (p_astra.z < near.get() || p_astra.z > far.get()) {
                    p[i][j].z = 0;
                    continue;
                }else {
                    p[i][j].z = p_astra.z;
                    mesh.addVertex(p_astra);
                }
            }
        }
        preTime = nowTime;
        
        /*
         //contourFinder.findContours(grayImage, 20, (640*480)/3, 10, true);
         
         //mesh
         mesh.clear();
         
         int w = astra.getDepthImage().getWidth();
         int h = astra.getDepthImage().getHeight();
         
         for (int y = 0; y < h; y+=10) {
         for (int x = 0; x < w; x+=10) {
         ofVec3f p = astra.getWorldCoordinateAt(x, y);
         ofVec3f p2 = ofVec3f(x, y, 0);
         
         //if (p.z == 0) continue;
         if (p.z < near.get() || p.z > far.get()) continue;
         
         mesh.addVertex(p2);
         }
         }
         */
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    background.draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    
    ofSetFullscreen(true);
    int a = ofMap(nowTime - startTime, 0, 10000, 0, 255);
    ofSetColor(255, 255, 255, 255-a);
    astra.drawDepth(0, 0, ofGetWidth(), ofGetHeight());
    //astra.draw(640, 0);
    
    //mesh
    ofSetColor(0, 255, 0, a);
    /*
     for (int i = 0; i<mesh.getNumVertices(); i++) {
     myFont.drawString(ofToString((int)ofRandom(0, 2)), mesh.getVertices()[i].x, mesh.getVertices()[i].y);
     }
     */
    
    for (int i = 0; i < 144; i++) {
        for (int j = 0; j < 90; j++) {
            if (count[i][j] > 10000 && (p[i][j].z!=0)) {
                myFont.drawString(ofToString((int)ofRandom(0, 2)), p[i][j].x, p[i][j].y);
                //ofDrawBitmapString(ofToString((int)ofRandom(0, 2)), p[i][j].x, p[i][j].y);
            }
        }
    }
    
    
    //contourFinder.draw(640, 0);
    gui.draw();
    
    ofSetColor(255, 0, 0,100);
    ofDrawBitmapString("r = reset", 10, 10);
    ofDrawBitmapString(mesh.getNumVertices(), 10, 20);
    
    ofSetColor (0,0,255);
    ofDrawRectangle(250, 250, 100, 100);
    
    if (stage == 0){
        ofSetColor(0, 255 ,0);
        ofDrawRectangle(ofGetWidth()/2.0-100, ofGetHeight()/2.0-100, 100, 100);
    }
    
    else if (stage == 1){
        ofSetColor(0, 0, 255);
        ofDrawRectangle(ofGetWidth()/2.0-100, ofGetHeight()/2.0-100, 100, 100);
    }else if (stage==2){
        ofSetColor(255, 0, 0);
      ofDrawRectangle(ofGetWidth()/2.0-100, ofGetHeight()/2.0-100, 100, 100);
        //ofDrawRectangle(250, 250, 100, 100);

    
    
    
    }
    
//     else if (stage == 3){
    
    if (!bDrawPointCloud) {
        ofSetColor(255);

        
        for (auto& hand : astra.getHandsDepth()) {
            auto& pos_real = hand.second;
            ofPoint pos = ofPoint((int)(pos_real.x*2.25),(int)(pos_real.y*1.875));
            ofSetColor(255, 0, 0);
            ofDrawCircle(pos, 10);
            //cursor.draw (pos);
            stringstream ss;
            ss << "id: " << hand.first << endl;
            ss << "pos: " << hand.second;
            ofDrawBitmapStringHighlight(ss.str(), pos.x, pos.y - 30);
        }
    } else {
        cam.begin();
        ofEnableDepthTest();
        ofRotateY(180);
//        ofScale(1.5, 1.5);
        
        mesh.draw();
        
        
        for (auto& hand : astra.getHandsWorld()) {
            auto& pos_real = hand.second;
            ofPoint pos = ofPoint((int)(pos_real.x*2.25),(int)(pos_real.y*1.875));
            ofSetColor(ofColor::white);
            ofDrawCircle(pos, 10);
            stringstream ss;
            ss << "id: " << hand.first << endl;
            ss << "pos: " << hand.second;
            ofDrawBitmapString(ss.str(), pos.x, pos.y - 30, pos.z);
        }
        
        ofDisableDepthTest();
        cam.end();
    
    //Old code
    /*
     ofSetColor(255,255,255);
     astra.drawDepth(0, 0);
     //astra.draw(640, 0);
     //mesh
     ofSetColor(0,255,0);
     for (int i = 0; i<mesh.getNumVertices(); i++) {
     // ofDrawBitmapString("0", mesh.getVertices()[i].x, mesh.getVertices()[i].y);
     //myFont.drawString("0", mesh.getVertices()[i].x,mesh.getVertices()[i].y);
     myFont.drawString(ofToString((int)ofRandom(0,2)), mesh.getVertices()[i].x,mesh.getVertices()[i].y);
     }
     
     //contourFinder.draw(640, 0);
     gui.draw();
     
     ofSetColor(255,0,0);
     ofDrawBitmapString(mesh.getNumVertices(), 10, 10);
     //ofDrawBitmapString(astra.getDepthImage().getWidth(), 10, 20);
     //ofDrawBitmapString(astra.getDepthImage().getHeight(), 10, 30);
     */
}
}
//}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r')
        reset();
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
