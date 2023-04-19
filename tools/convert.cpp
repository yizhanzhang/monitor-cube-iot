/**
 * 将图片转换为hexcode头文件
*/
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <string>

using namespace std;

void convertToByteCode(string filepath) {
  struct stat pstat;
  stat(filepath.c_str(), &pstat);
  int size = pstat.st_size;

  FILE *fp = fopen(filepath.c_str(), "rb");
  uint8_t data[size];
  fread(data, sizeof(uint8_t), size, fp);
  fclose(fp);

  string outFilepath = filepath + ".h";
  FILE *ofp = fopen(outFilepath.c_str(), "wb");
  string output = "const uint8_t OUTPUT[] = {\n";
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
  if (argc < 2) return 0;
  string filepath = argv[1];
  convertToByteCode(filepath);
  return 0;
}
