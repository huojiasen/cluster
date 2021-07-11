#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace std;
#pragma comment(lib, "cv200.lib")
#pragma comment(lib, "cxcore200.lib")
#pragma comment(lib, "highgui200.lib")



float val[10][10];

void split(string text, vector<string>& vec) {
	while(1){
		int pos = text.find(' ');
		if( pos==0) {
			text=text.substr(1);
			continue;
		}
		if( pos<0){
			vec.push_back(text);
			break;
		}
		string word = text.substr(0,pos);
		text = text.substr(pos+1);
		vec.push_back(word);
	}
}

void drawAndsave(IplImage* src, string name) {
	IplImage *img = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	cvCopy(src,img,NULL);
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++){
			int row = int((float(i)/float(img->height))*10.0);
			int col = int((float(j)/float(img->width))*10.0);

			double aphel=val[row][col];
			if (aphel > 0.0) {
				if (aphel>1.0) aphel = 1.0;
				CvScalar s = cvGet2D(img, i, j);
				s.val[1] = int((1-aphel)*255);
				cvSet2D(img,i,j,s);
			}
		}
	}
	cvSaveImage(name.c_str(),img);
	cvReleaseImage(&img);
}


int main() {
	IplImage *src = cvLoadImage("bg.jpg");
	ifstream in("file.txt");
	string line;
	int index = 0;
	string name;
	while(getline(in, line)) {
		if (index%11==0) {
			name = name + ".jpg";
			if (index !=0)
				drawAndsave(src, name);
			name = line;
			cout<<"name"<<name<<endl;
		} else {
			vector<string> tmp;
			split(line, tmp);
			for (int i=0;i<tmp.size();i++) {
				stringstream ss;
				string str = tmp[i];
				int v;ss<<str;ss>>v;
				val[index%11 -1][i] = float(v)/100.0;
			}
		}
		index ++;
	}
	
	
	/*cvNamedWindow("src",1);
	cvShowImage("src",src);
	cvWaitKey(0);*/
	cvReleaseImage(&src);
	return 0;
}
