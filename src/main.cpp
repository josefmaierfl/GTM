#include "argvparser.h"
#include "loadGTMfiles.h"

using namespace std;
using namespace CommandLineProcessing;


void SetupCommandlineParser(ArgvParser &cmd, int argc, char *argv[])
{
  cmd.setIntroductoryDescription("Program for reading and showing Ground Truth Matches (GTM).\n Example of usage:\n  "
                                     "loadGTMfiles "
                                     "--img_path=C:/your_img_path "
                                     "--gt_path=C:/your_gtm_path "
                                     "--l_img_pref=prefix_and_or_postfix_of_left_imgs_ "
                                     "--r_img_pref=prefix_and_or_postfix_of_right_imgs_ "
                                     "--gtm_postf=prefix_and_or_postfix_of_GTM");
  //define error codes
  cmd.addErrorCode(0, "Success");
  cmd.addErrorCode(1, "Error");

  cmd.setHelpOption("h", "help", "");
  cmd.defineOption("img_path", "<Path to the images>", ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
  cmd.defineOption("gtm_path",
                   "<Specifies the path to the GTM files.>"
                       "It must contain all GTM files that correspond to all image pairs specified by img_path, l_img_pref, and r_img_pref.>",
                   ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
  cmd.defineOption("l_img_pref",
                   "<Prefix and/or postfix for the left or first images.\n "
                       "It can include a folder structure that follows after the filepath, a file prefix, a '*' indicating the position of the number and a postfix. "
                       "If it is empty, all files from the folder img_path are used (also if l_img_pref only contains a folder ending with '/', every file within this folder is used). "
                       "It is possible to specify only a prefix with or without '*' at the end. "
                       "If a prefix is used, all characters until the first number (excluding) must be provided. "
                       "For a postfix, '*' must be placed before the postfix.\n "
                       "Valid examples: folder/pre_*post, *post, pre_*, pre_, folder/*post, folder/pre_*, folder/pre_, folder/, folder/folder/, folder/folder/pre_*post, ...\n "
                       "For non stereo images (consecutive images), r_img_pref must be empty.>",
                   ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
  cmd.defineOption("r_img_pref", "<Prefix and/or postfix for the right or second images.\n "
      "For non stereo images (consecutive images), r_img_pref must be empty.\n "
      "For further details see the description of l_img_pref.>", ArgvParser::OptionRequiresValue);
  cmd.defineOption("gtm_postf",
                   "<Specifies the postfix of GTM files.\n "
                       "It must include the intended inlier ratio (10 * inlier ratio in percent) and keypoint type. "
                       "E.g. 'inlRat950FAST.gtm'. Specifying an additional folder is also possible: e.g. 'folder/*inlRat950FAST.gtm'>",
                   ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
  // finally parse and handle return codes (display help etc...)
  int result = -1;
  result = cmd.parse(argc, argv);
  if(result != ArgvParser::NoParserError)
  {
    cout << cmd.parseErrorDescription(result);
  }
}

void startEvaluation(ArgvParser &cmd)
{

  string gtm_path, gtm_postf, img_path, l_img_pref, r_img_pref;

  if(cmd.foundOption("img_path"))
    img_path   = cmd.optionValue("img_path");
  if(cmd.foundOption("l_img_pref"))
    l_img_pref = cmd.optionValue("l_img_pref");
  if(cmd.foundOption("r_img_pref"))
    r_img_pref = cmd.optionValue("r_img_pref");
  if(cmd.foundOption("gtm_path"))
    gtm_path   = cmd.optionValue("gtm_path");
  if(cmd.foundOption("gtm_postf"))
    gtm_postf  = cmd.optionValue("gtm_postf");


  if(gtm_path.empty() || gtm_postf.empty() || img_path.empty() || l_img_pref.empty())
  {
    cout << "Parameters for showing GTM missing! Exiting." << endl;
    exit(1);
  }
  else
  {
    showGTM(img_path, l_img_pref, r_img_pref, gtm_path, gtm_postf);
  }
}

/** @function main */
int main(int argc, char *argv[])
{
  ArgvParser cmd;
  SetupCommandlineParser(cmd, argc, argv);
  startEvaluation(cmd);

  return 0;
}
