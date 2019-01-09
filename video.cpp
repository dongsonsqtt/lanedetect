#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
#define INPUT "raw_img.jpg"
#define INPUT2 "2.jpg"
Mat img_hsv;
Mat region_of_interest(Mat img, vector<vector<Point>> vertice)
{
    Mat masked_img;
    Mat mask = Mat::zeros(img.size(),CV_8UC1);
    //Mat mask(img.size(),CV_8UC3,Scalar(0,0,0));
   // Mat ignore(img.size(),CV_8UC3,Scalar(255,255,255));
    Mat ignore = (Mat_<int>(1,1) << 255);
    fillPoly(mask,vertice,255);
    bitwise_and(img,mask,masked_img);
    //imshow("masked",masked_img);
    
    return masked_img;
}
void draw_lines(){

}
void mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
    Mat &img_hsv = *((Mat*)param);
    if (event == EVENT_MOUSEMOVE) {
        Vec3b val = img_hsv.at<Vec3b>(y,x);
        cout << "x= " << x << " y= " << y << "val= "<<val<< endl;
    }
}
Mat hough_lines(Mat img, double rho,double theta, int threshold, int min, int max){
    Mat lines;
    vector<Point> ll;
    
    HoughLinesP(img,lines,rho,theta,threshold,min,max);
    
    Mat line_img = Mat::zeros(img.size(),CV_8UC3);
    //cout<< lines.size().width << " " << lines.at<int>(40) << endl;
    for(int i = 0; i<lines.size().height * 4; i=i+4)
    {
        Point p1,p2;
        p1.x = lines.at<int>(i);
        p1.y = lines.at<int>(i+1);
        p2.x = lines.at<int>(i+2);
        p2.y = lines.at<int>(i+3);
        line(line_img,p1,p2,Scalar(255,0,0),2);
    }
   // cout <<lines<<endl;
   
   namedWindow("line");
 //  setMouseCallback("line",mouse_callback);
    imshow("line",line_img);
   // imwrite("line.jpg",line_img);
    return line_img;
}

int main( int argc, char** argv )
{
    Mat image,gray,mask_white,mask_yellow,mask_yw,mask_yw_image,gauss_gray,canny_edge;
    
    image = imread(INPUT2, CV_LOAD_IMAGE_COLOR);   // Read the file
    cvtColor(image,gray,CV_BGR2GRAY);
    cvtColor(image,img_hsv,CV_BGR2HSV);
    namedWindow("HSV");
    setMouseCallback("HSV",mouse_callback,&img_hsv);
    imshow("HSV",img_hsv);
  //  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Origin image", image );
    imshow( "Gray image", gray );
  //  lower_yellow = np.array([20, 100, 100], dtype = “uint8”);
    Mat lower_yellow(1,1,CV_8U,Scalar(20,100,100));
  //  upper_yellow = np.array([30, 255, 255], dtype=”uint8");
    Mat upper_yellow(1,1,CV_8U,Scalar(30,255,255));
   // mask_yellow = cv2.inRange(img_hsv, lower_yellow, upper_yellow);
    inRange(img_hsv,lower_yellow,upper_yellow,mask_yellow);
    //Mat mask_white = cv2.inRange(gray_image, 200, 255);
    inRange(gray,200,255,mask_white);
  //  mask_yw = cv2.bitwise_or(mask_white, mask_yellow);
    bitwise_or(mask_white,mask_yellow,mask_yw);
   // mask_yw_image = cv2.bitwise_and(gray_image, mask_yw);
   bitwise_and(gray,mask_yw,mask_yw_image);
  // imshow( "Display window", mask_yw_image );
  imshow("mask",mask_yw_image);
   GaussianBlur(mask_yw_image,gauss_gray,Size(5,5),0);
   imshow("gauss",gauss_gray);
   Canny(gauss_gray,canny_edge,50,150);
   namedWindow("Canny");
   
  // setMouseCallback("Canny",mouse_callback);
   imshow("Canny", canny_edge);
  // imwrite("Canny.jpg",canny_edge);
  /* Point lower_left = Point(image.size().width/9,image.size().height);
   Point lower_right = Point(image.size().width - image.size().width/9 , image.size().height);
   Point top_left = Point(image.size().width/2 - image.size().width/8 , image.size().height/2 + image.size().height/10);
   Point top_right = Point(image.size().width/2 + image.size().width/8 , image.size().height/2 + image.size().height/10);*/
   Point lower_left = Point(0,400);
   Point lower_right = Point(image.size().width, 400);
   Point top_left = Point(230,230);
   Point top_right = Point(330,230);
    vector<Point> vertices;
    vertices.push_back(lower_left);
    vertices.push_back(top_left);
    vertices.push_back(top_right);
    vertices.push_back(lower_right);
    vector<vector<Point>> allPoint;
    allPoint.push_back(vertices);
   // cout << allPoint <<endl;
   // cout << canny_edge.channels() << endl;
   Mat roi_image = region_of_interest(canny_edge,allPoint);
    
    
   imshow("roi",roi_image);
  // imwrite("roi.jpg",roi_image);
    double rho = 2;
    double theta = M_PI/180;
    int threshold = 20;
    int min = 50;
    int max = 200;
    Mat hough_img = hough_lines(roi_image,rho,theta,threshold,min,max);
    Mat wi;
   addWeighted(hough_img,0.8,image,1,0,wi);
   imshow("wi",wi);
  // imwrite("wi.jpg",wi);
    waitKey(0);             // Wait for a keystroke in the window
    return 0;
}