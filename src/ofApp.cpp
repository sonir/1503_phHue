#include "ofApp.h"

//2015.3.23 -

//-------------------------------------------------------------------------- setup
void testApp::setup(){

	ofBackground(0);
    fps = 60;
	ofSetFrameRate(60); //ghetto limit for the api call rate
	ofSetWindowTitle("ofxPhilipsHue");

    //hue
    numLights = 5;
    switchState = false;
    on_off = 0;
    
    for(int i = 1; i <= numLights; i++){
        //hue
        state[i] = true;
        b[i] = DEFAULT_BRIGHTNESS;
        s[i] = DEFAULT_SATURATION;
        h[i] = DEFAULT_HUE;
//        b[i] = 1.0;
//        s[i] = 0.5;
//        h[i] = 0.3;
        d[i] = 100.0;
        clock[i] = -1;
    }
    
 	hue.setup("10.0.1.28", "newdeveloper");
    hue.setAllState(state[1], b[1], s[1], h[1], d[1]);
    
    ctrlAll = false;
    
    
    //OSC
    receiver.setup(PORT_R);
//    receiver.setup(PORT_Backdoor);  //test hue control by OSC
    sender.setup(HOST, PORT_S);
    
    
    //info
    text.loadFont("LucidaGrande.ttc", 40);
    infoRectWidth = ofGetWidth()/numLights;
}


//-------------------------------------------------------------------------- update
void testApp::update(){

}


//-------------------------------------------------------------------------- draw
void testApp::draw(){
    
    //receive OSC message
    while(receiver.hasWaitingMessages()){
        ofxOscMessage r;
        receiver.getNextMessage(&r);
        
        if(r.getAddress() == "/go_to"){
            bird_goto = r.getArgAsFloat(0)+1;  //place(0-4)
        }
        
        else if(r.getAddress() == "/arrived_at"){
            bird_arrive = r.getArgAsFloat(0)+1;  //place(0-4)
        }
        
        else if(r.getAddress() == "/hue"){
            for(int i=1; i<=numLights; i++){
                    h[i] = r.getArgAsFloat(3*i-3);
                    s[i] = r.getArgAsFloat(3*i-2);
                    b[i] = r.getArgAsFloat(3*i-1);
                    d[i] = r.getArgAsFloat(9);
                    clock[i] = d[i];
            }
        }

        else if(r.getAddress() == "/hue/all_off"){
            int all_off = r.getArgAsInt32(0);
            for(int i=1; i<=numLights; i++){
                state[i] = false;
            }
            switchState = 0;
        }

        else if(r.getAddress() == "/hue/on"){
            on_off = r.getArgAsInt32(0)+1;
            state[on_off] = true;
            switchState = 1;
        }
        
        
//        else{
//            for(int i=1; i<=numLights; i++){
//                    h[i] = r.getArgAsFloat(3*i-3);
//                    s[i] = r.getArgAsFloat(3*i-2);
//                    b[i] = r.getArgAsFloat(3*i-1);
//                    d[i] = r.getArgAsFloat(9);
////                    d[i] = 100;
//                    clock[i] = d[i];
//            }
//        }
    }

    
    //OSC, show info
    ofSetColor(255);
    for(int i=1; i<=numLights; i++){
        /* OSC */
        if(switchState==1){ /* hue/on */
            hue.setLightState(on_off, state[on_off]);
            switchState = -1;
        }else if(switchState==0){ /* hue/all_off */
            hue.setLightState(on_off, false);
            switchState = -1;
        }
        
        
//        clock[i] -= 1000/fps;
//        if(clock[i]<0){
//            hue.setLightState(
//                              i,		//light bulb ID #
//                              state[i],		//on-off
//                              b[i],	    //brightness
//                              s[i],		//sat
//                              h[i],		//hue
//                              d[i]		//transition duration in ms
//                              );
//        }
//        if(clock[i]<0){ clock[i]=0;
        
        
        /* info -current color, index, hue state- */
        //current color
         //background
        ofColor infoRect = ofColor::fromHsb(h[i]*255, s[i]*255, b[i]*255);
        if(state[i]==true){
            ofSetColor(infoRect);
        }else{
            ofSetColor(0);
        }
        ofRect(infoRectWidth*(i-1), 80, infoRectWidth*i, ofGetHeight());
        
         //partiton
        ofSetColor(255);
        ofLine(infoRectWidth*(i-1), 0, infoRectWidth*(i-1), ofGetHeight());
        
        //index
        string hue_index = ofToString(i);
        if(state[i]){
            ofSetColor(255);
        }else{
            ofSetColor(70);
        }
        text.drawString(" " + hue_index, infoRectWidth*(i-1), 58);
        
        //hue state
        ofSetColor(255);
        ofDrawBitmapString("state: " + string(state[i] ? "on" : "off") + "\n"
                           + "brigtness: " + ofToString(b[i]) + "\n"
                           + "saturation: " + ofToString(s[i]) + "\n"
                           + "hue: " + ofToString(h[i]) + "\n"
                           + "duration: " + ofToString(d[i]) + " ms" + "\n"
//                           + "trigger: " + ofToString(clock[i]) + " ms"
                           ,95+(infoRectWidth*(i-1)) ,15);
        }
    
        //debug
        ofDrawBitmapString(ofToString(switchState) + "\n"
                           + ofToString(bird_goto) + "\n"
                           + ofToString(bird_arrive) + "\n"
                           + ofToString(on_off) + "\n"
                           ,15 ,15);

    
    //color pickker
    int mPosX = mouseX%infoRectWidth;
    ofColor c;
    c.setHsb(
             255 * ofMap(mPosX, 0, infoRectWidth, 0, 1, true),
             255,
             255 * ofMap(mouseY, 0, ofGetHeight(), 1, 0, true)
             );
    ofSetColor(c);
    
    if(ctrlAll){
        for(int i=1; i<=numLights; i++){
            ofCircle(ofGetWidth()/numLights*(i-1)+mPosX, mouseY, 12);
        }
    }else{
        ofCircle(mouseX, mouseY, 12);
    }
}


