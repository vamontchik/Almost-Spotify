#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogNotice() << "Setup initialized!";

	//setups directory paths
	root = ofDirectory(""); // "data" folder
	std::string songPath = "songs";
	songFolder = ofDirectory(songPath);

	//creates songs folder if necessary
	if (!songFolder.exists()) {
		songFolder.create(true);
	}
	
	//populates the directories
	root.listDir();
	songFolder.listDir();

	//Log Info
	ofLogNotice() << "Root: " << root.getAbsolutePath();
	ofLogNotice() << "Size: " << root.size();
	for (int i = 0; i < root.size(); i++) {
		ofLogNotice() << "At Position " << i << ": " << root.getPath(i);
	}

	ofLogNotice() << "Song Folder: " << songFolder.getAbsolutePath();
	ofLogNotice() << "Size: " << songFolder.size();
	for (int i = 0; i < songFolder.size(); i++) {
		ofLogNotice() << "At Position " << i << ": " << songFolder.getPath(i);
	}

	//additional window setup
	ofSetWindowTitle("Music Player");
	ofSetFrameRate(GLOBAL_FRAME_RATE);
	ofSetBackgroundColor(ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (userSpecifiedPlay) {
		if (!songPlayer.getIsPlaying()) {
			if (queue.size() > 0) {
				ofFile nextSong = queue.front();
				queue.pop();

				ofLogNotice() << "Now Playing: " << nextSong.getFileName();

				songPlayer.unload();
				songPlayer.load(nextSong.getAbsolutePath());
				songPlayer.play();
			}
			else {
				songPlayer.unload();
				ofLogNotice() << "End of Queue!";
				userSpecifiedPlay = false;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofDrawBitmapString("Songs", GLOBAL_X_OFFSET, GLOBAL_Y_OFFSET);
	for (int i = 0; i < songFolder.size(); i++) {
		std::string name = std::to_string(i + 1) + ") " 
			+ songFolder.getFile(i).getFileName();
		ofDrawBitmapString(name, GLOBAL_X_OFFSET, 2 * GLOBAL_Y_OFFSET + i * GLOBAL_Y_OFFSET);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_1) {
		int songPos = -1;

		for (int i = 0; i < songFolder.size(); i++) {
			if (y >= (i + 1) * GLOBAL_Y_OFFSET && y <= (i + 2) * GLOBAL_Y_OFFSET) {
				songPos = i;
				break;
			}
		}

		if (songPos == -1) {
			// click in a location that wasn't in the y-bounds of any of the songs
			return;
		}

		//add to queue if a song is playing. else, play it.
		if (songPlayer.getIsPlaying()) {
			ofLogNotice() << "Added to Queue: " + songFolder.getFile(songPos).getFileName();
			queue.push(songFolder.getFile(songPos));
		}
		else {
			ofLogNotice() << "Now Playing: " + songFolder.getFile(songPos).getFileName();
			songPlayer.load(songFolder.getPath(songPos));
			songPlayer.play();
			userSpecifiedPlay = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}