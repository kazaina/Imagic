#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

static const int MAX_CORNERS = 100000;

bool compare(pair<float, float> a, pair<float, float> b) {
	if (a < b) return true;
	else if (a == b) return a.second < b.second;
	else false;
}

bool cmp(cv::Point2f a, cv::Point2f b) {
	if (a.x < b.x) return true;
	else if (a.x == b.x) return a.y < b.y;
	return false;
}

int main(int argc, char**argv) {

	string filename = "test2.png";
	Mat src = imread(filename); // �̹��� �ҷ�����.

								// Check if image is loaded fine
	if (!src.data)
		cerr << "Problem loading image!!!" << endl;

	Mat gray;

	if (src.channels() == 3) // R, G, B �� ���� �÷� ������ ��� �ִ� ��� gray��
	{
		cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
	}
	else {   // �̹� gray�� ���
		gray = src;
	}

	// Show gray image
	imshow("gray", gray);


	// ���� ��� : threshold�� �Ӱ谪���� ������ ��, ũ�� ������ ��. -> threshold() �Լ�
	// �Ӱ谪�� �̹��� ��ü�� �����Ͽ� ó���Ǽ� �ϳ��� �̹��� ������ �ٸ��� �Ϻ� ������ ��� ��� �Ǵ� ���������� �������� �Ǵ� ����
	// -> �̹����� ���� �������� thresholding �ϴ� �� -> adaptivethreshold() ��� 
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	Mat bw;
	adaptiveThreshold(~gray, bw, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -2);
	// adaptiveThreshold(src, dst, maxValue, adaptiveMethod, thresholdType, blockSize, C)
	// maxValue : �Ӱ谪(255�� �������)
	// adaptiveMethod : ��� ���
	// blockSize  : ���� ������
	// C : ����̳� ���� ��տ��� ������ ��
	// ADAPTIVE_THRESH_MEAN_C : �ֺ������� ��հ����� ����
	// ADAPTIVE_THRESH_GAUSSIAN_C �� ���� // ����þ����� ����ġ. �߽ɿ� 

	// Show binary image
	//imshow("binary", bw);

	// Create the images that will use to extract the horizonta and vertical lines
	Mat horizontal = bw.clone();
	Mat vertical = bw.clone();

	int scale;
	if (horizontal.cols > 1000)
		scale = 30; // play with this variable in order to increase/decrease the amount of lines to be detected
	else scale = 15;

	// Specify size on horizontal axis
	int horizontalsize = horizontal.cols / scale;
	cout << horizontal.cols << endl;
	// Create structure element for extracting horizontal lines through morphology operations
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1)); // Ŀ���� ����� ����� �ִ� ��.   [1, 1, 1, 1, 1, 1, 1, 1, .... 1] ���

																						  // Ŀ���� ��翡 ��� �� �Ϳ� ���� ����� �ٲ��ִ� ����. // ���� ���·� Ŀ���� ���� �־��ָ� ���� ���� ����.  0 1 1 1 1 0 -> 0 0 1 1 0 0 // �۾��� �پ��� ǥ�� �״��.

																						  // Apply morphology operations
	cv::morphologyEx(horizontal, horizontal, cv::MORPH_OPEN, horizontalStructure, Point(-1, -1), 1); // open ���� -> ħ�� �� ��â

																									 //erode(horizontal, horizontal, horizontalStructure, Point(-1, -1)); // Ŀ���� ũ�⿡ ���� ����ó�� �ȼ��� ������. -> ��� ������ �پ���. (��� ����� ����)
																									 //dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1)); // ��� ������ ������ �ٽ� �ø�.(����κ�)

																									 //imshow("horizontal", horizontal);

																									 // Specify size on vertical axis



	int verticalsize = vertical.rows / scale;

	// Create structure element for extracting vertical lines through morphology operations
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));

	// Apply morphology operations
	/*erode(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1));*/

	// Show extracted vertical lines
	cv::morphologyEx(vertical, vertical, cv::MORPH_OPEN, verticalStructure, Point(-1, -1), 1); // open ���� -> ħ�� �� ��â
																							   //erode(vertical, vertical, verticalStructure, Point(-1, -1));

																							   //imshow("vertical", vertical);

																							   // create a mask which includes the tables
	Mat mask = horizontal + vertical;
	Mat allStructure = getStructuringElement(MORPH_RECT, Size(2, 2));
	erode(vertical, vertical, verticalStructure, Point(-1, -1));

	imshow("mask", ~mask);

	Mat cannyImg;
	Canny(~mask, cannyImg, 80, 100);

	vector<Vec4i> lines;

	int threshold = 80; // r,�� ��鿡�� ��� ��� �������� ������ �� �������� �Ǵ������� ���� �ּҰ�
	int maxLineGap = 10;
	HoughLinesP(mask, lines, 1, CV_PI / 180, threshold, 2, 6);
	Mat houghImg(src.rows, src.cols, CV_8UC3, Scalar(255, 255, 255));

	Vec4i *temp = new Vec4i[lines.size()];
	for (size_t i = 0; i < lines.size(); i++) {
		temp[i] = lines[i];
		//cout << "check:" << Point(temp[i][0], temp[i][1]) << Point(temp[i][2], temp[i][3]) << endl;
	}








	bool *checking = new bool[lines.size()];



	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines.size(); j++) {
			if ((abs(temp[i][1] - temp[j][1]) < 5) && ((temp[i][1] != temp[j][1]))) {
				temp[j][1] = temp[i][1];
			}
		}
	}
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines.size(); j++) {
			if ((abs(temp[i][3] - temp[j][3]) < 5) && ((temp[i][3] != temp[j][3]))) {
				temp[j][3] = temp[i][3];
			}
		}
	}
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines.size(); j++) {
			if ((abs(temp[i][0] - temp[j][0]) < 5) && ((temp[i][0] != temp[j][0]))) {
				temp[j][0] = temp[i][0];
			}
		}
	}
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines.size(); j++) {
			if ((abs(temp[i][2] - temp[j][2]) < 5) && ((temp[i][2] != temp[j][2]))) {
				temp[j][2] = temp[i][2];
			}
		}
	}













	for (size_t i = 0; i < lines.size(); i++)
	{
		cout << Point(temp[i][0], temp[i][1]) << Point(temp[i][2], temp[i][3]) << endl;
		line(houghImg, Point(temp[i][0], temp[i][1]), Point(temp[i][2], temp[i][3]), Scalar(255, 0, 0), 2, LINE_AA);
	}







	//Mat mark2;
	//resize(~mask, mark2,Size(src.rows, src.cols),0,0,1);




	imshow("Hough", houghImg);
	//Mat pls = houghImg + ~mask;
	//imshow("pls", pls);

	//////////////////////////////////////////////////////////////////////////////
	//Mat img_edge;
	//ĳ�Ͽ����� �̿��Ͽ� ���� ������ �����մϴ�.  
	//Canny(mask, img_edge, 30, 90, 3);
	//imshow("mask1", img_edge);

	////���� ������ �����մϴ�.  
	//vector<Vec4i> lines;
	//HoughLinesP(img_edge, lines, 1, CV_PI / 180, 30, 30, 3);

	////������ ������ ���� �׷��ݴϴ�.  
	//for (int i = 0; i < lines.size(); i++)
	//{
	//   Vec4i L = lines[i];
	//   line(mask, Point(L[0], L[1]), Point(L[2], L[3]),
	//      Scalar(0, 0, 255), 1, LINE_AA);
	//}
	//imshow("mask2", mask);


	//cvNamedWindow("edge", WINDOW_AUTOSIZE);
	//imshow("edge", img_edge);

	//cvNamedWindow("result", WINDOW_AUTOSIZE);
	//imshow("result", img_original);


	/////////////////////////////////////////////////////////////////////////////
	// corner algorithm ���� ////////////////////////////////////////////////////
	cv::Mat imgA = ~mask;
	cv::Mat imgB = ~mask;
	cv::Mat imgC = ~mask;

	int win_size = 5;

	vector<cv::Point2f> cornerA;
	cv::goodFeaturesToTrack(imgA, cornerA, MAX_CORNERS, 0.01, 5, cv::noArray(), 3, false, 0.04);
	cv::cornerSubPix(mask, cornerA, cv::Size(win_size, win_size), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
		20, 0.03));

	int *fixed = new int[cornerA.size() + 1];
	int *score = new int[cornerA.size() + 1];
	int *range = new int[cornerA.size() + 1];

	for (int i = 0; i < (int)cornerA.size(); i++) {
		fixed[i] = 0;	//���� �̷� ����
		score[i] = 0;	//���� - �ڽŰ� ���� x��ǥ�� ������ �ִ� ������ ��
		range[i] = 0;	//���� �������� �Ÿ� - �ʹ� ���� ��� ������� �����ϱ� ���ؼ� ���
	}

	for (int i = 0; i < (int)cornerA.size(); i++) {
		for (int j = i; j < (int)cornerA.size(); j++) {
			if (abs(cornerA[i].x - cornerA[j].x) < 5 && i != j)	//x��ǥ�� �Ÿ��� 5�̸��� ��� ������ ��ǥ�� ����( �����ϱ�)
			{
				if (fixed[j] == 0)								//���� �̷��� ���� ��츸
				{	
					cornerA[j].x = cornerA[i].x;
					fixed[j] = 1;								//���� �Ǿ��ٸ� �����̷��� 1�� ����
				}
			}
		}
	}

	for (int i = 0; i < (int)cornerA.size(); i++) {				//���� ����
		for (int j = 0; j < (int)cornerA.size(); j++) {
			if (cornerA[i].x == cornerA[j].x && i != j)
			{
				score[i] +=1;
			}
		}
	}
	for (int i = 0; i < (int)cornerA.size(); i++) {
		for (int j = i; j < (int)cornerA.size(); j++) {
			if ((pow(cornerA[i].x - cornerA[j].x,2) + pow(cornerA[i].y - cornerA[j].y,2))<100)		//�Ÿ��� 10�̸��� ��� 
			{
				if (i != j) {
					if (score[i] > score[j])											//i�� ������ ������ j�� Ż����Ű��
						range[j] = -1;
					else																//j�� ������ ������ i�� Ż����Ų��.
						range[i] = -1;
				}
			}
		}
	}

	int sa = 0;
	for (int i = 0; i < (int)cornerA.size(); i++) {
		sa++;
		cout << cornerA[i] << sa << endl;

		if ( score[i] > 0&& range[i]==0) {											//���ھ ����̰� �Ÿ��� ������ �͸� ���
			circle(imgC, cornerA[i], 6, cv::LINE_AA);								// ���ǰ�� �ߺ�üũ�� ����⶧���� circle���
		}
	}


	cv::imshow("LK Optical Flow Example", imgC);																		///////	TO DO ::	X��ǥ�� ���� ��������� 
																														///////            ������� ���� X��ǥ�� ������ ��찡 ����



	Mat houghImggray;
	cv::cvtColor(houghImg, houghImggray, cv::COLOR_RGB2GRAY);

	cv::Mat imgA2 = houghImggray;
	cv::Mat imgB2 = houghImggray;
	cv::Mat imgC2 = houghImggray;


	vector<cv::Point2f> cornerA2, cornerB2;
	cv::goodFeaturesToTrack(imgA2, cornerA2, MAX_CORNERS, 0.01, 5, cv::noArray(), 3, false, 0.04);
	cv::cornerSubPix(mask, cornerA2, cv::Size(win_size, win_size), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
		20, 0.03));
	vector<uchar> features_found2;
	cv::calcOpticalFlowPyrLK(imgA2, imgB2, cornerA2, cornerB2, features_found2, cv::noArray(), cv::Size(win_size * 2 + 1, win_size * 2 + 1), 5,
		cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.3));


	for (int i = 0; i < (int)cornerA2.size(); i++) {
		if (!features_found2[i]) continue;
		line(imgC2, cornerA2[i], cornerB2[i], cv::Scalar(152, 125, 0), 6, cv::LINE_AA);
	}

	cv::imshow("LK Optical Flow Example2", imgC2);

	///////////////////////////////////////////////////////////////////////

	cv::waitKey(0);
	//cv::destroyWindow("Example1");
	return 0;
}