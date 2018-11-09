#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class ImageEdge{
	public:
		
	int numRows, numCols, minVal, maxVal;
	
	//constructor
	ImageEdge(int numRows, int numCols, int minVal, int maxVal, int** mirrorFramedAry, int** sobelRightDiag, int** sobelLeftDiag, int** gradientEdge){
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
		
		//initialize all arrays to 0
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				mirrorFramedAry[i][j] = 0;
				sobelRightDiag[i][j] = 0;
				sobelLeftDiag[i][j] = 0;
				gradientEdge[i][j] = 0;
			}
		}
	}
	
	void printAry(int** Array){
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				cout<<Array[i][j]<<" ";
			}
			cout<<endl;
		}
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
		for(int i = 0;i < numRows+2; i++){
			mirrorFramedAry[i] =  new int[numCols+2];
		}
		
		sobelRightDiag = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			sobelRightDiag[i] =  new int[numCols+2];
		}
		
		sobelLeftDiag = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			sobelLeftDiag[i] =  new int[numCols+2];
		}
		
		gradientEdge = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			gradientEdge[i] =  new int[numCols+2];
		}
		
		ImageEdge image(numRows, numCols, minVal, maxVal, mirrorFramedAry, sobelRightDiag, sobelLeftDiag, gradientEdge);
		image.loadImage(fileName, mirrorFramedAry);
		image.mirrorFrame(mirrorFramedAry);
		image.printAry(mirrorFramedAry);
	}
	
	else cout<<"Couldn't retrieve data.";
	inFile.close();
	return 0;
}
