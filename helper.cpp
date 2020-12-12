#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

// Where the magic happens
point3d::Point3D convert2D_to_3D(vector<frame_data*> f, vector<point3d::Point3D> *p) {
  point3d::Point3D ret;

  return ret;
}


float triangulate(KeyPoint one, KeyPoint two, int Y) {
  cout << "    triangulate: (" << one.pt.x << "," << one.pt.y << ")  (" << two.pt.x << "," << two.pt.y << ")" << endl;

  // Build theta from one.pt and two.pt
  Point2f P1(0,0);
  //Point2f P1(settings::camera_int::W/2,settings::camera_int::H/2);
  Point2f P2(one.pt);
  Point2f P3(two.pt);
  float theta = atan2(P3.y - P1.y, P3.x - P1.x) -
                atan2(P2.y - P1.y, P2.x - P1.x);
  theta = abs(theta);
  cout << "    theta: " << theta << " R" << endl;
  float H = Y / sin(theta);
  cout << "    Hyp: " << H << " meters" << endl;
  return H;

}

// This is a solution if we can get the speed of the camera.
void convert2D_to_3D_with_SPEED(vector<frame_data*> f, point3d::Point3D *p) {
  // Y is in meters
  float Y = 17.8816;
  cout << "Y: " << Y << endl;
  for(size_t i = 30; i < f.size(); i += 30) {
    cout << "FEWAFEWAFWEAFWEAFWEA" << endl;
    frame_data * f1 = f.at(i-30);
    frame_data * f2 = f.at(i);
    auto matches = match_frames(f2, f1);
    for(size_t j = 0; j < matches.size(); j++) {
      if(matches.size() > 0) {
        cout << "Found match for kp " << j << " on frame " << i << endl;
        int idx1 = matches.at(j).x;
        int idx2 = matches.at(j).y;
        //cout << "IDX: " << idx1 << ":" << idx2 << endl;
        //cout << f1->key_points.size() << ":" << f2->key_points.size() << endl;
        float dis = triangulate(f1->key_points.at(idx2), f2->key_points.at(idx1), Y);
        if(dis <= 1500){
          p->add_point(Point3d(f2->key_points.at(idx1).pt.x, f2->key_points.at(idx1).pt.y, dis));
        }
      } else {
        cout << "No match for kp " << j << " on frame " << i << endl;
      }
    }
  }
}
