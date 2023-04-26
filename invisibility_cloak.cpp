/*
* CS5330: Final Project
Author: RAVI MAHESHWARI
NUID:002104786
*/

#include "functions_and_filters.h"


int Invisibility_cloak(cv::Mat& src, cv::Mat& dst, cv::Mat& background)
{
    cv::Mat blurred, bkgd_blurred;
    src.copyTo(dst);
    dst.convertTo(dst, CV_8UC3);
    src.convertTo(src, CV_8UC3);
    blurQuantize(src, blurred, 5);
    //blurQuantize(background, bkgd_blurred, 5);

    for (int i = 0; i < src.rows - 1; i = i + 1)
    {
        for (int j = 0; j < src.cols - 1; j = j + 1)
        {
            if (blurred.at<cv::Vec3b>(i, j)[0]+ blurred.at<cv::Vec3b>(i, j)[1] > 350)
            {
                dst.at<cv::Vec3b>(i, j)[0] = background.at<cv::Vec3b>(i, j)[0];
                dst.at<cv::Vec3b>(i, j)[1] = background.at<cv::Vec3b>(i, j)[1];
                dst.at<cv::Vec3b>(i, j)[2] = background.at<cv::Vec3b>(i, j)[2];
            }
        }
    }
    convertScaleAbs(dst, dst);
	return(0);
}