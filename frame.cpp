#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>

using namespace std;

struct frame_data {
  int frame_num;
  cv::Mat mat;
  void print_frame() {
    cout << "** Frame " << frame_num << "**" << endl;
    cout << mat << endl;
  }
};




