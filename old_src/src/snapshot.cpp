////////////////////////////////////////////////////////////////////////
//
// Snapshot
//
// @author: Denis Meyer
//
// Used libraries:
//   - libimgflter
//   - opencv ( http://opencv.willowgarage.com )
//
// License:
// see file 'LICENSE'
//
////////////////////////////////////////////////////////////////////////

// #include "stdafx.h" // comment in under windows
// #define WINDOWS

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#ifndef WINDOWS
#include <time.h>
#endif // WINDOWS
#include "libimgfilter.hpp"

using std::string;
using std::ostringstream;
using std::cout;
using std::cerr;
using std::endl;

// name, version and build number
string g_name_version_build_t = "Snapshot v0.1.6 build 3";
#ifndef WINDOWS
string g_name_version_build   = g_name_version_build_t + " for Mac";
#endif //WINDOWS
#ifdef WINDOWS
string g_name_version_build   = g_name_version_build_t + " for Windows";
#endif //WINDOWS
// strings
char g_str_enabled            = '#';
char g_str_disabled           = '-';
char g_str_save               = 's';
char g_str_startStopVideo     = 'v';
char g_str_none               = '0';
char g_str_blurr              = '1';
char g_str_gray               = '2';
char g_str_invert             = '3';
char g_str_erode              = '4';
char g_str_dilate             = '5';
char g_str_fancy              = '6';
char g_str_swaprb             = '7';
char g_str_mirror             = '8';
char g_str_cannyEdge          = '9';
char g_str_rotate             = 'q';
char g_str_rotate1            = 'w';
char g_str_rotate5            = 'e';
char g_str_rotate10           = 'r';
char g_str_rotate45           = 't';
char g_str_manyimg            = 'a';
char g_str_floodfill          = 'f';
char g_str_fourimg            = 'y';
char g_str_fancyfourimg       = 'x';
char g_str_sixteenimg         = 'c';
char g_str_zoomIn             = '+';
char g_str_zoomOut            = '-';
// misc variables
#ifdef WINDOWS
int g_currImgNr               = 0;
int g_currVidNr               = 0;
#endif // WINDOWS
CvVideoWriter *writer;
CvPoint g_mousePoint;
bool g_continueLoop           = true;
bool g_keypressed             = false;
int g_initialWindowPosX       = 0;
int g_initialWindowPosY       = 1;
int g_trackbar1SwitchValue    = 0;
int g_trackbar2SwitchValue    = 0;
bool g_printLastMessage       = false;
bool g_saveVideo              = false;
bool g_saveVideoFirstTime     = true;
string g_lastMessage          = "";
int g_waitMs                  = 30;
double g_videoFPS             = g_waitMs - 10;
string g_suffix               = ".png";
#ifndef WINDOWS
string g_videoSuffix          = ".mp4";
#endif // WINDOWS
#ifdef WINDOWS
string g_videoSuffix          = ".avi";
#endif // WINDOWS
string g_videoFileName        = "";
string g_mainWin              = g_name_version_build + " | CallToPower Software";
string g_trackbar1            = "Low Threshold";
string g_trackbar2            = "High Threshold";
bool g_mouseMoved             = false;
bool g_allowMessages          = false;
int g_currAngle               = 0;
int g_anglePlus               = 90;
int g_cannyEdgeLow            = 50;
int g_cannyEdgeHigh           = 80;
int g_trackbar1MaxVal         = 98;
int g_trackbar2MaxVal         = 99;
int g_printFilterSpaces       = 25;
double g_scale                = 1.0;
// filters
bool g_blurr                  = false;
bool g_gray                   = false;
bool g_invert                 = false;
bool g_erode                  = false;
bool g_dilate                 = false;
bool g_floodFill              = false;
bool g_swapRB                 = false;
bool g_fancy                  = false;
bool g_rotate                 = false;
bool g_rotate1                = false;
bool g_rotate5                = false;
bool g_rotate10               = false;
bool g_rotate45               = false;
bool g_mirror                 = false;
bool g_cannyEdge              = false;
bool g_manyImages             = false;
bool g_fourImages             = false;
bool g_sixteenImages          = false;
bool g_fancyFourImages        = false;
bool g_zoomIn                 = false;
bool g_zoomOut                = false;
// restrictions
bool g_enable_startStopVideo  = true;
bool g_enable_blurr           = true;
bool g_enable_gray            = true;
bool g_enable_invert          = true;
bool g_enable_erode           = true;
bool g_enable_dilate          = true;
bool g_enable_fancy           = true;
bool g_enable_swaprb          = true;
bool g_enable_mirror          = true;
bool g_enable_cannyEdge       = true;
bool g_enable_rotate          = true;
bool g_enable_rotate1         = true;
bool g_enable_rotate5         = true;
bool g_enable_rotate10        = true;
bool g_enable_rotate45        = true;
bool g_enable_manyImages      = true;
bool g_enable_floodFill       = true;
bool g_enable_fourImages      = true;
bool g_enable_fancyFourImages = true;
bool g_enable_sixteenImages   = true;
bool g_enable_zoomIn          = true;
bool g_enable_zoomOut         = true;

