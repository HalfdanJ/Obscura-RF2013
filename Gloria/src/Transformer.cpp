//
//  QuickTrail.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#include "Transformer.h"

void Transformer::setup(){
    
    name = "Transformer";
    oscAddress = "/plw";
    
    drawWidth = OUTWIDTH/2;
	drawHeight = OUTHEIGHT/2;
	// process all but the density on 16th resolution
	flowWidth = drawWidth/4;
	flowHeight = drawHeight/4;
	
    // Flow & Mask
	opticalFlow.setup(flowWidth, flowHeight);
	velocityMask.setup(drawWidth, drawHeight);
    
    fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight, true);
    // create a texture from our scene structure
    //fluid.addObstacle(<#ofTexture &_tex#>)
    
	// Visualisation
	//displayScalar.allocate(flowWidth, flowHeight);
	velocityField.allocate(flowWidth / 4, flowHeight / 4);
	temperatureField.allocate(flowWidth / 4, flowHeight / 4);
    
    inputFbo.allocate(flowWidth, flowHeight);
    
    lastTime = ofGetElapsedTimef();
    
    inputFbo.begin(); ofClear(0); inputFbo.end();

}

void Transformer::update(){
    
    deltaTime = ofGetElapsedTimef() - lastTime;
	lastTime = ofGetElapsedTimef();
	
    
}

void Transformer::draw(){;
    ofClear(0);
    /*for(int i =0; i<mapping->triangles.size();i++) {
    }*/
    
    /*inputFbo.begin();
    ofClear(0,0);
    syphonIn->draw(0,0,inputFbo.getWidth(),inputFbo.getHeight());
    inputFbo.end();*/
	
    
   deltaTimeSyphonFrame = ofGetElapsedTimef() - lastSyphonFrame;
    
   if (deltaTimeSyphonFrame > 0.1) {
       
       lastSyphonFrame = ofGetElapsedTimef();
       
		ofPushStyle();
		//ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		inputFbo.begin();

        syphonIn->draw(0,0,inputFbo.getWidth(),inputFbo.getHeight());
        
		inputFbo.end();
		ofPopStyle();
		
		opticalFlow.setSource(inputFbo.getTextureReference());
		opticalFlow.update(deltaTime);
		
		velocityMask.setDensity(inputFbo.getTextureReference());
		velocityMask.setVelocity(opticalFlow.getOpticalFlow());
		velocityMask.update();
	}
    
	fluid.addVelocity(opticalFlow.getOpticalFlowDecay());
	fluid.addDensity(velocityMask.getColorMask());
	fluid.addTemperature(velocityMask.getLuminanceMask());
    
    // add more forces
    
    
    fluid.update();
    
    
    
    ofPushStyle();
    fluid.draw(0, 0, OUTWIDTH, OUTHEIGHT);

    ofPopStyle();
    
    //cameraFbo.draw(0,0,OUTWIDTH, OUTHEIGHT);
}

void Transformer::parseOscMessage(ofxOscMessage *m){
    
}

void Transformer::setGui(){
    
    //gui->addSlider("speed/x", -1, 1, &xSpeed);
    
}
