#include "ofApp.h"

/*
TODO:
	0.5) Shuffle button.
	1) Search?
		---> A box that the user is able to type the string they want to search for,
		and the scroll list will update itself to only show the relevant stuff.
		Once the search box is cleared, put back the full list.
*/

/*
	Destructor that deletes all the objects in this class.
*/
ofApp::~ofApp() {
	delete playButton_;
	delete leftButton_;
	delete rightButton_;
	delete volSlider_;
	delete songPositionSlider_;
	delete songInfoLabel_;
	delete songLengthLabel_;
	delete songSizeLabel_;
	delete songPosFractionLabel_;
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
		while (queue.size() > 0) {
			scroller_->add(queue.front().getBaseName());
			queue.pop();
		}
		
		scroller_->onScrollViewEvent(this, &ofApp::onScrollViewEvent);
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

	songLengthLabel_ = new ofxDatGuiLabel(SONG_LENGTH_TITLE);
	songLengthLabel_->setPosition(
		ofGetWidth() / 2,
		songInfoLabel_->getHeight()
	);
	songLengthLabel_->setTheme(theme_);
	songLengthLabel_->setWidth(ofGetWidth() / 2);
	songLengthLabel_->setLabelAlignment(ofxDatGuiAlignment::LEFT);

	songSizeLabel_ = new ofxDatGuiLabel(SONG_SIZE_TITLE);
	songSizeLabel_->setPosition(
		ofGetWidth() / 2, 
		songInfoLabel_->getHeight() +
		songLengthLabel_->getHeight()
	);
	songSizeLabel_->setTheme(theme_);
	songSizeLabel_->setWidth(ofGetWidth() / 2);
	songSizeLabel_->setLabelAlignment(ofxDatGuiAlignment::LEFT);

	songPosFractionLabel_ = new ofxDatGuiLabel(SONG_FRAC_TITLE);
	songPosFractionLabel_->setPosition(
		ofGetWidth() / 2,
		songInfoLabel_->getHeight() +
		songLengthLabel_->getHeight() +
		songSizeLabel_->getHeight()
	);
	songPosFractionLabel_->setTheme(theme_);
	songPosFractionLabel_->setWidth(ofGetWidth() / 2);
	songPosFractionLabel_->setLabelAlignment(ofxDatGuiAlignment::LEFT);

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

	//Play/Pause Button
	playButton_ = new ofxDatGuiButton(SEND_TO_PLAY);
	playButton_->setTheme(theme_);
	playButton_->setWidth(BUTTON_LENGTH);
	playButton_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	playButton_->setPosition(
		ofGetWidth() / 2,
		ofGetHeight() - playButton_->getHeight()
	);
	playButton_->onButtonEvent(this, &ofApp::onPlayButtonEvent);

	//Shuffle/Random Button
	shuffleButton_ = new ofxDatGuiButton(RANDOM);
	shuffleButton_->setTheme(theme_);
	shuffleButton_->setWidth(BUTTON_LENGTH);
	shuffleButton_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	shuffleButton_->setPosition(
		ofGetWidth() / 2 - BUTTON_LENGTH,
		ofGetHeight() - shuffleButton_->getHeight()
	);
	shuffleButton_->onButtonEvent(this, &ofApp::onShuffleEvent);

	//Left one button
	leftButton_ = new ofxDatGuiButton(TO_LEFT);
	leftButton_->setTheme(theme_);
	leftButton_->setWidth(BUTTON_LENGTH);
	leftButton_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	leftButton_->setPosition(
		ofGetWidth() / 2 - 2 * BUTTON_LENGTH,
		ofGetHeight() - leftButton_->getHeight()
	);
	leftButton_->onButtonEvent(this, &ofApp::onLeftButtonEvent);

	//Right one button
	rightButton_ = new ofxDatGuiButton(TO_RIGHT);
	rightButton_->setTheme(theme_);
	rightButton_->setWidth(BUTTON_LENGTH);
	rightButton_->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	rightButton_->setPosition(
		ofGetWidth() / 2 + BUTTON_LENGTH,
		ofGetHeight() - rightButton_->getHeight()
	);
	rightButton_->onButtonEvent(this, &ofApp::onRightButtonEvent);

	//Create position slider
	songPositionSlider_ = new ofxDatGuiSlider(SONG_POS_SLIDER_TITLE, 
		SLIDER_MIN_VAL, SLIDER_MAX_VAL, INITIAL_POSITION);
	songPositionSlider_->setPrecision(4);
	songPositionSlider_->setTheme(theme_);
	songPositionSlider_->setWidth(ofGetWidth(), LABEL_LENGTH);
	songPositionSlider_->setPosition(
		0,
		ofGetHeight() - 
		playButton_ ->getHeight() -
		songPositionSlider_->getHeight()
	);
	songPositionSlider_->onSliderEvent(this, &ofApp::onPosSliderEvent);

	//Create volume slider
	volSlider_ = new ofxDatGuiSlider(VOLUME_SLIDER_TITLE, 
		SLIDER_MIN_VAL, SLIDER_MAX_VAL, INITIAL_VOLUME);
	volSlider_->setPrecision(2);
	volSlider_->setTheme(theme_);
	volSlider_->setWidth(ofGetWidth(), LABEL_LENGTH);
	volSlider_->setPosition(
		0, 
		ofGetHeight() - 
		playButton_->getHeight() -
		songPositionSlider_->getHeight() - 
		volSlider_->getHeight()
	);
	volSlider_->onSliderEvent(this, &ofApp::onVolSliderEvent);
	player_->setVolume(INITIAL_VOLUME); //initial vol of the player

	// Now Playing label
	nowPlayingLabel_ = new ofxDatGuiLabel(NOW_PLAYING_INFO_TITLE);
	nowPlayingLabel_->setPosition(
		0,
		ofGetHeight() -
		playButton_->getHeight() -
		songPositionSlider_->getHeight() -
		volSlider_->getHeight() -
		nowPlayingLabel_->getHeight()
	);
	nowPlayingLabel_->setTheme(theme_);
	nowPlayingLabel_->setWidth(ofGetWidth());
	nowPlayingLabel_->setLabelAlignment(ofxDatGuiAlignment::CENTER);

	//total height --> for testing
	int totalHeight = 0;
	totalHeight += volSlider_->getHeight();
	totalHeight += songPositionSlider_->getHeight();
	totalHeight += playlistLabel_->getHeight();
	totalHeight += endLabel_->getHeight();
	if (scroller_) totalHeight += scroller_->getHeight();
	totalHeight += nowPlayingLabel_->getHeight();
	totalHeight += playButton_->getHeight();
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
	playButton_->update();
	leftButton_->update();
	rightButton_->update();
	shuffleButton_->update();
	volSlider_->update();
	songPositionSlider_->update();
	songInfoLabel_->update();
	songLengthLabel_->update();
	songSizeLabel_->update();
	songPosFractionLabel_->update();
	nowPlayingLabel_->update();
	playlistLabel_->update();
	endLabel_->update();
	if (scroller_) scroller_->update();

	//Player specific calls for updating components on each call to update()
	bool updated = player_->updateCurrentSong();
	if (updated) {
		player_->updateSongLengthLabel(SONG_LENGTH_TITLE, songLengthLabel_);
		player_->updateNowPlayingLabel(NOW_PLAYING_INFO_TITLE, nowPlayingLabel_);
		player_->updateSongSizeLabel(SONG_SIZE_TITLE, songSizeLabel_);
	}
	player_->updateSongPosition(songPositionSlider_);
	player_->updateSongPosFractionLabel(SONG_FRAC_TITLE, songPosFractionLabel_);
}

