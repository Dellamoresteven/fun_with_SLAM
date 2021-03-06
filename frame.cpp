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
          //key_points[i].pt,
          //2,
          //Scalar(0,255,0),
          //-1,
          //8,
          //0);
    //}
    auto plotPoint = [&](int x1, int y1, int x2, int y2){
        circle(
          mat,
          //key_points[i].pt,
          Point2f(x1, y1),
          2,
          Scalar(0,0,255),
          -1,
          8,
          0);
        circle(
          mat,
          //key_points[i].pt,
          Point2f(x2, y2),
          2,
          Scalar(0,255,0),
          -1,
          8,
          0);
    };
    //vector<Scalar> colors { Scalar(255,0,0), Scalar(255,50,0), Scalar(255,100,0), Scalar(255,150,0), Scalar(255,255,0), Scalar(0,255,0), Scalar(0,255,50), Scalar(0,255,100), Scalar(0,255,150), Scalar(0,255,255), Scalar(0,0, 255) };
    //for( size_t i = 0; i < key_points.size(); i++ ) {
      //Scalar c(0,0,0);
      //try {
        //cout << "c: " << (int(key_points[i].pt.y) / 100) << endl;
        //c = colors.at(int(key_points[i].pt.y) / 50);
      //} catch(exception e) {}
      //plotPoint(327,581,320,580);
      //plotPoint(1078,456,1080,452);
      //plotPoint(1064,521,1066,518);
      //plotPoint(1468,926,1484,938);
      //plotPoint(1474,941,1491,954);
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


vector<Point2f> match_frames(frame_data *f1, frame_data *f2) {
  BFMatcher bf(NORM_HAMMING, false);

  vector<vector<DMatch>> matches;
  bf.knnMatch(f1->des, f2->des, matches, 2);

  // draws matches between each frame. Only show the top matches_displayed
  //int matches_displayed = 20;
  //vector<vector<DMatch>> copy_matches;
  //for (int i=0; i < matches_displayed; i++)
    //copy_matches.push_back(f1->matches[i]);
  //drawMatches(f1->mat, f1->key_points, f2->mat, f2->key_points, copy_matches, f1->matched_mat);

  vector<Point2f> ret;

  for(const auto m : matches) {
    if(m.at(0).distance < 0.75*m.at(1).distance) {
      Point2f pt(m.at(0).queryIdx, m.at(0).trainIdx);
      if(m.at(0).distance < 32) {
        ret.push_back(pt);
      }
    }
  }

  cout << "Feature Matches: " << ret.size() << " / " << max(f1->key_points.size(), f2->key_points.size()) << endl;

  return ret;
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
    /*
     * P =
     * f 0 px 0
     * 0 f py 0
     * 0 0 1  0
     */
    using namespace settings::camera_int;
    // Build the camera intrinsics matrix K
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
      //if(i != settings::start_frame) {
        //match_frames(frame_list[i - settings::start_frame - 1], frame);
      //}

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

