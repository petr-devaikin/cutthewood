#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxXmlSettings.h"

#include <librealsense2/rs.hpp>
//#include "ofxCv.h"


class ofApp : public ofBaseApp{
private:
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    
    const int PIXEL_SIZE_PX = 80;
    const int ACTIVE_LINE_Y_PX = 300; // where to show words;
    
    const int MIN_DISTANCE_M = 2;
    

    bool camFound;
    
    rs2::pipeline pipe;
    rs2::frameset frames;
    float depthScale;
    
    rs2::decimation_filter dec_filter;
    rs2::spatial_filter spat_filter;
    rs2::temporal_filter temp_filter;
    rs2::hole_filling_filter hole_filter;
    
    rs2::disparity_transform depth_to_disparity;
    rs2::disparity_transform disparity_to_depth;
    
    ofShader invertShader;
    
    //ofFbo result;
    ofImage rgbImage;
    ofFbo fontCanvas;
    
    void drawWord(int start, int length);
    
    char * pixels;
    ofTrueTypeFont myfont;
    
    vector<string> words;
public:
    void setup();
    void update();
    void draw();
    void exit();

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
