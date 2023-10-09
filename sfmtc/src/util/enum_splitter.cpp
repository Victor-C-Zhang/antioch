#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: enum_splitter [full qualified path]" << std::endl;
    return 1;
  }
  std::ifstream t(argv[1]);
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);

  int n = buffer.size();
  int i = 0;
  vector<string> e;
  vector<string> names;
  while (1) {
    i = buffer.find("\"Id\": \"", i + 1);
    if (i >= n || i == string::npos) break;
    string s;
    int j = i + 7;
    while (buffer[j] != '"') {
      s += buffer[j];
      ++j;
    }
    e.push_back(s);

    i = buffer.find("\"Name\": \"", i + 1);
    if (i >= n || i == string::npos) break;
    string t;
    int k = i + 9;
    while (buffer[k] != '"') {
      t += buffer[k];
      ++k;
    }
    names.push_back(t);
  }
  for (int i = 0; i < e.size(); ++i) {
    auto s = e[i];
    string id = s;
    if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
    std::cout << id << " = " << i + 1 << ", // " << s << std::endl;
  }

  cout << "###########################################" << endl;
  cout << "###########################################" << endl;
  cout << "###########################################" << endl;

  for (int i = 0; i < e.size(); ++i) {
    auto s = e[i];
    string id = s;
    if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
    std::cout << "case " << id << ":\n  return \"" << names[i] << "\";" << std::endl;
  }

  cout << "###########################################" << endl;
  cout << "###########################################" << endl;
  cout << "###########################################" << endl;

  for (int i = 0; i < e.size(); ++i) {
    auto s = e[i];
    string id = s;
    if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
    std::cout << "case " << id << ":\n  return \"" << s << "\";" << std::endl;
  }

  cout << "###########################################" << endl;
  cout << "###########################################" << endl;
  cout << "###########################################" << endl;

  for (int i = 0; i < e.size(); ++i) {
    auto s = e[i];
    string id = s;
    if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
    std::cout << "if (name == \"" << s << "\") {\n  return " << id << ";\n}" << std::endl;
  }

  cout << "###########################################" << endl;
  cout << "###########################################" << endl;
  cout << "###########################################" << endl;

  for (int i = 0; i < e.size(); ++i) {
    auto s = e[i];
    string id = s;
    if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
    std::cout << "{\"" << s << "\", " << id << "}," << std::endl;
  }
}
