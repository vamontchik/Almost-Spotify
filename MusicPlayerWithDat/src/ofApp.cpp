#include "ofApp.h"

/*
TODO:
	1) Is there a way to display mm:ss of the song?
*/

/*
	Destructor that deletes all the objects in this class.
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

/*
	Setup method that acts like a constructor for this class.
*/
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

/*
	Called by setup() to initialize all the GUI components.
*/
void ofApp::setupGUI(int nVisible) {
	//Create theme to be used in components
	theme_ = new ModifiedWireframe();

	//Label above playlist
	playlistLabel_ = new ofxDatGuiLabel(PLAYLIST_TITLE);
	playlistLabel_->setPosition(0, 0);
	playlistLabel_->setTheme(theme_);
	playlistLabel_->setWidth(ofGetWidth() / 2);
	playlistLabel_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	playlistLabel_->setVisible(true);
	
	/*
	Scroll view for songs / buttons, Event listener for scroll view, and Info Labels
		- Sets the nVisible size to up to 10, but can be less if there are less songs!
		- Explicit check for 0 because 0 items to a scroll view breaks it :/
	*/
	if (nVisible > 0) {
		scroller_ = new ofxDatGuiScrollView("playlist", min(nVisible, 10));
		scroller_->setPosition(0, playlistLabel_->getHeight());
		scroller_->setTheme(theme_);
		scroller_->setWidth(ofGetWidth() / 2);
		std::queue<ofFile> queue = player_->getSongQueue();
		int count = 0;
		while (queue.size() > 0) {
			scroller_->add(queue.front().getBaseName());
			count++;
			queue.pop();
		}
		
		scroller_->onScrollViewEvent(this, &ofApp::onScrollViewEvent);
		scroller_->setVisible(true);
	}

	/*
		Labels concerning the information of the song
	*/
	songInfoLabel_ = new ofxDatGuiLabel(SONG_INFO_TITLE);
	songInfoLabel_->setPosition(
		ofGetWidth() / 2, 
		0
	);
	songInfoLabel_->setTheme(theme_);
	songInfoLabel_->setWidth(ofGetWidth() / 2);
	songInfoLabel_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	songInfoLabel_->setVisible(true);

	songLengthLabel_ = new ofxDatGuiLabel(SONG_LENGTH_TITLE);
	songLengthLabel_->setPosition(
		ofGetWidth() / 2,
		songInfoLabel_->getHeight()
	);
	songLengthLabel_->setTheme(theme_);
	songLengthLabel_->setWidth(ofGetWidth() / 2);
	songLengthLabel_->setLabelAlignment(ofxDatGuiAlignment::LEFT);
	songLengthLabel_->setVisible(true);

	songSizeLabel_ = new ofxDatGuiLabel(SONG_SIZE_TITLE);
	songSizeLabel_->setPosition(
		ofGetWidth() / 2, 
		songInfoLabel_->getHeight() +
		songLengthLabel_->getHeight()
	);
	songSizeLabel_->setTheme(theme_);
	songSizeLabel_->setWidth(ofGetWidth() / 2);
	songSizeLabel_->setLabelAlignment(ofxDatGuiAlignment::LEFT);
	songSizeLabel_->setVisible(true);

	//Label that signifies the end of the playlist
	endLabel_ = new ofxDatGuiLabel(END_TITLE);
	endLabel_->setTheme(theme_);
	endLabel_->setWidth(ofGetWidth() / 2);
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
	songPositionSlider_ = new ofxDatGuiSlider(SONG_POS_SLIDER_TITLE, 
		SLIDER_MIN_VAL, SLIDER_MAX_VAL, INITIAL_POSITION);
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
	volSlider_ = new ofxDatGuiSlider(VOLUME_SLIDER_TITLE, 
		SLIDER_MIN_VAL, SLIDER_MAX_VAL, INITIAL_VOLUME);
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
	nowPlayingLabel_ = new ofxDatGuiLabel(NOW_PLAYING_INFO_TITLE);
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

/*
	Update method that updates all the components. Called at the rate at which
	the framerate is specified.
*/
void ofApp::update() {
	//Explicit calls to update() to ensure they are updated correctly
	volSlider_->update();
	songPositionSlider_->update();
	songInfoLabel_->update();
	songLengthLabel_->update();
	songSizeLabel_->update();
	nowPlayingLabel_->update();
	playlistLabel_->update();
	endLabel_->update();
	if (scroller_) scroller_->update();

	//Player specific calls for updating components that rely on the state of the music player
	player_->updateCurrentSong(
		NOW_PLAYING_INFO_TITLE, nowPlayingLabel_,
		SONG_LENGTH_TITLE, songLengthLabel_,
		SONG_SIZE_TITLE, songSizeLabel_
	);
	player_->updateSongPosition(songPositionSlider_);
}

/*
	Draw method that (re)draws all the components. Called at the rate at which
	the framerate is specified.
*/
void ofApp::draw() {
	//Explicit calls to draw() to ensure they are drawn correctly
	volSlider_->draw();
	songPositionSlider_->draw();
	songInfoLabel_->draw();
	songLengthLabel_->draw();
	songSizeLabel_->draw();
	nowPlayingLabel_->draw();
	playlistLabel_->draw();
	endLabel_->draw();
	if (scroller_) scroller_->draw();
}

/*
	Event handler for the user selecting an element in scrollView.
*/
void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e) {
	player_->playSong(e.target->getLabel());

	player_->updateNowPlayingLabel(NOW_PLAYING_INFO_TITLE, nowPlayingLabel_);
	player_->updateSongLengthLabel(SONG_LENGTH_TITLE, songLengthLabel_);
	player_->updateSongSizeLabel(SONG_SIZE_TITLE, songSizeLabel_);
}

/*
	Event handler for when the user changes the volume slider,
	or types into the volume slider's text field and presses enter.
*/
void ofApp::onVolSliderEvent(ofxDatGuiSliderEvent e) {
	//e.scale is more accurate than value, since it uses the precision you specify 
	player_->setVolume(e.scale);
}

/*
	Event handler for when the user changes the song position slider,
	or types into the song position slider's text field and presses enter.

	Note: Typing into this slider's field is advisable only when the song is paused. See
	README.md for more details.
*/
void ofApp::onPosSliderEvent(ofxDatGuiSliderEvent e) {
	//prevents a wierd crash if you place it in this if statement
	if (player_->inPlaySession()) {
		//e.scale is more accurate than value, since it uses the precision you specify 
		player_->setPosition(e.scale);
	}
}

/*
	Event handler for when a key is pressed by the user.
*/
void ofApp::keyPressed(int key) {
	if (key == ' ') { //space bar
		if (player_->inPlaySession()) {
			player_->changePauseState();
		}
	}
}