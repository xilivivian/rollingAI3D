#pragma once
#include"point_cloud_recognise_function.h"


void compensate(Mat img_3D, vector<float>& list_px, int roi_min, int roi_max);

void ransac_recognise(Mat& image, Mat& image_mask, int ROI_min, int ROI_max);

void show_image(Mat image, int size);

void ransac_partiton(Mat img_3D, Mat& mask, Mat& image_mask, vector<int>& px_partition, double threshold_hight);

void get_deep_total(Mat& image_src, Mat& draw_image_mask, vector<float>& px);

void get_depth(Mat& image_mask, Mat& img_2D_show, recogn_OUT_for_point_cloud& out, vector<int>& px_partition, Mat& img_3D, Mat& img_2D);

void main_recognise_for_point_cloud_3D(Mat& img_3D, Mat& img_2D, recogn_OUT_for_point_cloud& out, int flat);