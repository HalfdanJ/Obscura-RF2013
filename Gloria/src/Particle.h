//
//  Particle.h
//  emptyExample
//
//  Created by vibber on 26/06/14.
//
//

#include "ofMain.h"

class Particle {
public:
    
    void setup(float length);
    void draw(float lineWidth);
    void update();
    
    ofVec2f pos,vel,acc,oldpos;
    float angle;
    ofColor color;
    int age;
};
