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

int apply_filter(cv::Mat& src, cv::Mat& dst, int* filt_arr1D_horz, int* filt_arr1D_vert)
{
    //src.copyTo(dst);
    //dst.convertTo(dst, CV_16SC3);
    cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);
    //apply_filter(src, dst, h, 0);
    dst = temp.clone();
    src.convertTo(src, CV_16SC3);
    //cv::Mat img1 = cv::Mat::zeros(img.size(), CV_16SC3);

    int f_size = sizeof(filt_arr1D_horz) / sizeof(int);
    int half_f_size = f_size / 2;
    int fsum = 0;

    int f_v_size = sizeof(filt_arr1D_vert) / sizeof(int);
    int half_f_v_size = f_v_size / 2;
    int f_v_sum = 0;
    //std::cout << "what is h " << filt_arr1D_horz[2] << std::endl;
    //src.copyTo(dst);
    for (int i = half_f_size; i < src.rows - half_f_size; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                fsum = 0;
                //dst.at<cv::Vec3s>(i, j)[c] = 0;
                for (int f = -half_f_size; f <= half_f_size; f++)
                {
                    temp.at<cv::Vec3s>(i, j)[c] += filt_arr1D_horz[f + half_f_size] * src.at<cv::Vec3s>(i + f, j)[c];
                    fsum = fsum + filt_arr1D_horz[f + half_f_size];
                }

                //if(fsum>0)dst.at<cv::Vec3s>(i, j)[c] = dst.at<cv::Vec3s>(i, j)[c] / fsum;
            }
        }
    }


    temp.copyTo(dst);
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = half_f_v_size; j < src.cols - half_f_v_size; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                fsum = 1;
                for (int f = -half_f_v_size; f <= half_f_v_size; f++)
                {
                    dst.at<cv::Vec3s>(i, j)[c] += filt_arr1D_vert[f + half_f_v_size] * temp.at<cv::Vec3s>(i, j + f)[c];
                    f_v_sum = f_v_sum + filt_arr1D_vert[f + half_f_v_size];
                }
                //if(f_v_sum >0)dst.at<cv::Vec3s>(i, j)[c] = dst.at<cv::Vec3s>(i, j)[c]/ f_v_sum;
            }


        }
    }

    // convertScaleAbs(dst, dst);
    return(0);
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'g' it displays a greyscale version of the image instead of color.
Here, the greyscale image is created by copying the channel value of the green channel into the other two color channels.
###################################################
*/
int greyscale(cv::Mat& src, cv::Mat& dst)
{
    //cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
    //cv::Mat res;
    src.copyTo(dst);
    dst.convertTo(dst, CV_8UC3);
    src.convertTo(src, CV_8UC3);

    //convertScaleAbs(img, res);

    //for (int t = 0; t < 1000; t++) 
    {
        for (int i = 0; i < src.rows - 1; i = i + 1)
        {
            for (int j = 0; j < src.cols - 1; j = j + 1)
            {
                dst.at<cv::Vec3b>(i, j)[0] = dst.at<cv::Vec3b>(i, j)[1];
                dst.at<cv::Vec3b>(i, j)[1] = dst.at<cv::Vec3b>(i, j)[1];
                dst.at<cv::Vec3b>(i, j)[2] = dst.at<cv::Vec3b>(i, j)[1];
            }
        }
    }
    //    dst.convertTo(dst, CV_16SC3);
    convertScaleAbs(dst, dst);

    return(0);
}


