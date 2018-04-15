#include "ofApp.h"

ofApp::~ofApp() {
	delete theme_;
	delete panel_;
	delete player_;
	delete scrollView_;
}

void ofApp::setup() {
	//MusicPlayer Setup
	player_ = new MusicPlayer();

	//Background Color
	ofSetBackgroundColor(ofColor::black);

	//GUI Setup ---> centered on screen
	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, ofGetScreenHeight() / 2 - ofGetWidth() / 2);
	
	scrollView_ = new ofxDatGuiScrollView("PLAYLIST", 10);
	theme_ = new ofxDatGuiThemeWireframe();
	scrollView_->setTheme(theme_);
	scrollView_->setBackgroundColor(ofColor::black);
	scrollView_->setWidth(ofGetWidth());
	scrollView_->onScrollViewEvent(this, &ofApp::onScrollViewEvent);

	/*
	panel_ = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	theme_ = new ofxDatGuiThemeSmoke();
	panel_->setTheme(theme_);

	panel_->addHeader("PLAYLIST");
	panel_->getHeader()->setDraggable(false);
	panel_->addFooter();
	panel_->getFooter()->setLabelWhenCollapsed("Expand!");
	panel_->getFooter()->setLabelWhenExpanded("Collapse!");
	panel_->setWidth(ofGetWidth());
	panel_->onButtonEvent(this, &ofApp::onButtonEvent);
	*/

	//frame limit to lessen work done by GPU
	ofSetFrameRate(20);
}

void ofApp::update() {
	player_->updateCurrentSong();
	player_->updateSongList(scrollView_);
}

void ofApp::draw() {
	scrollView_->draw();
}

void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e) {
	
}

void ofApp::keyPressed(int key) {
	if (key == 'l') {
		player_->loadSongsInDir();
	}
	else if (key == 'p') {
		player_->play();
	}
	else if (key == 'c') {
		player_->changePauseState();
	}
	else if (key == 's') {
		player_->skipToNext();
	}
}

void ofApp::mousePressed(int x, int y, int button) {

}