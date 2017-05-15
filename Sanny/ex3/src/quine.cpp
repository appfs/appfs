#include <iostream>
#include <string>

using namespace std;

int main(){
string s="#include <iostream>|"
"#include <string>|"
"|"
"using namespace std;|"
"int main(){|"
"string s=;|"
"|"
  "for(unsigned int i=0;i<s.length();i++){|"
     "if(s[i]=='|'){|"
       "cout<<endl;|"
     "} else {|"
       "cout<<s[i];|"
     "}|"
   "return 1;|"
"}|";

   for(unsigned int i=0;i<s.length();i++){
	   if(s[i]=='|'){
		   cout<<endl;
	   } else {
		   cout<<s[i];
	   }
   }
   return 1;
}
