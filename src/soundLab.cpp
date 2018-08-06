#include "ofApp.h"

// --------------------------------------------------------------

void ofApp::audioIn(ofSoundBuffer &input){
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++){
		mono[i]		= input[i * 2] * 0.5;
		curVol += mono[i] * mono[i];
		numCounted += 2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	 
	// this is how we get the root of rms :) 
	curVol = sqrt(curVol);

	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
}

// --------------------------------------------------------------

void ofApp::audioSetup(){
  int bufferSize = 256;
  ofSoundStreamSettings settings;
  // soundStream.printDeviceList();
  auto devices = soundStream.getMatchingDevices("default");
  if(!devices.empty()){
    settings.setInDevice(devices[0]);
  }
	
	mono.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

  settings.setInListener(this);
  settings.sampleRate = 44100;
  settings.numOutputChannels = 0;
  settings.numInputChannels = 2;
  settings.bufferSize = bufferSize;
  soundStream.setup(settings);
}

// --------------------------------------------------------------

void ofApp::audioUpdate(){

	// Map the volume 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	// Records the volume in an array
	volHistory.push_back(scaledVol);

	//  If we are bigger the the size we want to record - lets drop the oldest value
	if(volHistory.size() >= 400){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
}

// --------------------------------------------------------------

void ofApp::audioLineDraw(){
	
	ofSetLineWidth(3);
	int lines = 2000;
	for (int i = 1; i < lines; ++i){
		ofBeginShape();
		float step = 10;
		float range = i * step;
		float maxRange = ofGetHeight() / 2;
		float diff = ofMap(range, step, step * lines, 0, maxRange);

		ofSetColor((245 + i) % 255, (58 + (i * 2)) % 255, (135 + i) % 255); ofNoFill();	
		
		for (unsigned int j = 0; j < mono.size(); j++){
			float x = ofMap(j, 0, mono.size(), 0, ofGetWidth());

			float amp = ofGetHeight() / 2;
			float audioNorm = mono[j] * amp;	
			float amplitude = 200 + diff;
			float y = (ofGetHeight() / 2) - ofMap(audioNorm, -amp / 2, amp / 2, -amplitude , amplitude);
			
			ofVertex(x, y);
		}

	ofEndShape(false);
	}
}

// --------------------------------------------------------------

void ofApp::audioPanelSetup(){

}
