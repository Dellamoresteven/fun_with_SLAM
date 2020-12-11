#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include "settings.cpp"

#include <Eigen/Eigen>

using namespace Eigen;

using namespace std;
using namespace cv;

struct frame_data {
  int frame_num; // Frame number
  Mat mat; // the frame that will be displayed
  Mat des; // the descriptor
  Mat matched_mat;
  vector<Point2f> features; // A list of features found by "goodFeaturesToTrck"
  std::vector<KeyPoint> key_points; // a List of key points found by "orb->detect"
  vector<vector<DMatch>> matches; // A list of matched descriptors

  /* prints frame meta data */
  void print_frame() {
    cout << "** Frame " << frame_num << "**" << endl;
    cout << mat << endl;
  }

  /* extracts feature data from mat */
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
    //cout << "Feature Size: " << features.size() << endl;
  }

  /* draws circles on the frames mat */
  void draw_circles_on_frame() {
    //for( size_t i = 0; i < features.size(); i++ ) {
      //circle(
          //mat,
          //features[i],
          //2,
          //Scalar(0,255,0),
          //-1,
          //8,
          //0);
    //}
    //for( size_t i = 0; i < key_points.size(); i++ ) {
      //circle(
          //mat,
          //key_points[i].pt,
          //2,
          //Scalar(0,255,0),
          //-1,
          //8,
          //0);
    //}
  }

  /* extracts descriptors from mat */
  void extract_descriptors() {
    auto orb = ORB::create();

    /* I'm not using "detect" because it is not giving me enough
     * features to track. So we use "goodfeaturestotrack(...)" instead
     * so we get more data. */
    for( size_t i = 0; i < features.size(); i++ ) {
      KeyPoint kp(features[i], 1.f);
      key_points.push_back(kp);
    }
    //orb->detect(mat, key_points); // not using
    orb->compute(mat, key_points, des); // compute the des for each kp
  }
};


void match_frames(frame_data *f1, frame_data *f2) {
  // Should do Lowe's ratio test on the result of the knnMatch search instead
  // of just asking for one. @TODO


  // prob should change to flase and look through all the returns.
  BFMatcher bf(NORM_HAMMING, true);

  // Change 1 -> N for better results
  bf.knnMatch(f1->des, f2->des, f1->matches, 1);

  // draws matches between each frame. Only show the top matches_displayed
  int matches_displayed = 20;
  vector<vector<DMatch>> copy_matches;
  for (int i=0; i < matches_displayed; i++)
    copy_matches.push_back(f1->matches[i]);
  //drawMatches(f1->mat, f1->key_points, f2->mat, f2->key_points, copy_matches, f1->matched_mat);

  cout << "Feature Matches: " << f1->matches.size() << " / " << max(f1->key_points.size(), f2->key_points.size()) << endl;
}


void extract_frames(vector<frame_data*> &frame_list) {
  VideoCapture cap(settings::file_name);
  //cout << "F: " << cap.get(CAP_PROP_FRAME_COUNT) << endl;

  // Setting H and W
  settings::camera_int::W = cap.get(CAP_PROP_FRAME_WIDTH);
  settings::camera_int::H = cap.get(CAP_PROP_FRAME_HEIGHT);

  // Downsize the W and H if it's > max_W
  if(cap.get(CAP_PROP_FRAME_WIDTH) > settings::camera_int::max_W) {
    float d = settings::camera_int::max_W / settings::camera_int::W;
    settings::camera_int::F *= d;
    settings::camera_int::H = int(d * settings::camera_int::H);
    settings::camera_int::W = settings::camera_int::max_W;
  }

  {
    using namespace settings::camera_int;
    // Build the camera matrix K
    K << F, 0, W/2,
         0, F, H/2,
         0, 0, 1;
  }
  cout << "K: " << settings::camera_int::K << endl;
  //auto K_inv = settings::camera_int::K.inverse();
  //cout << "K_inv: " << K_inv << endl;


  if(!cap.isOpened()) {
    cout << "Error opening video stream or file" << endl;
    exit(-1);
  }

  for(int i = 0; i < settings::num_frames + settings::start_frame; i++) {
    if(i >= settings::start_frame) {
      cout << "** Processing frame " << i << " **" << endl;
      frame_data *frame = new frame_data();
      Mat full_size;
      cap >> full_size;
      resize(full_size, frame->mat, Size(settings::camera_int::W,settings::camera_int::H));

      // build frame data
      frame->frame_num = i;
      frame->extract_features();
      frame->extract_descriptors();
      frame->draw_circles_on_frame();
      if(i != settings::start_frame) {
        match_frames(frame_list[i - settings::start_frame - 1], frame);
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

