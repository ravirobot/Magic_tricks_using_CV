/*
* CS5330: Final Project
Author: RAVI MAHESHWARI
NUID:002104786
*/

#include "functions_and_filters.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/types_c.h>
#include "fstream"
//#include "common.h"
//#include <opencv2/imgproc/imgproc.hpp>
#define HOLD_RABBIT_VALUE 120
using namespace cv;
using namespace std;

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};
double THETA = 0;
int hold_rabbit = 0;

int draw3D(Mat& src, Mat& dst, InputArray drawingW,
    InputArray rvec, InputArray tvec,
    InputArray cameraMatrix, InputArray distCoeffs,
    OutputArray imagePointsW, int rabbit_up) {
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<cv::Vec3f> drawing;
    std::vector<cv::Vec3f> drawing2;
    Mat imagePoints, result_frame;
    
    float t = rabbit_up % 5000;
    t = t / 50;
    float scale3d = 0.1;
    int translate = -2;
    std::ifstream file("teddy.obj");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }
    int num_points = 0;
    std::string line;
    while (std::getline(file, line)) {
        num_points++;
        if (line.empty()) continue;

        std::string prefix = line.substr(0, 2);
        if (prefix == "v ") {
            Vertex vertex;
            sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
            float a = vertex.z * scale3d;
            float b = vertex.y * scale3d + t;
            float c = vertex.x * scale3d;

            float A = a * cos(THETA) - c * sin(THETA);
            float C = -a * sin(THETA) + c * cos(THETA);

            if((num_points>350)&&(num_points<450))
                drawing.push_back({ A, b,  C });
            else
                drawing2.push_back({ A, b+translate,  C });
        }
        else if (prefix == "f ") {
            Face face;
            sscanf(line.c_str(), "f %d %d %d", &face.v1, &face.v2, &face.v3);
            faces.push_back(face);
        }
    }

    file.close();

    //std::cout << "Vertices: " << vertices.size() << std::endl;
    //for (const auto& vertex : vertices) {
    //    std::cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    //}

    //std::cout << "Faces: " << faces.size() << std::endl;
    //for (const auto& face : faces) {
    //    std::cout << face.v1 << ", " << face.v2 << ", " << face.v3 << std::endl;
    //}
    //tvec.getMat(0) = tvec.getMat(0)+0.1;
    Mat tvec2;
    tvec.copyTo(tvec2);
    //tvec2.at<Vec3f>(2) = t;
    tvec2.at<Vec3f>(1) = tvec2.at<Vec3f>(1); //+ Vec3f(0.01);
    //rvec.at<Vec3f>(1,1) = rvec.at<Vec3f>(1); //+ Vec3f(0.01);
    projectPoints(drawing, rvec, tvec2, cameraMatrix, distCoeffs, imagePoints);
    for (int p = 1;p < drawing.size();p++)
    {
        Point pt1(imagePoints.at<float>(p - 1, 0), imagePoints.at<float>(p - 1, 1));
        Point pt2(imagePoints.at<float>(p, 0), imagePoints.at<float>(p, 1));
        Scalar font_Color(0, 255-  hold_rabbit, 0);
        cv::line(src, pt1, pt2, font_Color, 1);
    }

    projectPoints(drawing2, rvec, tvec2, cameraMatrix, distCoeffs, imagePoints);
    for (int p = 1;p < drawing2.size();p++)
    {
        Point pt1(imagePoints.at<float>(p - 1, 0), imagePoints.at<float>(p - 1, 1));
        Point pt2(imagePoints.at<float>(p, 0), imagePoints.at<float>(p, 1));
        Scalar font_Color(0, 255- hold_rabbit, 0);
        cv::line(src, pt1, pt2, font_Color, 1);
    }
    dst = src;
    return 0;
}
int pull_the_rabbit()
{
    cv::VideoCapture* capdev;
    cv::Mat camera_matrix;
    std::vector<cv::Vec3f> point_set, drawing;
    std::vector<std::vector<cv::Vec3f> > point_list;
    std::vector<std::vector<cv::Point2f> > corner_list;
    cv::Mat rvec, tvec, img_pts;
    int index = 0;
    int key1=0, key2 = 0, key3 = 0, key4 = 0, key5 = 0;
  
    Size patternsize(9, 6); //interior number of corners
    Mat gray;//cv::imread("C:/Northeastern/PRCV/visual C/ObjectRecognition/ObjectRecognition/objects_database/checkerboard.png");; //source image
    vector<Point2f> corners; //this will be filled by the detected corners
    vector<Point2f> corners_prev;
    std::vector<float>  x = { 1.2, 3.2 };
    bool hide_rabbit = false;
    int save_in_csv = 0;
    int time = 0;
    char Lab[25], filename[255];
    std::vector<char*> filenames;
    std::vector<std::vector<float>> data;
    cv::Mat frame, final_frame, result_frame;
    int dim[2] = { 3,3 };
    cv::Mat cam_cal, distCoeffs, R, T;// = cv::Mat::zeros(2, dim, CV_64FC1);
    int enable_video = 1;
    float reprojection_error = 0;
    bool  show_axis = false, sift = false;
    int usePnp = 0;
    int rabbit_up=0, rabb2=0;
    float rabbit_rot = 0;
    cv::Mat input;
    cv::Ptr<cv::SiftFeatureDetector> detector;

    for (int i = 0;i < 6;i++)
        for (int j = 0;j < 9;j++)
            point_set.push_back({ float(j),-float(i),0 });
    //cam_cal =  [[1, 0, 200], [ 0,1,400 ], [ 0,0,1 ]];
    //cam_cal = 1;
    /*Draw Cube*/
    drawing.push_back({ 0, 0, 0 });
    drawing.push_back({ 5, 0, 0 });
    drawing.push_back({ 0, 0, 0 });
    drawing.push_back({ 0, -5, 0 });
    drawing.push_back({ 0, 0, 0 });
    drawing.push_back({ 0, 0, 5 });

    /*Capture from cell:
    do not START, but open both DroidCam clients*/
    //capdev = new cv::VideoCapture("http://10.0.0.29:4747/mjpegfeed?640x480");

    /*Capture from laptop*/
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    for (;enable_video > 0;) {
        time++;
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        //CALIB_CB_FAST_CHECK saves a lot of time on images
        //that do not contain any chessboard corners
        //cv::Mat crop = frame(cv::Range(0, 439), cv::Range(0, 320)).clone(); //Rabbit's mask
        Mat rabbit_mask;
        frame.copyTo(rabbit_mask);

        cvtColor(frame, gray, COLOR_RGB2GRAY);
        bool patternfound = findChessboardCorners(gray, patternsize, corners,
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
            + CALIB_CB_FAST_CHECK);
        // printf("Corner size %d", corners.size());
        // 
        if (patternfound)
        {
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            corners_prev = corners; /*store last found corners for saving purpose*/

            /*Project the indices*/
            Point text_position(corners[0].x, corners[0].y);
            putText(gray, "origin", text_position, FONT_HERSHEY_COMPLEX, 1, 100, 2);//Putting the text in the matrix//
        }

        drawChessboardCorners(gray, patternsize, corners, patternfound);

        final_frame = gray;

        //frame.copyTo(result_frame);
        //cv::imshow("Original  Video", frame);

        /*Break on 'q' press*/
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
        if (key == 's') {
            x = { corners[0].x, corners[0].y };
            corner_list.push_back(corners_prev);
            point_list.push_back(point_set);
            index++;
            printf("\n\n");
            for (int i = 0;i < 10; i++) printf("Corner %d: %f %f", i, corners_prev[i].x, corners_prev[i].y);
        }
        if (key == 'c') {
      
            reprojection_error = cv::calibrateCamera(point_list, corner_list, cv::Size(gray.rows, gray.cols), cam_cal, distCoeffs, R, T);
            std::cout << "cameraMatrix : " << cam_cal << std::endl;
            std::cout << "distCoeffs : " << distCoeffs << std::endl;
            std::cout << "Rotation vector : " << R << std::endl;
            std::cout << "Translation vector : " << T << std::endl;
            std::cout << "Reprojection Error : " << reprojection_error << std::endl;


        }
        if ((key == 'd') || (show_axis == true)) {
            bool pnp = cv::solvePnP(point_list[0], corners_prev, cam_cal, distCoeffs, rvec, tvec);

            projectPoints(drawing, rvec, tvec, cam_cal, distCoeffs, img_pts);

            for (int p = 1;p < drawing.size();p++)
            {
                Point pt1(img_pts.at<float>(p - 1, 0), img_pts.at<float>(p - 1, 1));
                Point pt2(img_pts.at<float>(p, 0), img_pts.at<float>(p, 1));
                Scalar font_Color(time % 255, 2 * time % 255, 5 * time % 255);
                cv::line(frame, pt1, pt2, font_Color, 10);
            }
            final_frame = frame;
            show_axis = true;
        }
        if (key == 't') { usePnp = (usePnp + 1) % 2; }
         if(usePnp == 1) 
         {
            bool pnp = cv::solvePnP(point_list[0], corners_prev, cam_cal, distCoeffs, rvec, tvec);
            //rvec.at<Vec2f>(0, 1)[1] = rvec.at<Vec2f>(0, 1)[1] + rabbit_rot; //rotate the rabbit
            draw3D(frame, final_frame, drawing, rvec, tvec, cam_cal, distCoeffs, img_pts, rabbit_up);
            //std::cout << "Rotation vector" << rvec.at<Vec2f>(0,2)[1] << endl;
            //std::cout << "Translation vector" << tvec << endl;
            
            
         }
         if (key == 'u') { key3 = (key3 + 1) % 2; key1 = 0; rabbit_up = 0; key2 = 0; key1 = 0; rabb2 = 40; rabbit_rot = 0; }
         if (key3 == 1)
         {
             hide_rabbit = true;
             if ((key1 == 0) && (key2 == 0))
             {
                 rabbit_up = rabbit_up + 6;
                 if(rabb2<100)rabb2++;
             }

             if ((rabbit_up > 350)&&(key1 == 0))
             {
                 key1 = 1;
                 hold_rabbit = 0; //hold timer
             }
             if ((key1 == 1) && (key2 == 0))
             {

                 if (rabbit_up <= 0) 
                 {
                     //end movement
                     key2 == 1;
                     key3 = 0;
                 } 
                 else if (hold_rabbit < HOLD_RABBIT_VALUE)
                 {
                     hold_rabbit++;
                     THETA += 0.01;
                 }
                 else
                     rabbit_up = rabbit_up - 6;
             }


             //printf("%d", rabbit_up);
             //final_frame(cv::Range(0, 439), cv::Range(0, 320)) = rabbit_mask;
         }
         if(hide_rabbit == true)
         {
             for (int i = 320; i < final_frame.rows; i = i + 1)
             {

                 for (int j = 0; j < final_frame.cols; j = j + 1)
                 {

                     final_frame.at<cv::Vec3b>(i, j) = rabbit_mask.at<cv::Vec3b>(i, j);

                 }
             }
         }

        cv::imshow("chess image", final_frame);
    }
    delete capdev;
    return(0);

}