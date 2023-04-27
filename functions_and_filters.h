#pragma once
/*
* CS5330: Assignment 1
Author: RAVI MAHESHWARI
NUID:002104786
*/

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

int greyscale(cv::Mat& src, cv::Mat& dst);
int blur5x5(cv::Mat& src, cv::Mat& dst);
int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);
int apply_filter(cv::Mat& src, cv::Mat& dst, int* filt_arr1D_horz, int* filt_arr1D_vert);
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold);
int negative(cv::Mat& src, cv::Mat& dst);
int extension_heatwaves(cv::Mat& src, cv::Mat& dst, int time);
int extension_symmetry(cv::Mat& src, cv::Mat& dst, int t);
int extension_snowfall(cv::Mat& src, cv::Mat& dst, int t);
int ravi4(cv::Mat& src, cv::Mat& dst, int t, int key);
int extension_blinds(cv::Mat& src, cv::Mat& dst, int t, int k);
int optical_flow(cv::Mat& frame, cv::Mat& temp1, cv::Mat& result_frame);
int Invisibility_cloak(cv::Mat& src, cv::Mat& dst, cv::Mat& background);
int virtual_mirror();
int card_trick();
int imagematching(cv::Mat& img1, cv::Mat& img2, std::vector<cv::Point2f>& first_keypoints, std::vector<cv::Point2f>& second_keypoints, cv::Mat& homography);

int applyMask(cv::Mat& src, cv::Mat& dst, cv::Mat& mask,int threshold );
int pull_the_rabbit();
int applyBorder(cv::Mat& src);
int portal(cv::Mat& src, cv::Mat& temp1, cv::Mat& dst);