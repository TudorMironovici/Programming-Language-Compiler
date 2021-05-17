#include <fstream>
using std::ifstream;
#include "parse.h"
#include "lex.h"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
    map<string, Value> symbols;
	int lineNumber = 0;
	istream *in = &cin;
	ifstream file;
	for(int i=1; i<argc; i++) {
		string arg(argv[i]);		
		if( in != &cin ) {
			cerr << "ONLY ONE FILE NAME ALLOWED" << endl;
			return 0;
		}
		else 
        {
			file.open(arg);
			if( file.is_open() == false ) {
				cerr << "CANNOT OPEN " << arg << endl;
				return 0;
			}
			in = &file;
		}
	}  
    Pt *prog = Prog(*in,lineNumber);
     
    if(prog != 0) {
        try {
            prog->Eval(symbols);
        }
        catch(string m) {
            cout << "RUNTIME ERROR " << m << endl;
        }
    }
	return 0;
}