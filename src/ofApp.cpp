#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //  OF Settings
    ofBackground(0);
    ofSetFrameRate(60);
    ofHideCursor();
    
    //  Particles
    particles.setup(x_dim, y_dim);
}

//--------------------------------------------------------------
void ofApp::update(){

    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    particles.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    cam.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    particles.draw();
    ofDisableBlendMode();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
