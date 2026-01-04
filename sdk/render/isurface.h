#pragma once
#include "../utils/memory.h"

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		memory::Call<void>(this, 14, r, g, b, a);
	}
	void DrawFilledRect(int x, int y, int xx, int yy)
	{
		memory::Call<void>(this, 15, x, y, xx, yy);
	}
	void DrawOutlinedRect(int x, int y, int xx, int yy)
	{
		memory::Call<void>(this, 17, x, y, xx, yy);
	}
	void DrawLine(int x, int y, int x1, int y1)
	{
		memory::Call<void>(this, 15, x, y, x1, y1);
	}
private:

};
