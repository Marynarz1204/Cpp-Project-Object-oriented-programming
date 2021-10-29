#include "Window.h"
#include <ctime>
#include <sysinfoapi.h>
#include <stdio.h>

global_variable Window* window = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//Event when the window will be created
		window->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		//Event when the window will be destroyed
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;

		int size = render_state.width * render_state.height * sizeof(u32);


		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
		render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
		render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
		render_state.bitmap_info.bmiHeader.biPlanes = 1;
		render_state.bitmap_info.bmiHeader.biBitCount = 32;
		render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
	}
	default:
		return::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	ShowCursor(FALSE);
	srand(time(NULL));
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"WindowClass";
	wc.lpfnWndProc = WndProc;

	if (!::RegisterClass(&wc))
		return false;

	if (!window)
		window = this;

	m_hwnd = ::CreateWindow(L"WindowClass", L"JumpTower", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);
	{
		SetWindowLong(m_hwnd, GWL_STYLE, GetWindowLong(m_hwnd, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(m_hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	hdc = GetDC(m_hwnd);

	if (!m_hwnd)
		return false;

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	delta_time = 0.016666f;
	
	m_is_run = true;
	return true;
	

}

bool Window::broadcast()
{
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);
	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}
	//INPUT
	MSG msg;
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		input.button[i].change = false;
	}

	while (::PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE) > 0)
	{
		switch (msg.message)
		{
		case WM_KEYUP:
		case WM_KEYDOWN:
		{
			u32 vk_code = (u32)msg.wParam;
			bool is_down = ((msg.lParam &(1<<31))==0);

#define process_button(b, vk) \
case vk:\
{\
	input.button[b].is_down = is_down;\
	input.button[b].change = true;\
} break;

			switch (vk_code)
			{
				process_button(BUTTON_LEFT, VK_LEFT);
				process_button(BUTTON_RIGHT, VK_RIGHT);
				process_button(BUTTON_UP, VK_UP);
				process_button(BUTTON_DOWN, VK_DOWN);
				process_button(BUTTON_ESC, VK_ESCAPE);
				process_button(BUTTON_R, 0x52);
			}

		} break;

		default:
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		}
		
		
	}
	window->onUpdate(&input);
	StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

	LARGE_INTEGER frame_end_time;
	QueryPerformanceCounter(&frame_end_time);
	delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
	frame_begin_time = frame_end_time;

	return true;
}

bool Window::release()
{
	if(!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

void Window::onCreate()
{
}

void Window::onUpdate(Input* input)
{
}

void Window::onDestroy()
{
	m_is_run = false;
}


void Window::draw_rectangle_in_pixels(int x1, int x2, int y1, int y2, u32 color)
{
	x1 = clamp(0, x1, render_state.width);
	x2 = clamp(0, x2, render_state.width);
	y1 = clamp(0, y1, render_state.height);
	y2 = clamp(0, y2, render_state.height);
	for (int y = y1; y < y2; y++)
	{
		u32* pixel = (u32*)render_state.memory + x1 + y * render_state.width;
		for (int x = x1; x < x2; x++)
		{
			*pixel++ = color;
		}
	}
}

void Window::draw_rectangle(float x, float y, float half_x, float half_y, u32 color)
{
	float scale = .01f;
	x *= render_state.width * scale;
	y *= render_state.width * scale;
	x += render_state.width / 2.f;
	y += render_state.height / 2.f;
	half_y *= render_state.width * scale;
	half_x *= render_state.width * scale;
	int x1 = int(x - half_x);
	int x2 = int(x + half_x);
	int y1 = int(y - half_y);
	int y2 = int(y + half_y);
	draw_rectangle_in_pixels(x1, x2, y1, y2, color);
}

void Window::clear_screen(u32 color)
{
	draw_rectangle(0, 0, 40, 100, 0);
}

void Window::draw_player(Player player)
{
	draw_rectangle(player.getX(), player.getY(), (player.getWidth()/ 2), (player.getHeight()/ 2), player.getColor());
}

void Window::draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			draw_rectangle(x - size, y, half_size, 2.5f * size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x, y + size * 2.f, half_size, half_size, color);
			draw_rectangle(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			draw_rectangle(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x, y, 1.5f * size, half_size, color);
			draw_rectangle(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x + size, y + size, half_size, half_size, color);
			draw_rectangle(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_rectangle(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rectangle(x - half_size, y, size, half_size, color);
			draw_rectangle(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x - size, y + size, half_size, 1.5f * size, color);
			draw_rectangle(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_rectangle(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x, y, 1.5f * size, half_size, color);
			draw_rectangle(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x - size, y + size, half_size, half_size, color);
			draw_rectangle(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_rectangle(x + half_size, y + size * 2.f, size, half_size, color);
			draw_rectangle(x + half_size, y, size, half_size, color);
			draw_rectangle(x + half_size, y - size * 2.f, size, half_size, color);
			draw_rectangle(x - size, y, half_size, 2.5f * size, color);
			draw_rectangle(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_rectangle(x - size, y, half_size, 2.5f * size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x, y + size * 2.f, half_size, half_size, color);
			draw_rectangle(x, y - size * 2.f, half_size, half_size, color);
			draw_rectangle(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_rectangle(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rectangle(x - half_size, y, size, half_size, color);
			draw_rectangle(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	}
}

void Window::draw_text(const char* text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;
	const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",

	"0",
	"0",
	" ",
	" ",
	" ",
	"0",
	"0",
	};


	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else if (*text == 58) letter = letters[28];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rectangle(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}

float Window::getDT()
{
	return delta_time;
}


Window::~Window()
{
}