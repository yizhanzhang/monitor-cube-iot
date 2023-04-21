/**
 * 将图片转换为hexcode头文件
*/
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <string>

using namespace std;

string findJPGFilename(string filepath) {
  int slashPos = filepath.rfind('/');
  string fullFilename = filepath.substr(slashPos + 1);

  int extPos = fullFilename.find(".jpg") > 0 ? fullFilename.find(".jpg") : fullFilename.find(".jpeg");
  if (extPos == -1) return "";

  string realFilename = fullFilename.substr(0, extPos);
  return realFilename;
}

void convertToByteCode(string filepath) {
  // check filename
  string realFilename = findJPGFilename(filepath);
  if (realFilename == "") {
    cout << "[ERROR]: file name is invalid, must end with .jpg or .jpeg" << endl;
    return;
  };

  // check file stat
  struct stat pstat;
  int status = stat(filepath.c_str(), &pstat);
  if (status == -1) {
    cout << "[ERROR]: file stat is invalid, please check filepath" << endl;
    return;
  }

  // get image data
  int size = pstat.st_size;
  FILE *fp = fopen(filepath.c_str(), "rb");
  uint8_t data[size];
  fread(data, sizeof(uint8_t), size, fp);
  fclose(fp);

  // write header file
  string outFilepath = filepath + ".h";
  FILE *ofp = fopen(outFilepath.c_str(), "wb");
  string output = "#include <pgmspace.h>\nconst uint8_t output_" + realFilename + "[] = {\n";
  for (int i = 0; i < size; i++) {
    std::ostringstream hex;
    hex << std::hex << (int)data[i];
    string hexStr = hex.str();
    hexStr = hexStr.length() == 1 ? '0' + hexStr : hexStr;
    output += "0x" + hexStr;
    if (i < size - 1) {
      output += ", ";
    }
  }
  output += "\n};";
  fwrite(output.c_str(), 1, output.length(), ofp);
  fclose(ofp);

  return void();
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "[ERROR]: no file path" << endl;
    return 0;
  };
  string filepath = argv[1];
  convertToByteCode(filepath);
  return 0;
}
