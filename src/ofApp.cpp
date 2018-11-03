#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    // init camera
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, WIDTH, HEIGHT);
    cfg.enable_stream(RS2_STREAM_COLOR, WIDTH, HEIGHT);
    rs2::context ctx;
    auto device_list = ctx.query_devices();
    
    if (device_list.size() > 0) {
        rs2::pipeline_profile profile = pipe.start(cfg);
        auto depth_sensor = profile.get_device().first<rs2::depth_sensor>();
        
        // get scale from the sensor
        depth_scale = depth_sensor.get_depth_scale();
        
        frames = pipe.wait_for_frames();
        cout << "RealSense found!\n";
        camFound = true;
    }
    else {
        cout << "Camera not found!\n";
        camFound = false;
    }
    
    // gui
    gui.setup();
    
    //gui.add(modeNormal.setup("Normal mode", true));
    
    // init images and buffers
    rgbImage.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    depthCameraData = new unsigned short [WIDTH * HEIGHT]();
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
    
    memcpy(rgbImage.getPixels().getData(), rgbFrame.get_data(), WIDTH * HEIGHT * 3);
    rgbImage.update();
    memcpy(depthCameraData, depthFrame.get_data(), WIDTH * HEIGHT * 2);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (!camFound) return;
    
    rgbImage.draw(200, 10);
}

//--------------------------------------------------------------
void ofApp::exit(){
    pipe.stop();
    delete[] depthCameraData;
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