/*************************************
 * filter related functions
 *************************************/

bool anyFilterActivated()
{
  return g_blurr || g_gray || g_invert
    || g_erode || g_dilate || g_floodFill
    || g_rotate || g_rotate1 || g_rotate5
    || g_rotate10 || g_rotate45 || g_manyImages
    || g_mirror || g_fourImages || g_sixteenImages
    || g_fancy || g_fancyFourImages || g_cannyEdge
    || g_zoomIn || g_zoomOut;
}

void updateRestrictions()
{
  g_enable_startStopVideo  = !g_gray && !g_cannyEdge;
  g_enable_blurr           = !g_floodFill;
  g_enable_gray            = !g_fancy && !g_fancyFourImages && !g_cannyEdge
    && !g_floodFill && !g_saveVideo;
  g_enable_invert          = true;
  g_enable_erode           = true;
  g_enable_dilate          = true;
  g_enable_fancy           = !g_gray && !g_cannyEdge && !g_floodFill;
  g_enable_swaprb          = true;
  g_enable_mirror          = !g_manyImages && !g_floodFill;
  g_enable_cannyEdge       = !g_fancyFourImages && !g_fancy && !g_gray
    && !g_floodFill && !g_saveVideo;
  g_enable_rotate          = true;
  g_enable_rotate1         = true;
  g_enable_rotate5         = true;
  g_enable_rotate10        = true;
  g_enable_rotate45        = true;
  g_enable_manyImages      = !g_mirror && !g_floodFill;
  g_enable_floodFill       = !g_fourImages && !g_fancyFourImages && !g_sixteenImages
    && !g_cannyEdge && !g_blurr && !g_gray
    && !g_mirror && !g_fancy && !g_manyImages
    && (g_scale == 1.0);
  g_enable_fourImages      = !g_floodFill;
  g_enable_fancyFourImages = !g_gray && !g_floodFill && !g_cannyEdge;
  g_enable_sixteenImages   = !g_floodFill;
  g_enable_zoomIn          = !g_floodFill;
  g_enable_zoomOut         = !g_floodFill;
}

/*************************************
 * output related functions
 *************************************/

void clearScreen()
{
  cout << "\033[2J";
}

void printHeader()
{
  string header = g_name_version_build;
  cout << endl << header << endl;
  for(unsigned int i = 0; i < header.size(); ++i)
    {
      cout << "-";
    }
  cout << endl << "CallToPower Software | http://www.calltopower.de/software" << endl << endl;
}

