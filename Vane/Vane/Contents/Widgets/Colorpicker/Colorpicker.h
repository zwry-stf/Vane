#pragma once
#include "../Widget.h"
#include <string>
#include "../../../Renderer/XyphraDef.h"

struct HSV {
	float h, s, v, a;
};

class ColorPicker : public Overlay {
public:
	ColorPicker(XyColor* color, bool has_alpha) : pColor(color), hasAlpha(has_alpha) 
	{
		isColorPicker = true;

		loadFromColor(*pColor);
		loadFromHSV();
		oldColor = *pColor;
	}

	void Draw(const int id, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened) override;

	const XyColor& getColor() 
	{ 
		return *pColor; 
	}
	const XyColor& getColorIA()
	{ 
		return XyColor(pColor->r, pColor->g, pColor->b, 1.f);
	}
	void setColor(const XyColor& color) 
	{ 
		*pColor = color;
		loadFromColor(*pColor);
		loadFromHSV();
		oldColor = *pColor;
	}
	bool hasAlpha = false;

private:
	XyColor* pColor;
	XyColor oldColor;
	void loadFromHSV();
	void loadFromColor(XyColor color);

private:
	HSV hsv{ 0, 0, 0 };
	XyColor hue;
	float space_offset = 0.f;
	float square_size = 0.f;
	float bar_width = 0.f;
	int moving = -1;
};

class ColorPickerWidget : public Widget {
public:
	ColorPickerWidget(std::string label) : strLabel(label) 
	{
		default_height = 26.f;
		buffer.resize(16); // Should be enough
	}

	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) override;

	void stopTyping();
private: // Typing
	std::string buffer;
	bool typing = 0.f;
	int typing_pos = 0; // Starting from last
	float typing_animation = 0.f;
	bool typing_toggle = false;
	int64_t last_toggle_time = 0;
	float selected_char_begin = 0.f;
	int str_length = 0;

private:
	int _hovered = -1;
	float label_width = 0.f;
	float color_width = 0.f;
	float typed_pos = 0.f; // Set at wndproc, evaluated in draw
	std::string strLabel;
};