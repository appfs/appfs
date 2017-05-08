#include <iostream>
#include <vector>
int main(){
  std::vector<std::string> ps;
  std::string trigger;
  ps.push_back("#include <iostream>");
  ps.push_back("#include <vector>");
  ps.push_back("int main(){");
  ps.push_back("  std::vector<std::string> ps;");
  ps.push_back("  std::string trigger;");
  ps.push_back("  for(int i=0; i<ps.size(); i++){");
  ps.push_back("  std::cout << ps[i] << std::endl;");
  ps.push_back("    if (ps[i]==\"  std::string trigger;\"){");
  ps.push_back("      for(int j=0; j<ps.size(); j++){");
  ps.push_back("	std::cout<<\"ps.push_back(\"<<ps[j]<<\");\"<<std::endl;}}}");
  ps.push_back("return 0;}");
  for(int i=0; i<ps.size(); i++){
    std::cout << ps[i] << std::endl;
    if (ps[i]=="  std::string trigger;"){
      for(int j=0; j<ps.size(); j++){
	std::cout<<"  ps.push_back("<<ps[j]<<");"<<std::endl;}}}
return 0;}