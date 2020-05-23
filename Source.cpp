#include<iostream>
#include<vector>

using namespace std;
const float PI = 3.1415;



struct ComplexValue {
public:
	float realComp, imgComp;

	ComplexValue() {
		realComp = 0.0;
		imgComp = 0.0;
	}
};

void PrintComplexArray(vector<ComplexValue> vectorForDisplay, int arraySize);
std::vector<ComplexValue> FurieTransform(float *signalValues, int sampleSize);
void FastFurieTransform(std::vector<ComplexValue> &signalsBeforeFFT, int sampleSize);

void main() {
	cout << "hello world, lets try to implement a FT here" << endl; 
	float inputSignals[8] = { 1, 2, 3, 4, 5, 6, 1, 2};
	int arraySize = sizeof(inputSignals) / 4;
	std::vector<ComplexValue> results;
	for (int i = 0; i < arraySize; i++)
	{
		results.push_back(ComplexValue());
		results[i].realComp = inputSignals[i];
	}
	FastFurieTransform(results, arraySize);
	////= FurieTransform(inputSignals, arraySize);
	PrintComplexArray(results, arraySize);
	cin.ignore();
}


// euler formula  e^ix = cosx + i*sinx
//Furie transform  Xk = SUM(xn * (cos(2*PI*k*n/N) - i * sin(2PI*k*n/N))
std::vector<ComplexValue> FurieTransform(float *signalValues, int sampleSize) {
	const int resultArraySize = 16;
	ComplexValue signalAfterFT[resultArraySize];
	std::vector<ComplexValue> resultFT;

	for (int i = 0; i < sampleSize; i++)
	{
		for (int j = 0; j < sampleSize - 1; j++)
		{
			signalAfterFT[i].realComp += signalValues[j] * cos((2.0 * PI * i * j )/ sampleSize);
			signalAfterFT[i].imgComp += signalValues[j] *(-1) * sin((2.0 * PI * i * j) / sampleSize);
		}
		resultFT.push_back(signalAfterFT[i]);
	}

	return resultFT;
};

//yk = yk0 + wn^k*yk^1
//yk+n/2 =yk0 - wn^k*yk^1
void FastFurieTransform(std::vector<ComplexValue> &signalsBeforeFFT, int sampleSize) {
	if (sampleSize <= 1) {
		return;
	}

	if (sampleSize % 2 != 0) {
		throw "Wrong input sample, please, change its size";
	}

	std::vector<ComplexValue> odd;
	std::vector<ComplexValue> even;

	for(int i = 0; i < sampleSize; i++)
	{
		if (i % 2 == 0)
		{
			even.push_back(signalsBeforeFFT[i]);
		}
		else
		{
			odd.push_back(signalsBeforeFFT[i]);
		}
	}

	FastFurieTransform(odd, odd.size());
	FastFurieTransform(even, even.size());
	
	int halfOfSize = sampleSize / 2;
	for (int i = 0; i < halfOfSize; i++)
	{
		float eulorCoef = (-2.0 * PI * i) / sampleSize;
		float sinusW = sin(eulorCoef);
		float cosinusW = cos(eulorCoef);
		
		signalsBeforeFFT[i].realComp = even[i].realComp + cosinusW * odd[i].realComp - sinusW *odd[i].imgComp;
		signalsBeforeFFT[i].imgComp = even[i].imgComp + sinusW * odd[i].realComp + cosinusW * odd[i].imgComp;
		signalsBeforeFFT[i + halfOfSize].realComp = even[i].realComp - cosinusW * odd[i].realComp + sinusW *odd[i].imgComp;
		signalsBeforeFFT[i + halfOfSize].imgComp = even[i].imgComp - sinusW * odd[i].realComp - cosinusW * odd[i].imgComp;
	}
};

void PrintComplexArray(vector<ComplexValue> inputVector, int arraySize) {
	for (std::vector<ComplexValue>::iterator it = inputVector.begin(); it != inputVector.end(); ++it) {
		cout << (*it).realComp <<"  " << (*it).imgComp << endl;
	}
};
