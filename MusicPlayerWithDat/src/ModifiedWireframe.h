#pragma once

#include "ofxDatGui.h"

/*
	How to modify the font size on a theme:
	https://github.com/braitsch/ofxDatGui/issues/103
*/
class ModifiedWireframe : public ofxDatGuiThemeWireframe {
public:
	ModifiedWireframe() {
		font.size = 8;
		init();
	}
};
