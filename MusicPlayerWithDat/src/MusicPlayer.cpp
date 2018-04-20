#include "MusicPlayer.h"

MusicPlayer::MusicPlayer()
	: songFolder_(nullptr), songPlayer_(nullptr), songQueue_(nullptr),
	inPlay_(false), isPaused_(false)
{
	initFolderProcess();
	songPlayer_ = new ofSoundPlayer();
	songQueue_ = new std::queue<ofFile>();
	loadSongsFromDir();
}

MusicPlayer::~MusicPlayer()
{
	delete songFolder_;
	delete songPlayer_;
	delete songQueue_;
}

/*
	Initializes the ofDirectory object, as necessary.
*/
void MusicPlayer::initFolderProcess() {
	songFolder_ = new ofDirectory("songs");
	std::cout << SONG_FOLDER_PATH_PROMPT << songFolder_->getAbsolutePath() << std::endl;

	if (!songFolder_->exists()) {
		std::cout << SONG_FOLDER_CREATED << std::endl;
		songFolder_->create();
	}

	//necessary (according to documentation) to allow other parts of 
	//the ofDirectory object to work, such as size()
	songFolder_->listDir();
}

/*
	Loads all the files from the directory (stored the ofDirectory object)
	into the internal queue.
*/
void MusicPlayer::loadSongsFromDir() {
	std::cout << LOADING_SONGS_FROM_DIR << std::endl;
	std::vector<ofFile> songs = songFolder_->getFiles();

	if (songs.size() == 0) {
		std::cout << EMPTY_SONG_QUEUE << std::endl;
		return;
	}

	for (ofFile song : songs) {
		addSongToQueue(song);
	}

	std::cout << FINISHED_LOADING_FROM_DIR << std::endl;
}

/*
	Adds a song file to the internal queue.
*/
void MusicPlayer::addSongToQueue(ofFile fileToAdd) {
	songQueue_->push(fileToAdd);
	std::cout << ADDITION_TO_Q << fileToAdd.getFileName() << std::endl;
}

/*
	Unloads the song from the player. If the specified value is true,
	will push the song at the front of the queue to the back.
*/
void MusicPlayer::unloadSong(bool pushSongBack) {
	std::cout << SONG_UNLOADED << songQueue_->front().getFileName() << std::endl;

	songPlayer_->unload();
	if (pushSongBack) {
		songQueue_->push(songQueue_->front());
		songQueue_->pop();
	}
}

/*
	Loads a song file into internal sound player.
*/
void MusicPlayer::loadSongIntoPlayer(ofFile songFile) {
	// Use "true" so it streams files instead of loading the into memory...
	songPlayer_->load(songFile.getAbsolutePath(), true);
	std::cout << LOADED_TO_PLAYER << songFile.getFileName() << std::endl;
}

/*
	Plays the song from the front of the queue, if there is one.
	Else, return a message saying there is none.
*/
void MusicPlayer::play() {
	if (songQueue_->empty()) {
		std::cout << EMPTY_SONG_QUEUE << std::endl;
		return;
	}
	inPlay_ = true;
	playSongAtFront();
}

/*
	Changes whether the player is paused or unpaused.
*/
void MusicPlayer::changePauseState() {
	if (!isPaused_) {
		std::cout << PAUSING_SONG << songQueue_->front().getFileName() << std::endl;
		songPlayer_->setPaused(true);
		isPaused_ = true;
	}
	else {
		std::cout << UNPAUSING_SONG << songQueue_->front().getFileName() << std::endl;
		songPlayer_->setPaused(false);
		isPaused_ = false;
	}
}

/*
	Skips to the next song in the queue.
*/
void MusicPlayer::skipToNext() {
	if (songPlayer_->getIsPlaying()) {
		songPlayer_->stop(); //the next update call will start the next song
	}
}

/*
	Plays the song at the front of the queue.
*/
void MusicPlayer::playSongAtFront() {
	songPlayer_->play();
	std::cout << NOW_PLAYING << songQueue_->front().getFileName() << std::endl;
}

/*
	Called by update() to transition to the next song in the queue, if possible.
*/
void MusicPlayer::updateCurrentSong(
	std::string nowPlayingPrefix, ofxDatGuiLabel* nowPlayingPtr,
	std::string songLengthPrefix, ofxDatGuiLabel* songLengthPtr,
	std::string songSizePrefix, ofxDatGuiLabel* songSizePtr) 
{
	if (inPlay_) {
		//check if either:
		//	(1) a song has been stopped with the stop() method
		//	(2) a song has reached its end using (getPosition() == 1.0)
		if (!songPlayer_->getIsPlaying() || songPlayer_->getPosition() == 1.0) {
			//it's ok to use front() because the front has been pushed to back!
			unloadSong(true);

			loadSongIntoPlayer(songQueue_->front());

			//play the song at the front!
			playSongAtFront();

			//update the GUI elements for song info and "now playing"
			updateNowPlayingLabel(nowPlayingPrefix, nowPlayingPtr);
			updateSongLengthLabel(songLengthPrefix, songLengthPtr);
			updateSongSizeLabel(songSizePrefix, songSizePtr);
		}
	}
}

