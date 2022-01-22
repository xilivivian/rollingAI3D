#include"main_recognise_for_point_cloud_3D.h"


void compensate(Mat img_3D, vector<float>& list_px, int roi_min, int roi_max)
{

	int img_3D_row = img_3D.rows;

	Rect rect(roi_min, 0, roi_max - roi_min, img_3D_row);

	Mat img = Mat(img_3D, rect);

	int img_row = img.rows;
	int img_col = img.cols;

	Mat sort_mask = Mat::zeros(img.size(), CV_16FC1);

	img.copyTo(sort_mask);

	for (int i = 0; i < img_row; i++)
	{

		float* data = sort_mask.ptr<float>(i);

		sort(data, data + img_col);

		list_px.push_back(data[img_col / 2]);

	}

	sort(list_px.begin(), list_px.end());

	float line_px_median = list_px[list_px.size() / 2];
}


void ransac_recognise(Mat& image, Mat& image_mask, int ROI_min, int ROI_max)
{

	int image_row = image.rows;
	int image_col = image.cols;

	if (image_col < ROI_min || image_col < ROI_max)
	{
		return;
	}


	int step = 100;

	int step_size = image_row / step;

	srand((unsigned)time(NULL));

	//show_image(image, 3);

	int times = 50;

	int number = 3;

	float threshold_value = 0.005;

	float good_A, good_B, good_C, good_D;

	int max_num, sum_num;

	int ROI_row, ROI_col;

	Mat ROI;

	float x0, x1, x2;

	float y0, y1, y2;

	float z0, z1, z2;

	float A, B, C, D;

	float pre, loss;

	int start, end;

	for (int i = 0; i < step; i++)
	{
		start = i * step_size;
		if (i != (step - 1))
		{
			end = (i + 1) * step_size;
		}
		else
		{
			end = image_row;
		}


		ROI_row = end - start;
		ROI_col = ROI_max - ROI_min;

		max_num = 0;

		// 0.004s
		for (int j = 0; j < times;)
		{

			x0 = float(rand() % ROI_row);
			x1 = float(rand() % ROI_row);
			x2 = float(rand() % ROI_row);

			y0 = float(rand() % ROI_col);
			y1 = float(rand() % ROI_col);
			y2 = float(rand() % ROI_col);

			z0 = image.at<float>(int(x0) + start, int(y0) + ROI_min);
			z1 = image.at<float>(int(x1) + start, int(y1) + ROI_min);
			z2 = image.at<float>(int(x2) + start, int(y2) + ROI_min);

			if ((y2 - y1) / (x2 - x1) != (y1 - y0) / (x1 - x0))
			{

				j++;

				A = (y0 - y1) * (z0 - z2) - (z0 - z1) * (y0 - y2);
				B = (z0 - z1) * (x0 - x2) - (x0 - x1) * (z0 - z2);
				C = (x0 - x1) * (y0 - y2) - (y0 - y1) * (x0 - x2);

				D = A * x0 + B * y0 + C * z0;

				sum_num = 0;
				for (int x = 0; x < ROI_row; x++)
				{
					float* data = image.ptr<float>(x + start);
					for (int y = 0; y < ROI_col; y++)
					{
						pre = (D - A * x - B * y) / C;

						if (abs(pre - data[y + ROI_min]) < threshold_value)
						{
							sum_num += 1;
						}
					}
				}

				if (sum_num > max_num)
				{
					max_num = sum_num;
					good_A = A;
					good_B = B;
					good_C = C;
					good_D = D;
				}
			}
		}

		// 0.00009s
		for (int x = 0; x < ROI_row; x++)
		{
			float* data = image_mask.ptr<float>(x + start);
			for (int y = 0; y < ROI_col; y++)
			{
				pre = (good_D - good_A * x - good_B * y) / good_C;

				loss = pre - image.at<float>(x + start, y + ROI_min);

				if (!data[y + ROI_min] || data[y + ROI_min] > abs(loss))
				{
					data[y + ROI_min] = loss;
				}
			}
		}
	}
}


void ransac_partiton(Mat img_3D, Mat& mask, Mat& image_mask, vector<int>& px_partition, double threshold_hight)
{

	int image_row = img_3D.rows;
	int image_col = img_3D.cols;

	image_mask = Mat::zeros(img_3D.size(), CV_32FC1);

	int px_partition_length = px_partition.size();

	thread* threads = new thread[px_partition_length / 2];

	for (int i = 0; i < px_partition_length; i += 2)
	{
		//ransac_recognise(img_3D, image_mask, px_partition[i], px_partition[i + 1]);
		threads[i / 2] = thread(ransac_recognise, ref(img_3D), ref(image_mask), px_partition[i], px_partition[i + 1]);
	}

	for (int i = 0; i < px_partition_length; i += 2)
	{
		threads[i / 2].join();
	}

	// 0.014ms
	mask = Mat::zeros(img_3D.size(), CV_8UC1);
	for (int i = 0; i < image_row; i++)
	{
		float* data_image_mask = image_mask.ptr<float>(i);
		for (int j = 0; j < image_col; j++)
		{
			float px = img_3D.at<float>(i, j);
			if (abs(data_image_mask[j]) > threshold_hight && px != -2.0)
			{
				mask.at<uchar>(i, j) = 255;
			}
		}
	}

}


