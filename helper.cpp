#include <iostream>

using namespace std;
using namespace cv;

// Where the magic happens
point3d::Point3D convert2D_to_3D(vector<frame_data*> f, vector<point3d::Point3D> *p) {
  point3d::Point3D ret;

  return ret;
}


float triangulate(KeyPoint one, KeyPoint two, int Y) {
  cout << "    triangulate: (" << one.pt.x << "," << one.pt.y << ")  (" << two.pt.x << "," << two.pt.y << ")" << endl;
  return 0.0;

}

// This is a solution if we can get the speed of the camera.
void convert2D_to_3D_with_SPEED(vector<frame_data*> f, vector<point3d::Point3D> *p) {
  // Y is in meters
  float Y = 17.8816 / settings::camera_int::frame_rate;
  cout << "Y: " << Y << endl;
  for(size_t i = 1; i < f.size(); i += 1) {
    frame_data * f1 = f.at(i-1);
    frame_data * f2 = f.at(i);
    auto matches = match_frames(f2, f1);
    for(size_t j = 0; j < matches.size(); j++) {
      if(matches.size() > 0) {
        cout << "Found match for kp " << j << " on frame " << i << endl;
        int idx1 = matches.at(j).x;
        int idx2 = matches.at(j).y;
        //cout << "IDX: " << idx1 << ":" << idx2 << endl;
        //cout << f1->key_points.size() << ":" << f2->key_points.size() << endl;
        triangulate(f1->key_points.at(idx2), f2->key_points.at(idx1), Y);
        //cout << "size: " << f1->matches.at(i).size() << endl;
        //cout << f1->matches.at(j).at(0).distance << ":" << f1->matches.at(j).at(0).imgIdx << ":" << f1->matches.at(j).at(0).queryIdx << ":" << f1->matches.at(j).at(0).trainIdx << endl;
      } else {
        cout << "No match for kp " << j << " on frame " << i << endl;
      }
    }
  }
}
