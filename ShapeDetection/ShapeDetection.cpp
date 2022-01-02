#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


Mat imgGray, imgBlur, imgCanny, imgDil;

void getContours(Mat imgDil, Mat img) {

    vector<vector<Point>> contours;
    vector<Vec4i>hierarchy;
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //drawContours(img, contours, -1, Scalar(255, 0, 255),2 );
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    string objectType;
    for (int i = 0; i < contours.size(); i++) {

        int area = contourArea(contours[i]);
                
        if (area > 1000) {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i],conPoly[i],0.02*peri,true);
            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            cout << conPoly[i].size()<<endl;
            boundRect[i]=boundingRect(conPoly[i]);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

            int objCor = conPoly[i].size();

            if (objCor == 3) { objectType = "Tri"; }

            else if (objCor == 4) { 
                
                float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                cout << aspRatio << endl;

                if (aspRatio>0.95 && aspRatio < 1.05) 
                {
                    objectType = "Square";
                }
                else { objectType = "Rect"; }
               }

            else { objectType = "Circle"; }
            putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 255));

        }
    }
}

int main()
{
    string imgPath = "Resources/shapes.png";
    Mat img = imread(imgPath);

    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3,3),3,0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);

    getContours(imgDil, img);

    imshow("Image",img);
    waitKey(0);
    return 0;
}
