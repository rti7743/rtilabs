#include <iostream>
#include <string>
#include <stdlib.h>
#include "oll.hpp"

static const char* OLL_PROGNAME = "oll_train";

void usage(){
  fprintf(stderr, 
	  "%s (P|AP|PA|PA1|PA2|CW) trainfile modelfile [devfile] -C=FLOAT -b=FLOAT -I=INT\n"
	  "P   : Perceptron\n"
	  "AP  : Averaged Perceptron\n"
	  "PA  : Passive Agressive\n"
	  "PA1 : Passive Agressive I\n"
	  "PA2 : Passive Agressive II\n"
	  "PAK : Kernelized Passive Agressive\n"
	  "CW  : Confidence Weighted\n"
	  "AL  : ALMA HD (heavy)\n"
	  "-C    Regularization Paramter (Default 1.0)\n"
	  "-b    Bias (Default 0.0)\n"
	  "-I    Iteration (Default 10, 0 = one pass without storing)\n"
	  , OLL_PROGNAME);
}

void parseARGV(const char* s, float& C, float& bias, int& iter, std::string& devFileName){
  std::string str(s);
  if (str.size() >= 3){
    if (str.substr(0, 3) == "-C="){
      C = atof(str.substr(3).c_str());    
      return;
    } else if (str.substr(0, 3) == "-b="){
      bias = atof(str.substr(3).c_str());
      return;
    } else if (str.substr(0, 3) == "-I="){
      iter = atoi(str.substr(3).c_str());
      return;
    }
  }
  devFileName = s;
}

int main(int argc, char* argv[]){
  if (argc < 4){
    usage();
    return -1;
  }

  // argv[2] = trainfile
  // argv[3] = modelfile

  float C    = 1.0f; // regularization parameter
  float bias = 0.f;  // bias
  int   iter = 0;    // iteration
  std::string devFileName;
  for (int i = 4; i < argc; i++){
    parseARGV(argv[i], C, bias, iter, devFileName);
  }

  oll_tool::trainMethod tm = oll_tool::P;
  std::string argv1(argv[1]);
  if      (argv1 == "P" )  {tm = oll_tool::P;}
  else if (argv1 == "AP")  {tm = oll_tool::AP;}
  else if (argv1 == "PA")  {tm = oll_tool::PA;}
  else if (argv1 == "PA1") {tm = oll_tool::PA1;}
  else if (argv1 == "PA2") {tm = oll_tool::PA2;}
  else if (argv1 == "PAK") {tm = oll_tool::PAK;}
  else if (argv1 == "CW")  {tm = oll_tool::CW;}
  else if (argv1 == "AL")  {tm = oll_tool::AL;}
  else {
    usage();
    return -1;
  }
  
  if (oll_tool::trainFile(argv[2], argv[3], tm, C, bias, iter, true) == -1){
    return -1;
  }
  
  if (devFileName != ""){
    std::vector<int> confMat;
    if (oll_tool::testFile(devFileName.c_str(), argv[3], confMat, true) == -1){
      return -1;
    }
  }

  return 0;
}
