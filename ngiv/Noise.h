#pragma once
#include <vector>

namespace ngiv {

	class Noise
	{
	public:
				
		static std::vector<float>* get1DNoise(int seed, int size, float fBias, int nOctaves);
		static std::vector<std::vector<float>>* get2DNoise(int seed, int width, int height, float fBias, int nOctaves);
				

	};

}