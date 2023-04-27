/*
* CS5330: Final Project
Author: RAVI MAHESHWARI
NUID:002104786
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "functions_and_filters.h"

int portal1_X = 50, portal2_X = 339;

int optical_flow(cv::Mat& src, cv::Mat& temp1, cv::Mat& dst)
{
    //blurQuantize(src, dst, 10);
    src.copyTo(dst);
    dst.convertTo(dst, CV_8UC3);
    src.convertTo(src, CV_8UC3);
    temp1.convertTo(temp1, CV_8UC3);
    int ind = 0;
    int f = 0;
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {

            for (int c = 0; c < 3; c++)
            {
                if (src.at<cv::Vec3b>(i, j)[c] - temp1.at<cv::Vec3b>(i, j)[c] > 0)
                    dst.at<cv::Vec3b>(i, j)[c] = src.at<cv::Vec3b>(i, j)[c] - temp1.at<cv::Vec3b>(i, j)[c];
                else// if (src.at<cv::Vec3b>(i, j)[c] - temp1.at<cv::Vec3b>(i, j)[c] < 0)
                    dst.at<cv::Vec3b>(i, j)[c] = -src.at<cv::Vec3b>(i, j)[c] + temp1.at<cv::Vec3b>(i, j)[c];
                //else
                  //  dst.at<cv::Vec3b>(i, j)[c] = src.at<cv::Vec3b>(i, j)[c];
            }
        }
    }
    convertScaleAbs(dst, dst);
    return(0);
}

int portal(cv::Mat& src, cv::Mat& temp1, cv::Mat& dst)
{
    //blurQuantize(src, dst, 10);
    src.copyTo(dst);
    dst.convertTo(dst, CV_8UC3);
    src.convertTo(src, CV_8UC3);
    temp1.convertTo(temp1, CV_8UC3);
    int ind = 0;
    int f = 0;
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = portal2_X; j < src.cols; j = j + 1)
        {

            for (int c = 0; c < 3; c++)
            {
                if (src.at<cv::Vec3b>(i, j)[c] - temp1.at<cv::Vec3b>(i, j)[c] > 0)
                    dst.at<cv::Vec3b>(i, j)[c] = src.at<cv::Vec3b>(i, j)[c] - temp1.at<cv::Vec3b>(i, j)[c];
                else// if (src.at<cv::Vec3b>(i, j)[c] - temp1.at<cv::Vec3b>(i, j)[c] < 0)
                    dst.at<cv::Vec3b>(i, j)[c] = -src.at<cv::Vec3b>(i, j)[c] + temp1.at<cv::Vec3b>(i, j)[c];
                //else
                  //  dst.at<cv::Vec3b>(i, j)[c] = src.at<cv::Vec3b>(i, j)[c];
            }
        }
    }

    cv::Mat crop = dst(cv::Range(0, src.rows-1), cv::Range(portal2_X, portal2_X+300)).clone();
    src.copyTo(dst);
 
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = portal1_X; j < portal1_X + 300; j = j + 1)
        {
            if (crop.at<cv::Vec3b>(i, j)[0]>20)
            for (int c = 0; c < 3; c++)
            {
                
                dst.at<cv::Vec3b>(i, j)[c] = src.at<cv::Vec3b>(portal2_X +i - portal1_X, j)[c];
                    //crop.at<cv::Vec3b>(i, j)[c];//+temp1.at<cv::Vec3b>(portal2_X + i - portal1_X, j)[c];
            }
        }
    }
    convertScaleAbs(dst, dst);
    //portal(src, temp1, dst);

    return(0);
}