/*
	Draw method that (re)draws all the components. Called at the rate at which
	the framerate is specified.
*/
void ofApp::draw() {
	//Explicit calls to draw() to ensure they are drawn correctly
	playButton_->draw();
	leftButton_->draw();
	rightButton_->draw();
	shuffleButton_->draw();
	volSlider_->draw();
	songPositionSlider_->draw();
	songInfoLabel_->draw();
	songLengthLabel_->draw();
	songSizeLabel_->draw();
	songPosFractionLabel_->draw();
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

	//This is to modifiy the GUI elements on each click so they correspond
	//to the correct element... relying on update() doesn't 
	//seem to work
	player_->updatePlayPauseButton(SEND_TO_PLAY, SEND_TO_PAUSE, playButton_);

	//update to song length can't be called in update()... because
	//of the trick it uses to get the length
	player_->updateSongLengthLabel(SONG_LENGTH_TITLE, songLengthLabel_);

	//calls to update other GUI elements on event
	player_->updateNowPlayingLabel(NOW_PLAYING_INFO_TITLE, nowPlayingLabel_);
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
	//e.scale is more accurate than value, since it uses the precision you specify 
	player_->setPosition(e.scale);
}

/*
	Event handler for when a key is pressed by the user.
*/
void ofApp::keyPressed(int key) {
	if (key == ' ') { //space bar
		player_->changePauseState();
		player_->updatePlayPauseButton(SEND_TO_PLAY, SEND_TO_PAUSE, playButton_);
	}
}

/*
	Event handler for pause/play button: changes it between pause and play state.
*/
void ofApp::onPlayButtonEvent(ofxDatGuiButtonEvent e) {
	player_->changePauseState();
	player_->updatePlayPauseButton(SEND_TO_PLAY, SEND_TO_PAUSE, playButton_);
}

/*
	Event handler for moving left one song.
*/
void ofApp::onLeftButtonEvent(ofxDatGuiButtonEvent e) {
	player_->shiftLeftOneSong();

	//update to song length can't be called in update()... because
	//of the trick it uses to get the length
	player_->updateSongLengthLabel(SONG_LENGTH_TITLE, songLengthLabel_);

	//calls to update other GUI elements on event
	player_->updateNowPlayingLabel(NOW_PLAYING_INFO_TITLE, nowPlayingLabel_);
	player_->updateSongSizeLabel(SONG_SIZE_TITLE, songSizeLabel_);
}

/*
	Event handler for moving right one song. Uses a trick, so 
	doesn't need extra calls.
*/
void ofApp::onRightButtonEvent(ofxDatGuiButtonEvent e) {
	player_->shiftRightOneSong();

	//update to song length can't be called in update()... because
	//of the trick it uses to get the length
	player_->updateSongLengthLabel(SONG_LENGTH_TITLE, songLengthLabel_);

	//calls to update other GUI elements on event
	player_->updateNowPlayingLabel(NOW_PLAYING_INFO_TITLE, nowPlayingLabel_);
	player_->updateSongSizeLabel(SONG_SIZE_TITLE, songSizeLabel_);
}

void ofApp::onShuffleEvent(ofxDatGuiButtonEvent e) {
	player_->shuffleRightRandomAmount();

	//update to song length can't be called in update()... because
	//of the trick it uses to get the length
	player_->updateSongLengthLabel(SONG_LENGTH_TITLE, songLengthLabel_);

	//calls to update other GUI elements on event
	player_->updateNowPlayingLabel(NOW_PLAYING_INFO_TITLE, nowPlayingLabel_);
	player_->updateSongSizeLabel(SONG_SIZE_TITLE, songSizeLabel_);
}

