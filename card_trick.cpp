/*
* CS5330: Final Project
Author: RAVI MAHESHWARI
NUID:002104786
*/

#include "functions_and_filters.h"
#include <opencv2/features2d.hpp>
#include "opencv2/opencv.hpp"
//#include <opencv2/nonfree/features2d.hpp>
using namespace std;
using namespace cv;

Mat img_matches;
long MASK_AREA;
long MASK_LEFT;
long MASK_RIGHT;
long MASK_UP;
long MASK_DOWN;
int card_trick()
{
	cv::VideoCapture cap(0);
	cv::Mat target = cv::imread("C:/Northeastern/PRCV/visual C/Illuminati/Illuminati/8spade.jpg");
	cv::Mat source = cv::imread("C:/Northeastern/PRCV/visual C/Illuminati/Illuminati/badshah.jpg");
    int down_width = target.cols;
   int down_height = target.rows;
   Mat resized_source;
   //resize down
   resize(source, resized_source, Size(down_width, down_height), INTER_LINEAR);
    cv::Mat frame,input, final_frame;
    cv::Ptr<cv::SiftFeatureDetector> detector = cv::SiftFeatureDetector::create();
    std::vector<cv::KeyPoint> keypoints, target_keypoints;
    cv::Mat warped_frame;
    cv::Point2f inpMat[3];
    cv::Point2f outMat[3];

    inpMat[0] = cv::Point2f(0.0, 0.0);
    inpMat[1] = cv::Point2f(72, 0);
    inpMat[2] = cv::Point2f(0, 128);

    outMat[0] = cv::Point2f(0.0, 10);
    outMat[1] = cv::Point2f(70, 0);
    outMat[2] = cv::Point2f(0, 60);


    detector->detect(target, target_keypoints);

    bool sift = false, bat=false;

    for (;;) {

        cap >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }

        if ((key == 'f') || (sift == true)) {
            // 
            frame.copyTo(input);
            sift = true;

            detector->detect(input, keypoints);

            // Add results to image and save.
            //cv::Mat output;
           // cv::drawKeypoints(input, keypoints, final_frame);

            ////output.copyTo(final_frame);
           // cv::imshow("Features", final_frame);

            //cv::SurfDescriptorExtractor extractor;
            //Mat descriptors1, descriptors2;
            //extractor.compute(img1, keypoints1, descriptors1);
            //extractor.compute(img2, keypoints2, descriptors2);

            cv::Mat good_matches,img_matches;
            //// matching descriptors
            //cv::BFMatcher matcher(NORM_L2);
            vector<cv::DMatch> matches;
            //matcher.match(descriptors1, descriptors2, matches)
            cv::drawMatches(input,keypoints, target, target_keypoints,
                matches, img_matches/*, cv::Scalar::all(-1), cv::Scalar::all(-1),
                vector<char>(), cv::DrawMatchesFlags::DEFAULT*/);
            imshow("matches", img_matches);
            //cv::waitKey(0);
        }

        if ((key == 'b') || (bat == true)) {
            // 
           
            frame.copyTo(input);
            bat = true;
            detector->detect(input, keypoints);
            std::vector<Point2f> first_keypoints, second_keypoints;
            Mat homography;
            bool h = false;
            h=imagematching(input, target, first_keypoints, second_keypoints, homography);

            //outMat[0] = cv::Point2f(0.0, 100);
            //outMat[1] = cv::Point2f(700, 0);
            //outMat[2] = cv::Point2f(0, 600);

            //bounding box
            //Point2f c1, c2, c3, c4;
            //c1 = first_keypoints[0];
            //c2 = first_keypoints[1];
            //c3 = first_keypoints[2];
            //c4 = first_keypoints[3];
            //for (int i = 0;i <= img_matches.rows;i++)
            //{
            //    img_matches.at<cv::Vec2b>(i, c1.y)[0] = 0;
            //    img_matches.at<cv::Vec2b>(i, c1.y)[1] = 0;
            //    img_matches.at<cv::Vec2b>(i, c1.y)[2] = 0;
            //}
            //for (int p = 1;p < first_keypoints.size();p++)
            //{
            //   // Point pt1(first_keypoints[p].x, img_pts.at<float>(p - 1, 1));
            //    //Point pt2(img_pts.at<float>(p, 0), img_pts.at<float>(p, 1));
            //    Scalar font_Color(255,  255,  255);
            //    cv::line(img_matches, first_keypoints[p], first_keypoints[p-1], font_Color, 10);
            //}

            cv::Mat M = cv::getAffineTransform(inpMat, outMat);
            if ((h)) {
                M.at<Vec3f>(0) = homography.at<Vec3f>(0);
                M.at<Vec3f>(1) = homography.at<Vec3f>(1);

                input.copyTo(warped_frame);
                //cv::warpAffine(target, warped_frame, M, warped_frame.size());
                warpPerspective(resized_source, warped_frame, homography, warped_frame.size());
                MASK_AREA = 0;
                 MASK_LEFT = 0;
                 MASK_RIGHT = 0;
                 MASK_UP = 0;
                 MASK_DOWN = 0;
                applyMask(frame, input, warped_frame,0);

                cv::waitKey(10);
            }
            if ((h==true)&&(MASK_AREA < 50000) && (MASK_RIGHT - MASK_LEFT < 500) && (MASK_DOWN - MASK_UP < 1000))
               // cv::imshow("Mirror", input);
            input.copyTo(frame);
            else
            {
            } //cv::imshow("Mirror", frame);
            //imshow("Good Matches & Object detection", img_matches);
            //std::cout << "Transformatgion" << homography << endl;
            //std::cout << "TraM" << M << endl;

        }
        cv::imshow("Real", frame);
    }
	return 0;
}


