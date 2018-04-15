#include "ofApp.h"

ofApp::~ofApp() {
	delete theme;
	delete panel;
	delete player;
}

void ofApp::setup() {
	//MusicPlayer Setup
	player = new MusicPlayer();

	//GUI Setup
	//TODO: is there a way to get the screen size???
	ofSetWindowPosition(1920 / 2 - 800 / 2, 1080 / 2 - 600 / 2);
	 
	panel = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	theme = new ofxDatGuiThemeWireframe();
	panel->setTheme(theme);

	panel->addButton("Click Me!");
	panel->addHeader("Drag Me Around!");
	panel->addFooter();
	panel->getFooter()->setLabelWhenCollapsed("Expand!");
	panel->getFooter()->setLabelWhenExpanded("Collapse!");

	panel->onButtonEvent(this, &ofApp::onButtonEvent);
}

void ofApp::update() {
	player->updateCurrentSong();
}

void ofApp::draw() {
	
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	
}

void ofApp::keyPressed(int key) {
	if (key == 'l') {
		player->loadSongsInDir();
	}
	else if (key == 'p') {
		player->play();
	}
	else if (key == 'c') {
		player->changePauseState();
	}
	else if (key == 's') {
		player->skipToNext();
	}
}

void ofApp::mousePressed(int x, int y, int button) {

}