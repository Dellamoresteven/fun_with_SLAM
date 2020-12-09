#include "opencv2/opencv.hpp"
#include <iostream>
#include "frame.cpp"
#include <string>
#include <vector>
#include "point.cpp"
#include "helper.cpp"

using namespace std;
using namespace cv;

string file_name = "test_drone.mp4";
int num_frames = 200;
int start_frame = 0;


int main(){
  vector<frame_data*> frames;
  extract_frames(frames, file_name, num_frames, start_frame);

  vector<point3d::Point3D> p;
  convert2D_to_3D(frames, &p);

  for(int i = 0; i < num_frames; i++){
    imshow("Frame", frames.at(i)->matched_mat);
    char c=(char)waitKey(0);

    // frame by frame
    while(c != 27 && c != 'c'){
      c = (char)waitKey(0);
    }
    if(c==27)
      break;
  }

  destroyAllWindows();

  return 0;
}