/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'b' it displays a gaussian blurred version of the image instead of color.
Here, the gaussian blur was performed by multiplying the image by [1 2 4 2 1] horizontally and then vertically.
###################################################
*/
int blur5x5(cv::Mat& src, cv::Mat& dst)
{
    int g[5] = { 3, 4, 5, 7, 3 };
    cv::Mat temp;
    src.copyTo(dst);
    //std::cout << "what is h " << sizeof(g)/sizeof(int) << std::endl;


    //apply_filter(temp, dst, g, 1);

    //apply_filter(temp, dst, g, 1);
    //apply_filter(temp, dst, g, 'c');
    //dst = temp;

    dst.convertTo(dst, CV_16SC3);
    src.convertTo(src, CV_16SC3);
    /*apply_filter(src, dst, g, 0);
    dst.copyTo(temp);
    temp.convertTo(temp, CV_16SC3);
    apply_filter(temp, dst, g, 1);*/

    for (int i = 2; i < src.rows - 2; i = i + 1)
    {

        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3s>(i, j)[c] = 1 * src.at<cv::Vec3s>(i - 2, j)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 2 * src.at<cv::Vec3s>(i - 1, j)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 4 * src.at<cv::Vec3s>(i, j)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 2 * src.at<cv::Vec3s>(i + 1, j)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 1 * src.at<cv::Vec3s>(i + 2, j)[c];
                dst.at<cv::Vec3s>(i, j)[c] = dst.at<cv::Vec3s>(i, j)[c] / 10;
            }


        }
    }

    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 2; j < src.cols - 2; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3s>(i, j)[c] = 5 * src.at<cv::Vec3s>(i, j - 2)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 2 * src.at<cv::Vec3s>(i, j - 1)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 4 * src.at<cv::Vec3s>(i, j)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 2 * src.at<cv::Vec3s>(i, j + 1)[c];
                dst.at<cv::Vec3s>(i, j)[c] += 1 * src.at<cv::Vec3s>(i, j + 2)[c];
                dst.at<cv::Vec3s>(i, j)[c] = dst.at<cv::Vec3s>(i, j)[c] / 10;
            }


        }
    }

    convertScaleAbs(dst, dst);

    return(0);
}

int normalized(cv::Mat& src, cv::Mat& dst)
{
    int g[5] = { 1, 2, 4, 2, 1 };
    int a;

    for (int i = 0; i < src.rows - 1; i = i + 1)
    {
        for (int j = 0; j < src.cols - 1; j = j + 1)
        {
            a = dst.at<cv::Vec3b>(i, j)[0] + dst.at<cv::Vec3b>(i, j)[1] + dst.at<cv::Vec3b>(i, j)[2];
            dst.at<cv::Vec3b>(i, j)[0] = dst.at<cv::Vec3b>(i, j)[0] * (a / 255);
            dst.at<cv::Vec3b>(i, j)[1] = dst.at<cv::Vec3b>(i, j)[1] * (a / 255);
            dst.at<cv::Vec3b>(i, j)[2] = dst.at<cv::Vec3b>(i, j)[2] * (a / 255);

        }
    }
    return(0);
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'x' it displays a sobel x version of the image instead of color.
Here, the sobel x operation was performed by multiplying the image by [1 0 -1] horizontally and then [1 2 1] vertically.
###################################################
*/
int sobelX3x3(cv::Mat& src, cv::Mat& dst)
{
    int h[3] = { 1,0,-1 };
    int v[3] = { 1, 2, 1 };
    /*
    convertScaleAbs(dst, dst);
    return(0);*/
    src.copyTo(dst);
    dst.convertTo(dst, CV_16SC3);
    cv::Mat temp;
    //apply_filter(src, dst, h, 0);
    temp = src.clone();
    temp.convertTo(temp, CV_16SC3);


    for (int i = 1; i < src.rows - 1; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                temp.at<cv::Vec3s>(i, j)[c] = (1 * src.at<cv::Vec3b>(i - 1, j)[c])
                    + (0 * src.at<cv::Vec3b>(i, j)[c])
                    + (-1 * src.at<cv::Vec3b>(i + 1, j)[c]);
            }
        }
    }

    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 1; j < src.cols - 1; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3s>(i, j)[c] = (1 * temp.at<cv::Vec3s>(i, j - 1)[c])
                    + (2 * temp.at<cv::Vec3s>(i, j)[c])
                    + (1 * temp.at<cv::Vec3s>(i, j + 1)[c]);
            }
        }
    }

    convertScaleAbs(dst, dst);

    return(0);
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'y' it displays a sobel magnitude version of the image instead of color.
Here, the sobel magnitude operation was performed by calculating sx and sy and then performing root(sx^2 + sy^2)
###################################################
*/

int sobelY3x3(cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
    dst.convertTo(dst, CV_16SC3);
    cv::Mat temp;
    //apply_filter(src, dst, h, 0);
    temp = src.clone();
    temp.convertTo(temp, CV_16SC3);


    for (int i = 1; i < src.rows - 1; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                temp.at<cv::Vec3s>(i, j)[c] = (1 * src.at<cv::Vec3b>(i - 1, j)[c])
                    + (2 * src.at<cv::Vec3b>(i, j)[c])
                    + (1 * src.at<cv::Vec3b>(i + 1, j)[c]);
            }
        }
    }

    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 1; j < src.cols - 1; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3s>(i, j)[c] = (1 * temp.at<cv::Vec3s>(i, j - 1)[c])
                    + (0 * temp.at<cv::Vec3s>(i, j)[c])
                    + (-1 * temp.at<cv::Vec3s>(i, j + 1)[c]);
            }
        }
    }

    convertScaleAbs(dst, dst);

    return(0);
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'm' it displays a sobel y version of the image instead of color.
Here, the sobel y operation was performed by multiplying the image by  [1 2 1]  horizontally and then[1 0 -1] vertically.
###################################################
*/

