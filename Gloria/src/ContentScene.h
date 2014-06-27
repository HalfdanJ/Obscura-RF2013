#pragma once

#include "ofMain.h"
#include "Defines.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "mapping.h"
#include "ofxSyphon.h"

class ContentScene {
    
public:
    
    ContentScene(){}
    virtual ~ContentScene(){};
    
    Mapping * mapping;
    ofxSyphonClient * syphonIn;
    ofxUICanvas* gui;
    ofxSyphonServer syphonOut;
    
    int index;
    string name;
    string oscAddress = "/default";
    
    float minSpeed = 0;
    float maxSpeed = 1;
    int long time = 0;
    
    ofFbo fbo;
    bool enabled;
    bool solo = true;
    float opacity;
    float speed;
    
    int width;
    int height;
    int numSamples = 4;
    
    virtual void init();
    virtual void setup();
    virtual void update();
    virtual void draw();
    virtual void exit();
    virtual void setGui();
    
    void guiEvent(ofxUIEventArgs &e);
    void setSceneGui();
    void addSlider();
    void checkMsg();
    virtual void parseOscMessage(ofxOscMessage * m);
    void setupScene(int _width, int _height, int _i);
    void updateScene();
    void drawScene();
    
    
};
