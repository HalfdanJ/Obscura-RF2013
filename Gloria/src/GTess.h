//
//  QuickTrail.h
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"


class GTess : public ContentScene {

public:

    void setup();
    void draw();
    void debugDraw();
    void update();

    Mapping * mapping;

};
