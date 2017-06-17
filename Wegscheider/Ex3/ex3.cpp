#include <iostream>;
using namespace std;
int main (int numargs, char *args[]) {
	const int start = 8;
	const int numOfLines = 22;
	char quote = (char) 34;
	char space = (char) 32;
	string code[22] = {
	"#include <iostream>;",
	"using namespace std;",
	"int main (int numargs, char *args[]) {",
	"    const int start = 8;",
	"    const int numOfLines = 22;",
	"    char quote = (char) 34;",
	"    char space = (char) 32;",
	"    string code[22] = {",
	"    };",
	"    for (int i = 0; i < start; i++) {",
	"        cout << code[i] << endl;",
	"    }",
	"    for (int i = 0; i < numOfLines; i++) {",
	"        cout << space << space << space << space << quote << code[i] << quote;",
	"        if (i != numOfLines-1) cout << (char) 44;",
	"        cout << endl;",
	"    }",
	"    for (int i = start; i < numOfLines; i++) {",
	"        cout << code[i] << endl;",
	"    }",
	"    return 1;",
	"}"
	};
	for (int i = 0; i < start; i++) {
		cout << code[i] << endl;
	}
	for (int i = 0; i < numOfLines; i++) {
		cout << space << space << space << space << quote << code[i] << quote;
		if (i != numOfLines-1) cout << (char) 44;
		cout << endl;
	}
	for (int i = start; i < numOfLines; i++) {
		cout << code[i] << endl;
	}
	return 1;
}