void printUsage()
{
  cout << "Change the trackbars with activated 'Canny Edge'-filter (" << g_str_cannyEdge << ") to set the high and low threshold." << endl << endl;
  cout << "General keys:" << endl;
  cout << "  Esc: Quit" << endl;
  cout << "  " << g_str_save << "  : Save snapshot" << endl;
  if(g_enable_startStopVideo)
    {
#ifndef WINDOWS
      cout << "  " << g_str_startStopVideo << "  : " << (g_saveVideo ? "Stop" : "Start") << " recording a video" << endl;
#endif // WINDOWS
#ifdef WINDOWS
      cout << "  " << g_str_startStopVideo << "  : " << (g_saveVideo ? "Stop" : "Start") << " recording a video (select 'Microsoft Video 1' when prompted)" << endl;
#endif // WINDOWS
    }
  cout << endl;
}

void printFilter(bool print, char abbr, string name, bool isEnabled)
{
  if(print)
    {
      cout << abbr << " - ";
      cout << name << ":";
      for(unsigned int i = 0; i < (g_printFilterSpaces - strlen(name.c_str())); ++i)
	{
	  cout << " ";
	}
      cout << (isEnabled ? g_str_enabled : g_str_disabled) << endl;
    } else
    {
      cout << endl;
    }
}

void printFilterStatus()
{
  cout << "    Filter Name               Status (enabled: '" << g_str_enabled << "', disabled: '" << g_str_disabled << "')" << endl;
  printFilter(true,                     g_str_none,         "Disable all filters", !anyFilterActivated() );
  printFilter(g_enable_blurr,           g_str_blurr,        "Blurr",               g_blurr               );
  printFilter(g_enable_gray,            g_str_gray,         "Gray",                g_gray                );
  printFilter(g_enable_invert,          g_str_invert,       "Invert",              g_invert              );
  printFilter(g_enable_erode,           g_str_erode,        "Erode",               g_erode               );
  printFilter(g_enable_dilate,          g_str_dilate,       "Dilate",              g_dilate              );
  printFilter(g_enable_fancy,           g_str_fancy,        "Fancy",               g_fancy               );
  printFilter(g_enable_swaprb,          g_str_swaprb,       "Swap red and blue",   g_swapRB              );
  printFilter(g_enable_mirror,          g_str_mirror,       "Mirror",              g_mirror              );
  printFilter(g_enable_cannyEdge,       g_str_cannyEdge,    "Canny Edge",          g_cannyEdge           );
  cout << endl;
  printFilter(g_enable_rotate,          g_str_rotate,       "Rotate",              (g_currAngle != 0)    );
  printFilter(g_enable_rotate1,         g_str_rotate1,      "Rotate 1 degrees",    (g_currAngle != 0)    );
  printFilter(g_enable_rotate5,         g_str_rotate5,      "Rotate 5 degrees",    (g_currAngle != 0)    );
  printFilter(g_enable_rotate10,        g_str_rotate10,     "Rotate 10 degrees",   (g_currAngle != 0)    );
  printFilter(g_enable_rotate45,        g_str_rotate45,     "Rotate 45 degrees",   (g_currAngle != 0)    );
  cout << endl;
  printFilter(g_enable_manyImages,      g_str_manyimg,      "Many Images",         g_manyImages          );
  printFilter(g_enable_floodFill,       g_str_floodfill,    "Flood Fill",          g_floodFill           );
  cout << endl;
  printFilter(g_enable_fourImages,      g_str_fourimg,      "4 Images",            g_fourImages          );
  printFilter(g_enable_fancyFourImages, g_str_fancyfourimg, "Fancy 4 Images",      g_fancyFourImages     );
  printFilter(g_enable_sixteenImages,   g_str_sixteenimg,   "16 Images",           g_sixteenImages       );
  cout << endl;
  printFilter(g_enable_zoomIn,          g_str_zoomIn,       "Zoom in",             (g_scale > 1.0)       );
  printFilter(g_enable_zoomOut,         g_str_zoomOut,      "Zoom out",            (g_scale < 1.0)       );
  cout << endl;
}

