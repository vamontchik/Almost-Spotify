#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "MusicPlayer.h"
#include "ModifiedWireframe.h"

class ofApp : public ofBaseApp {

private:
	ofxDatGuiButton* playButton_;
	ofxDatGuiButton* leftButton_;
	ofxDatGuiButton* rightButton_;
	ofxDatGuiSlider* volSlider_;
	ofxDatGuiSlider* songPositionSlider_;
	ofxDatGuiLabel* songInfoLabel_;
	ofxDatGuiLabel* songLengthLabel_;
	ofxDatGuiLabel* songSizeLabel_;
	ofxDatGuiLabel* songPosFractionLabel_;
	ofxDatGuiLabel* nowPlayingLabel_;
	ofxDatGuiLabel* playlistLabel_;
	ofxDatGuiLabel* endLabel_;
	ofxDatGuiScrollView* scroller_;
	ofxDatGuiTheme* theme_;
	MusicPlayer* player_;

	const double SLIDER_MIN_VAL = 0.0;
	const double SLIDER_MAX_VAL = 1.0;
	const int LABEL_LENGTH = 150;
	const int BUTTON_LENGTH = 150;
	const double INITIAL_VOLUME = 0.2;
	const double INITIAL_POSITION = 0.0;

	const std::string SEND_TO_PLAY = " |> ";
	const std::string SEND_TO_PAUSE = " || ";
	const std::string TO_LEFT = " < ";
	const std::string TO_RIGHT = " > ";
	const std::string PLAYLIST_TITLE = "---- PLAYLIST ----";
	const std::string SONG_INFO_TITLE = "Song Info";
	const std::string SONG_SIZE_TITLE = "Size: ";
	const std::string SONG_LENGTH_TITLE = "Length: ";
	const std::string END_TITLE = "---- SCROLL UP/DOWN FOR MORE SONGS ----";
	const std::string SONG_POS_SLIDER_TITLE = "Song Position";
	const std::string VOLUME_SLIDER_TITLE = "Volume";
	const std::string NOW_PLAYING_INFO_TITLE = "Now Playing: ";
	const std::string SONG_FRAC_TITLE = "Position: ";

private:
	~ofApp();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void onScrollViewEvent(ofxDatGuiScrollViewEvent);
	void onVolSliderEvent(ofxDatGuiSliderEvent);
	void onPosSliderEvent(ofxDatGuiSliderEvent);
	void onPlayButtonEvent(ofxDatGuiButtonEvent);
	void onLeftButtonEvent(ofxDatGuiButtonEvent);
	void onRightButtonEvent(ofxDatGuiButtonEvent);
	void setupGUI(int);

	/*
	Default event listeners from base gui. Most not used.

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
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