void get_deep_total(Mat& image_src, Mat& draw_image_mask, vector<float>& px)
{

	int image_row = image_src.rows;
	int image_col = image_src.cols;

	int num = 0;

	for (int i = 0; i < image_row; i++)
	{
		uchar* data = draw_image_mask.ptr<uchar>(i);
		for (int j = 0; j < image_col; j++)
		{
			if (data[j])
			{
				px.push_back(image_src.at<float>(i, j));
			}
		}
	}
}


void get_depth(Mat& image_mask, Mat& img_2D_show, recogn_OUT_for_point_cloud& out, vector<int>& px_partition, Mat& img_3D, Mat& img_2D)
{

	int image_row = image_mask.rows;
	int image_col = image_mask.cols;

	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	findContours(image_mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	int contours_length = contours.size();

	cvtColor(img_2D, img_2D_show, COLOR_GRAY2BGR);

	int max_size = 1000;
	int min_size = 50;
	int depth = 0;
	int type = 0;

	int px_partition_size = px_partition.size();

	for (int i = 0; i < contours_length; i++)
	{

		int point_size = contourArea(contours[i]);

		if (point_size<min_size || point_size>max_size)
		{
			continue;
		}

		Mat mask = Mat::zeros(image_mask.size(), CV_8UC1);

		drawContours(mask, contours, i, 1, -1);

		vector<float>deep_total;

		get_deep_total(img_3D, image_mask, deep_total);

		double num_max = deep_total[deep_total.size() - 1];
		double num_min = deep_total[0];

		double depth = 0.0;

		if (abs(num_max) > abs(num_min))
		{
			depth = num_max;
		}
		else
		{
			depth = num_min;
		}

		Point2f center;
		float radius;

		minEnclosingCircle(contours[i], center, radius);

		drawContours(img_2D_show, contours, i, Scalar(0, 255, 0), 1);

		double area = contourArea(contours[i]);

		if (depth > 0)
		{
			type = 0;
		}
		else
		{
			type = 1;
		}

		int position = int(center.x) % 7;

		for (int j = 0; j < px_partition_size; j += 2)
		{
			if (px_partition[j] > center.x && px_partition[j + 1] > center.x)
			{
				position = j + 1;
			}
		}

		out.flaw_total.push_back(flawimformation{ contours[i],center,area,(2 * radius),position,type,depth });

		putText(img_2D_show, to_string(depth), Point(int(center.x), int(center.y)),
			FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 255), 1);
	}
	//out.image.push_back(img_2D_show);
}


void main_recognise_for_point_cloud_3D(Mat& img_3D, Mat& img_2D, recogn_OUT_for_point_cloud& out, int flat)
{

	int image_row = img_3D.rows;
	int image_col = img_3D.cols;

	vector<int>px_partition;

	switch (flat)
	{
	case 0: {
		/*px_partition = { 0, 450, 350, 550, 500, 950,
				800, 1500, 1350, 1650, 1550, 1850, 1670, image_col };
		  px_partition = { 0, 270, 230, 400, 330, 810,
				730, 1270, 1200, 1450, 1370, 1630, 1570, image_col };*/
		px_partition = { 0, 370, 320, 480, 480, 840,
					800, 1330, 1280, 1550, 1450, 1720, 1670, image_col };

	}break;
	case 1: {
		px_partition = { 0, 450, 350, 550, 500, 950,
				800, 1500, 1350, 1650, 1550, 1850, 1670, image_col };

	}break;
	default:
		break;
	}

	Mat mask, image_mask;

	double threshold_hight = 0.01;

	ransac_partiton(img_3D, mask, image_mask, px_partition, threshold_hight);

	//show_image(mask, 2);

	Mat mask_rgb;
	cvtColor(mask, mask_rgb, COLOR_GRAY2RGB);

	Mat img2D_show;

	get_depth(mask, img2D_show, out, px_partition, image_mask, img_2D);

    //Mat image_out;
    //hstack(mask_rgb, img2D_show, image_out);

    out.image.push_back(img2D_show);

}

