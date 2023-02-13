/**
  * Snapshot
  * Copyright 2014 Denis Meyer, CallToPower Software
  */
#ifndef LIBIMGFILTER
#define LIBIMGFILTER

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const int blurFactor = 21;

cv::Mat filter_grey(cv::Mat img)
{
    cv::Mat imgTmp;
    cv::cvtColor(img, imgTmp, CV_BGR2GRAY);
    return imgTmp;
}

cv::Mat filter_blur(cv::Mat img)
{
    cv::Mat imgTmp;
    for (int i = 1; i < blurFactor; i = i + 2) {
        cv::blur(img, imgTmp, cv::Size(i, i));
    }
    return imgTmp;
}

cv::Mat filter_invert(cv::Mat img)
{
    cv::Mat imgTmp(img);

    int tmp_height    = imgTmp.size().height;
    int tmp_width     = imgTmp.size().width;
    int tmp_step      = imgTmp.step;
    int tmp_channels  = imgTmp.channels();
    uchar *tmp_data   = (uchar *)imgTmp.data;

    for(int i = 0; i < tmp_height; ++i)
    {
        for(int j = 0; j < tmp_width; ++j)
        {
            for(int k = 0; k < tmp_channels; ++k)
            {
                tmp_data[i * tmp_step + j * tmp_channels + k] = 255 - tmp_data[i * tmp_step + j * tmp_channels + k];
            }
        }
    }

    return imgTmp;
}

cv::Mat filter_fancy1(cv::Mat img)
{
    cv::Mat imgTmp;
    cv::cvtColor(img, imgTmp, CV_BGR2HSV);
    return imgTmp;
}

cv::Mat filter_fancy2(cv::Mat img)
{
    cv::Mat imgTmp;
    cv::cvtColor(img, imgTmp, CV_BGR2HLS);
    return imgTmp;
}

cv::Mat filter_foggyRetro(cv::Mat img)
{
    cv::Mat imgTmp;
    cv::cvtColor(img, imgTmp, CV_BGR2YUV);
    return imgTmp;
}

cv::Mat filter_switchRB(cv::Mat img)
{
    cv::Mat imgTmp;
    cv::cvtColor(img, imgTmp, CV_BGR2RGB);
    return imgTmp;
}

cv::Mat filter_partyEdges(cv::Mat img, int threshold, int thickness, bool imgIsGrey)
{
    cv::RNG rng(12345);
    if(!imgIsGrey) {
        img = filter_grey(img);
    }
    cv::Mat canny_output;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    // Detect edges using canny
    cv::Canny(img, canny_output, threshold, threshold * 2, 3);
    // Find contours
    cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    // Draw contours
    cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
    for(size_t i = 0; i< contours.size(); ++i) {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        cv::drawContours(drawing, contours, i, color, thickness, 8, hierarchy, 0, cv::Point());
    }

    return drawing;
}

cv::Mat filter_mirror(cv::Mat img)
{
    cv::Mat imgTmp(img);

    int tmp_height    = imgTmp.size().height;
    int tmp_width     = imgTmp.size().width;
    int tmp_step      = imgTmp.step;
    int tmp_channels  = imgTmp.channels();
    uchar *tmp_data   = (uchar *)imgTmp.data;

    for(int i = 0; i < tmp_height; ++i)
    {
        for(int j = tmp_width / 2, cnt = tmp_width / 2; j < tmp_width; ++j, --cnt)
        {
            for(int k = 0; k < tmp_channels; ++k)
            {
                ((uchar *)(tmp_data + i * tmp_step))[j * tmp_channels + k] = ((uchar *)(tmp_data + i * tmp_step))[cnt * tmp_channels + k];
            }
        }
    }

    return imgTmp;
}

cv::Mat filter_many(cv::Mat img)
{
    cv::Mat imgTmp(img);

    int tmp_height    = imgTmp.size().height;
    int tmp_width     = imgTmp.size().width;
    int tmp_step      = imgTmp.step;
    int tmp_channels  = imgTmp.channels();
    uchar *tmp_data   = (uchar *)imgTmp.data;

    for(int i = 0; i < tmp_height; ++i)
    {
        for(int j = tmp_width / 2, cnt = tmp_width / 2; j < tmp_width; ++j, --cnt)
        {
            for(int k = 0; k < tmp_channels; ++k)
            {
                ((uchar *)(tmp_data + i * tmp_step))[j * tmp_channels + k] = ((uchar *)(tmp_data + i * tmp_step))[(j - cnt) * tmp_channels + k];
            }
        }
    }

    return imgTmp;
}

cv::Mat filter_rotate(cv::Mat img, double angle)
{
    cv::Mat imgTmp;

    cv::Point2f pt(img.size().width / 2., img.size().height / 2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
    cv::warpAffine(img, imgTmp, r, cv::Size(img.size().width, img.size().height));

    return imgTmp;
}

// direction: 0 around x-axis, positive around y-axis, negative around both axes
cv::Mat filter_flip(cv::Mat img, int direction)
{
    cv::Mat imgTmp;

    cv::flip(img, imgTmp, direction);

    return imgTmp;
}

/**
 * @brief makeCanvas Makes composite image from the given images
 * @param vecMat Vector of Images.
 * @param windowHeight The height of the new composite image to be formed.
 * @param nRows Number of rows of images. (Number of columns will be calculated depending on the value of total number of images).
 * @return new composite image.
 */
cv::Mat filter_manyInOne(cv::Mat img, int nrOfImgs, int nRows)
{
    std::vector<cv::Mat> vecMat;
    for(int i = 0; i < nrOfImgs; ++i) {
        vecMat.push_back(cv::Mat(img));
    }
    int windowHeight = img.size().height;
    int N = vecMat.size();
    nRows  = nRows > N ? N : nRows;
    int edgeThickness = 10;
    int imagesPerRow = ceil(double(N) / nRows);
    int resizeHeight = floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
    int maxRowLength = 0;

    std::vector<int> resizeWidth;
    for (int i = 0; i < N;) {
        int thisRowLen = 0;
        for (int k = 0; k < imagesPerRow; k++) {
            double aspectRatio = double(vecMat[i].cols) / vecMat[i].rows;
            int temp = int( ceil(resizeHeight * aspectRatio));
            resizeWidth.push_back(temp);
            thisRowLen += temp;
            if (++i == N) break;
        }
        if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength) {
            maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
        }
    }
    int windowWidth = maxRowLength;
    cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, cv::Scalar(0, 0, 0));

    for (int k = 0, i = 0; i < nRows; i++) {
        int y = i * resizeHeight + (i + 1) * edgeThickness;
        int x_end = edgeThickness;
        for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
            int x = x_end;
            cv::Rect roi(x, y, resizeWidth[k], resizeHeight);
            cv::Mat target_ROI = canvasImage(roi);
            cv::resize(vecMat[k], target_ROI, target_ROI.size());
            x_end += resizeWidth[k] + edgeThickness;
        }
    }
    return canvasImage;
}

#endif // LIBIMGFILTER
