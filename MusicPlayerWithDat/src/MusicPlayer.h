#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

class MusicPlayer {

private:
	ofDirectory* songFolder_;
	ofSoundPlayer* songPlayer_;
	std::queue<ofFile>* songQueue_;
	bool inPlay_;
	bool inChangingState_;
	bool isPaused_;

	const std::string SONG_FOLDER_NAME = "songs";
	const std::string SONG_FOLDER_PATH_PROMPT = "Path to Song Folder: ";
	const std::string SONG_FOLDER_CREATED = "Song Folder Created!";
	const std::string NOW_PLAYING = "Now Playing: ";
	const std::string ADDITION_TO_Q = "Added Song to Queue: ";
	const std::string LOADED_TO_PLAYER = "Loaded Song into Player: ";
	const std::string SONG_UNLOADED = "Song Unloaded: ";
	const std::string LOADING_SONGS_FROM_DIR = "Loading Songs from Directory...";
	const std::string FINISHED_LOADING_FROM_DIR = "Finished Loading Songs from Directory!";
	const std::string EMPTY_SONG_QUEUE = "Empty Song Queue!";
	const std::string PAUSING_SONG = "Pausing Song: ";
	const std::string UNPAUSING_SONG = "Unpausing Song: ";
	
private:
	void initFolderProcess();
	void loadSongsInDir();
	void addSong(ofFile);
	void loadSongIntoPlayer(ofFile);
	void playSongAtFront();
	void play();

public:
	MusicPlayer();											//default constructor
	MusicPlayer(const MusicPlayer&) = delete;				//copy constructor
	MusicPlayer(MusicPlayer&&) = delete;					//move constructor
	MusicPlayer& operator=(const MusicPlayer&) = delete;	//copy assignment operator
	MusicPlayer& operator=(MusicPlayer&&) = delete;			//move assignment operator
	~MusicPlayer();											//destructor

	void unloadSong(bool);
	void changePauseState();
	void skipToNext();
	void updateCurrentSong();
	bool inPlaySession();
	std::queue<ofFile> getSongQueue();
	bool isPaused();
	void playSong(std::string);
	void setVolume(double);
	void setPosition(double);
	void updateSongPosition(ofxDatGuiSlider*);
	void updateNowPlayingLabel(ofxDatGuiLabel*);
};