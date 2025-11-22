#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

Mat gaussianFilterForCanny(Mat src) {
	int height = src.rows;
	int width = src.cols;
	int M = height * width;
	float sigma = 0.8;
	int w = ceil(sigma * 6);
	Mat dst = src;

	if (w % 2 == 0) {
		w = w + 1;
	}

	int x0 = w / 2;
	int y0 = w / 2;
	float G[10][10];
	float sum = 0;
	float constanta = 1 / (2 * PI * sigma * sigma);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < w; y++) {
			G[x][y] = constanta * exp(-((x - x0) * (x - x0) + (y - y0) * (y - y0)) / (2 * sigma * sigma));
			sum += G[x][y];
		}
	}


	for (int i = w / 2; i < height - w / 2; i++) {
		for (int j = w / 2; j < width - w / 2; j++) {
			float result = 0;
			for (int k = 0; k < w; k++) {
				for (int l = 0; l < w; l++) {
					result = result + (src.at<uchar>(i - w / 2 + k, j - w / 2 + l) * G[k][l]);
					result /= sum;
				}
			}
			dst.at<uchar>(i, j) = saturate(result);
		}
	}
	return dst;
}

Mat color2Gray(Mat src)
{
	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vec3b v3 = src.at<Vec3b>(i, j);
			uchar b = v3[0];
			uchar g = v3[1];
			uchar r = v3[2];
			dst.at<uchar>(i, j) = (r + g + b) / 3;
		}
	}
	return dst;
}

Mat preprocessImage(const char image_path[]) {
	// Load color image
	Mat image = imread(image_path);
	// Convert to grayscale
	Mat gray = color2Gray(image);
	// Apply GaussianBlur to reduce noise and improve contour detection
	Mat blurred = gaussianFilterForCanny(gray);
	imshow("Reduced noise", blurred);
	return blurred;
}

vector<Point> detectLicensePlate(Mat blurredImage) {
	// Edge detection
	Mat edged;
	Canny(blurredImage, edged, 30, 200);
	// Find contours
	vector<vector<Point>> contours;
	findContours(edged, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	// Sort contours based on area and keep the largest one (assuming it is the license plate)
	sort(contours.begin(), contours.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
		return contourArea(c1, false) > contourArea(c2, false);
		});

	for (const auto& contour : contours) {
		// Approximate the contour
		vector<Point> approx;
		approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
		if (approx.size() == 4) {
			return approx;
		}
	}
	return {};
}

Mat extractLicensePlate(Mat image, vector<Point> contour) {
	// Create a mask to extract the license plate
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	vector<vector<Point>> contours = { contour };
	drawContours(mask, contours, -1, Scalar(255), FILLED);
	Mat licensePlate;
	image.copyTo(licensePlate, mask);
	// Crop the license plate region
	Rect rect = boundingRect(contour);
	imshow("License plate", licensePlate(rect));
	return licensePlate(rect);
}

string ocrLicensePlate(const Mat& licensePlate) {

	// Initialize Tesseract API
	tesseract::TessBaseAPI ocr;
	if (ocr.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY) != 0) {
		cerr << "Could not initialize tesseract." << endl;
		return "";
	}

	ocr.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

	// Convert OpenCV Mat to Leptonica Pix
	Pix* pix = pixCreate(licensePlate.cols, licensePlate.rows, 8);
	if (!pix) {
		cerr << "Failed to create Pix structure." << endl;
		ocr.End();
		return "";
	}

	for (int y = 0; y < licensePlate.rows; ++y) {
		memcpy(pixGetData(pix) + y * pixGetWpl(pix), licensePlate.ptr(y), licensePlate.cols);
	}

	// Set image for Tesseract to process
	ocr.SetImage(pix);

	// Get OCR result
	char* outText = ocr.GetUTF8Text();
	if (!outText) {
		cerr << "Failed to get OCR text." << endl;
		pixDestroy(&pix);
		ocr.End();
		return "";
	}
	string text(outText);

	// Clean up
	delete[] outText;
	ocr.End();
	pixDestroy(&pix);

	return text;
}

void project() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat src = preprocessImage(fname);
		vector<Point> edges = detectLicensePlate(src);
		Mat licensePlate = extractLicensePlate(src, edges);
		cout << ocrLicensePlate(licensePlate);
		waitKey();
	}
}

int main() {
	project();
	return 0;
}