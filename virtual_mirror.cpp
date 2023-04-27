/*
* CS5330: Final Project
Author: RAVI MAHESHWARI
NUID:002104786
*/

#include "functions_and_filters.h"

int virtual_mirror()
{
	cv::VideoCapture* myVid;
    myVid = new cv::VideoCapture(0);//("C:/Northeastern/PRCV/Final Project/playing.mp4");
    cv::VideoCapture cap(0);//("C:/Northeastern/PRCV/Final Project/playing.mp4");
    cv::VideoCapture cap_banana_thief("C:/Northeastern/PRCV/Final Project/banana.mp4");
    // get some properties of the image
    cv::Size refS((int)myVid->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)myVid->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);
    int Mirror_pos_x = 360;
    int banana_flag = 0;

    cv::namedWindow("Mirror"); // identifies a window
    //cv::namedWindow("Resultant  Video", 2); // identifies a window
    cv::Mat frame,warped_frame,result_frame, frame_rev, frame_banana_thief;
    cv::Point2f inpMat[3];
    cv::Point2f outMat[3];
    inpMat[0] = cv::Point2f(0.0,0.0);
    inpMat[1] = cv::Point2f(640, 0);
    inpMat[2] = cv::Point2f(0, 480);

    outMat[0] = cv::Point2f(Mirror_pos_x, 00);
    outMat[1] = cv::Point2f(640, 100);
    outMat[2] = cv::Point2f(Mirror_pos_x, 480);

    for (;;) {

        //bool success = myVid->read(frame);
        //printf("\t %d", i);
        cap >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        //if (success)
        cv::imshow("Real", frame);
        char key=cv::waitKey(40);

        frame.copyTo(warped_frame);
        frame.copyTo(result_frame);
        cv::Mat crop = frame(cv::Range(0, 439), cv::Range(0, Mirror_pos_x));

        if(key=='b'){ 
            banana_flag = (banana_flag + 1) % 2;
        }
        if(banana_flag==1)
        {
            cap_banana_thief >> frame_banana_thief;
            if (frame_banana_thief.empty()) {
                banana_flag = 0;
            }
            else
            {
                crop = frame_banana_thief(cv::Range(0, 479), cv::Range( 100,460));
            }
        }


        //MIND WARP

        cv::Mat M = cv::getAffineTransform(inpMat, outMat);
        //applyBorder(crop);
        extension_symmetry(crop, frame_rev, 0);
        
        cv::warpAffine(frame_rev, warped_frame, M, warped_frame.size());
        
        applyMask(frame, result_frame, warped_frame);
        cv::imshow("Mirror", result_frame);

 
        if (key == 'q') {
            break;
        }

    }
	return 0;
}

int applyBorder(cv::Mat& src)
{
    for (int i = 0; i < src.rows; i = i + 1)
    {

        for (int j = 0; j < 0; j = j + 1)
        {
            for (int c = 0;c < 3;c++)

                src.at<cv::Vec3b>(i, j)[c] = 0;
        }
    }
    for (int i = 0; i < src.rows; i = i + 1)
    {

        for (int j = src.cols-3; j < src.cols; j = j + 1)
        {
            for (int c = 0;c < 3;c++)

                src.at<cv::Vec3b>(i, j)[c] = 0;
        }
    }
    return 0;
}