#pragma once
#ifndef POINT_CLOUD_RECOGNISE_FUNCTION_H

#include"opencv2/opencv.hpp"
#include<stdlib.h>
#include<algorithm>
#include<time.h>
#include<vector>
#include<string>
#include<string.h>
#include<iostream>
#include<thread>

#endif // !POINT_CLOUD_RECOGNISE_FUNCTION_H
using namespace cv;
using namespace std;

struct flawimformation
{

	/// <summary>
	/// contour ȱ�ݵ���ͨ����Ϣ
	/// center ȱ�ݵ��������� x��y
	/// area ȱ�ݵ����
	/// length ȱ�ݵĳ���
	/// position ȱ�ݵ�λ��
	/// type ȱ������
	/// depth ȱ�����
	/// </summary>
	vector<Point>contour;

	Point2f center;

	double area;
	double length;
	double depth;
	int position;
	int type;


	flawimformation(vector<Point>contour, Point2f center, double area,
		double length, int position, int type, double depth) :contour(contour), center(center), area(area), length(length), position(position), type(type), depth(depth) {}
};


struct recogn_OUT_for_point_cloud
{

	vector<int> max_compare;

	vector<Mat> image;

	vector<flawimformation>flaw_total;

	recogn_OUT_for_point_cloud(vector<int> max_compare, vector<Mat> image, vector<flawimformation>flaw_total) :max_compare(max_compare), image(image), flaw_total(flaw_total) {}
};

void hstack(Mat image_src1, Mat image_src2, Mat& image);

void image_auto_cut(Mat image, vector<int>& ROI, int border, int flat);

void show_image(Mat image, int gan);
