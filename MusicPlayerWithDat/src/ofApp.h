#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "MusicPlayer.h"

class ofApp : public ofBaseApp {

private:
	~ofApp();
	void setup();
	void update();
	void draw();
		
private:
	ofxDatGuiScrollView* scrollView_;
	ofxDatGui* panel_;
	ofxDatGuiTheme* theme_;
	MusicPlayer* player_;

private:
	void onScrollViewEvent(ofxDatGuiScrollViewEvent e);
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);

	/*
	Default event listeners from base gui. Not used.

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	*/	
};
