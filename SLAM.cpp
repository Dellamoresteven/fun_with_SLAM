#include "opencv2/opencv.hpp"
#include <iostream>
#include "frame.cpp"
#include <string>
#include <vector>

using namespace std;
using namespace cv;

string file_name = "vid.mp4";
int num_frames = 100;

void extract_frames(vector<frame_data*> &frame_list, string file_n) {
  VideoCapture cap(file_n);

  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    exit(-1);
  }

  for(int i = 0; i < num_frames; i++) {
    frame_data *frame = new frame_data();
    cap >> frame->mat;

    // build frame data
    frame->frame_num = i;
    frame->extract_features();

    // Add to list
    frame_list.push_back(frame);
  }
}

void print_frames(vector<frame_data*> d) {
  for(const auto &f : d) {
    cout << "** Frame " << f->frame_num << "**" << endl;
    cout << f->mat << endl;
  }
}

int main(){
  vector<frame_data*> frames;
  extract_frames(frames, file_name);

  for(int i = 0; i < num_frames; i++){
    imshow("Frame", frames.at(i)->mat);
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }

  destroyAllWindows();

  return 0;
}
