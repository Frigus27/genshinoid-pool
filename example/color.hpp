#ifndef GENSHINOID_COLOR_H
#define GENSHINOID_COLOR_H

#include <windows.h>
#include <iostream>
#include <string>

enum class ColorID {
	Default=8, Black=0, Blue, Green, Celeste, Red, Purple, Yellow, White
};

using std::string;
using std::cout;

class Color {
	private:
	ColorID foreColor;
	ColorID backColor;

	public:
	const ColorID & Fore = foreColor;
	const ColorID & Back = backColor;

	public:
	Color() {
		foreColor = ColorID::Default;
		backColor = ColorID::Black;
	}

	Color(ColorID newForeColor) {
		foreColor = newForeColor;
		backColor = ColorID::Black;
	}

	Color(ColorID newForeColor, ColorID newBackColor) {
		foreColor = newForeColor;
		backColor = newBackColor;
	}

	public:
	operator int() {
		return int(foreColor) + int(backColor) * 16;
	}

	Color operator=(Color color) {
		foreColor = color.foreColor;
		backColor = color.backColor;
		return *this;
	}

	bool operator==(Color color) {
		return (foreColor == color.foreColor && backColor == color.backColor);
	}
};

inline void setColor(Color color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | int(color));
}


#endif