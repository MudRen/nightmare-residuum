#include "living.h"

private string __Gender = "neither";
private int __Level = 0;

string query_gender () {
    return __Gender;
}
void set_gender (string str) {
    switch(str) {
    case "male": case "female": case "neither":
        __Gender = str;
        break;
    default:
        __Gender = "none";
    }
}

int query_level () {
    return __Level;
}
void set_level (int l) {
    if (l < 0) {
        l = abs(l);
    }
    __Level = l;
}