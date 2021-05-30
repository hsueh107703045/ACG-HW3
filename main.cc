#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <iostream>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;


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
	
	string inputPath = "einstein100.jpg";//"einsteinSample.bmp";
	    
	int iw, ih, n;

	// load images
	unsigned char *idata = stbi_load(inputPath.c_str(), &iw, &ih, &n, 0);
	cout<<"w: "<<iw<<", h: "<<ih<<", n: "<<n<<endl;

	int ow = iw;
	int oh = ih;
	unsigned char *odata = (unsigned char *) malloc(ow * oh * n);

	/*
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
*/
	
	int startPx = 15;
	int startPy = 35;

	int sizeA = 5;



	// unknown area matX
	Matrix <double, 25, 1> matX;
	matX.setZero();
	
	int counter = 0;
	for(int j = startPy ; j < startPy+5 ; j++) {
		for(int i = startPx ; i < startPx+5 ; i++) {
			
			matX(counter) = idata[i*3+0+j*iw*3];
			counter++;	
//			cout << i << "," << j << endl;

		}
	}	

	cout << "\n" << "matX: " << "\n" << matX << endl;



	// 5*5 X 5*5
	Matrix <double, 25, 25> matA;
	matA.setZero();

	Matrix <double, 25, 1> matB;
	matB.setZero();


	for(int index = 0 ; index < 25 ; index++){
		// self
		matA(index,index) = -4;
		
		// look up
		if(index > 4){
			matA(index,index-5) = 1;
		}
		else{	// upper pixel is known
			matB(index) -= idata[xyn2oneD((index%5)+startPx,(index/5)+startPy-1,0,iw)];
			cout << index << "known: " << (index%5)+startPx << ", " << (index/5)+startPy-1 << endl;
		}

		// look down
		if(index < 20){
			matA(index,index+5) = 1;
		}
		else{
			matB(index) -= idata[xyn2oneD((index%5)+startPx,(index/5)+startPy+1,0,iw)];
			cout << index << "known2: " << (index%5)+startPx << ", " << (index/5)+startPy+1 << endl;
		}

		// look right
		if((index%5) != 4){
			matA(index,index+1) = 1;
		}
		else{
			matB(index) -= idata[xyn2oneD((index%5)+startPx+1,(index/5)+startPy,0,iw)];

		}

		// look left
		if((index%5) != 0){
			matA(index,index-1) = 1;
		}
		else{
			matB(index) -= idata[xyn2oneD((index%5)+startPx-1,(index/5)+startPy,0,iw)];

		}


	}

	cout << "\n" << "matA: " << "\n" << matA << endl;
	cout << "\n" << "matB: " << "\n" << matB << endl;


// solving matrix matX
	Matrix <double, 25, 25> matAinv;
	matAinv = matA.inverse();
	cout << "inverse of A: " << matAinv <<endl;


	matX = matAinv * matB;

	cout << "\n" << "matX: " << "\n" << matX << endl;



	odata = idata;


// put matX back to image	
	counter = 0;
	for(int j = startPy ; j < startPy+5 ; j++) {
		for(int i = startPx ; i < startPx+5 ; i++) {
			
			odata[i*3+0+j*iw*3] = matX(counter);
			odata[i*3+1+j*iw*3] = matX(counter);
			odata[i*3+2+j*iw*3] = matX(counter);

			//matX(counter) = idata[i*3+0+j*iw*3];
			counter++;	
			//cout << i << "," << j << endl;

		}
	}	




	//output
	string outputPath = "out.jpg";

	// write
	stbi_write_png(outputPath.c_str(), ow, oh, n, odata, 0);

	stbi_image_free(idata);
	stbi_image_free(odata);

	return 0;
}

