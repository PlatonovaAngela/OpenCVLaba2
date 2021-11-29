#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <typeinfo>
#include <cmath>
using namespace std;
using namespace cv;

void matrix_operator(const Mat&,Mat&,const Mat&);

void my_gaus_blur(const Mat&,Mat&,const int, const double sigma = 0.5);

void my_gradient(const Mat&, Mat&, const string option = "north");

int main()
{
    Mat picha = imread("Picha.jpg");
    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", picha);
    waitKey();


    Mat gaus;
    my_gaus_blur(picha,gaus,21, 10);
    namedWindow("image_gaus", WINDOW_AUTOSIZE);
    imshow("image_gaus", gaus);
    waitKey();

    Mat grad;
    my_gradient(picha,grad,"west");
    namedWindow("image_gradient", WINDOW_AUTOSIZE);
    imshow("image_gradient", grad);
    waitKey();
    return 0;
}

void matrix_operator(const Mat& input,Mat& output,const Mat& op)
{
    if(input.channels() != 3)
    {
        cerr << "This is not good matrix of RGB picture." << endl;
        exit(1);
    }
    output.create(input.size(),input.type());

    for(int i = 0; i < input.rows; ++i)
    {
        for(int j = 0; j < input.cols; ++j)
        {
            for(int c = 0; c < input.channels(); ++c)
            {
                output.at<Vec3b>(i,j)[c] = op.at<float>(op.rows/2, op.cols/2) *
                        input.at<Vec3b>(i,j)[c];
                for(int i1 = 0; i1 <= op.rows/2; ++i1)
                {
                    for(int j1 = 0; j1 <= op.cols/2; ++j1)
                    {
                        if(i1 == 0 && j1 == 0)
                            continue;
                        else
                        {
                            int coord1[2];
                            int coord2[2];

                            if(i1 != 0)
                            {
                                if(!((i-i1)<0) && ((i+i1) >= output.rows))
                                {
                                    coord1[1] = coord1[0] = i - i1;
                                }
                                else if(((i-i1)<0) && !((i+i1) >= output.rows))
                                {
                                    coord1[1] = coord1[0] = i + i1;
                                }
                                else if(!((i-i1)<0) && !((i+i1) >= output.rows))
                                {
                                    coord1[0] = i - i1;
                                    coord1[1] = i + i1;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            if(j1 != 0)
                            {
                                if(!((j-j1)<0) && ((j+j1) >= output.cols))
                                {
                                    coord2[1] = coord2[0] = j - j1;
                                }
                                else if(((j-j1)<0) && !((j+j1) >= output.cols))
                                {
                                    coord2[1] = coord2[0] = j + j1;
                                }
                                else if(!((j-j1)<0) && !((j+j1) >= output.cols))
                                {
                                    coord2[0] = j - j1;
                                    coord2[1] = j + j1;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            if(i1 == 0)
                            {
                                output.at<Vec3b>(i,j)[c] += op.at<float>(op.rows/2, op.cols/2 - j1)*
                                        input.at<Vec3b>(i,coord2[0])[c] +
                                        op.at<float>(op.rows/2, op.cols/2 + j1)*
                                        input.at<Vec3b>(i,coord2[1])[c];
                            }
                            else if(j1 == 0)
                            {
                                output.at<Vec3b>(i,j)[c] += op.at<float>(op.rows/2- i1, op.cols/2 )*
                                        input.at<Vec3b>(coord1[0],j)[c] +
                                        op.at<float>(op.rows/2 + i1, op.cols/2)*
                                        input.at<Vec3b>(coord1[1],j)[c];
                            }
                            else
                            {
                                output.at<Vec3b>(i,j)[c] += op.at<float>(op.rows/2 - i1, op.cols/2 - j1)*
                                        input.at<Vec3b>(coord1[0],coord2[0])[c] +
                                        op.at<float>(op.rows/2 - i1, op.cols/2 + j1)*
                                        input.at<Vec3b>(coord1[0],coord2[1])[c] +
                                        op.at<float>(op.rows/2 + i1, op.cols/2 - j1)*
                                        input.at<Vec3b>(coord1[1],coord2[0])[c] +
                                        op.at<float>(op.rows/2 + i1, op.cols/2 + j1)*
                                        input.at<Vec3b>(coord1[1],coord2[1])[c];
                            }

                        }
                    }
                }
            }
        }
    }

}

void my_gaus_blur(const Mat& input, Mat& output,const int kernel, const double sigma)
{
    Mat op;
    op.create(kernel,kernel,CV_32FC1);
    if (!sigma)
    {
        cerr << "Sigma = 0. This is not good idea." << endl;
        exit(2);
    }
    const double pi = 4 * atan(1.);
    for(int i = 0; i < kernel; ++i)
    {
        for(int j = 0; j < kernel; ++j)
        {
           double a  = 1./(2* pi * sigma) *
                                exp(-((i-kernel/2)*(i-kernel/2))/(2. * sigma))*
                                exp(-((j-kernel/2)*(j-kernel/2))/(2. * sigma));
           op.at<float>(i,j) = a;

        }
    }
    matrix_operator(input,output,op);
}

void my_gradient(const Mat& input, Mat& output, const string option)
{
    Mat op = (Mat_<float>(3,3) << 0, 0, 0, 0, 1, 0, 0, 0, 0);
    if(option == "south")
        op.at<float>(0,1) = -1;
    else if(option == "west")
        op.at<float>(1,2) = -1;
    else if(option == "north")
        op.at<float>(2,1) = -1;
    else if(option == "east")
        op.at<float>(1,0) = -1;
    else
    {
        cerr << "Error" <<endl;
        exit(3);
    }

    matrix_operator(input,output,op);
}