void printFilterPressed()
{
  clearScreen();
  printHeader();
  if(g_printLastMessage)
    {
      cout << g_lastMessage << endl << endl;
      g_printLastMessage = false;
    }
  printUsage();
  printFilterStatus();
}

void printMessage(string msg)
{
  if(g_allowMessages)
    {
      cout << "Message: " << msg << "." << endl;
    }
}

void printError(string error)
{
  cerr << "Error: " << error << "." << endl;
}

void printLastMessage(string str)
{
  g_lastMessage      = "Program notice: ";
  g_lastMessage      += str;
  g_printLastMessage = true;
}

bool saveImage(IplImage *img)
{
  ostringstream file(ostringstream::out);
  
#ifndef WINDOWS
  time_t rawtime;
  time(&rawtime);
  file << ctime(&rawtime) << g_suffix;
#endif // WINDOWS
#ifdef WINDOWS
  file << "img_" << (g_currImgNr++) << g_suffix;
#endif // WINDOWS
  if(cvSaveImage(file.str().c_str(), img))
    {
      return true;
    }
  return false;
}

/*************************************
 * filter related functions
 *************************************/

void resetAllFilters()
{
  g_blurr           = false;
  g_gray            = false;
  g_invert          = false;
  g_erode           = false;
  g_dilate          = false;
  g_floodFill       = false;
  g_swapRB          = false;
  g_rotate          = false;
  g_rotate1         = false;
  g_rotate5         = false;
  g_rotate10        = false;
  g_rotate45        = false;
  g_currAngle       = 0;
  g_manyImages      = false;
  g_fourImages      = false;
  g_sixteenImages   = false;
  g_fancy           = false;
  g_fancyFourImages = false;
  g_mirror          = false;
  g_cannyEdge       = false;
  g_zoomIn          = false;
  g_zoomOut         = false;
  g_scale           = 1.0;
}