int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst)
{
    sx.copyTo(dst);
    dst.convertTo(dst, CV_16SC3);
    sx.convertTo(sx, CV_16SC3);
    sy.convertTo(sy, CV_16SC3);

    for (int i = 0; i < sx.rows; i = i + 1)
    {
        for (int j = 0; j < sx.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3s>(i, j)[c] = sqrt(pow(sx.at<cv::Vec3s>(i, j)[c], 2) + pow(sy.at<cv::Vec3s>(i, j)[c], 2));
            }
        }
    }
    convertScaleAbs(dst, dst);
    return(0);
}
/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'l' it displays a blurred and quantized version of the image instead of color.
Here, the quantization was performed by multiplying the color value by the 'bin' that color value lay in
###################################################
*/
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels)
{
    //src.copyTo(dst);

    blur5x5(src, dst);
    dst.convertTo(dst, CV_16SC3);
    int b = 255 / levels;
    short xt, xf;
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                xt = dst.at<cv::Vec3s>(i, j)[c] / b;
                xf = xt * b;
                dst.at<cv::Vec3s>(i, j)[c] = xf;
            }
        }
    }
    convertScaleAbs(dst, dst);
    return(0);
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'c' it displays a cartoonized version of the image instead of color.
Here, the cartoonization was performed by calculating sobel magnitude to identify the edges, then edges were blackened while the rest of the pixels were quantized to obtain simplistic cartoon images.
###################################################
*/
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold)
{
    cv::Mat temp1;
    cv::Mat temp2;
    cv::Mat temp3;
    sobelX3x3(src, temp1);
    sobelY3x3(src, temp2);
    temp1.convertTo(temp1, CV_16SC3);
    temp2.convertTo(temp2, CV_16SC3);
    temp3.convertTo(temp3, CV_16SC3);
    magnitude(temp1, temp2, temp3);
    //temp3.convertTo(temp3, CV_16SC3);
    blurQuantize(src, dst, levels);
    //dst.convertTo(dst, CV_16SC3);
    for (int i = 0; i < temp3.rows; i = i + 1)
    {
        for (int j = 0; j < temp3.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                if (temp3.at<cv::Vec3b>(i, j)[c] > magThreshold)  dst.at<cv::Vec3b>(i, j)[c] = 0;
            }
        }
    }
    return(0);
}
/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  if the user types 'n' it displays a negative version of the image instead of color.
Negative color is simply (255-x) where x is the original pixel value
###################################################
*/
int negative(cv::Mat& src, cv::Mat& dst)
{

    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3b>(i, j)[c] = 255 - src.at<cv::Vec3b>(i, j)[c];
            }
        }
    }
    return 0;
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  Extension 1 : HEATWAVES

Explanation: This function takes as an input 'time'. With the passage of time, if the user types '1' ,
pixels are darkened and therefore the effect looks like 'waves' where the higher (or hotter)
valued pixels get hotter and hotter till they overflow and become black.
Then the process starts over. This give the impression of 'waves' flowing over the image.
###################################################
*/
int extension_heatwaves(cv::Mat& src, cv::Mat& dst, int t)
{
    //dst.convertTo(dst, CV_16SC3);
    //cartoon(src, dst, 15, 20);
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 0; j < src.cols; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                if (src.at<cv::Vec3b>(i, j)[c] > 0) dst.at<cv::Vec3b>(i, j)[c] = t + src.at<cv::Vec3b>(i, j)[c];
                else if (src.at<cv::Vec3b>(i, j)[c] > 0) dst.at<cv::Vec3b>(i, j)[c] = t - src.at<cv::Vec3b>(i, j)[c];
                else if (src.at<cv::Vec3b>(i, j)[c] < 150) dst.at<cv::Vec3b>(i, j)[c] = t + src.at<cv::Vec3b>(i, j)[c];
                else if (src.at<cv::Vec3b>(i, j)[c] < 200) dst.at<cv::Vec3b>(i, j)[c] = t - src.at<cv::Vec3b>(i, j)[c];
            }
        }
    }
    convertScaleAbs(dst, dst);
    return 0;
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  Extension 2 : SYMMETRY

