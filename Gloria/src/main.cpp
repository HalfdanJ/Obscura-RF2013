#include "ofApp.h"
//#include "ofGlProgrammableRenderer.h"

//--------------------------------------------------------------
int main(){
    
    //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    //ofSetupOpenGL(1600, 800, OF_WINDOW);
    
	ofSetupOpenGL(1100,1200, OF_WINDOW);			// <-------- setup the GL context
    
	ofRunApp(new ofApp()); // start the app
}
