#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
#define INPUT "raw_img.jpg"
#define INPUT1 "1.avi"
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
    if (event == EVENT_MOUSEMOVE) {
        cout << "(" << x << ", " << y << ")" << endl;
    }
}
Mat hough_lines(Mat img, double rho,double theta, int threshold, int min, int max){
    Mat lines,axleft,ayleft,axright,ayright;
    vector<Point> ll;
    Point minleft,topleft,topright,minright;
    minleft.x = img.size().width;
    minleft.y = img.size().height;
    minright.x = 0;
    minright.y = img.size().height;
    topleft.x = img.size().width;
    topleft.y = img.size().height;
    topright.x = 0;
    topright.y = img.size().height;
    HoughLinesP(img,lines,rho,theta,threshold,min,max);
    Mat line_img = Mat::zeros(img.size(),CV_8UC3);
 //   cout<< img << endl;
    for(int i = 0; i<lines.size().height * 4; i=i+4)
    {
        Point p1,p2;
        p1.x = lines.at<int>(i);
        p1.y = lines.at<int>(i+1);
        p2.x = lines.at<int>(i+2);
        p2.y = lines.at<int>(i+3);
        if(p1.x<minleft.x) minleft.x = p1.x;
        if(p2.x<minleft.x) minleft.x = p2.x;
        if(p1.x>minright.x) minright.x = p1.x;
        if(p2.x>minright.x) minright.x = p2.x;
        float slope =(float) (p2.y - p1.y) / (p2.x - p1.x);
       //slope = abs(slope);
       // cout << slope <<endl;
        if(slope < -1 || slope > 1)
        {
        line(line_img,p1,p2,Scalar(255,0,0),2);
      //  if (slope < -1){ axleft.push_back(p1.x);}
        }
       //imshow("line",line_img);
       //waitKey(0);
    }
    Point center1,center2;
    center1.x = (minright.x - minleft.x)/2 + minleft.x;
    center1.y = minleft.y;
    center2.x = (topright.x - topleft.x)/2 + topleft.x;
    center2.y = topleft.y;
   // line(line_img,center1,Point(line_img.size().width/2,0), Scalar(0,0,255),2);
   // cout <<lines<<endl;
    //line(line_img,minleft,Point(0,0),Scalar(0,0,255),2);
    //line(line_img,minright,Point(0,0),Scalar(0,0,255),2);
   // line(line_img,topleft,Point(0,0),Scalar(0,0,255),2);
   // line(line_img,topright,Point(0,0),Scalar(0,0,255),2);
   namedWindow("line");
 //  setMouseCallback("line",mouse_callback);
    imshow("line",line_img);
   // imwrite("line.jpg",line_img);
    return line_img;
}

int main( int argc, char** argv )
{
     Mat gray,mask_white,img_hsv,mask_yellow,mask_yw,mask_yw_image,gauss_gray,canny_edge;
    VideoCapture videoCapture;
    videoCapture.open(INPUT1);
    while(true){
    Mat image;
    videoCapture.read(image);
    cvtColor(image,gray,CV_BGR2GRAY);
    cvtColor(image,img_hsv,CV_BGR2HSV);
  
    imshow( "Origin image", image );
    imshow( "Gray image", gray );
  
    Mat lower_yellow(1,1,CV_8U,Scalar(20,100,100));
 
    Mat upper_yellow(1,1,CV_8U,Scalar(30,255,255));
   
    inRange(img_hsv,lower_yellow,upper_yellow,mask_yellow);
    
    inRange(gray,100,255,mask_white);
  
    bitwise_or(mask_white,mask_yellow,mask_yw);
   
   bitwise_and(gray,mask_yw,mask_yw_image);
  
  imshow("mask",mask_yw_image);
   GaussianBlur(mask_white,gauss_gray,Size(5,5),0);
  // namedWindow("gauss");
   //setMouseCallback("gauss",mouse_callback); 
   imshow("gauss",gauss_gray);
   /* Point lower_left = Point(image.size().width/9,image.size().height);
   Point lower_right = Point(image.size().width - image.size().width/9 , image.size().height);
   Point top_left = Point(image.size().width/2 - image.size().width/8 , image.size().height/2 + image.size().height/10);
   Point top_right = Point(image.size().width/2 + image.size().width/8 , image.size().height/2 + image.size().height/10);*/
   Point lower_left = Point(100,image.size().height);
   Point lower_right = Point(530,image.size().height);
   Point top_left = Point(310,330);
   Point top_right = Point(375,330);
    vector<Point> vertices;
    vertices.push_back(lower_left);
    vertices.push_back(top_left);
    vertices.push_back(top_right);
    vertices.push_back(lower_right);
    vector<vector<Point>> allPoint;
    allPoint.push_back(vertices);
   // cout << allPoint <<endl;
   // cout << canny_edge.channels() << endl;
   Mat roi_image = region_of_interest(gauss_gray,allPoint);
   imshow("roi",roi_image);
   Canny(roi_image,canny_edge,50,150);
   
   imshow("Canny", canny_edge);

   
  // imwrite("roi.jpg",roi_image);
    double rho = 2;
    double theta = M_PI/180;
    int threshold = 20;
    int min = 50;
    int max = 200;
    Mat hough_img = hough_lines(canny_edge,rho,theta,threshold,min,max);
    Mat wi;
   addWeighted(hough_img,0.8,image,1,0,wi);
   imshow("wi",wi);
  // imwrite("wi.jpg",wi);
    waitKey(0);             // Wait for a keystroke in the window
    }
    return 0;
}