IplImage* applyCurrentFilterOn(IplImage *img)
{
  IplImage *tmp1, *tmp2;
  tmp1 = cvCloneImage(img);
  if(g_swapRB)
    {
      tmp2 = tmp1;
      tmp1 = getSwappedRBImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_gray)
    {
      tmp2 = tmp1;
      tmp1 = getGrayedImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_blurr)
    {
      tmp2 = tmp1;
      tmp1 = getBlurredImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_fancy)
    {
      tmp2 = tmp1;
      tmp1 = getFancyImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_invert)
    {
      tmp2 = tmp1;
      tmp1 = getInvertedImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_erode)
    {
      tmp2 = tmp1;
      tmp1 = getErodedImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_dilate)
    {
      tmp2 = tmp1;
      tmp1 = getDilatedImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_cannyEdge)
    {
      tmp2 = tmp1;
      tmp1 = getCannyEdgedImage(tmp1, g_cannyEdgeLow, g_cannyEdgeHigh);
      cvReleaseImage(&tmp2);
    }
  if(g_floodFill)
    {
      tmp2 = tmp1;
      tmp1 = getFloodFilledImage(g_mouseMoved, tmp1, g_mousePoint);
      cvReleaseImage(&tmp2);
    }
  if(g_mirror)
    {
      tmp2 = tmp1;
      tmp1 = getMirroredImage(tmp1);
      cvReleaseImage(&tmp2);
    }
  if(g_manyImages)
    {
      tmp2 = tmp1;
      tmp1 = getManyImages(tmp1);
      cvReleaseImage(&tmp2);
    }

  if(g_zoomOut)
    {
      if((g_scale - 0.1) >= 0.1)
	{
	  g_scale -= 0.1;
	} else
	{
	  printLastMessage("Cannot zoom out any more, maximum reached.");
	}
      g_zoomOut = false;
    }
  if(g_zoomIn)
    {
      if((g_scale + 0.1) <= 10.0)
	{
	  g_scale += 0.1;
	} else
	{
	  printLastMessage("Cannot zoom in any more, maximum reached.");
	}
      g_zoomIn = false;
    }

  if(g_rotate || g_rotate1 || g_rotate5 || g_rotate10 || g_rotate45)
    {
      int anglePlusTmp = g_anglePlus;
      if(g_rotate1)
	{
	  anglePlusTmp = 1;
	} else if(g_rotate5)
	{
	  anglePlusTmp = 5;
	} else if(g_rotate10)
	{
	  anglePlusTmp = 10;
	} else if(g_rotate45)
	{
	  anglePlusTmp = 45;
	} else
	{
	  anglePlusTmp = g_anglePlus;
	}
      g_currAngle += anglePlusTmp;
      if(g_currAngle >= 360)
	{
	  g_currAngle = g_currAngle - 360;
	}
      g_rotate   = false;
      g_rotate1  = false;
      g_rotate5  = false;
      g_rotate10 = false;
      g_rotate45 = false;
    }
  tmp2 = tmp1;
  tmp1 = getRotatedImage(tmp1, g_currAngle, g_scale);
  cvReleaseImage(&tmp2);

  if(g_fancyFourImages)
    {
      IplImage *img0    = cvCloneImage(tmp1);
      IplImage *img0Tmp = img0;
      img0              = getFancyImage(img0);
      cvReleaseImage(&img0Tmp);
      img0Tmp           = img0;
      img0              = getInvertedImage(img0);
      cvReleaseImage(&img0Tmp);

      IplImage *img1    = cvCloneImage(tmp1);
      IplImage *img1Tmp = img1;
      img1              = getInvertedImage(img1);
      cvReleaseImage(&img1Tmp);
      img1Tmp           = img1;
      img1              = getFancyImage(img1);
      cvReleaseImage(&img1Tmp);

      IplImage *img2    = cvCloneImage(tmp1);
      IplImage *img2Tmp = img2;
      img2              = getFancyImage(img2);
      cvReleaseImage(&img2Tmp);

      IplImage *img3    = cvCloneImage(tmp1);
      IplImage *img3Tmp = img3;
      img3              = getInvertedImage(img3);
      cvReleaseImage(&img3Tmp);
      img3Tmp           = img3;
      img3              = getFancyImage(img3);
      cvReleaseImage(&img3Tmp);
      cvReleaseImage(&img3Tmp);
      img3Tmp           = img3;
      img3              = getSwappedRBImage(img3);
      cvReleaseImage(&img3Tmp);

      tmp2              = tmp1;
      tmp1              = getFourImages(img0, img1, img2, img3);

      cvReleaseImage(&img0);
      cvReleaseImage(&img1);
      cvReleaseImage(&img2);
      cvReleaseImage(&img3);

      if(tmp1 == 0)
	{
	  tmp1 = tmp2;
	} else
	{
	  cvReleaseImage(&tmp2);
	}
    } else if(g_fourImages)
    {
      tmp2 = tmp1;
      tmp1 = getFourImages(tmp1, tmp1, tmp1, tmp1);
      if(tmp1 == 0)
	{
	  tmp1 = tmp2;
	} else
	{
	  cvReleaseImage(&tmp2);
	}
    } else if(g_sixteenImages)
    {
      tmp2 = tmp1;
      tmp1 = getSixteenImages(tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1, tmp1);
      if(tmp1 == 0)
	{
	  tmp1 = tmp2;
	} else
	{
	  cvReleaseImage(&tmp2);
	}
    }

  return tmp1;
}

/*************************************
 * mouse & trackbar related functions
 *************************************/

void onMouseMove(int event, int x, int y, int flags, void *params)
{
  g_mouseMoved = true;
  if(g_floodFill)
    {
      g_mousePoint = cvPoint(x, y);
    }
}

