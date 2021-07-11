#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>

#include <fstream>
#include <sstream>

#include <stdio.h>
#include <math.h>
#include <cstdlib>

#define PI 3.14159265
#define RAD (PI/180.0)
#define EARTH_RADIUS  6378137

using namespace std;

void stringSplit(string s, vector<string>& vec, char sp) {
    if (vec.size()>0)
        vec.clear();
    int length = s.length();
    int start = 0;
    for (int i=0;i<length;i++) {
        if (s[i]==sp && i==0) {
            start++;
        } else if (s[i] == sp) {
            vec.push_back(s.substr(start, i-start));
            start = i+1;
        } else if (i==length-1) {
            vec.push_back(s.substr(start, i+1-start));
        }
    }
}
double string2double(string s) {
    double res = atof(s.c_str());
    return res;
}
int string2int(string s) {
    stringstream ss;
    ss<<s;
    int res;
    ss>>res;
    return res;
}

long is_distance(double lng1, double lat1, double lng2,double lat2) {
    double radLat1 = lat1 * RAD;
    double radLat2 = lat2 * RAD;
    double a = radLat1 - radLat2;
    double b = (lng1 - lng2) * RAD; 
    double s = 2
        * asin(
                sqrt(
                    pow(sin(a / 2), 2)
                    + cos(radLat1) * cos(radLat2)
                    * pow(sin(b / 2), 2)) );
    s = s * EARTH_RADIUS; 
    
    return (long) (s);
}

