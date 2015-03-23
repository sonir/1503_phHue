#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxPhilipsHue.h"
#include "ofxOsc.h"


//Color Settings
#define DEFAULT_BRIGHTNESS 0.606f;
#define DEFAULT_SATURATION 0.45f;
#define DEFAULT_HUE 0.259f;


#define PORT_R 57142  //receive port
#define PORT_S 57137  //send port
#define PORT_Backdoor 6282  //test hue control by OSC

#define HOST "224.0.0.1"  //multicast
#define LOCAL "localhost"  //localhost

//#define HOST_test "10.0.1.16"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed( int x, int y, int button );

    
        //hue
		ofxPhilipsHue hue;
        bool state[6]; //on-off
        float b[6];    //brigtness
        float s[6];    //saturation
        float h[6];    //hue
        float d[6];    //transition duration in ms
        float clock[6];    //send command trigger
    
        int numLights;
        bool switchState;
        int on_off;
    
        bool ctrlAll;
    
        int fps;

        //OSC
        ofxOscReceiver receiver;
        ofxOscSender sender;
        float bird_goto = 1, bird_arrive = 1;
        float ch6_15 = 0.0;
        float ch6 = 0.0;
    
        //info
        ofTrueTypeFont text;
        int infoRectWidth;
    
        //flap
//        ofVideoPlayer flapMov;
};

#endif
