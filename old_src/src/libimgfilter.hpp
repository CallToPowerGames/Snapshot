////////////////////////////////////////////////////////////////////////
//
// Image filter library
//
// @author: Denis Meyer
//
// Used libraries:
//   - opencv ( http://opencv.willowgarage.com )
//
// License:
// see file 'LICENSE'
//
////////////////////////////////////////////////////////////////////////
#ifndef _LIBIMGFILTER_
#define _LIBIMGFILTER_

#include <cv.h>
#include <highgui.h>

/*************************************
 * filter functions
 *************************************/

IplImage *getGrayedImage(IplImage *img)
{
  IplImage *imgGray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  cvCvtColor(img, imgGray, CV_BGR2GRAY);
  return imgGray;
}

IplImage* getBlurredImage(IplImage *img, int method = CV_GAUSSIAN)
{
  IplImage *imgS = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
  cvSmooth(img, imgS, method, 3, 3);
  return imgS;
}

IplImage *getInvertedImage(IplImage *img)
{
  IplImage *imgInv = cvCloneImage(img);
  int tmp_height    = imgInv->height;
  int tmp_width     = imgInv->width;
  int tmp_step      = imgInv->widthStep;
  int tmp_channels  = imgInv->nChannels;
  uchar *tmp_data      = (uchar *)imgInv->imageData;
  for(int i = 0; i < tmp_height; ++i)
    {
      for(int j = 0; j < tmp_width; ++j)
        {
	  for(int k = 0; k < tmp_channels; ++k)
            {
	      tmp_data[i*tmp_step + j*tmp_channels + k] = 255 - tmp_data[i*tmp_step + j*tmp_channels + k];
            }
        }
    }
  return imgInv;
}

IplImage* getErodedImage(IplImage *img)
{
  IplImage *imgE = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
  cvErode(img, imgE, NULL, 1);
  return imgE;
}

IplImage* getDilatedImage(IplImage *img)
{
  IplImage *imgD = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
  cvDilate(img, imgD, NULL, 1);
  return imgD;
}

IplImage* getFloodFilledImage(bool mouseMoved, IplImage *img, CvPoint &p)
{
  IplImage *imgF = cvCloneImage(img);
  CvPoint seed_point;
  if(mouseMoved)
    {
      seed_point = cvPoint(p.x, p.y);
    }
  else
    {
      seed_point = cvPoint(imgF->width / 2, imgF->height / 2);
    }
  CvScalar color = CV_RGB(250, 0, 0);
  cvFloodFill(imgF, seed_point, color, cvScalarAll(5.0), cvScalarAll(5.0), NULL, 4, NULL );
  return imgF;
}

IplImage *getMirroredImage(IplImage *img)
{
  IplImage *imgInv = cvCloneImage(img);
  int height    = imgInv->height;
  int width     = imgInv->width;
  int step      = imgInv->widthStep;
  int nchannels = imgInv->nChannels;

  for(int i = 0; i < height; ++i)
    {
      for(int j = width / 2, cnt = width / 2; j < width; ++j, --cnt)
        {
	  for(int k = 0; k < nchannels; ++k)
            {
	      ((uchar *)(imgInv->imageData + i*step))[j*nchannels + k] = ((uchar *)(imgInv->imageData + i*step))[cnt*nchannels + k];
	    }
	}
    }
  return imgInv;
}

IplImage* getCannyEdgedImage(IplImage *img, int low, int high)
{
  IplImage *tmp1 = cvCloneImage(img);
  IplImage *tmp2;
  tmp2 = tmp1;
  tmp1 = getGrayedImage(img);
  cvReleaseImage(&tmp2);
  tmp2 = cvCloneImage(tmp1);
  cvCanny(tmp1, tmp2, low, high);
  cvReleaseImage(&tmp1);
  return tmp2;
}

IplImage *getManyImages(IplImage *img)
{
  IplImage *imgInv = cvCloneImage(img);
  int height    = imgInv->height;
  int width     = imgInv->width;
  int step      = imgInv->widthStep;
  int nchannels = imgInv->nChannels;

  for(int i = 0; i < height; ++i)
    {
      for(int j = width / 2, cnt = width / 2; j < width; ++j, --cnt)
        {
	  for(int k = 0; k < nchannels; ++k)
            {
	      ((uchar *)(imgInv->imageData + i*step))[j*nchannels + k] = ((uchar *)(imgInv->imageData + i*step))[(j - cnt)*nchannels + k];
	    }
	}
    }
  return imgInv;
}

IplImage *getFancyImage(IplImage *img)
{
  IplImage *imgInv = cvCloneImage(img);

  cvCvtColor(img, imgInv, CV_BGR2HSV);
  return imgInv;
}

