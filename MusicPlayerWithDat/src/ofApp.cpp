#include "ofApp.h"

/*
TODO:
	1) Is there a way to display mm:ss of the song?
*/

ofApp::~ofApp() {
	delete volSlider_;
	delete songPositionSlider_;
	delete nowPlayingLabel_;
	delete playlistLabel_;
	delete endLabel_;
	delete scroller_;
	delete theme_;
	delete player_;
}

void ofApp::setup() {
	//MusicPlayer setup ---> loads songs now and only once!
	player_ = new MusicPlayer();

	//Screen settings
	ofSetWindowPosition(
		ofGetScreenWidth() / 2 - ofGetWidth() / 2, 
		ofGetScreenHeight() / 2 - ofGetHeight() / 2
	);

	//Setup for all GUI components
	setupGUI(player_->getSongQueue().size());

	//Frame limit to lessen work done by GPU
	ofSetFrameRate(60);
}

void ofApp::setupGUI(int nVisible) {
	//Create theme to be used in components
	theme_ = new ModifiedWireframe();

	//Label above playlist
	playlistLabel_ = new ofxDatGuiLabel("---- PLAYLIST ----");
	playlistLabel_->setPosition(0, 0);
	playlistLabel_->setTheme(theme_);
	playlistLabel_->setWidth(ofGetWidth());
	playlistLabel_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	playlistLabel_->setVisible(true);
	
	/*
	Scroll view for songs / buttons, Event listener for scroll view
		- Sets the nVisible size to up to 10, but can be less if there are less songs!
		- Explicit check for 0 because 0 items to a scroll view breaks it :/
	*/
	if (nVisible > 0) {
		scroller_ = new ofxDatGuiScrollView("PLAYLIST", min(nVisible, 10));
		scroller_->setPosition(0, playlistLabel_->getHeight());
		scroller_->setTheme(theme_);
		scroller_->setWidth(ofGetWidth());
		std::queue<ofFile> queue = player_->getSongQueue();
		while (queue.size() > 0) {
			scroller_->add(queue.front().getBaseName());
			queue.pop();
		}
		scroller_->onScrollViewEvent(this, &ofApp::onScrollViewEvent);
	}

	//Label that signifies the end of the playlist
	endLabel_ = new ofxDatGuiLabel("---- SCROLL UP/DOWN FOR MORE SONGS ----");
	endLabel_->setTheme(theme_);
	endLabel_->setWidth(ofGetWidth());
	endLabel_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	if (scroller_) {
		endLabel_->setPosition(
			0, 
			playlistLabel_->getHeight() + 
			scroller_->getHeight()
		);
	}
	else {
		endLabel_->setPosition(
			0, 
			playlistLabel_->getHeight()
		);
	}
	endLabel_->setVisible(true);


	//Create position slider
	songPositionSlider_ = new ofxDatGuiSlider("Song Position", SLIDER_MIN_VAL, SLIDER_MAX_VAL, INITIAL_POSITION);
	songPositionSlider_->setPrecision(4);
	songPositionSlider_->setTheme(theme_);
	songPositionSlider_->setWidth(ofGetWidth(), LABEL_LENGTH);
	songPositionSlider_->setPosition(
		0,
		ofGetHeight() - songPositionSlider_->getHeight()
	);
	songPositionSlider_->onSliderEvent(this, &ofApp::onPosSliderEvent);
	songPositionSlider_->setVisible(true);


	//Create volume slider
	//	- Also sets this init value to the volume of the player, to keep it consistent.
	volSlider_ = new ofxDatGuiSlider("Volume", SLIDER_MIN_VAL, SLIDER_MAX_VAL, INITIAL_VOLUME);
	volSlider_->setPrecision(2);
	volSlider_->setTheme(theme_);
	volSlider_->setWidth(ofGetWidth(), LABEL_LENGTH);
	volSlider_->setPosition(
		0, 
		ofGetHeight() - 
		songPositionSlider_->getHeight() - 
		volSlider_->getHeight()
	);
	volSlider_->onSliderEvent(this, &ofApp::onVolSliderEvent);
	volSlider_->setVisible(true);

	// Player volume init
	player_->setVolume(volSlider_->getValue());

	// Now Playing label
	nowPlayingLabel_ = new ofxDatGuiLabel("Now Playing: ");
	nowPlayingLabel_->setPosition(
		0,
		ofGetHeight() -
		volSlider_->getHeight() -
		songPositionSlider_->getHeight() -
		nowPlayingLabel_->getHeight()
	);
	nowPlayingLabel_->setTheme(theme_);
	nowPlayingLabel_->setWidth(ofGetWidth());
	nowPlayingLabel_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	nowPlayingLabel_->setVisible(true);

	//total height --> for testing
	int totalHeight = 0;
	totalHeight += volSlider_->getHeight();
	totalHeight += songPositionSlider_->getHeight();
	totalHeight += playlistLabel_->getHeight();
	totalHeight += endLabel_->getHeight();
	if (scroller_) totalHeight += scroller_->getHeight();
	totalHeight += nowPlayingLabel_->getHeight();
	std::cout << "Total Height: " << totalHeight << std::endl;

	//Console output for all available fonts
	ofxSmartFont::list();
}

void ofApp::update() {
	//Explicit calls to update() to ensure they are updated correctly
	volSlider_->update();
	songPositionSlider_->update();
	nowPlayingLabel_->update();
	playlistLabel_->update();
	endLabel_->update();
	if (scroller_) scroller_->update();

	//Player specific calls for updating components that rely
	//on the state of the music player
	player_->updateCurrentSong();
	if (!player_->isPaused()) {
		player_->updateSongPosition(songPositionSlider_);
	}
	player_->updateNowPlayingLabel(nowPlayingLabel_);
}

void ofApp::draw() {
	//Explicit calls to draw() to ensure they are drawn correctly
	volSlider_->draw();
	songPositionSlider_->draw();
	nowPlayingLabel_->draw();
	playlistLabel_->draw();
	endLabel_->draw();
	if (scroller_) scroller_->draw();
}

void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e) {
	player_->playSong(e.target->getName());
}

void ofApp::onVolSliderEvent(ofxDatGuiSliderEvent e) {
	player_->setVolume(e.scale);
}

void ofApp::onPosSliderEvent(ofxDatGuiSliderEvent e) {
	//prevents a wierd crash if you place it in this if statement
	if (player_->inPlaySession()) {
		player_->setPosition(e.scale);
	}
}

void ofApp::keyPressed(int key) {
	if (key == ' ') { //space bar
		if (player_->inPlaySession()) {
			player_->changePauseState();
		}
	}
}