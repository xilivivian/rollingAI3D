#ifndef MAIN_RECOGNISE_FOR_POINT_CLOUD_H
#define MAIN_RECOGNISE_FOR_POINT_CLOUD_H
#include<iostream>
#include <fstream>
#include <sstream>
#include"main_recognise_for_point_cloud_3D.h"
#include"main_recognise_for_point_cloud_2D.h"


void main_recognise_for_point_cloud(const float* zValues, const float* intensityValues, int image_width, int image_height, double xStep, double yStep, recogn_OUT_for_point_cloud& out, int flat);

#endif // MAIN_RECOGNISE_FOR_POINT_CLOUD_H
