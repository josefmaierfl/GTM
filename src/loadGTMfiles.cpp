/**********************************************************************************************************
FILE: loadGTMfiles.cpp

PLATFORM: Windows 7, MS Visual Studio 2010, OpenCV 2.4.2

CODE: C++

AUTOR: Josef Maier, AIT Austrian Institute of Technology

DATE: May 2017

LOCATION: TechGate Vienna, Donau-City-Stra�e 1, 1220 Vienna

VERSION: 1.0

DISCRIPTION: This file provides functionalities for loading and showing the GTMs.
**********************************************************************************************************/

#include "loadGTMfiles.h"
#include "io_helper.h"
#include "readGTM.h"

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//Show a fraction of the matches
int showMatches(std::vector <cv::DMatch> matches,
                std::vector <cv::KeyPoint> keypL,
                std::vector <cv::KeyPoint> keypR,
                cv::Mat imgs[2],
                size_t keepNMatches = 20);

/* Reads all images and GTMs and shows the matches of each image pair in the given folder
*
* string img_path    Input  -> Path to the images
* string l_img_pref	 Input  -> Prefix and/or postfix with optional subfolder for the left/first images.
*									   It can include a folder structure that follows after the filepath, a file prefix,
*									   a '*' indicating the position of the number and a postfix. If it is empty,
*									   all files from the folder img_path are used (also if l_img_pref only contains a
*									   folder ending with '/', every file within this folder is used). It is possible to
*									   specify only a prefix with or without '*' at the end. If a prefix is used, all
*									   characters until the first number (excluding) must be provided. For a
*									   postfix, '*' must be placed before the postfix. Valid examples :
*									   folder/pre_*post, *post, pre_*, pre_, folder/*post, folder/pre_*, folder/pre_,
*									   folder/, folder/folder/, folder/folder/pre_*post, ...
*									   For non stereo images (consecutive images), r_img_pref must be empty.
* string r_img_pref	 Input  -> Prefix and/or postfix with optional subfolder for the right/second images.
*									   For non stereo images (consecutive images), r_img_pref must be empty.
*									   For further details see the description of l_img_pref.
* string gtm_path		 Input  -> Path to the ground truth images. It must contain all GTM files that correspond to all
*									   image pairs specified by img_path, l_img_pref, and r_img_pref.
* string gtm_postfix Input  -> Specifies the postfix of GTM files. It must include the intended inlier
*									   ratio (10 * inlier ratio in percent) and keypoint type. E.g. 'inlRat950FAST.gtm'.
* 									 Specifying an additional folder is also possible: e.g. 'folder/*inlRat950FAST.gtm'
*
* Return value:      0:  Everything ok
*							      -1:  Error while reading GTM file
*/
int showGTM(std::string img_path, std::string l_img_pref, std::string r_img_pref,
            std::string gtm_path, std::string gtm_postfix)
{
  int             err;
  cv::Mat         src[2];
  string          fileprefl, fileprefr;
  vector <string> filenamesl, filenamesr, filenamesgtm;

  fileprefl   = l_img_pref;
  if(r_img_pref.empty())
    fileprefr = fileprefl;
  else
    fileprefr = r_img_pref;

  //Load corresponding image names
  err = loadImgStereoSequence(img_path, fileprefl, fileprefr, filenamesl, filenamesr);
  if(err || filenamesl.empty() || filenamesr.empty() || (filenamesl.size() != filenamesr.size()))
  {
    cout << "Could not find flow images! Exiting." << endl;
    exit(0);
  }

  //load GTM names
  err = loadGTMSequence(gtm_path, gtm_postfix, filenamesgtm);
  if(err || filenamesgtm.empty() || (filenamesgtm.size() != filenamesl.size()))
  {
    cout << "Could not find GTM files! Exiting." << endl;
    exit(0);
  }

  // read images and GTMs
  err                          = 0;
  std::vector<bool>          leftInlier;
  std::vector <cv::DMatch>   matchesGT;
  std::vector <cv::KeyPoint> keypL;
  std::vector <cv::KeyPoint> keypR;
  double                     inlRatioL, inlRatioR, inlRatioO, positivesGT, negativesGTl, negativesGTr, usedMatchTH;
  for(int                    k = 0; k < (int) filenamesl.size(); k++)
  {
    cv::Mat flowimg;
    src[0] = cv::imread(filenamesl[k]);
    src[1] = cv::imread(filenamesr[k]);
    if(readGTMatchesDisk(filenamesgtm[k],
                         leftInlier,
                         matchesGT,
                         keypL,
                         keypR,
                         &inlRatioL,
                         &inlRatioR,
                         &inlRatioO,
                         &positivesGT,
                         &negativesGTl,
                         &negativesGTr,
                         &usedMatchTH))
    {
      cout << "Succesfully read GTM file " << filenamesgtm[k] << endl;
      cout << "Inlier ratio in first/left image: " << inlRatioL << endl;
      cout << "Inlier ratio in second/right image: " << inlRatioR << endl;
      cout << "Mean inlier ratio of both images: " << inlRatioO << endl;
      cout << "Number of true positive matches: " << positivesGT << endl;
      cout << "Number of left negatives (having no corresponding right match): " << negativesGTl << endl;
      cout << "Number of right negatives (having no corresponding left match): " << negativesGTr << endl;
      cout << "Threshold used to generate GTM: " << usedMatchTH << endl << endl;
      showMatches(matchesGT, keypL, keypR, src, 20);
    }
    else
    {
      cout << "Error while reading GTM file " << filenamesgtm[k] << endl << endl;
      err = -1;
    }
  }

  return err;
}

/* Shows a fraction of the matches.
*
* vector<DMatch> matches	Input  -> Matches
* vector<KeyPoint> keypL	Input  -> left/first keypoints
* vector<KeyPoint> keypR	Input  -> right/second keypoints
* Mat imgs[2]				Input  -> Images used for matching [0] = left/first image, [1] = right/second image
* size_t keepNMatches		Input  -> Number of matches to display [Default = 20]
*
* Return value:				 0:		  Everything ok
*/
int showMatches(std::vector <cv::DMatch> matches,
                std::vector <cv::KeyPoint> keypL,
                std::vector <cv::KeyPoint> keypR,
                cv::Mat imgs[2],
                size_t keepNMatches)
{
  vector<char> matchesMask(matches.size(), false);;
  Mat   img_correctMatches;
  float keepXthMatch;
  float oldremainder, newremainder = 0;

  //Reduce number of displayed matches
  keepXthMatch   = 1.0f;
  if(matches.size() > keepNMatches)
    keepXthMatch = (float) matches.size() / (float) keepNMatches;
  oldremainder   = 0;
  for(unsigned int i = 0; i < matches.size(); i++)
  {
    newremainder = fmod((float) i, keepXthMatch);
    if(oldremainder >= newremainder)
    {
      matchesMask[i] = true;
    }
    oldremainder = newremainder;
  }
  //Draw true positive matches
  drawMatches(imgs[0],
              keypL,
              imgs[1],
              keypR,
              matches,
              img_correctMatches,
              Scalar::all(-1),
              Scalar::all(-1),
              matchesMask,
              cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

  //Show result
  cvNamedWindow("Ground Truth Matches");
  imshow("Ground Truth Matches", img_correctMatches);
  waitKey(0);
  cv::destroyWindow("Ground Truth Matches");

  return 0;
}
