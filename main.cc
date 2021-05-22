#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <iostream>

using namespace std;

int xyn2oneD(int x, int y, int n, int iw) {
	    return x*3+n+(y*iw*3);
}

bool comparecolor(const unsigned char * data, int index1D, unsigned char r, unsigned char g, unsigned char b)
{
	if(data[index1D]==r && data[index1D+1]==g && data[index1D+2]==b) {	//add +1,+2 in this line
		return true;
	} else {
		return false;
	}
}


int main(int argc, char **argv){
	
	string inputPath = "ei.bmp";//"einsteinSample.bmp";
	    
	int iw, ih, n;

	// load images
	unsigned char *idata = stbi_load(inputPath.c_str(), &iw, &ih, &n, 0);
	cout<<"w: "<<iw<<", h: "<<ih<<", n: "<<n<<endl;

	int ow = iw;
	int oh = ih;
	unsigned char *odata = (unsigned char *) malloc(ow * oh * n);

	int unknown = 0;

	for(int j=0; j<ih; j++) {
		for(int i=0; i<iw; i++) {
			int index = i*3+0+j*iw*3;

			if(comparecolor(idata, index, 255, 0, 0)) {	//255,255,255
				odata[i*3+0+j*iw*3] = 0;//R
				odata[i*3+1+j*iw*3] = 255;//G
				odata[i*3+2+j*iw*3] = 0;//B
				// 255,0,0
				unknown++;
			} else {
				odata[i*3+0+j*iw*3] = idata[i*3+0+j*iw*3];
				odata[i*3+1+j*iw*3] = idata[i*3+1+j*iw*3];
				odata[i*3+2+j*iw*3] = idata[i*3+2+j*iw*3];
			}

		}
	}
	cout << "unknown pixel: " << unknown << endl;	

	string outputPath = "out.bmp";

	// write
	stbi_write_png(outputPath.c_str(), ow, oh, n, odata, 0);

	stbi_image_free(idata);
	stbi_image_free(odata);

	return 0;
}