//-------------------------------------------------------------------------- keyPressed
void testApp::keyPressed(int key){
    
    if(key == ' '){  // turn off all hue
        for(int i = 1; i <= numLights; i++){
            state[i] = false;
            hue.setLightState(i, false);
            b[i] = 0;
        }
    }
    
    else if(key=='a'){ // enable/disable 
        ctrlAll = !ctrlAll;
        if(ctrlAll){
            
        }else{
            
        }
    }
}


//-------------------------------------------------------------------------- mousePressed
void testApp::mousePressed( int x, int y, int button ){

	float lastBrightness = ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
	float lastHue = ofMap(mouseX%infoRectWidth, 0, infoRectWidth, 0, 1, true);

    if(ctrlAll){
        hue.setAllState(
                        true,			//on-off
                        lastBrightness,	//brightness
                        1.0,			//sat
                        lastHue,		//hue
                        100
                        );
        for(int i = 1; i <= numLights; i++){
            b[i] = lastBrightness;
            h[i] = lastHue;
            state[i] = true;
        }
    }else{
        hue.setAllState(false);
        for(int i = 1; i <= numLights; i++){
            b[i] = 0.0;
            h[i] = 0.0;
            d[i] = 100.0;
            state[i] = false;
        }
        
        int index = mouseX/infoRectWidth + 1;
        b[index] = lastBrightness;
        h[index] = lastHue;
        state[index] = true;
        hue.setLightState(
                          index,		//light bulb ID #
                          true,			//on-off
                          b[index],	//brightness
                          1.0,			//sat
                          h[index],		//hue
                          100			//transition duration in ms
                          );
    }
	
}