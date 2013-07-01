#pragma once

#include "ofMain.h"

class ContentScene {

public:

    ofFbo fbo;
    bool enabled;
    float opacity;

    int width;
    int height;

    ContentScene() {
    }

    virtual ~ContentScene(){}

    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}

    void setupScene(int width, int height) {
	fbo.allocate(width, height);
    }

    void updateScene() {
	if(enabled) {
	    update();
	}
    }

    void drawScene() {
	if(enabled) {
	    fbo.begin();
	    draw();
	    fbo.end();
	}
    }

};
