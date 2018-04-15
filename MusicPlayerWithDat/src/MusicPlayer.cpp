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
	  inPlay_(false), updatedQueue_(false)
{
	initFolderProcess();
	songPlayer_ = new ofSoundPlayer();
	songQueue_ = new std::queue<ofFile>();
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
		songPlayer_->load(fileToAdd.getAbsolutePath());
		songQueue_->push(fileToAdd);

		std::cout << ADDITION_TO_Q << fileToAdd.getFileName() << std::endl;
		std::cout << LOADED_TO_PLAYER << fileToAdd.getFileName() << std::endl;
	}
	
	//flag for gui reset
	updatedQueue_ = true;
}

/*
	Unloads a song from the player and queue.
*/
void MusicPlayer::unloadSong() {
	std::cout << SONG_UNLOADED << songQueue_->front().getFileName() << std::endl;
	songPlayer_->unload();
	songQueue_->pop();

	//flag for gui reset
	updatedQueue_ = true;
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

	if (songPlayer_->getIsPlaying()) {
		std::cout << SONG_PLAYING_RIGHT_NOW << std::endl;
		return;
	}

	inPlay_ = true;
	songPlayer_->play();
	std::cout << NOW_PLAYING << songQueue_->front().getFileName() << std::endl;
}

void MusicPlayer::changePauseState() {
	static bool notPaused = true; // bad solution to the fact that there's no isPaused() ...
	if (notPaused) {
		std::cout << PAUSING_SONG << songQueue_->front().getFileName() << std::endl;
		songPlayer_->setPaused(true);
		notPaused = false;
	}
	else {
		std::cout << UNPAUSING_SONG << songQueue_->front().getFileName() << std::endl;
		songPlayer_->setPaused(false);
		notPaused = true;
	}
}

void MusicPlayer::skipToNext() {
	if (songPlayer_->getIsPlaying()) {
		songPlayer_->stop(); //the next update call will start the next song

		//trigger gui flag to reset
		updatedQueue_ = true;
	}
}

/*
	Called by update() to check whether or not it's time to 
	transition to the next song in the queue. If it hits the end,
	a log message is printed and inPlay_ is set to false.
*/
void MusicPlayer::updateCurrentSong() {
	if (inPlay_) {
		if (!songPlayer_->getIsPlaying()) {
			if (songQueue_->size() > 1) {
				//grab next song (2nd item in queue) from front of the queue. 
				unloadSong();
				ofFile nextSong = songQueue_->front();

				//load and play the song using the reference to the file
				songPlayer_->load(nextSong.getAbsolutePath());
				std::cout << LOADED_TO_PLAYER << nextSong.getFileName() << std::endl;
				
				songPlayer_->play();
				std::cout << NOW_PLAYING << nextSong.getFileName() << std::endl;
			}
			else {
				//unload the last song to prevent it from 
				//hogging memory in both queue and player
				unloadSong();

				//print out end state
				std::cout << END_OF_Q << std::endl;

				//set inPlay_ variable to false to prevent
				//repeated calls to unload() and pop()
				inPlay_ = false;
			}

			//flag for gui reset
			updatedQueue_ = true;
		}
	}
}

//Use scroll view b/c it has a clear() method... :3
void MusicPlayer::updateSongList(ofxDatGuiScrollView* scrollViewPtr) {
	if (updatedQueue_) {
		std::deque<ofFile> allowTraversal = songQueue_->_Get_container();

		//add in a scroll view for each song
		scrollViewPtr->clear();
		for (ofFile songFile : allowTraversal) {
			scrollViewPtr->add(songFile.getBaseName());
		}

		//reset GUI flag because it's been updated
		updatedQueue_ = false;
	}
}