Explanation: if the user types '2' This filter simply flips half the image.
The pixels from the left hand edge are populated at the right most edge, and then inward, making the image symmetrical.


###################################################
*/
int extension_symmetry(cv::Mat& src, cv::Mat& dst, int t)
{
    //dst.convertTo(dst, CV_16SC3);
    //cartoon(src, dst, 15, 20);

    //apply_filter(src, dst, h, v);
    src.copyTo(dst);
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 0; j < src.cols ; j = j + 1)
        {
            for (int c = 0; c < 3; c = c + 1)
            {
                dst.at<cv::Vec3b>(i, src.cols - j)[c] = src.at<cv::Vec3b>(i, j)[c];

            }
        }
    }
    convertScaleAbs(dst, dst);
    return 0;
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  Extension 3 : SNOWFALL

Explanation: Deployed if the user types '3' .This effect was created by whitening pixels in a particular spatial pattern and incorporating randomness into the process.
The 'falling' effect is created by introducing a time component, and rows are moved down by this amount (with the passage of time)


###################################################
*/
int extension_snowfall(cv::Mat& src, cv::Mat& dst, int t)
{
    src.copyTo(dst);


    //if (t % 10 == 0) blocksize =  rand() % 5;
    //convertScaleAbs(img, res);
    int blocksize = 5;

    {
        int r1 = 10;
        int r2 = 10;
        for (int i = t % 10; i < src.rows - 1 - blocksize; i = i + r1)
        {
            //r1= 5+rand()%10;
            for (int j = rand() % 2; j < src.cols - 1 - blocksize; j = j + r2)
            {
                //if (t % 100 == 0) r2 = 5 + rand() % 10;
                blocksize = 3 + rand() % 2;
                for (int b = 0; b < blocksize; b++)
                {
                    dst.at<cv::Vec3b>(i + b, j)[0] = 255;
                    dst.at<cv::Vec3b>(i + b, j)[1] = 255;
                    dst.at<cv::Vec3b>(i + b, j)[2] = 255;
                }

            }
        }
    }
    convertScaleAbs(dst, dst);
    return(0);
}

int ravi4(cv::Mat& src, cv::Mat& dst, int t, int k)
{
    src.copyTo(dst);
    dst.convertTo(dst, CV_16SC3);
    src.convertTo(src, CV_16SC3);
    int bins = 0;
    bins = pow(k, 1);
    float compression = 1;
    for (int b = 1; b < 2;b++)
    {
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j = j + 3)
            {
                for (int c = 0; c < 3; c = c + 1)
                {
                    dst.at<cv::Vec3s>(i, j)[c] = src.at<cv::Vec3s>(i, j)[c];
                    dst.at<cv::Vec3s>(i, j + 1)[c] = 255 - (dst.at<cv::Vec3s>(i, j + 1)[c] + src.at<cv::Vec3s>(i, j)[c]) / 2;
                    dst.at<cv::Vec3s>(i, j + 2)[c] = (dst.at<cv::Vec3s>(i, j + 1)[c] + src.at<cv::Vec3s>(i, j)[c]) / 2;
                }
            }
        }
    }

    convertScaleAbs(dst, dst);
    return(0);
}

/*
#############Function Description#################
Input : source image
Output: destination image
Return value: Returns zero
Funactionality:  Extension 4 : BLINDS

Explanation: Deployed if the user types '4' .Explanation: This effect is a variation of the snowfall effect explained above,
only instead of whitening the pixels, the live video stream is warped around itself from the top.

###################################################
*/
int extension_blinds(cv::Mat& src, cv::Mat& dst, int t, int k)
{
    src.copyTo(dst);
    dst.convertTo(dst, CV_8UC3);
    src.convertTo(src, CV_8UC3);
    int ind = 0;
    int f = 0;
    for (int i = 0; i < src.rows; i = i + 1)
    {
        for (int j = 0; j < src.cols - 10; j = j + 10)
        {

            for (int g = 0; g < 7;g = g + 1)
                for (int c = 0; c < 3; c++)
                {

                    ind = (i + t * g) % src.rows;


                    dst.at<cv::Vec3b>(ind, j + g)[c] = src.at<cv::Vec3b>(i, j + g)[c];

                }
        }
    }


    convertScaleAbs(dst, dst);
    return(0);
}

