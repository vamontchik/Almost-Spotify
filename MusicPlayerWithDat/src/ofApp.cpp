#include "ofApp.h"

ofApp::~ofApp() {
	delete theme_;
	delete scroller_;
	delete player_;
}

void ofApp::setup() {
	//MusicPlayer Setup ---> LOAD SONGS NOW! and only once
	player_ = new MusicPlayer();

	ofSetBackgroundColor(ofColor::black);

	//Centered on screen
	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, ofGetScreenHeight() / 2 - ofGetWidth() / 2);
	
	//create theme
	//theme_ = new ofxDatGuiThemeWireframe();
	theme_ = new ModifiedWireframe();

	//GUI Setup ---> put all songs up on the screen as buttons, add event listener, etc
	setupGUI();

	//frame limit to lessen work done by GPU
	ofSetFrameRate(60);
}

void ofApp::setupGUI() {
	scroller_ = new ofxDatGuiScrollView("---- PLAYLIST ----", 10);
	scroller_->setAnchor(ofxDatGuiAnchor::TOP_LEFT);
	scroller_->setTheme(theme_);
	scroller_->setWidth(ofGetWidth());

	std::queue<ofFile> queue = player_->getSongQueue();
	while (queue.size() > 0) {
		scroller_->add(queue.front().getBaseName());
		queue.pop();
	}

	scroller_->onScrollViewEvent(this, &ofApp::onScrollViewEvent);

	ofxSmartFont::list();
}

void ofApp::update() {
	scroller_->update();
	player_->updateCurrentSong();
}

void ofApp::draw() {
	scroller_->draw();
}

void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e) {
	//essentially, skips through song list until we get to the correct song...
	player_->playSong(e.target->getName());
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