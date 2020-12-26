#include "pch.h"
#include <dlib/geometry/rectangle.h>
#include <opencv2/core.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

void calculate_ear(vector<Point>& eye, double* ear)
{
    double A, B, C;
    double temp_x[6], temp_y[6];

    for (int i = 0; i < 6; i++)
    {
        temp_x[i] = (double)eye[i].x;
        temp_y[i] = (double)eye[i].y;
    }

    A = (temp_x[5] - temp_x[1]) * (temp_x[5] - temp_x[1]);
    A = sqrt(A + ((temp_y[5] - temp_y[1]) * (temp_y[5] - temp_y[1])));

    B = (temp_x[4] - temp_x[2]) * (temp_x[4] - temp_x[2]);
    B = sqrt(B + ((temp_y[4] - temp_y[2]) * (temp_y[4] - temp_y[2])));

    C = (temp_x[3] - temp_x[0]) * (temp_x[3] - temp_x[0]);
    C = sqrt(C + ((temp_y[3] - temp_y[0]) * (temp_y[3] - temp_y[0])));

    *ear = (A + B) / (2 * C);
}

void convert_rect_CV2DLIB(vector<Rect>& cvrect, vector<dlib::rectangle>& dlibrect, int pos)
{
    Rect temp_cv;
    dlib::rectangle temp_dlib;

    temp_cv = cvrect[pos];
    temp_dlib.set_left((long)temp_cv.x);
    temp_dlib.set_top((long)temp_cv.y);
    temp_dlib.set_right((long)(temp_cv.x + temp_cv.width));
    temp_dlib.set_bottom((long)(temp_cv.y + temp_cv.height));
    dlibrect.push_back(temp_dlib);
}

TEST(Utility_functions, CV2DLIB) 
{
    vector<dlib::rectangle> drect;
    vector<Rect> crect;
    Rect a;

    a.x         = rand() % 200;
    a.y         = rand() % 200;
    a.width     = rand() % 10;
    a.height    = rand() % 10;

    crect.push_back(a);

    convert_rect_CV2DLIB(crect, drect, 0);

    EXPECT_EQ(crect[0].x, drect[0].left());
    cout << "CV.x: " << crect[0].x << "\tDLIB.x: " << drect[0].left() << endl;

    EXPECT_EQ(crect[0].y, drect[0].top());
    cout << "CV.y: " << crect[0].y << "\tDLIB.y: " << drect[0].top() << endl;

    EXPECT_EQ(crect[0].x + crect[0].width, drect[0].right());
    cout << "CV.width: " << crect[0].x + crect[0].width << "\tDLIB.width: " << drect[0].right() << endl;

    EXPECT_EQ(crect[0].y + crect[0].height, drect[0].bottom());
    cout << "CV.height: " << crect[0].y + crect[0].height << "\tDLIB.height: " << drect[0].bottom() << endl;

    EXPECT_TRUE(true);
}

TEST(Utility_functions, Eye_Aspect_Ratio)
{
    Point p1[6], p2[6];
    vector<Point> left, right;
    double ear1,ear2;

    for (int i = 0; i < 6; i++)
    {
        p1[i].x = (i + 1) * 13;     p1[i].y = (i + 1) * 15;
        p2[i].x = (i + 1) * 9;      p2[i].y = (i + 1) * 4;
        left.push_back(p1[i]);
        right.push_back(p2[i]);
    }

    calculate_ear(left, &ear1);
    calculate_ear(right, &ear2);
    ear1 = (ear1 + ear2) / 2;

    for (int i = 0; i < 6; i++)
    {
        cout << "Left[" << i << "] = " << left[i] << "\tRight[" << i << "] = " << right[i] << endl;
    }

    cout << "EAR : " << ear1 << endl;

    EXPECT_DOUBLE_EQ(ear1, 1);
    EXPECT_TRUE(true);
}

GTEST_API_ int main(int argc, char** argv)
{
    cout << "Running test";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}