void onTrackbar1Change(int pos)
{
  if(g_cannyEdge)
    {
      g_cannyEdgeLow = (pos <= 0) ? 1 : pos;
    }
}

void onTrackbar2Change(int pos)
{
  if(g_cannyEdge)
    {
      g_cannyEdgeHigh = (pos < g_cannyEdgeLow) ? (g_cannyEdgeLow + 1) : pos;
    }
}

/*************************************
 * main function + helper
 *************************************/

void checkFilterKeys(char k)
{
  if(k == g_str_startStopVideo)
    {
      if(g_enable_startStopVideo)
	{
	  g_saveVideo       = !g_saveVideo;
	  g_keypressed      = true;
	}
    }
  // no filters
  else if(k == g_str_none)
    {
      resetAllFilters();
      g_keypressed          = true;
    }
  // blurr filter
  else if(k == g_str_blurr)
    {
      if(g_enable_blurr)
	{
	  g_blurr           = !g_blurr;
	  g_keypressed      = true;
	}
    }
  // gray filter
  else if(k == g_str_gray)
    {
      if(g_enable_gray)
	{
	  g_gray            = !g_gray;
	  g_keypressed      = true;
	}
    }
  // invert filter
  else if(k == g_str_invert)
    {
      if(g_enable_invert)
	{
	  g_invert          = !g_invert;
	  g_keypressed      = true;
	}
    }
  // erode filter
  else if(k == g_str_erode)
    {
      if(g_enable_erode)
	{
	  g_erode           = !g_erode;
	  g_keypressed      = true;
	}
    }
  // dilate filter
  else if(k == g_str_dilate)
    {
      if(g_enable_dilate)
	{
	  g_dilate          = !g_dilate;
	  g_keypressed      = true;
	}
    }
  // fancy filter
  else if(k == g_str_fancy)
    {
      if(g_enable_fancy)
	{
	  g_fancy           = !g_fancy;
	  g_keypressed      = true;
	}
    }
  // swap red and blue filter
  else if(k == g_str_swaprb)
    {
      if(g_enable_swaprb)
	{
	  g_swapRB          = !g_swapRB;
	  g_keypressed      = true;
	}
    }
  // mirror filter
  else if(k == g_str_mirror)
    {
      if(g_enable_mirror)
	{
	  g_mirror          = !g_mirror;
	  g_keypressed      = true;
	}
    }
  // canny edge filter
  else if(k == g_str_cannyEdge)
    {
      if(g_enable_cannyEdge)
	{
	  g_cannyEdge       = !g_cannyEdge;
	  g_keypressed      = true;
	}
    }
  // rotate filter
  else if(k == g_str_rotate)
    {
      if(g_enable_rotate)
	{
	  g_rotate          = true;
	  g_keypressed      = true;
	}
    }
  // rotate 1 degrees filter
  else if(k == g_str_rotate1)
    {
      if(g_enable_rotate1)
	{
	  g_rotate1         = true;
	  g_keypressed      = true;
	}
    }
  // rotate 5 degree filter
  else if(k == g_str_rotate5)
    {
      if(g_enable_rotate5)
	{
	  g_rotate5         = true;
	  g_keypressed      = true;
	}
    }
  // rotate 10 degree filter
  else if(k == g_str_rotate10)
    {
      if(g_enable_rotate5)
	{
	  g_rotate10        = true;
	  g_keypressed      = true;
	}
    }
  // rotate 45 degree filter
  else if(k == g_str_rotate45)
    {
      if(g_enable_rotate45)
	{
	  g_rotate45        = true;
	  g_keypressed      = true;
	}
    }
  // many images filter
  else if(k == g_str_manyimg)
    {
      if(g_enable_manyImages)
	{
	  g_manyImages      = !g_manyImages;
	  g_keypressed      = true;
	}
    }
  // four images display
  else if(k == g_str_fourimg)
    {
      if(g_enable_fourImages)
	{
	  g_fancyFourImages = false;
	  g_sixteenImages   = false;
	  g_fourImages      = !g_fourImages;
	  g_keypressed      = true;
	}
    }
  // sixteen images display
  else if(k == g_str_sixteenimg)
    {
      if(g_enable_sixteenImages)
	{
	  g_fancyFourImages = false;
	  g_fourImages      = false;
	  g_sixteenImages   = !g_sixteenImages;
	  g_keypressed      = true;
	}
    }
  // fancy four images display
  else if(k == g_str_fancyfourimg)
    {
      if(g_enable_fancyFourImages)
	{
	  g_fourImages      = false;
	  g_sixteenImages   = false;
	  g_fancyFourImages = !g_fancyFourImages;
	  g_keypressed      = true;
	}
    }
  // flood fill filter
  else if(k == g_str_floodfill)
    {
      if(g_enable_floodFill)
	{
	  g_floodFill       = !g_floodFill;
	  g_keypressed      = true;
	}
    }
  // zoom in
  else if(k == g_str_zoomIn)
    {
      if(g_enable_zoomIn)
	{
	  g_zoomIn          = true;
	  g_keypressed      = true;
	}
    }
  // zoom out
  else if(k == g_str_zoomOut)
    {
      if(g_enable_zoomOut)
	{
	  g_zoomOut         = true;
	  g_keypressed      = true;
	}
    }
}

