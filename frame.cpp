#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

RNG rng(12345);

struct frame_data {
  int frame_num;
  Mat mat;
  vector<Point2f> features;
  void print_frame() {
    cout << "** Frame " << frame_num << "**" << endl;
    cout << mat << endl;
  }
  void extract_features() {
    Mat mat_gray;
    cvtColor(mat, mat_gray, COLOR_BGR2GRAY);
    goodFeaturesToTrack(
        mat_gray,
        features,
        10000, // max corners
        0.05, // quality level
        10, // min distance
        Mat(), // mask
        3, // block size
        false, // harris detector?
        0.04 // k
    );
    cout << "Feature Size: " << features.size() << endl;
    for( size_t i = 0; i < features.size(); i++ ) {
      circle(
          mat,
          features[i],
          4,
          Scalar(0,255,0),
          -1,
          8,
          0);
    }
  }
};




