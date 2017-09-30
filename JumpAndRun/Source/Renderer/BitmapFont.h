#pragma once
#include<string>

namespace RENDERER
{

	class BitmapFont
	{
		friend class Renderer;
	private:
		BITMAP* Bitmap;
		int Size;
		int* BaseWidth;
		int CellWidth;
		int CellHeight;
		int XCount, YCount;
	public:
		void Load(const std::string& name);
		void Release()
		{
			if (BaseWidth)
			{
				delete[] BaseWidth;
				BaseWidth = NULL;
			}
		}
	};

}