#include <iostream>

using namespace std;
using namespace cv;


namespace point3d {

  // A Point is a 3D point on the space-time continuum
  class Point3D {

    public:

    Point3D() {
      cout << "Point default ctor" << endl;
    }

    // @TODO
    float get_dist() {
      return 0.00;
    }

    // @TODO
    float get_x() {
      return 0;
    }

    // @TODO
    float get_y() {
      return 0;
    }
  };
}
