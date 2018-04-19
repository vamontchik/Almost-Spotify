#include "MusicPlayer.h"

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

MusicPlayer::MusicPlayer() 
	: songFolder_(nullptr), songPlayer_(nullptr), songQueue_(nullptr), 
	  inPlay_(false), inChangingState_(false), isPaused_(false)
{
	initFolderProcess();
	songPlayer_ = new ofSoundPlayer();
	songQueue_ = new std::queue<ofFile>();
	loadSongsInDir();
}

MusicPlayer::~MusicPlayer() 
{
	delete songFolder_;
	delete songPlayer_;
	delete songQueue_;
}


void MusicPlayer::loadSongsInDir() {
	std::cout << LOADING_SONGS_FROM_DIR << std::endl;
	std::vector<ofFile> songs = songFolder_->getFiles();

	if (songs.size() == 0) {
		std::cout << EMPTY_SONG_QUEUE << std::endl;
		return;
	}

	for (ofFile song : songs) {
		addSong(song);
	}

	std::cout << FINISHED_LOADING_FROM_DIR << std::endl;
}

/*
	If the queue is empty, load into both the queue and player.
	Else, only load it into the queue.
*/
void MusicPlayer::addSong(ofFile fileToAdd) {
	if (!songQueue_->empty()) {
		songQueue_->push(fileToAdd);

		std::cout << ADDITION_TO_Q << fileToAdd.getFileName() << std::endl;
	}
	else {
		loadSongIntoPlayer(fileToAdd);
		songQueue_->push(fileToAdd);

		std::cout << ADDITION_TO_Q << fileToAdd.getFileName() << std::endl;
	}
}

/*
	Unloads a song from the player and queue, putting the song to the end of the queue.
*/
void MusicPlayer::unloadSong(bool pushSongBack) {
	std::cout << SONG_UNLOADED << songQueue_->front().getFileName() << std::endl;

	songPlayer_->unload();

	if (pushSongBack) {
		songQueue_->push(songQueue_->front());
		songQueue_->pop();
	}
}

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

void MusicPlayer::skipToNext() {
	if (songPlayer_->getIsPlaying()) {
		songPlayer_->stop(); //the next update call will start the next song
	}
}

void MusicPlayer::playSongAtFront() {
	songPlayer_->play();
	std::cout << NOW_PLAYING << songQueue_->front().getFileName() << std::endl;
}

/*
	Called by update() to transition to the next song in the queue, if possible.
*/
void MusicPlayer::updateCurrentSong() {
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
		}
	}
}

bool MusicPlayer::inPlaySession() {
	return inPlay_;
}

std::queue<ofFile> MusicPlayer::getSongQueue() {
	return *songQueue_;
}

bool MusicPlayer::isPaused() {
	return isPaused_;
}

void MusicPlayer::playSong(std::string baseName) {
	//lock to prevent updateCurrentSong() from changing state at the same time.
	inPlay_ = false;

	while (!inPlay_) {
		if (songQueue_->front().getBaseName() == baseName) {
			loadSongIntoPlayer(songQueue_->front());
			playSongAtFront();
			inPlay_ = true;
			return;
		}
		else {
			unloadSong(true);
		}
	}
}

void MusicPlayer::setVolume(double value) {
	songPlayer_->setVolume(value);
}

void MusicPlayer::setPosition(double value) {
	songPlayer_->setPosition(value);
}

void MusicPlayer::updateSongPosition(ofxDatGuiSlider* sliderPtr) {
	sliderPtr->setValue(songPlayer_->getPosition());
}