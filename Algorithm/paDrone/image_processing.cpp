#include <ros/ros.h>
#include <thread>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc/segmentation.hpp"

using namespace cv;
using namespace cv::ximgproc::segmentation;
/*
**************************************
package.xml Dosyasına Kopyala
**************************************

  <build_depend>cv_bridge</build_depend>
  <build_depend>image_transport</build_depend>
  <build_export_depend>cv_bridge</build_export_depend>
  <build_export_depend>image_transport</build_export_depend>
  <exec_depend>cv_bridge</exec_depend>
  <exec_depend>image_transport</exec_depend>


**************************************
CMakeLists.txt Dosyasına Kopyala
**************************************

## OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(catkin REQUIRED COMPONENTS
  tf
  image_transport
  cv_bridge
  mavros
  mavlink
  std_msgs
  message_generation
)

add_executable(image_processing image_processing.cpp)
target_link_libraries(image_processing ${catkin_LIBRARIES} ${OpenCV_LIBS})

*/



//https://www.learnopencv.com/selective-search-for-object-detection-cpp-python/
//Bu linkin deneme fonksiyonudur. Objeleri karelere alma ile ilgili.
cv::Mat process(cv::Mat& im)
{
        // resize image
    int newHeight = 200;
    int newWidth = im.cols*newHeight/im.rows;
    resize(im, im, Size(newWidth, newHeight));

        // create Selective Search Segmentation Object using default parameters
    Ptr<SelectiveSearchSegmentation> ss = createSelectiveSearchSegmentation();
    // set input image on which we will run segmentation
    ss->setBaseImage(im);

        //argv'yi fonksiyona almadığım için if else statemanta girmeden ilk ifteki argümanı çalıştırdım
        ss->switchToSelectiveSearchFast();/*
    // Switch to fast but low recall Selective Search method
    if (argv[2][0] == 'f') {
        ss->switchToSelectiveSearchFast();
    }
    // Switch to high recall but slow Selective Search method
    else if (argv[2][0] == 'q') {
        ss->switchToSelectiveSearchQuality();
    }
    // if argument is neither f nor q print help message
    else {
        help();
        return -2;
    }*/

        // run selective search segmentation on input image
    std::vector<Rect> rects;
    ss->process(rects);
    std::cout << "Total Number of Region Proposals: " << rects.size() << std::endl;

        // number of region proposals to show
    int numShowRects = 100;
    // increment to increase/decrease total number
    // of reason proposals to be shown
    int increment = 50;

    while(1) {
        // create a copy of original image
        Mat imOut = im.clone();

        // itereate over all the region proposals
        for(int i = 0; i < rects.size(); i++) {
            if (i < numShowRects) {
                rectangle(imOut, rects[i], Scalar(0, 255, 0));
            }
            else {
                break;
            }
        }
                return imOut;
        }
}


void imageCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  try
  {
        cv::Mat img = cv_bridge::toCvShare(msg, "bgr8")->image;
        cv::Mat outImg = process(img);
    cv::imshow("view", outImg);
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  //cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/iris_fpv_cam/usb_cam/image_raw", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
}
