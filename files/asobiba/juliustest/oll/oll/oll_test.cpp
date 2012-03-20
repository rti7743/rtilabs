#include <vector>
#include <string>
#include "oll.hpp"

static const char* PROGNAME = "oll_test";

void usage(){
  fprintf(stderr, "%s testfile modelfile [-v]\n", PROGNAME);
}

int main(int argc, char* argv[]){
  if (argc < 3){
    usage();
    return -1;
  }

  int verb = 1;
  std::vector<std::string> fileNames;
  for (size_t i = 1; i < argc; i++){
    if (argv[i][0] == '-'){
      std::string argv_s(argv[i]);
      if (argv_s == "-v"){
	verb = 2;
      } else {
	std::cerr << "unknown option " << argv[i] << std::endl;
	usage();
	return -1;
      }
    } else {
      fileNames.push_back(argv[i]);
    }
  }

  if (fileNames.size() != 2){
    usage();
    return -1;
  }
  
  std::vector<int> confMat;
  if (oll_tool::testFile(fileNames[0].c_str(), fileNames[1].c_str(), confMat, verb) == -1){
    return -1;
  }
 
  return 0;
}
