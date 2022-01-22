#include "main_recognise_for_point_cloud.h"
//#include"main_recognise_for_point_cloud_3D.h"
//#include"main_recognise_for_point_cloud_2D.h"
#include <QDebug>

#include"../API/PeakStructure.h"
using namespace std;
#if 0
void read_csv(string filename, Mat& img_2D, Mat& img_3D, int x_rate, int y_rate);

void read_csv(string filename, Mat& img_2D, Mat& img_3D, int x_rate, int y_rate)
{
	ifstream inFile(filename, ios::in);
	string lineStr;
	vector<vector<double>> strArray;

	double max_x = -10000000.0, max_y = -10000000.0;

	double min_x = 10000000.0, min_y = 10000000.0;

	while (getline(inFile, lineStr))
	{
		// 打印整行字符串
		//cout << lineStr << endl;
		// 存成二维表结构
		stringstream ss(lineStr);
		string str;
		vector<double> lineArray;
		// 按照' '分隔

		int i = 0;
		while (getline(ss, str, ' '))
		{
			double data = stof(str);
			if (i == 0)
			{
				data *= 1000000.0;
				max_x = max(max_x, data);
				min_x = min(min_x, data);
			}
			else if (i == 1)
			{
				data *= 1000000.0;
				max_y = max(max_y, data);
				min_y = min(min_y, data);
			}
			lineArray.push_back(data);
			i += 1;
		}
		strArray.push_back(lineArray);
	}

	int strArray_length = strArray.size();

	int image_col = int((max_y - min_y) / y_rate) + 1;
	int image_row = int((max_x - min_x) / x_rate) + 1;

	img_3D = Mat::ones(Size(image_row, image_col), CV_64FC1);

	img_3D *= -2;

	img_2D = Mat::zeros(Size(image_row, image_col), CV_8UC1);

	for (int i = 0; i < strArray_length; i++)
	{

		int x = (strArray[i][0] - min_x) / x_rate;
		int y = (strArray[i][1] - min_y) / y_rate;

		img_3D.at<double>(y, x) = strArray[i][2];

		img_2D.at<uchar>(y, x) = int(strArray[i][3]);
	}
}
#endif
// 先屏蔽
///// <summary>
///// flat 
///// 输入 0  3D上半区
///// 输入 1  3D下半区
///// </summary>
///// <param name="img_3D"></param>
///// <param name="img_2D"></param>
///// <param name="flat"></param>
//void main_recognise_for_point_cloud(Mat& img_3D, Mat& img_2D, recogn_OUT_for_point_cloud& out, int flat)
//{
//
//	//main_recognise_for_point_cloud_3D(img_3D, img_2D, out, flat);
//	//main_recognise_for_point_cloud_2D(img_2D, out, flat);
//	thread thread_fo_3D = thread(main_recognise_for_point_cloud_3D, ref(img_3D), ref(img_2D), ref(out), flat);
//	//thread thread_fo_2D = thread(main_recognise_for_point_cloud_2D, ref(img_2D), ref(out), flat);
//
//	thread_fo_3D.join();
//	//thread_fo_2D.join();
//}
bool SaveBatchPointCloud(Mat& img_2D, Mat& img_3D, const float* zValues, const float* intensityValues, int image_width, int image_height)
{
    static auto batchNumber = 0;
    const int max_column_pos = 50;

    const std::string fullPath = "batch_" + std::to_string(batchNumber++) + "_layer#" + std::to_string(0) + ".csv";

    img_3D = Mat::ones(Size(image_width, image_height), CV_32FC1);
    img_3D *= -2;
    img_2D = Mat::zeros(Size(image_width, image_height), CV_8UC1);

    for (auto j = 0; j < image_height; j++)
    {
        float* data_3D = img_3D.ptr<float>(j);
        uchar* data_2D = img_2D.ptr<uchar>(j);

        for (auto i = 0; i < image_width; i++)
        {
            const auto intensity = intensityValues[j * image_width + i];
            const auto zValue = zValues[j * image_width + i];
  // qDebug()<<"------------------------------------------intensity"<<intensity<<"-----zValue"<<zValue;
//qDebug()<<"---------------SRC-------intensity"<<intensityValues[j * image_width + i]<<"-----zValue"<<zValues[j * image_width + i];

            if (NOT_MEASURED_POINT(intensity) || intensity < 0.1)
                continue;
            if (NOT_MEASURED_POINT(fabs(zValue)))
                continue;

            data_3D[i] = static_cast<float>(zValue);
            data_2D[i] = int(intensity);
        }
    }

   // delete zValues;
  //  delete intensityValues;

    return true;
}

/// <summary>
/// 点云识别函数（空，先定义接口）
/// </summary>
/// <param name="zValues"></param>
/// <param name="intensityValues"></param>
/// <param name="image_width"></param>
/// <param name="image_height"></param>
/// <param name="xStep"></param>
/// <param name="yStep"></param>
/// <param name="out"></param>
/// <param name="flat"></param>
void main_recognise_for_point_cloud(const float* zValues, const float* intensityValues, int image_width, int image_height, double xStep, double yStep, recogn_OUT_for_point_cloud& out, int flat)
{
        Mat img_2D;
        Mat img_3D;

        SaveBatchPointCloud(img_2D, img_3D, zValues, intensityValues, image_width, image_height);

//        thread thread_fo_3D = thread(main_recognise_for_point_cloud_3D, ref(img_3D), ref(img_2D), ref(out), flat);
//            thread thread_fo_2D = thread(main_recognise_for_point_cloud_2D, ref(img_2D), ref(out), flat);

//            thread_fo_3D.join();
//            thread_fo_2D.join();

        main_recognise_for_point_cloud_3D(img_3D, img_2D, out, flat);
                main_recognise_for_point_cloud_2D(img_2D, out, flat);

}

#if 0

int main()
{

	for (int i = 0; i < 1; i++)
	{
		string filename = "./dataset/O-" + to_string(i) + ".csv";

		Mat img_3D, img_2D;
		int x_rate = 5593;
		int y_rate = 20833;

		clock_t start = clock();
		read_csv(filename, img_2D, img_3D, x_rate, y_rate);
		clock_t end = clock();
		printf("%d s\n", (end - start) / CLOCKS_PER_SEC);

		clock_t start1 = clock();
		for (int j = 0; j < 1; j++)
		{
			vector<int> max_compare;
			vector<Mat> img_res;
			vector<flawimformation>flaw_total;

			recogn_OUT_for_point_cloud out(max_compare, img_res, flaw_total);

			//main_recognise_for_point_cloud(img_3D, img_2D, out, 0);

			imwrite((to_string(j) + "_" + "3D.bmp"), out.image[0]);
			//imwrite((to_string(j) + "_" + "2D.bmp"), out.image[1]);
		}

		clock_t end1 = clock();
		printf("%d s\n", (end1 - start1) / CLOCKS_PER_SEC);
	}

	return 0;
}
#endif
