#include <iostream>
#include <string>

using namespace std;

int main(){
string s="#include <iostream>\\n"
"#include <string>\\n"
"\\n"
"using namespace std;\\n"
"\\n"
"int main(){\\n"
"string s=;\\n"
"\\n"
"  for(unsigned int i=0; i<s.length(); i++){\\n"
"     if(88 == i){\\n"
"         cout << \"\\\"\";\\n"
"         for(unsigned int j=0; j<s.length(); j++){"
"             string sub = s.substr(j, 2);"
"             if(sub == \"\\\\n\"){"
"                 cout << \"\\\\\\\\n\" << \"\\\"\"<< endl << \"\\\"\";"
"                 j ++;"
"                 continue"
"             }"
"             cout<<s[j];"
"         }"
"     }\\n"
"     if(\"\\n\" == s[i]){\\n"
"         cout << endl;\\n"
"     }\\n"
"     cout<<s[i];\\n"
"   }\\n"
"   return 1;\\n"
"}\\n";

   for(unsigned int i=0; i<s.length(); i++){
	   if(88 == i){
		   cout << "\"";
		   for(unsigned int j=0; j<s.length(); j++){
		   	   string sub = s.substr(j, 2);
		   	   if(sub == "\\n"){
		   		   cout << "\\\\n" << "\""<< endl << "\"";
		   		   j ++;
		   		   continue;
		   	   }
		   	   cout<<s[j];
		    }
	   }
	   string sub = s.substr(i, 2);
	   if(sub == "\\n" && i != 140){
		   cout << endl;
		   i ++;
		   continue;
	   }
	   cout<<s[i];
   }
   return 1;
}
