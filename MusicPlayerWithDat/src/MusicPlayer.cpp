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
	: songFolder_(nullptr), songPlayer_(nullptr), songQueue_(nullptr), inPlay_(false), inChangingState_(false)
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
	}
}

void MusicPlayer::playSongAtFront() {
	songPlayer_->play();
	std::cout << NOW_PLAYING << songQueue_->front().getFileName() << std::endl;
}

/*
	Called by update() to check whether or not it's time to 
	transition to the next song in the queue. If it hits the end,
	a log message is printed and inPlay_ is set to false.
*/
void MusicPlayer::updateCurrentSong() {
	if (inPlay_) {
		if (!songPlayer_->getIsPlaying()) {
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

void MusicPlayer::playSong(std::string baseName) {
	//lock to prevent updateCurrentSong() from changing state at the same time.
	inPlay_ = false;

	ofFile songGrab;
	std::queue<ofFile> copy = getSongQueue();

	while (copy.size() > 0) {
		
		ofFile frontFile = copy.front();
		if (frontFile.getBaseName() == baseName) {
			unloadSong(false);
			putSongInFront(frontFile);
			loadSongIntoPlayer(songQueue_->front());
			playSongAtFront();
			inPlay_ = true;
			return;
		}
		
		//iteration through queue
		copy.pop();
	}
}

void MusicPlayer::putSongInFront(ofFile songToMove) {
	std::queue<ofFile> currentQueue = getSongQueue();
	std::queue<ofFile> modifiedQueue;

	modifiedQueue.push(songToMove);
	while (currentQueue.size() > 0) {

		//prevent duplicates
		if (currentQueue.front().getBaseName() == songToMove.getBaseName()) {
			currentQueue.pop();
			continue;
		}
		modifiedQueue.push(currentQueue.front());
		currentQueue.pop();
	}

	//change the queue to what we need --> move constructor makes it faster?
	delete songQueue_;
	songQueue_ = new std::queue<ofFile>(std::move(modifiedQueue));
}