IplImage* getScaledImage(IplImage *img, int sX, int sY)
{
  IplImage *imgO;
  if((sX <= 0) || (sY <= 0))
    {
      imgO = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels);
    }
  else
    {
      imgO = cvCreateImage(cvSize(sX, sY), img->depth, img->nChannels);
    }
  cvResize(img, imgO);
  return imgO;
}

IplImage *getFourImages(IplImage *img1, IplImage *img2, IplImage *img3, IplImage *img4)
{
  IplImage *DispImage;
  DispImage = cvCloneImage(img1);

  int height = DispImage->height;
  int width  = DispImage->width;

  IplImage *tmp;
  tmp = cvCloneImage(img1);
  cvSetImageROI(DispImage, cvRect(0, 0, DispImage->width / 2, DispImage->height / 2));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img2);
  cvSetImageROI(DispImage, cvRect(width / 2, 0, DispImage->width / 2, DispImage->height / 2));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img3);
  cvSetImageROI(DispImage, cvRect(0, height / 2, DispImage->width / 2, DispImage->height / 2));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img4);
  cvSetImageROI(DispImage, cvRect(width / 2, height / 2, DispImage->width / 2, DispImage->height / 2));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  
  return DispImage;
}

IplImage *getSixteenImages(IplImage *img1, IplImage *img2, IplImage *img3,
			   IplImage *img4, IplImage *img5, IplImage *img6,
			   IplImage *img7, IplImage *img8, IplImage *img9,
			   IplImage *img10, IplImage *img11, IplImage *img12,
			   IplImage *img13, IplImage *img14, IplImage *img15,
			   IplImage *img16)
{
  IplImage *DispImage;
  DispImage = cvCloneImage(img1);

  int height = DispImage->height;
  int width  = DispImage->width;

  IplImage *tmp;

  // top left
  tmp = cvCloneImage(img1);
  cvSetImageROI(DispImage, cvRect(0, 0, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img2);
  cvSetImageROI(DispImage, cvRect(width / 4, 0, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img3);
  cvSetImageROI(DispImage, cvRect(0, height / 4, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img4);
  cvSetImageROI(DispImage, cvRect(width / 4, height / 4, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);

  // top right
  tmp = cvCloneImage(img5);
  cvSetImageROI(DispImage, cvRect(width / 2, 0, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img6);
  cvSetImageROI(DispImage, cvRect(width - (width /4), 0, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img7);
  cvSetImageROI(DispImage, cvRect(width / 2, height / 4, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img8);
  cvSetImageROI(DispImage, cvRect(width - (width /4), height / 4, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);

  // bottom left
  tmp = cvCloneImage(img9);
  cvSetImageROI(DispImage, cvRect(0, height / 2, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img10);
  cvSetImageROI(DispImage, cvRect(width / 4, height / 2, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img11);
  cvSetImageROI(DispImage, cvRect(0, height - (height / 4), DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img12);
  cvSetImageROI(DispImage, cvRect(width / 4, height - (height / 4), DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);

  // bottom right
  tmp = cvCloneImage(img13);
  cvSetImageROI(DispImage, cvRect(width / 2, height / 2, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img14);
  cvSetImageROI(DispImage, cvRect(width - (width / 4), height / 2, DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img15);
  cvSetImageROI(DispImage, cvRect(width / 2, height -  (height / 4), DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  tmp = cvCloneImage(img16);
  cvSetImageROI(DispImage, cvRect(width - (width / 4), height - (height / 4), DispImage->width / 4, DispImage->height / 4));
  cvResize(tmp, DispImage);
  cvResetImageROI(DispImage);
  cvReleaseImage(&tmp);
  
  return DispImage;
}

IplImage* getSwappedRBImage(IplImage *img)
{
  IplImage *imgD = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
  cvConvertImage(img, imgD, CV_CVTIMG_SWAP_RB);
  return imgD;
}

IplImage* getRotatedImage(IplImage *img, int angle, double scale)
{
  CvMat *rot_mat   = cvCreateMat(2, 3, CV_32FC1);
  IplImage *imgInv = cvCloneImage(img);
  int tmp_height   = imgInv->height;
  int tmp_width    = imgInv->width;

  // rotate image
  CvPoint2D32f center = cvPoint2D32f(tmp_width / 2, tmp_height / 2);
  cv2DRotationMatrix(center, angle, scale, rot_mat);
  // transform
  cvWarpAffine(img, imgInv, rot_mat, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
  cvReleaseMat(&rot_mat);

  return imgInv;
}

#endif /* _LIBIMGFILTER_ */
