#pragma once
#include <GLAD\glad.h>

namespace ngiv {
	class ColorRGBA8{
	public:
		ColorRGBA8(GLubyte R,GLubyte G,	GLubyte B, GLubyte A){
			r = R;
			g = G;
			b = B;
			a = A;
		}
		ColorRGBA8(){
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}
		
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

}