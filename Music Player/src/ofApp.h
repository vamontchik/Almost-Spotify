#ifndef OF_APP_H
#define OF_APP_H

#include "ofMain.h"
#include "ofFileUtils.h"

class ofApp : public ofBaseApp {
	std::queue<ofFile> queue;
	ofSoundPlayer songPlayer;
	ofDirectory root, songFolder;
	const int GLOBAL_X_OFFSET = 10;
	const int GLOBAL_Y_OFFSET = 15;
	const int GLOBAL_FRAME_RATE = 60;
	bool userSpecifiedPlay = false;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};

#endif //OF_APP_H
