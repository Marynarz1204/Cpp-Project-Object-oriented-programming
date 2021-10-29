#pragma once
#include <Windows.h>
#include "utility.cpp"
#include "platform_common.cpp"
#include "Player.h"

struct Render_State {
	void* memory;
	int width;
	int height;
	BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

class Window
{
protected:
	HWND m_hwnd;
	bool m_is_run;
	HDC hdc;
	Input input = {};
	float delta_time;
public:
	bool init();
	bool broadcast();
	bool release();
	bool isRun();
	~Window();

	virtual void onCreate()=0;
	virtual void onUpdate(Input*)=0;
	virtual void onDestroy();
	void clear_screen(u32 color);
	void draw_rectangle_in_pixels(int x1, int x2, int y1, int y2, u32 color);
	void draw_rectangle(float x, float y, float half_x, float half_y, u32 color);
	void draw_player(Player player);
	void draw_number(int, float, float, float, u32);
	void draw_text(const char* text, float x, float y, float size, u32 color);
	float getDT();

};
