#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
#define INPUT "raw_img.jpeg"
#define INPUT1 "color.avi"
#define LINE1 300
#define LINE2 350
#define LINE_COLOR 100
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
   /* #defining a 3 channel or 1 channel color to fill the mask with depending on the input image
    if len(img.shape) > 2:
        channel_count = img.shape[2]  # i.e. 3 or 4 depending on your image
        ignore_mask_color = (255,) * channel_count
    else:
        ignore_mask_color = 255
    
    #filling pixels inside the polygon defined by "vertices" with the fill color
    cv2.fillPoly(mask, vertices, ignore_mask_color)

    #returning the image only where mask pixels are nonzero
    masked_image = cv2.bitwise_and(img, mask)*/
}
void draw_lines(){

}
Mat hough_lines(Mat img, double rho,double theta, int threshold, int min, int max){
    Mat lines;
    vector<Point> ll;
    
    HoughLinesP(img,lines,rho,theta,threshold,min,max);
    Mat line_img = Mat::zeros(img.size(),CV_8UC3);
    for(int i = 0; i<lines.size().height; i=i+4)
    {
        Point p1,p2;
        p1.x = lines.at<int>(i);
        p1.y = lines.at<int>(i+1);
        p2.x = lines.at<int>(i+2);
        p2.y = lines.at<int>(i+3);
        line(line_img,p1,p2,Scalar(255,0,0),2);
    }
   // addWeighted(line_img,0.8,)
    imshow("line",line_img);
    return lines;
}
int main( int argc, char** argv )
{
    Mat image,gray,mask_white,img_hsv,mask_yellow,mask_yw,mask_yw_image,gauss_gray,canny_edge;
    VideoCapture videoCapture;
    videoCapture.open(INPUT1);
    while(true){
        videoCapture.read(image);
    cvtColor(image,gray,CV_BGR2GRAY);
    cvtColor(image,img_hsv,CV_BGR2HSV);
  //  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Origin image", image );
   // imshow( "Gray image", gray );
    Mat lower_yellow(1,1,CV_8U,Scalar(20,100,100));
    Mat upper_yellow(1,1,CV_8U,Scalar(30,255,255));
    inRange(img_hsv,lower_yellow,upper_yellow,mask_yellow);
    inRange(gray,200,255,mask_white);
    bitwise_or(mask_white,mask_yellow,mask_yw);
   bitwise_and(gray,mask_white,mask_yw_image); //mask_yw
  // imshow( "Display window", mask_yw_image );
 // imshow("mask",mask_yw_image);
   GaussianBlur(mask_yw_image,gauss_gray,Size(5,5),0);
   imshow("gauss",gauss_gray);
  
   Point lower_left = Point(image.size().width/9,image.size().height);
   Point lower_right = Point(image.size().width - image.size().width/9 , image.size().height);
   Point top_left = Point(image.size().width/2 - image.size().width/8 , image.size().height/2 + image.size().height/10);
   Point top_right = Point(image.size().width/2 + image.size().width/8 , image.size().height/2 + image.size().height/10);
    vector<Point> vertices;
    vertices.push_back(lower_left);
    vertices.push_back(top_left);
    vertices.push_back(top_right);
    vertices.push_back(lower_right);
    vector<vector<Point>> allPoint;
    allPoint.push_back(vertices);
    Mat roi_image = region_of_interest(gauss_gray,allPoint);
    threshold(gauss_gray,roi_image,150,255,CV_THRESH_BINARY);
   Point p1,p2,p3,p4,p5,p6,p7,p8;
   
   
  //  cout<<roi_image.size().width << " " << " " << roi_image.cols << endl;
   Mat array,array2;
   array = roi_image.row(LINE1);
   array2 = roi_image.row(LINE2);
   int pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
   for(int i = 0; i<array.cols; i++){
       if( (int)array.at<uchar>(i) > LINE_COLOR && pos1 == 0)
       {
           pos1 = i;
       }
       if((int)array2.at<uchar>(i) > LINE_COLOR && pos3 == 0)
       {
           pos3 = i;
       }
       if(pos1 != 0 && pos3 != 0) break;
   }
  for(int i = array.cols; i>0; i--){
       if( (int)array.at<uchar>(i) > LINE_COLOR && pos2 == 0)
       {
           pos2 = i;
       }
       if((int)array2.at<uchar>(i) > LINE_COLOR && pos4 == 0)
       {
           pos4 = i;
       }
       if(pos2 != 0 && pos4 != 0) break;
   }
    p1.x = pos1; p1.y = LINE1;
    p2.x = pos2; p2.y = LINE1;
    p3.x = pos3; p3.y = LINE2;
    p4.x = pos4; p4.y = LINE2;
    p5.x = (pos2 - pos1)/2 + pos1; p5.y = LINE1;
    p6.x = (pos4 - pos3)/2 + pos3; p6.y = LINE2;
    p7.x = roi_image.cols/2; p7.y = LINE1;
    p8.x = roi_image.cols/2; p8.y = LINE2;
    line(roi_image,p1,p2,230,2);
    line(roi_image,p3,p4,230,2);
    line(roi_image,p5,p6,230,2);
    line(roi_image,p7,p8,255,2);
  /* array.convertTo(array,CV_64F);
  // cout << array <<endl;
   Ptr<plot::Plot2d> plot = plot::createPlot2d(array);
    plot->setPlotBackgroundColor( Scalar( 50, 50, 50 ) );
    plot->setPlotLineColor( Scalar( 50, 50, 255 ) );
    plot->render( roi_image );*/
   imshow("roi",roi_image);
    waitKey(0);
    }
                 // Wait for a keystroke in the window
    return 0;
}