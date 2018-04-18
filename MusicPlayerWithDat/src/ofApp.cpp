#include "ofApp.h"

/*
	BUGS:
		- When the window is not in focused, it's still possible to
		click one of the buttons. Doing so crashes the program...
*/

ofApp::~ofApp() {
	delete theme_;
	delete panel_;
	delete player_;
}

void ofApp::setup() {
	//MusicPlayer Setup ---> LOAD SONGS NOW! and only once
	player_ = new MusicPlayer();

	ofSetBackgroundColor(ofColor::black);

	//Centered on screen
	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, ofGetScreenHeight() / 2 - ofGetWidth() / 2);
	
	//create theme
	theme_ = new ofxDatGuiThemeWireframe();

	//GUI Setup ---> put all songs up on the screen as buttons, add event listener, etc
	setupGUI();

	//frame limit to lessen work done by GPU
	ofSetFrameRate(20);
}

void ofApp::setupGUI() {
	panel_ = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	panel_->setTheme(theme_);
	panel_->setWidth(ofGetWidth());

	std::queue<ofFile> queue = player_->getSongQueue();
	while (queue.size() > 0) {
		panel_->addButton(queue.front().getBaseName());
		queue.pop();
	}

	panel_->addHeader("--- PLAYLIST ---");
	panel_->getHeader()->setDraggable(false);
	panel_->addFooter();
	panel_->getFooter()->setLabelWhenCollapsed("Click to expand playlist");
	panel_->getFooter()->setLabelWhenExpanded("Click to collapse playlist");

	panel_->onButtonEvent(this, &ofApp::onButtonEvent);
}

void ofApp::update() {
	if (!changingGui_) {
		//this needs to be here so that when the gui is changing,
		//update is called and breaks the program :(
		panel_->update(); 
		player_->updateCurrentSong();
	}
}

void ofApp::draw() {
	if (!changingGui_) {
		panel_->draw();
	}
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	//set flag to prevent GUI change
	changingGui_ = true;

	//changes playlist so the selected song goes to the front, and edit
	//the gui with the new playlist
	player_->playSong(e.target->getName());
	delete panel_;
	setupGUI();

	//flag set to allow GUI change
	changingGui_ = false;
}

void ofApp::keyPressed(int key) {
	if (key == ' ') { //space bar
		if (player_->inPlaySession()) {
			player_->changePauseState();
		}
	}
	else if (key == 's') {
		player_->skipToNext();
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	//do nothing
}