/*
	Returns whether or not this player is in a play session.

	Note: This variable is mostly used to guard againts wierd update
	issues, such as when the internal state is changing and we don't
	want our update method to do anything.
*/
bool MusicPlayer::inPlaySession() {
	return inPlay_;
}

/*
	Returns a copy of the internal song queue.
*/
std::queue<ofFile> MusicPlayer::getSongQueue() {
	return *songQueue_;
}

/*
	Returns whether or not this player is paused. This is changed solely
	by the changePauseState() method.

	Note: Upon initialization, this is set to false to allow the changePauseState()
	method to work properly.
*/
bool MusicPlayer::isPaused() {
	return isPaused_;
}

/*
	Skips through the contents of the queue, pushing song files to the back,
	until we reach the file that the user has selected. Then, start
	play of the song.
*/
void MusicPlayer::playSong(std::string baseName) {
	//lock to prevent updateCurrentSong() from changing state at the same time.
	inPlay_ = false;

	while (!inPlay_) {
		if (songQueue_->front().getBaseName() == baseName) {
			loadSongIntoPlayer(songQueue_->front());
			playSongAtFront();
			inPlay_ = true;
		}
		else {
			unloadSong(true);
		}
	}
}

/*
	Sets the volume of the player, based on user input.

	Note: Values range from 0.0 to 1.0, where 0.0 is muted, and 1.0 is the max volume.
*/
void MusicPlayer::setVolume(double value) {
	songPlayer_->setVolume(value);
}

/*
	Sets the position at which the song will play at.

	Note: Values range from 0.0 to 1.0, where 0.0 is the start, and 1.0 is the end of the song.
	However, the player will ignore the value of 1.0 (for whatever iternal reason), so skipping
	to the end by value means typing in 0.9999, not 1.0
*/
void MusicPlayer::setPosition(double value) {
	songPlayer_->setPosition(value);
}

/*
	Updates the value in song position slider's text field, if not paused.
	This is called on each call to update() in the GUI.
*/
void MusicPlayer::updateSongPosition(ofxDatGuiSlider* sliderPtr) {
	if (!isPaused_) {
		sliderPtr->setValue(songPlayer_->getPosition());
	}
}

/*
	Updates the Now Playing label with the name of the current song,
	if we are in a play session.
*/
void MusicPlayer::updateNowPlayingLabel(std::string prefix, ofxDatGuiLabel* labelPtr) {
	if (inPlay_) {
		labelPtr->setLabel(prefix + songQueue_->front().getBaseName());
	}
}

/*
	Updates the song length label with the correct data and format.

	Note: This will enter undefined behavior if the songQueue has a file
	that is longer than 60 hours, due to the array operator[] call.
	Thankfully, most users won't have that issue.
*/
void MusicPlayer::updateSongLengthLabel(std::string prefix, ofxDatGuiLabel* labelPtr) {
	if (inPlay_) {

		// LENGTH TRICK GRAB: 
		// https://forum.openframeworks.cc/t/ofsoundplayer-length/11560
		//
		// Note: This *is* a race condition but we ok because it's pretty fast ;)
		ofFile toBePlayed = songQueue_->front();
		songPlayer_->setPosition(0.9999999); //closest approx I can get is seven decimal places
		double length_inSeconds = songPlayer_->getPositionMS() / 1000.0;
		songPlayer_->setPosition(0);
		// END OF MAGIC TRICK

		std::string postfixes[] = {
			" seconds",
			" minutes",
			" hours"
		};

		double modLength = length_inSeconds;
		int postfixToUse = 0;

		while (modLength > 60) {
			modLength /= 60.0;
			postfixToUse++;
		}

		//truncated to two decimal places
		std::string truncated = std::to_string(modLength).substr(0, 4);

		labelPtr->setLabel(prefix + truncated + postfixes[postfixToUse]);
	}
}

/*
	Updates the song size label with the correct data and format.

	Note: This will enter undefined behavior if the songQueue has a file
	that is larger than 1024 terabytes, due to the array operator[] call.
	Thankfully, most users don't have that issue.
*/
void MusicPlayer::updateSongSizeLabel(std::string prefix, ofxDatGuiLabel* labelPtr) {
	if (inPlay_) {
		uint64_t size = songQueue_->front().getSize();
		
		std::string postfixes[] = {
			" bytes", 
			" kilobytes", 
			" megabytes", 
			" gigabytes", 
			" terabytes"
		};

		double modSize = size;
		int postfixToUse = 0;

		while (modSize > 1024) {
			modSize /= 1024.0;
			postfixToUse++;
		}

		//truncated to two decimal places
		std::string truncated = std::to_string(modSize).substr(0, 4);

		labelPtr->setLabel(prefix + truncated + postfixes[postfixToUse]);
	}
}