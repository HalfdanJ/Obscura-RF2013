#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"

class ContentScene {
    
public:
    
    string name = "unnamed scene";
    string oscAddress = "/default";

    // bool syphonDirect;

    ofFbo fbo;
    bool enabled = true;
    float opacity = 1;
    float speed = 1;
    
    int width;
    int height;
    
    ContentScene() {
    }
    
    virtual ~ContentScene(){}
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void setGui(ofxUICanvas * gui){}
    virtual void oscMessage(ofxOscMessage * m){}
    
    void setupScene(int width, int height) {
        fbo.allocate(width, height);
        setup();
    }
    
    void updateScene() {
        if(enabled) {
            update();
        }
    }
    
    void drawScene() {
        if(enabled) {
            fbo.begin();
            ofClear(0, 0);
            
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            
            draw();
            
            //glDisable(GL_BLEND);
            
            fbo.end();
        }
    }
    
};
