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
	Mat src = imread(filename); // 이미지 불러오기.

								// Check if image is loaded fine
	if (!src.data)
		cerr << "Problem loading image!!!" << endl;

	Mat gray;

	if (src.channels() == 3) // R, G, B 세 개의 컬러 정보가 들어 있는 경우 gray로
	{
		cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
	}
	else {   // 이미 gray인 경우
		gray = src;
	}

	// Show gray image
	imshow("gray", gray);


	// 기존 방식 : threshold로 임계값보다 작으면 흑, 크면 백으로 함. -> threshold() 함수
	// 임계값이 이미지 전체에 적용하여 처리되서 하나의 이미지 음영이 다르면 일부 영역이 모두 흰색 또는 검정색으로 보여지게 되는 단점
	// -> 이미지를 작은 영역별로 thresholding 하는 것 -> adaptivethreshold() 사용 
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	Mat bw;
	adaptiveThreshold(~gray, bw, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -2);
	// adaptiveThreshold(src, dst, maxValue, adaptiveMethod, thresholdType, blockSize, C)
	// maxValue : 임계값(255는 흰색까지)
	// adaptiveMethod : 계산 방법
	// blockSize  : 영역 사이즈
	// C : 평균이나 가중 평균에서 차감할 값
	// ADAPTIVE_THRESH_MEAN_C : 주변영역의 평균값으로 결정
	// ADAPTIVE_THRESH_GAUSSIAN_C 도 있음 // 가우시안으로 가중치. 중심에 

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
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1)); // 커널의 모양을 만들어 주는 것.   [1, 1, 1, 1, 1, 1, 1, 1, .... 1] 모양

																						  // 커널의 모양에 들어 온 것에 대한 모양을 바꿔주는 것임. // 직선 형태로 커널을 만들어서 넣어주면 수평만 남을 것임.  0 1 1 1 1 0 -> 0 0 1 1 0 0 // 글씨는 줄어들고 표는 그대로.

																						  // Apply morphology operations
	cv::morphologyEx(horizontal, horizontal, cv::MORPH_OPEN, horizontalStructure, Point(-1, -1), 1); // open 연산 -> 침식 후 팽창

																									 //erode(horizontal, horizontal, horizontalStructure, Point(-1, -1)); // 커널의 크기에 따라 경계근처의 픽셀들 버려짐. -> 흰색 영역이 줄어든다. (흰색 노이즈를 없앰)
																									 //dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1)); // 흰색 노이즈 제거후 다시 늘림.(흰색부분)

																									 //imshow("horizontal", horizontal);

																									 // Specify size on vertical axis



	int verticalsize = vertical.rows / scale;

	// Create structure element for extracting vertical lines through morphology operations
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));

	// Apply morphology operations
	/*erode(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1));*/

	// Show extracted vertical lines
	cv::morphologyEx(vertical, vertical, cv::MORPH_OPEN, verticalStructure, Point(-1, -1), 1); // open 연산 -> 침식 후 팽창
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

	int threshold = 80; // r,θ 평면에서 몇개의 곡선이 한점에서 만났을 때 직선으로 판단할지에 대한 최소값
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
	//캐니에지를 이용하여 에지 성분을 검출합니다.  
	//Canny(mask, img_edge, 30, 90, 3);
	//imshow("mask1", img_edge);

	////직선 성분을 검출합니다.  
	//vector<Vec4i> lines;
	//HoughLinesP(img_edge, lines, 1, CV_PI / 180, 30, 30, 3);

	////검출한 직선을 영상에 그려줍니다.  
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
	// corner algorithm 적용 ////////////////////////////////////////////////////
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
		fixed[i] = 0;	//수정 이력 여부
		score[i] = 0;	//점수 - 자신과 같은 x좌표를 가지고 있는 점들의 수
		range[i] = 0;	//점과 점사이의 거리 - 너무 붙을 경우 노이즈로 간주하기 위해서 사용
	}

	for (int i = 0; i < (int)cornerA.size(); i++) {
		for (int j = i; j < (int)cornerA.size(); j++) {
			if (abs(cornerA[i].x - cornerA[j].x) < 5 && i != j)	//x좌표의 거리가 5미만일 경우 동일한 좌표로 수정( 정렬하기)
			{
				if (fixed[j] == 0)								//수정 이력이 없는 경우만
				{	
					cornerA[j].x = cornerA[i].x;
					fixed[j] = 1;								//정렬 되었다면 수정이력을 1로 수정
				}
			}
		}
	}

	for (int i = 0; i < (int)cornerA.size(); i++) {				//점수 갱신
		for (int j = 0; j < (int)cornerA.size(); j++) {
			if (cornerA[i].x == cornerA[j].x && i != j)
			{
				score[i] +=1;
			}
		}
	}
	for (int i = 0; i < (int)cornerA.size(); i++) {
		for (int j = i; j < (int)cornerA.size(); j++) {
			if ((pow(cornerA[i].x - cornerA[j].x,2) + pow(cornerA[i].y - cornerA[j].y,2))<100)		//거리가 10미만일 경우 
			{
				if (i != j) {
					if (score[i] > score[j])											//i의 점수가 높으면 j를 탈락시키고
						range[j] = -1;
					else																//j의 점수가 높으면 i를 탈락시킨다.
						range[i] = -1;
				}
			}
		}
	}

	int sa = 0;
	for (int i = 0; i < (int)cornerA.size(); i++) {
		sa++;
		cout << cornerA[i] << sa << endl;

		if ( score[i] > 0&& range[i]==0) {											//스코어가 양수이고 거리가 정상인 것만 출력
			circle(imgC, cornerA[i], 6, cv::LINE_AA);								// 점의경우 중복체크가 힘들기때문에 circle사용
		}
	}


	cv::imshow("LK Optical Flow Example", imgC);																		///////	TO DO ::	X좌표에 의한 노이즈설정시 
																														///////            노이즈끼리 같은 X좌표를 가지는 경우가 존재



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