void setTrackbar1()
{

  cvCreateTrackbar(
		   g_trackbar1.c_str(),
		   g_mainWin.c_str(),
		   &g_trackbar1SwitchValue,
		   g_trackbar1MaxVal,
		   onTrackbar1Change
		   );
  cvSetTrackbarPos(
		   g_trackbar1.c_str(),
		   g_mainWin.c_str(),
		   g_cannyEdgeLow
		   );
}

void setTrackbar2()
{
  cvCreateTrackbar(
		   g_trackbar2.c_str(),
		   g_mainWin.c_str(),
		   &g_trackbar2SwitchValue,
		   g_trackbar2MaxVal,
		   onTrackbar2Change
		   );
  cvSetTrackbarPos(
		   g_trackbar2.c_str(),
		   g_mainWin.c_str(),
		   g_cannyEdgeHigh
		   );
}

/**
 * main
 * @param argc
 * @param argv
 * Usage: ./prog_name
 */
int main(int argc, const char *argv[])
{
  for(int i = 0; i < 10; ++i)
    {
      clearScreen();
    }
  printHeader();
  printUsage();
  printFilterStatus();
  
  g_continueLoop = true;
  
  CvCapture *cam = cvCreateCameraCapture(0);
  
  if(cam != 0)
    {
      printMessage("Camera detected");
#ifndef WINDOWS
      cvNamedWindow(g_mainWin.c_str(), CV_WINDOW_KEEPRATIO);
      cvResizeWindow(
		     g_mainWin.c_str(),
		     200,
		     100
		     );
#endif // WINDOWS
#ifdef WINDOWS
      cvNamedWindow(g_mainWin.c_str(), CV_WINDOW_NORMAL);
#endif // WINDOWS
      cvMoveWindow(g_mainWin.c_str(), g_initialWindowPosX, g_initialWindowPosY);

#ifdef WINDOWS
      setTrackbar1();
#endif // WINDOWS
      setTrackbar2();
#ifndef WINDOWS
      setTrackbar1();
#endif // WINDOWS

      // set mouse callback
      cvSetMouseCallback(g_mainWin.c_str(), onMouseMove, NULL);
      
      IplImage *frame;
      while(g_continueLoop)
	{
	  g_keypressed = false;
	  // get new frame
	  frame      = cvQueryFrame(cam);
	  if(!frame)
	    {
	      printError("No frame to display, quitting ..");
	      break;
	    }

	  char k = cvWaitKey(g_waitMs);
	  // check whether to quit
	  if(k == 27)
	    {
	      printMessage("Quit");
	      g_continueLoop        = false;
	    }
	  // check whether to save image
	  else if(k == g_str_save)
	    {
	      IplImage *img         = applyCurrentFilterOn(frame);
	      if(saveImage(img))
		{
		  printLastMessage("Successfully saved a snapshot.");
		}
	      else
		{
		  printLastMessage("Could not save image.");
		}
	      cvReleaseImage(&img);
	      g_printLastMessage    = true;
	      g_keypressed          = true;
	    }
	  // check all filters
	  checkFilterKeys(k);
	
	  // update trackbar positions
	  cvSetTrackbarPos(
			   g_trackbar1.c_str(),
			   g_mainWin.c_str(),
			   g_cannyEdgeLow
			   );
	  onTrackbar2Change(g_cannyEdgeHigh);
	  cvSetTrackbarPos(
			   g_trackbar2.c_str(),
			   g_mainWin.c_str(),
			   g_cannyEdgeHigh
			   );

	  // apply selected filter to frame
	  IplImage *img             = applyCurrentFilterOn(frame);
	  cvShowImage(g_mainWin.c_str(), img);

	  if(g_saveVideo)
	    {
	      if(g_saveVideoFirstTime)
		{
		  ostringstream file(ostringstream::out);
#ifndef WINDOWS
		  time_t rawtime;
		  time(&rawtime);
		  file << ctime(&rawtime) << g_videoSuffix;
#endif // WINDOWS
#ifdef WINDOWS
		  file << "video_" << (g_currVidNr++) << g_videoSuffix;
#endif // WINDOWS
		  g_videoFileName = file.str();
		  CvSize size = cvSize(
				       (int)cvGetCaptureProperty(cam, CV_CAP_PROP_FRAME_WIDTH),
				       (int)cvGetCaptureProperty(cam, CV_CAP_PROP_FRAME_HEIGHT)
				       );
#ifndef WINDOWS
		  writer = cvCreateVideoWriter(
					       g_videoFileName.c_str(),
					       CV_FOURCC('M', 'P', '4', 'V'),
					       // CV_FOURCC('X', 'V', 'I', 'D'),
					       g_videoFPS,
					       size
					       );
#endif // WINDOWS
#ifdef WINDOWS
		  writer = cvCreateVideoWriter(
					       g_videoFileName.c_str(),
					       CV_FOURCC_PROMPT,
					       g_videoFPS,
					       size
					       );
#endif // WINDOWS
		  g_saveVideoFirstTime = false;
		}
	      
	      IplImage *tmp;
	      // double swap rb to record correctly
	      tmp = img;
	      img = getSwappedRBImage(img);
	      cvReleaseImage(&tmp);
	      tmp = img;
	      img = getSwappedRBImage(img);
	      cvReleaseImage(&tmp);
	      cvWriteFrame(writer, img);

	      ostringstream msg(ostringstream::out);
	      msg << "Currently recording to file '" << g_videoFileName << "'.";
	      printLastMessage(msg.str());
	    } else
	    {
	      if(!g_saveVideoFirstTime)
		{
		  cvReleaseVideoWriter(&writer);
		  g_saveVideoFirstTime = true;
		  g_printLastMessage = false;
		}
	    }

	  cvReleaseImage(&img);
	  
	  // update filter restrictions
	  if(g_keypressed)
	    {
	      updateRestrictions();
	      printFilterPressed();
	    }
	}
      cvReleaseCapture(&cam);
      cvDestroyWindow(g_mainWin.c_str());
    }
  else
    {
      cerr << "Error: No camera detected." << endl;
    }
  
  if(!g_saveVideoFirstTime)
    {
      cvReleaseVideoWriter(&writer);
      g_saveVideoFirstTime = true;
    }
  
  cout << endl;
  
  return EXIT_SUCCESS;
}
