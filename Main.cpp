#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
using namespace std;

class ImageEdge{
	public:
		
	int numRows, numCols, minVal, maxVal;
	
	//constructor
	ImageEdge(int numRows, int numCols, int minVal, int maxVal){
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
	}
	
	void loadImage(string input, int** mirrorFramedAry){
		ifstream in;
		in.open(input.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;

		for(int i=1;i<numRows+1;i++){
			for(int j=1;j<numCols+1;j++){
				in>>mirrorFramedAry[i][j];
			}
		}
	}
	
	void mirrorFrame(int** mirrorFramedAry){
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				mirrorFramedAry[0][j] = mirrorFramedAry[1][j];
				mirrorFramedAry[numRows+1][j] = mirrorFramedAry[numRows][j];
				mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
				mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];
			}
		}
	}
	
	void loadMaskArys(int maskRightDiag[3][3] , int maskLeftDiag[3][3]){
		maskLeftDiag[0][0] = 0;
		maskLeftDiag[0][1] = 1;
		maskLeftDiag[0][2] = 2;
		maskLeftDiag[1][0] = -1;
		maskLeftDiag[1][1] = 0;
		maskLeftDiag[1][2] = 1;
		maskLeftDiag[2][0] = -2;
		maskLeftDiag[2][1] = -1;
		maskLeftDiag[2][2] = 0;
		
		maskRightDiag[0][0] = 2;
		maskRightDiag[0][1] = 1;
		maskRightDiag[0][2] = 0;
		maskRightDiag[1][0] = 1;
		maskRightDiag[1][1] = 0;
		maskRightDiag[1][2] = -1;
		maskRightDiag[2][0] = 0;
		maskRightDiag[2][1] = -1;
		maskRightDiag[2][2] = -2;
	}
	
	int convolute(int row, int col, int** mirrorFramedAry, int mask[3][3]){
		int convolution = 0, r = 0, c = 0;
		for(int i=row-1; i<row+2; i++){
			for(int j=col-1; j<col+2; j++){
				convolution += (mirrorFramedAry[i][j] * mask[r][c]);
				c++;
			}
			c = 0;
			r++;
		}
		return convolution;
	}
	
	int computeGradient(int row, int col, int** mirrorFramedAry){
		int computation = 0;
		int x = mirrorFramedAry[row][col];
		int a = mirrorFramedAry[row][col+1];
		int b = mirrorFramedAry[row+1][col];
		computation = sqrt(pow(x-b,2) + pow(x-a,2));
		return computation;
	}
	
	void write(int** Ary, ofstream& file){
		file<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		for(int i=1; i<numRows+1; i++){
			for(int j=1; j<numCols+1; j++){
				file<<Ary[i][j]<<" ";
			}
			file<<endl;
		}
	}
	
	void prettyPrint(int** Ary, ofstream& file){
		for(int i=1; i<numRows+1; i++){
			for(int j=1; j<numCols+1; j++){
				if(Ary[i][j]>25)file<<". ";
				else file<<"  ";
			}
			file<<endl;
		}
		file<<endl;
	}
};

int main(int argc, char* argv[]){
	ifstream inFile (argv[1]);
	string fileName = argv[1];
	ofstream sobel1 (argv[2]);
	ofstream sobel2 (argv[3]);
	ofstream gradient (argv[4]);
	ofstream prettyPrint (argv[5]);
	int numRows, numCols, minVal, maxVal;
	int** mirrorFramedAry;
	int maskRightDiag[3][3];
	int maskLeftDiag[3][3];
	int** sobelRightDiag;
	int** sobelLeftDiag;
	int** gradientEdge;
	
	if(inFile.is_open()){
		inFile>>numRows>>numCols>>minVal>>maxVal;
		
		mirrorFramedAry = new int*[numRows+2];
		sobelRightDiag = new int*[numRows+2];
		sobelLeftDiag = new int*[numRows+2];
		gradientEdge = new int*[numRows+2];

		for(int i = 0;i < numRows+2; i++){
			mirrorFramedAry[i] =  new int[numCols+2];
			sobelRightDiag[i] =  new int[numCols+2];
			sobelLeftDiag[i] =  new int[numCols+2];
			gradientEdge[i] =  new int[numCols+2];
		}	
		
		//initialize all arrays to 0
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				mirrorFramedAry[i][j] = 0;
				sobelRightDiag[i][j] = 0;
				sobelLeftDiag[i][j] = 0;
				gradientEdge[i][j] = 0;
			}
		}
		ImageEdge image(numRows, numCols, minVal, maxVal);
		image.loadImage(fileName, mirrorFramedAry);
		image.mirrorFrame(mirrorFramedAry);
		image.loadMaskArys(maskRightDiag, maskLeftDiag);
				
		for(int i = 1; i < numRows+1; i++){
			for(int j = 1; j < numCols+1; j++){
				sobelRightDiag[i][j] = abs(image.convolute(i, j, mirrorFramedAry, maskRightDiag));
				sobelLeftDiag[i][j] = abs(image.convolute(i, j, mirrorFramedAry, maskLeftDiag));
				gradientEdge[i][j] = image.computeGradient(i, j, mirrorFramedAry);
			}
		}

		image.write(sobelRightDiag, sobel1);
		image.write(sobelLeftDiag, sobel2);
		image.write(gradientEdge, gradient);
		
		image.prettyPrint(mirrorFramedAry, prettyPrint);
		image.prettyPrint(sobelRightDiag, prettyPrint);
		image.prettyPrint(sobelLeftDiag, prettyPrint);
		image.prettyPrint(gradientEdge, prettyPrint);

	}
	
	else cout<<"Couldn't retrieve data.";
	inFile.close();
	return 0;
}
