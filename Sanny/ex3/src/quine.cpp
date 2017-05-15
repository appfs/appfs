#include <iostream>
#include <string>

using namespace std;

int main(){
string s="#include <iostream>\n"
"#include <string>\n"
"\n"
"using namespace std;\n"
"int main(){\n"
"string s=;\n"
"\n"
  "for(unsigned int i=0;i<s.length();i++){\n"
     "int count = 0;\n"
     "if(s[i]=='\n'){\n"
       "cout<<endl;\n"
     "} else {\n"
       "cout<<s[i];\n"
     "}\n"
     "count++;\n"
   "return 1;\n"
"}\n";

   for(unsigned int i=0;i<s.length();i++){
	   if(126 == i){
		   cout << s;
	   }
	   if(s[i]=='\n'){
		   cout<<endl;
	   } else {
		   cout<<s[i];
	   }
   }
   return 1;
}
