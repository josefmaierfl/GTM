# GTM
Interface for Ground Truth Matches (GTM) to test vision based algorithms.

## Description
Feature matching quality strongly influences the accuracy of most computer vision tasks. This led to impressive advances in keypoint detection, descriptor calculation, and feature matching itself. To compare different approaches and evaluate their quality, datasets from related tasks are used (KITTI, Oxford (Mikolajczyk et al.), HCI Training 1K, ...). Unfortunately, none of these datasets actually provide ground truth (GT) feature matches. Thus, matches can only be approximated due to repeatability errors of keypoint detectors and inaccuracies of GT. Here, we introduce ground truth matches (GTM) for several well known datasets. Based on the provided spacial ground truth, we automatically generated them using popular feature types. 

Further information can be found in our paper: [1](https://github.com/josefmaierfl/GTM/blob/master/README.md#citation)

Currently, we provide GTM for 
* KITTI 2015 flow & disparity (http://www.cvlibs.net/datasets/kitti/)
* Oxford (Mikolajczyk et al.) sequences bark, bikes, boat, graffiti, JPEG, light, and wall (http://www.robots.ox.ac.uk/~vgg/research/affine/)

The GTM of the above mentioned datasets are available for the following keypoint types:
* AKAZE
* KAZE
* BRISK
* FAST
* MSD
* ORB
* SIFT

For each dataset-keypoint combination, GTMs are available for various inlier ratios ranging from 1% to 100% (1, 2, ..., 10, 12, ..., 20, 25, ..., 100). For testing without a specific inlier ratio, data with the maximum possible (depends on the images and keypoint type) number of true feature matches and true negatives (keypoints without a match in the other image) is also available.

The provided code provides functionalities to read and display the Ground Truth Matches. After reading, the information of the GTM-files is stored in the OpenCV format for matches and keypoints (cv::DMatch and cv::KeyPoint). Moreover, the following information is available for each corresponding image pair:
* Left inlier mask (`std::vector<bool>`)
* Matches (`std::vector <cv::DMatch>`)
* Left keypoints (`std::vector <cv::KeyPoint>`)
* Right keypoints (`std::vector <cv::KeyPoint>`)
* Left inlier ratio (`double`)
* Right inlier ratio (`double`)
* Average inlier ratio of both images (`double`)
* Number of true positive matches (`double`)
* Number of true negatives in the left image (`double`)
* Number of true negatives in the right image (`double`)
* Estimated threshold for generating GTM (`double`)

### Naming Convention for GTM Files
Using a specific inlier ratio:

`[dataset image 1]_[dataset image 2]_inlRat[inlier ratio in percent multiplied by 10][keypoint type].gtm`

Without a specific inlier ratio: 

`[dataset image 1]_[dataset image 2]_inlRatInitial[keypoint type].gtm`

### GTM Download
http://www.vitro-testing.com/test-data/gtm

## Using the Code
The project provides functionalities to read and display the GTM of multiple corresponding images and GTM-files. By providing the path to the images and GTM files in addition to the prefix and/or postfix of the image names and GTM file names, the GTM of all image pairs within the specified folders are displayed.

To show the different options for reading and displaying GTM, use the option `-h` in cunjuction with the compiled executable.

For testing, this repository includes a few sample image pairs from KITTI and Oxford datasets with their corresponding GTM in the folder [data](data). For testing, use one of the following command arguments:
* KITTI disparity image pairs with GTM, AKAZE keypoints, and an inlier ratio of 75%:  
`--img_path=YOURPATH/data/KITTI/imgs/ --l_img_pref=/image_2/*_10 --r_img_pref=/image_3/*_10 --gtm_path=YOURPATH/data/KITTI/GTMs/disp --gtm_postf=/*750AKAZE`
* KITTI flow image pairs with GTM, AKAZE keypoints, and an inlier ratio of 75%:  
`--img_path=YOURPATH/data/KITTI/imgs/image_2/ --l_img_pref=*_10 --r_img_pref=*_11 --gtm_path=YOURPATH/data/KITTI/GTMs/flow --gtm_postf=/*750AKAZE`
* Oxford wall image pair with GTM, FAST keypoints, and an inlier ratio of 50%:  
`--img_path=YOURPATH/data/Oxford-wall/imgs --l_img_pref=img1 --r_img_pref=img2 --gtm_path=YOURPATH/data/Oxford-wall/GTMs --gtm_postf=img1_img2_inlRat500FAST`

**If only the core functionality of reading GTM files is desired, the header file [include/readGTM.h](include/readGTM.h) can be included in your own code.**

## Platform
This cmake project was tested under Linux and Windows (Visual Studio 2015)

## Dependencies
OpenCV 3.2

## Citation
If you use our test data please cite: 

[1]: J. Maier, M. Humenberger, O. Zendel, M. Vincze. Ground Truth Accuracy and Performance of the Matching Pipeline. In IEEE Conference on Computer Vision and Pattern Recognition Workshops (CVPR Workshops), 2017
