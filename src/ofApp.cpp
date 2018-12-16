#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    // init images and buffers
    //result.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    
    //rgbImage.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    irImage.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    depthImage.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    //depthCameraData = new unsigned short [WIDTH * HEIGHT]();
    
    // init camera
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, WIDTH, HEIGHT);
    cfg.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT);
    cfg.enable_stream(RS2_STREAM_INFRARED, WIDTH, HEIGHT);
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
    
    // gui
    gui.setup();
    
    //gui.add(modeNormal.setup("Normal mode", true));
    
    
    // load shaders
    maskShader.load("shadersGL3/depthMask");
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
    
    //memcpy(rbgImage.getPixels().getData(), irFrame.get_data(), WIDTH * HEIGHT * 1);
    //rgbImage.update();
    
    // apply depth filters
    //depthFrame = depth_to_disparity.process(depthFrame);
    //depthFrame = spat_filter.process(depthFrame);
    //depthFrame = temp_filter.process(depthFrame);
    //depthFrame = disparity_to_depth.process(depthFrame);
    depthFrame = hole_filter.process(depthFrame);
    
    // copy depth bits to r and g channels of depthImage
    // copy ir bits
    
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        depthImage.getPixels().getData()[3 * i] = ((const char *) depthFrame.get_data())[2 * i];
        depthImage.getPixels().getData()[3 * i + 1] = ((const char *) depthFrame.get_data())[2 * i + 1];
        depthImage.getPixels().getData()[3 * i + 2] = 0;
        
        irImage.getPixels().getData()[3 * i] =
            irImage.getPixels().getData()[3 * i + 1] =
            irImage.getPixels().getData()[3 * i + 2] = ((const char *) irFrame.get_data())[i];
    }
    depthImage.update();
    irImage.update();
    
    // process images
    /*
    result.begin();
    ofClear(0, 0, 0);
    
    maskShader.begin();
    maskShader.setUniformTexture("mask", depthImage.getTexture(), 1);
    //cout << depthScale << "\n";
    maskShader.setUniform1f("threshold", 1.5 / depthScale); // 1.5m
    maskShader.setUniform1f("fadingDistance", 0.1 / depthScale); // 0.3m
    
    ofSetColor(255);
    //irImage.draw(0, 0);
    
    maskShader.end();
    
    result.end();
     */
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);
    
    //result.begin();
    ofClear(0, 0, 0);
    
    maskShader.begin();
    maskShader.setUniformTexture("mask", depthImage.getTexture(), 1);
    //cout << depthScale << "\n";
    maskShader.setUniform1f("threshold", 1.5 / depthScale); // 1.5m
    maskShader.setUniform1f("fadingDistance", 0.1 / depthScale); // 0.3m
    
    ofSetColor(255);
    irImage.draw(0, 0);
    
    maskShader.end();
    //result.end();
    return;
    
    //rgbImage.draw(200, 10);
    
    ofSetColor(255);
    //result.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::exit(){
    if (camFound) pipe.stop();
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