//From batuman
int imagematching(Mat& img1, Mat& img2, std::vector<Point2f>& first_keypoints, std::vector<Point2f>& second_keypoints, Mat& homography) {
    int max_keypoints = 500;
    bool homo_found=false;
    Ptr<SIFT> detector = SIFT::create();
    Ptr<SIFT> extractor = SIFT::create();

    //--Step 1: Key point detection
    std::vector<KeyPoint> keypoints1, keypoints2;
    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors1, descriptors2;

    detector->detect(img1, keypoints1);
    detector->detect(img2, keypoints2);

    extractor->compute(img1, keypoints1, descriptors1);
    extractor->compute(img2, keypoints2, descriptors2);

    FlannBasedMatcher matcher;


    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    double max_dist = 0; double min_dist = 999999;

    //-- Quick calculation of max and min distances between keypoints
    for (int i = 0; i < descriptors1.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    //printf("-- Max dist : %f \n", max_dist);
    //printf("-- Min dist : %f \n", min_dist);
    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for (int i = 0; i < descriptors1.rows; i++)
    {
        if (matches[i].distance < 2 * min_dist)
        {
            good_matches.push_back(matches[i]);
        }
    }
    matches.clear();
   // std::sort(good_matches.begin(), good_matches.end());// Sort to get best match in c1

    drawMatches(img1, keypoints1, img2, keypoints2,
        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    descriptors1.release();
    descriptors2.release();

    //-- Localize the object
    //std::vector<Point2f> first_keypoints;
    //std::vector<Point2f> second_keypoints;

    for (int i = 0; i < good_matches.size(); i++)
    {
        //cout << i << " :";
        //-- Get the keypoints from the good matches
        if (keypoints1[good_matches[i].queryIdx].pt.x > 0 && keypoints1[good_matches[i].queryIdx].pt.y > 0
            && keypoints2[good_matches[i].trainIdx].pt.x > 0 && keypoints2[good_matches[i].trainIdx].pt.y > 0) {
            first_keypoints.push_back(keypoints1[good_matches[i].queryIdx].pt);
            //cout << "first point" << keypoints1[ good_matches[i].queryIdx ].pt << endl;

            second_keypoints.push_back(keypoints2[good_matches[i].trainIdx].pt);
            //cout << "second point" << keypoints2[ good_matches[i].trainIdx ].pt << endl;
        }
    }
    keypoints1.clear();
    keypoints2.clear();
    if (good_matches.size() > 30)
    {
        homography = findHomography( second_keypoints, first_keypoints, RANSAC, 5);
        homo_found = true;
        //std::cout <<"Transformatgion" << hh << endl;
    }
    good_matches.clear();
    //-- Show detected matches

    //waitKey(0);

    return homo_found;
}


int applyMask(cv::Mat& src, cv::Mat& dst, cv::Mat& mask, int threshold)
{

    src.copyTo(dst);



    for (int i = 0; i < src.rows ; i = i + 1)
    {

        for (int j = 0; j < src.cols; j = j + 1)
        {

            if (mask.at<cv::Vec3b>(i, j)[0] + mask.at<cv::Vec3b>(i, j)[1] + mask.at<cv::Vec3b>(i, j)[2] <= threshold)
                j = j; //do nothing in the dark
            else {
                dst.at<cv::Vec3b>(i, j) = mask.at<cv::Vec3b>(i, j);
                MASK_AREA++;
                if( MASK_LEFT == 0) MASK_LEFT = j; //first time loading
                if (j>MASK_RIGHT) MASK_RIGHT = j; //everytime
                if(MASK_UP == 0)MASK_UP = i; //first time loading
                if (i>MASK_DOWN) MASK_DOWN = i;//everytime
            }

        }
    }



    convertScaleAbs(dst, dst);

    return(0);
}