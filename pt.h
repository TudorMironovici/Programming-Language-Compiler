#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "val.h"


class Pt {
protected:
	int			linenum;
	Pt	*left;
	Pt	*right;

public:
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~Pt() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }

	int Traverse(bool (Pt::*func)(void) const) const {
        int cnt = 0;
		if( left ) cnt += left->Traverse(func);
		if( right ) cnt += right->Traverse(func);
        if( (this->*func)() ) {
            ++cnt;
        }
		return cnt;
	}

	int MaxDepth() const {
        int lc = 0, rc = 0;
		if( left ) lc = left->MaxDepth();
		if( right ) rc += right->MaxDepth();
		if( lc > rc ) return lc + 1;
        return rc + 1;
	}

    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
    virtual Value Eval(map<string,Value>& symbols) const=0;

    void CheckAssignments(map<string,bool>& var) {
		if( left ) left->CheckAssignments(var);
		if( right ) right->CheckAssignments(var);
        this->CheckAssign(var);
    }
        
	virtual void CheckAssign(map<string,bool>& var) {
		return;
	}
};

class StmtList : public Pt {

public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    Value Eval(map<string,Value>& symbols) const {
        left->Eval(symbols);
        if(right) {
            right->Eval(symbols);
        }
        return Value();
    }  
};

class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
    Value Eval(map<string,Value>& symbols) const {
        cout << left->Eval(symbols);
        if(right) {
            cout << right->Eval(symbols);
        }
        return Value();
    }
};

class Println : public Pt {
public:
	Println(int line, Pt *e) : Pt(line, e) {}
    Value Eval(map<string,Value>& symbols) const {
        cout << left->Eval(symbols) << endl;
        if(right) {
            cout << right -> Eval(symbols) << endl;
        }
        return Value();
    }
};

class Repeat : public Pt {
public:
	Repeat(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    Value Eval(map<string, Value>& symbols) const {
       Value holder = left -> Eval(symbols);
       if(holder.IsInt()) {
           int counter = holder.GetInt();
           while(counter > 0) {
               symbols[left->getId()] = right->Eval(symbols);
               counter--;
           }
           return Value(symbols[left->getId()]);
       }
       else {
           throw "at " + to_string(GetLineNumber() + 1) + ": control expression not an integer";           
       }
    }
};

class Assign : public Pt {
    string id;
public:
	Assign(int line, Pt *l, Pt *r) : Pt(line,l,r) {}   
    
    bool isEq() const { return true; }
    
	virtual void CheckAssign(map<string,bool>& var) {
		right->CheckAssign(var);
		var[ this->id ] = true;
	}   
    Value Eval(map<string, Value>& symbols) const {
        Value vR = right -> Eval(symbols);
        symbols[left -> getId()] = vR;
        return vR;
    }
     
};

class PlusExpr : public Pt {
public:
	PlusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    bool isPlus() const { return true; }
    
    Value Eval(map<string, Value>& symbols) const {
        Value leftValue = left->Eval(symbols);
        Value rightValue = right->Eval(symbols);   
        if(leftValue.IsErr() || rightValue.IsErr()) {
            cout << "RUNTIME ERROR at " <<linenum<<": Type error on addition"<< endl;
            exit(0);
        }
        else {
        return Value(leftValue + rightValue);
        }
    }   
};

class MinusExpr : public Pt {
public:
	MinusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    	Value Eval(map<string, Value>& symbols) const {
        Value leftValue = left->Eval(symbols);
        Value rightValue = right->Eval(symbols); 
        if(leftValue.IsErr() || rightValue.IsErr()) {
            cout << "Runtime ERROR at " << linenum << ": Type error on subtraction"<<endl;
            exit(0);
        }
        else {
        return Value(leftValue - rightValue);
        }
    }  
};

class TimesExpr : public Pt {
public:
	TimesExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map<string, Value>& symbols) const {
       Value ret = left->Eval(symbols) * right -> Eval(symbols);
       
       if(ret.IsErr()) {
           throw("at " + to_string(linenum + 1) + ": Type error on multiplication");
       }
       return ret;
    }
};

class DivideExpr : public Pt {
public:
	DivideExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    	Value Eval(map<string, Value>& symbols) const {
      	Value ret = left->Eval(symbols) / right -> Eval(symbols);
       if(ret.IsErr()) {
           throw("at " + to_string(linenum + 1) + ": Type error on division");
       }
       
       return ret;
    }
};

class IConst : public Pt {
	int id;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		id = stoi(t.GetLexeme());
	}
    Value Eval(map<string, Value>& symbols) const {
        return Value(id);
    }  
};

class SConst : public Pt {
	string id;

public:
	SConst(Tok& t) : Pt(t.GetLinenum()) {
		id = t.GetLexeme();
	}
    Value Eval(map<string, Value>& symbols) const {
        return Value(id);
    }  
};

class Ident : public Pt {
	string id;

public:
	Ident(Tok& t) : Pt(t.GetLinenum()), id(t.GetLexeme()) {}

	bool isIdent() const { return true; }
	string getId() const { return id; }
	virtual void CheckAssign(map<string,bool>& var) {
        if( var.find(getId()) == var.end() )
            std::cout << "UNDECLARED VARIABLE " << getId() << std::endl;
    }
    //No assigned variable -self reminder
    Value Eval(map<string, Value>& symbols) const {
      if(symbols.find(getId()) != symbols.end()) {
         return symbols[id]; 
      }
      else {
         throw "at " + to_string(GetLineNumber() + 1) + ": Variable used but not assigned";
      }
    }   
};

#endif /* PT_H_ */