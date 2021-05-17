#include<iostream>
#include "val.h"

Value Value::operator+(const Value& op) const {
    if(this->GetType() == VSTR && op.T == VSTR)
        return Value(S + op.S);
    if(this->GetType() == VINT && op.T == VINT)
        return Value(I + op.I);
    return Value();
}

Value Value::operator-(const Value& op) const {
	if(this->GetType() != op.GetType() || IsErr() ) {
        return Value();
    }
    if(IsInt()) {
        return Value(I-op.GetInt());
    }
    auto srch = S.find(op.GetStr());
    string result = S;
    if(srch != string::npos) {
        result.erase(srch,op.GetStr().length());
    }
    return result;
}

Value Value::operator*(const Value& op) const {
    if(this->GetType() == VINT && op.GetType() == VINT) {
        return Value(I*op.GetInt());
    }
    
    else if((this->GetType() == VSTR && op.GetType() == VINT) || (this->GetType() == VINT && op.GetType() == VSTR)) {
        string str;
        string nstr;
        int n;
        if(this->GetType() == VINT || op.GetType() == VINT) {
            if(this->GetType() == VINT) {
                str = op.GetStr();
                n = GetInt();
            }
            if(op.GetType() == VINT) {
                str = GetStr();
                n = op.GetInt();
            }
        }
        for(int i=0; i<n; i++) {
            nstr += str;
        }
        return Value(nstr);
    }
    else {
        return Value();
    }
    
}

Value Value::operator/(const Value& op) const {
    if(this->GetType() == VINT && op.T == VINT) {
        return Value(I/op.I);
    }
    else {
    	return Value();
    }
}