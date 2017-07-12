# GTM
Interface for Ground Truth Matches (GTM) to test vision based algorithms.

## Description
Feature matching quality strongly influences the accuracy of most computer vision tasks. This led to impressive advances in keypoint detection, descriptor calculation, and feature matching itself. To compare different approaches and evaluate their quality, datasets from related tasks are used (KITTI, Oxford (Mikolajczyk et al.), HCI Training 1K, ...). Unfortunately, none of these datasets actually provide ground truth (GT) feature matches. Thus, matches can only be approximated due to repeatability errors of keypoint detectors and inaccuracies of GT. Here, we introduce ground truth matches (GTM) for several well known datasets. Based on the provided spacial ground truth, we automatically generated them using popular feature types. 

Currently, we provide GTM for 
..* KITTI 2015 flow & disparity (http://www.cvlibs.net/datasets/kitti/)
..* Oxford (Mikolajczyk et al.) sequences bark, bikes, boat, graffiti, JPEG, light, and wall (http://www.robots.ox.ac.uk/~vgg/research/affine/)

The GTM of the above mentioned datasets are available for the following keypoint types:
..* AKAZE
..* KAZE
..* BRISK
..* FAST
..* MSD
..* ORB
..* SIFT

For each dataset-keypoint combination, GTMs are available for various inlier ratios ranging from 1% to 100% (1, 2, ..., 10, 12, ..., 20, 25, ..., 100). For testing without a specific inlier ratio, data with the maximum possible (depends on the images and keypoint type) number of true feature matches and true negatives (keypoints without a match in the other image) is also available.

### Naming Convention for GTM Files
Using a specific inlier ratio:
`[dataset image 1]_[dataset imgage 2]_inlRat[inlier ratio in percent multiplied by 10][keypoint type].gtm`
Without a specific inlier ratio:
`[dataset image 1]_[dataset imgage 2]_inlRatInitial[keypoint type].gtm`

### GTM Download
http://www.vitro-testing.com/test-data/gtm



## Platform
The project was tested under Linux and Windows (Visual Studio 2015)

## Dependencies
OpenCV 3.2

## Citation
If you use our test data please cite:
J. Maier, M. Humenberger, O. Zendel, M. Vincze. Ground Truth Accuracy and Performance of the Matching Pipeline. In IEEE Conference on Computer Vision and Pattern Recognition Workshops (CVPR Workshops), 2017
