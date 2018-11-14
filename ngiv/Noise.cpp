#include "Noise.h"

namespace ngiv {

	void PerlinNoise1D(int nCount, std::vector<float> *fSeed, int nOctaves, float fBias, std::vector<float> *fOutput)
	{
		// Used 1D Perlin Noise
		for (int x = 0; x < nCount; x++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nCount >> o;
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % nCount;

				float fBlend = (float)(x - nSample1) / (float)nPitch;

				float fSample = (1.0f - fBlend) * (*fSeed)[nSample1] + fBlend * (*fSeed)[nSample2];

				fScaleAcc += fScale;
				fNoise += fSample * fScale;
				fScale = fScale / fBias;
			}

			// Scale to seed range
			fOutput->push_back((float)fNoise / (float)fScaleAcc);
		}
	}

	void PerlinNoise2D(int nWidth, int nHeight, std::vector<float> *fSeed, int nOctaves, float fBias, std::vector<std::vector<float>>* fOutput)
	{
		fOutput->emplace_back();
		// Used 1D Perlin Noise
		for (int x = 0; x < nWidth; x++)
		{
			for (int y = 0; y < nHeight; y++)
			{
				float fNoise = 0.0f;
				float fScaleAcc = 0.0f;
				float fScale = 1.0f;

				for (int o = 0; o < nOctaves; o++)
				{
					int nPitch = nWidth >> o;
					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
					int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - fBlendX) * (*fSeed)[nSampleY1 * nWidth + nSampleX1] + fBlendX * (*fSeed)[nSampleY1 * nWidth + nSampleX2];
					float fSampleB = (1.0f - fBlendX) * (*fSeed)[nSampleY2 * nWidth + nSampleX1] + fBlendX * (*fSeed)[nSampleY2 * nWidth + nSampleX2];

					fScaleAcc += fScale;
					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScale = fScale / fBias;
				}

				// Scale to seed range
				fOutput->back().push_back((float)fNoise / (float)fScaleAcc);
			}
			fOutput->emplace_back();
		}

	}
	
	std::vector<float>* getRandomVec(int seed, int size) {
		static std::vector<float>* randvec = new std::vector<float>();
		randvec->clear();
		for (int i = 0; i < size; i++) {
			randvec->push_back((float)rand() / (float)RAND_MAX);
		}
		return randvec;
	}

	std::vector<float>* Noise::get1DNoise(int seed,int size, float fBias, int nOctaves) {
		srand(seed);
		std::vector<float>* fNoiseSeed1D = getRandomVec(seed,size);
		std::vector<float>* fPerlinNoise1D = new std::vector<float>();

		PerlinNoise1D(size, fNoiseSeed1D, nOctaves, fBias, fPerlinNoise1D);
		return fPerlinNoise1D;
	}

	std::vector<std::vector<float>>* Noise::get2DNoise(int seed,int width,int height, float fBias, int nOctaves) {
		srand(seed);
		std::vector<float>* fNoiseSeed2D = getRandomVec(seed, width * height);
		std::vector<std::vector<float>>* fPerlinNoise2D = new std::vector<std::vector<float>>();

		PerlinNoise2D(width,height, fNoiseSeed2D, nOctaves, fBias, fPerlinNoise2D);
		return fPerlinNoise2D;
	}
	
}