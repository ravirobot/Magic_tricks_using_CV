
/*
* CS5330: Final Project
Author: RAVI MAHESHWARI
NUID:002104786
*/


#include "functions_and_filters.h"
using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;
    cv::Mat img2;
    int key1 = 0;
    int key2 = 0;
    int key3 = 0;
    int key4 = 0;
    int key5 = 0;
    int key6 = 0;
    int key7 = 0;
    int key8 = 0;
    int key9 = 0;
    int key10 = 0;
    int key11 = 0;
    int key12 = 0;
    int key13 = 0;
    int key14 = 0;
    int key15 = 0;
    int time = 0;
    cv::Mat result_frame, background_frame, temp1;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    //cv::namedWindow("Original Video", 1); // identifies a window
    //cv::namedWindow("Resultant  Video", 2); // identifies a window
    cv::Mat frame;

    for (;;) {
        time++;
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        frame.copyTo(result_frame);
        cv::imshow("Original  Video", frame);
        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }



        if (key == 'b') key1 = (key1 + 1) % 2;
        if (key1 == 1)
        {
            blurQuantize(frame, result_frame, 5);
            result_frame.copyTo(frame);
        }
        
        if (key == 's') {
            cv::imwrite("C:/Northeastern/PRCV/visual C/Illuminati/Illuminati/newImage.jpg", result_frame);
            frame.copyTo(background_frame);
        }

        if (key == 'i') { key2 = (key2 + 1) % 2; }
        if (key2 == 1)
        {
            //invisibility cloak
            Invisibility_cloak(frame, result_frame, background_frame);

        }

        if (key == 'm') { key3 = (key3 + 1) % 2; }
        if (key3 == 1)
        {
     
            break;

        }
        if (key == 'c') { key4 = (key4 + 1) % 2; }
        if (key4 == 1)
        {

            break;

        }
        if (key == 'r') { key5 = (key5 + 1) % 2; }
        if (key5 == 1)
        {

            break;

        }
        if (key == '1') {
            key15 = (key15 + 1) % 2;time = 0;frame.copyTo(temp1);
        }
        if (key15 == 1)
        {

            optical_flow(frame, temp1, result_frame);
            if (time%10==0)
                result_frame.copyTo(temp1);
            //if (time % 100 == 0) 
            //frame.copyTo(temp1);
        }
        if (key == '2') {
            key11 = (key11 + 1) % 2;time = 0;frame.copyTo(temp1);
        }
        if (key11 == 1)
        {

            portal(frame, temp1, result_frame);
            //if (time%2==0)
                //result_frame.copyTo(temp1);
            //if (time % 50 == 0) 
            frame.copyTo(temp1);
        }

        cv::imshow("Resultant  Video", result_frame);
    }

    if (key3 == 1)    virtual_mirror();    
    if (key4 == 1)    card_trick();
    if (key5 == 1)    pull_the_rabbit();
    delete capdev;
    return(0);
}

