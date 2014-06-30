#include "ofApp.h"
//#include "ofGlProgrammableRenderer.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
    
    //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    //ofSetupOpenGL(1600, 800, OF_WINDOW);
    
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
    
    
	ofRunApp(new ofApp()); // start the app
}
