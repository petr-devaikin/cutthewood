#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    myfont.load("opensans.ttf", 80);
    
    // init words
    words.push_back("f");
    words.push_back("fu");
    words.push_back("fuc");
    words.push_back("fuck");
    words.push_back("fuck!");
    words.push_back("fuck!f");
    words.push_back("fuck!fu");
    words.push_back("fuck!fuc");
    words.push_back("fuck!fuck");
    words.push_back("fuck!fuck!");

    pixels = new char[WIDTH / PIXEL_SIZE_PX]();
    // init images and buffers
    
    rgbImage.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    fontCanvas.allocate(WIDTH, HEIGHT, GL_RGBA);
    
    // init camera
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, WIDTH, HEIGHT);
    cfg.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT);
    //cfg.enable_stream(RS2_STREAM_INFRARED, WIDTH, HEIGHT);
    rs2::context ctx;
    auto device_list = ctx.query_devices();
    
    if (device_list.size() > 0) {
        rs2::pipeline_profile profile = pipe.start(cfg);
        auto depth_sensor = profile.get_device().first<rs2::depth_sensor>();
        
        // get scale from the sensor
        depthScale = depth_sensor.get_depth_scale();
        
        frames = pipe.wait_for_frames();
        cout << "RealSense found!\n";
        camFound = true;
    }
    else {
        cout << "Camera not found!\n";
        camFound = false;
    }
    
    // init filters
    depth_to_disparity = rs2::disparity_transform(true);
    disparity_to_depth = rs2::disparity_transform(false);
    
    // load shaders
    invertShader.load("shadersGL3/invert");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (!camFound) return;
    
    rs2::frameset newFrames;
    if (pipe.poll_for_frames(&newFrames))
        frames = newFrames;
    
    // update rgb picture
    rs2::frame rgbFrame = frames.get_color_frame();
    rs2::depth_frame depthFrame = frames.get_depth_frame();
    rs2::video_frame irFrame = frames.get_infrared_frame();
    
    memcpy(rgbImage.getPixels().getData(), rgbFrame.get_data(), WIDTH * HEIGHT * 3);
    rgbImage.update();
    
    // apply depth filters
    depthFrame = depth_to_disparity.process(depthFrame);
    depthFrame = spat_filter.process(depthFrame);
    depthFrame = temp_filter.process(depthFrame);
    depthFrame = disparity_to_depth.process(depthFrame);
    depthFrame = hole_filter.process(depthFrame);
    
    int i = 0;
    for (int x = PIXEL_SIZE_PX / 2; x < WIDTH; x += PIXEL_SIZE_PX) {
        pixels[i++] = depthFrame.get_distance(x, ACTIVE_LINE_Y_PX) < MIN_DISTANCE_M;
    }
    
    // prepare font layer
    fontCanvas.begin();
    ofClear(0);
    
    ofSetColor(255);
    // looking for long sequences
    int wordStart = -1;
    // start from 1 to ignore left side of depth picture (glitch)
    for (int i = 1; i < WIDTH / PIXEL_SIZE_PX; i++) {
        if (pixels[i] && wordStart == -1) { // start of the word
            wordStart = i;
        }
        else if (!pixels[i] && wordStart != -1) { // end of the word
            drawWord(wordStart, i - wordStart);
            wordStart = -1;
        }
    }
    
    if (wordStart != -1) { // end of the last word
        drawWord(wordStart, WIDTH / PIXEL_SIZE_PX - wordStart);
    }
    
    fontCanvas.end();
}

//--------------------------------------------------------------
void ofApp::drawWord(int start, int length) {
    string text = length <= words.size() ? words[length - 1] : words[words.size() - 1];
    
    for (int i = 0; i < text.length(); i++) {
        //ofDrawCircle((start + i + 0.5) * PIXEL_SIZE_PX, ACTIVE_LINE_Y_PX, 10);
        // center letter
        float x = (start + i + 0.5) * PIXEL_SIZE_PX;
        float y = ACTIVE_LINE_Y_PX;
        ofRectangle bbox = myfont.getStringBoundingBox(text, x, y);
        x -= bbox.getWidth() / 2;
        y -= bbox.getHeight() / 2;
        myfont.drawString(text.substr(i, 1), x, y);
        //ofDrawCircle(x, y, 20);
    }
}

void ofApp::draw(){
    ofBackground(ofColor::black);
    
    ofSetColor(255);
    rgbImage.draw(0, 0);
    
    invertShader.begin();
    invertShader.setUniformTexture("img", rgbImage.getTexture(), 1);
    fontCanvas.draw(0, 0);
    
    
    invertShader.end();
}

//--------------------------------------------------------------
void ofApp::exit(){
    if (camFound) pipe.stop();
    delete[] pixels;
    //delete[] depthCameraData;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
