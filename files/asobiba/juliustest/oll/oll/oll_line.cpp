#include <iostream>
#include <string>
#include <stdlib.h>
#include "oll.hpp"

const char* OLL_PROGNAME = "oll_online";

void usage(){
  fprintf(stderr, 
	  "%s (P|AP|PA|PA1|PA2|CW) -C=FLOAT -b=FLOAT -I=INT\n"
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
	  "\nStdin Format\n"
	  "----------------------------\n"
	  "A (1|-1) id:val id:val ... (Add training example)\n"
	  "T (1|-1) id:val id:val ... (Test Example)\n"
	  "S modelfile (Save current model to modelfile)\n"
	  "L modelfile (Load current model to modelfile)\n"
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

template<class T>
int processLine(oll_tool::oll& ol, const std::string& line){
  if (line == "EOF"){
    return 1;
  }
  
  if (line == ""){
    std::cerr << "WARNING:empty line" << std::endl;
    return 0;
  }
  
  if (line[0] == 'S'){
    if (ol.save(line.substr(2).c_str()) == -1){
      std::cerr << ol.getErrorLog() << std::endl;
      return -1;
    }
    return 0;
  } else if (line[0] == 'L'){
    if (ol.load(line.substr(2).c_str()) == -1){
      std::cerr << ol.getErrorLog() << std::endl;
      return -1;
    }
    return 0;
  }

  if (line[0] != 'A' &&
      line[0] != 'T'){
    std::cerr << "WARNING: invalid line: [" << line << "]" << std::endl;
    return 0;
  }

  oll_tool::fv_t fv;
  int y = 0;

  if (ol.parseLine(std::string(line.substr(1)), fv, y) == -1){
    std::cerr << ol.getErrorLog() << std::endl;
    return -1;
  }

  if (line[0] == 'A'){
    T a;
    ol.trainExample(a, fv, y);
  } else if (line[0] == 'T'){
    std::cout << ol.classify(fv) << std::endl;
  } else {
    std::cerr << "Unknown operator: " << line.c_str() << std::endl;
    return -1;
  }
  return 0;
}

int main(int argc, char* argv[]){
  if (argc < 2){
    usage();
    return -1;
  }
  oll_tool::trainMethod tm = oll_tool::PA;
  float C = 1.f;
  float bias = 0.f;
  for (int i = 1; i < argc; i++){
    if (argv[i][0] == '-'){
      std::string argv_s(argv[i]);
      if (argv_s.size() >= 3){
	if (argv_s.substr(0, 3) == "-C="){
	  C = atof(argv_s.substr(3).c_str());
	} else if (argv_s.substr(0, 3) == "-b=") {
	  bias = atof(argv_s.substr(3).c_str());
	} else {
	  usage();
	  return -1;
	}
      } else {
	usage();
	return -1;
      }
    } else {
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
    }
  }

  oll_tool::oll ol;
  ol.setC(C);
  ol.setBias(bias);

  std::string line;

  switch(tm){
  case oll_tool::P :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::P_s>(ol, line) != 0) break;
    }
    break;
  case oll_tool::AP :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::AP_s>(ol, line) != 0) break;
    }
    break;
  case oll_tool::PA :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::PA_s>(ol, line) != 0) break;
    }
    break;
  case oll_tool::PA1 :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::PA1_s>(ol, line) != 0) break;
    }
    break;
  case oll_tool::PA2 :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::PA2_s>(ol, line) != 0) break;
    }
    break;
  case oll_tool::PAK :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::PAK_s>(ol, line) != 0) break;
    }
    break;
  case oll_tool::CW :
    while (getline(std::cin, line)){
      if (processLine<oll_tool::CW_s>(ol, line) != 0) break;
    }
    break;
  default:
    // should not come here
    std::cerr << "unkwnon training method" << std::endl;
    return -1;
  }

  return 0;
}
