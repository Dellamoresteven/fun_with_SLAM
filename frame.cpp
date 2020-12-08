#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

RNG rng(12345);

struct frame_data {
  int frame_num;
  Mat mat;
  Mat des;
  vector<Point2f> features;
  std::vector<KeyPoint> key_points;
  vector<vector<DMatch>> matches;
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
        0.01, // quality level
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
  void extract_descriptors() {
    auto orb = ORB::create();
    orb->detect(mat, key_points);
    orb->compute(mat, key_points, des);
  }
};


void match_frames(frame_data *f1, frame_data *f2) {
  // Should do Lowe's ratio test on the result of the knnMatch search instead
  // of just asking for one. @TODO


  // prob should change to flase and look through all the returns.
  BFMatcher bf(NORM_L2, true);

  // Change 1 -> N for better results
  bf.knnMatch(f1->des, f2->des, f1->matches, 1);

  cout << f1->matches.size() << endl;
}


void extract_frames(vector<frame_data*> &frame_list, string file_n, int num_frames, int start_frame) {
  VideoCapture cap(file_n);

  if(!cap.isOpened()) {
    cout << "Error opening video stream or file" << endl;
    exit(-1);
  }

  for(int i = 0; i < num_frames + start_frame; i++) {
    if(i >= start_frame) {
      frame_data *frame = new frame_data();
      cap >> frame->mat;

      // build frame data
      frame->frame_num = i;
      frame->extract_features();
      frame->extract_descriptors();
      if(i != start_frame) {
        cout << i << endl;
        cout << frame_list[i-start_frame-1]->frame_num << endl;
        cout << frame->frame_num << endl;
        match_frames(frame_list[i - start_frame - 1], frame);
      }

      // Add to list
      frame_list.push_back(frame);
    } else {
      Mat mat;
      cap >> mat;
    }
  }
}

void print_frames(vector<frame_data*> d) {
  for(const auto &f : d) {
    cout << "** Frame " << f->frame_num << "**" << endl;
    cout << f->mat << endl;
  }
}

