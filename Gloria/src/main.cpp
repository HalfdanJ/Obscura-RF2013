#include "ofApp.h"
#include "ofGlProgrammableRenderer.h"

//--------------------------------------------------------------
int main(){
    
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofSetupOpenGL(1600, 800, OF_WINDOW);
    
	ofRunApp(new ofApp()); // start the app
}
