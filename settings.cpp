#include <string>

#include <Eigen/Eigen>

using namespace Eigen;
using namespace std;

namespace settings {
  string file_name = "test_drone.mp4";
  int num_frames = 40;
  int start_frame = 0;

  namespace camera_int {
    float frame_rate = 29.97002997002997; // We can calc this later. CAP_PROP_FRAME_COUNT / time
    float cam_speed  = 40; // in mph
    float F          = 525; // has to be given or guessed
    int   W;
    int   H;
    Matrix<double, 3, 3> K; // calibration matrix
    float max_W      = 1800.0; // must be floating point
  }
}
