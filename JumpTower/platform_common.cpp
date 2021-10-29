struct Button_State {
	bool is_down;
	bool change;
};


enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ESC,
	BUTTON_R,

	BUTTON_COUNT, 
};

struct Input {
	Button_State button[BUTTON_COUNT];
};