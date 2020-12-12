#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;


namespace point3d {

  // A Point is a 3D point on the space-time continuum
  class Point3D {
    private:
    public:
      vector<Point3d> map;

    Point3D() {
      cout << "building point" << endl;
    }

    bool add_point(Point3d p) {
      // @TODO check if the point is already in the map
      map.push_back(p);
      return true;
    }

    float get_dist(int idx) {
      return map.at(idx).x;
    }

    float get_x(int idx) {
      return map.at(idx).y;
    }

    float get_y(int idx) {
      return map.at(idx).z;
    }

    void write() {
      ofstream myfile;
      myfile.open ("data.txt");
      for(const auto p : map) {
        myfile << p.x << " " << p.y << " " << p.z << "\n";
      }
      myfile.close();
    }
  };
}
