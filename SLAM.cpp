#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "frame.cpp"
#include "point.cpp"
#include "helper.cpp"
//#include "settings.cpp"

using namespace std;
using namespace cv;


// huge credit to this post on stack overflow
// https://stackoverflow.com/questions/2135116/how-can-i-determine-distance-from-an-object-in-a-video#:~:text=When%20you%20have%20moving%20video,object%20from%20slightly%20different%20angles.

int main(){
  vector<frame_data*> frames;
  extract_frames(frames);

  vector<point3d::Point3D> p;
  convert2D_to_3D_with_SPEED(frames, &p);

  for(int i = 0; i < settings::num_frames; i++){
    imshow("Frame", frames.at(i)->mat);
    char c=(char)waitKey(0);

    // frame by frame
    while(c != 27 && c != 'c'){
      c = (char)waitKey(0);
    }
    while(i == 30 && c != 'r') {
      c = (char)waitKey(0);
    }
    if(c == 'r'){
      i = 0;
    }
    if(c==27)
      break;
  }

  destroyAllWindows();

  return 0;
}
