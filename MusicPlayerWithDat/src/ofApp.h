#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "MusicPlayer.h"
#include "ModifiedWireframe.h"

class ofApp : public ofBaseApp {

private:
	ofxDatGuiSlider* volSlider_;
	ofxDatGuiSlider* songPositionSlider_;
	ofxDatGuiLabel* nowPlayingLabel_;
	ofxDatGuiLabel* playlistLabel_;
	ofxDatGuiLabel* endLabel_;
	ofxDatGuiScrollView* scroller_;
	ofxDatGuiTheme* theme_;
	MusicPlayer* player_;

	const double SLIDER_MIN_VAL = 0.0;
	const double SLIDER_MAX_VAL = 1.0;
	const int LABEL_LENGTH = 150;
	const double INITIAL_VOLUME = 0.2;
	const double INITIAL_POSITION = 0.0;

private:
	~ofApp();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void onScrollViewEvent(ofxDatGuiScrollViewEvent);
	void onVolSliderEvent(ofxDatGuiSliderEvent);
	void onPosSliderEvent(ofxDatGuiSliderEvent);
	void setupGUI(int);

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
