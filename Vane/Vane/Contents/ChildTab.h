#pragma once
#include <optional>
#include <stdint.h>

#include "../Renderer/XyphraDef.h"
#include "Widgets/Checkbox/Checkbox.h"
#include "Widgets/Slider/Slider.h"
#include "Widgets/Button/Button.h"
#include "Widgets/Hotkey/Hotkey.h"
#include "Widgets/Dropdown/Dropdown.h"
#include "Widgets/Colorpicker/Colorpicker.h"
#include "ChildWindow.h"

class ChildTab {
public:
	ChildTab(const std::string& name, int num_areas) : Name(name), NumAreas(num_areas) 
	{
		PositionsAreas = new float[num_areas];
		SizesAreas = new XyVec2[num_areas];
	}

	virtual float Render(float x, float y, float alpha, int id, int* hovered, int* selected);
	virtual std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected);

	void EndGroup(int area);
	void AddGroupTitle(const std::string& title, int area);
	Checkbox* AddCheckbox(const std::string& label, bool* value, int area);
	Slider* AddSlider(const std::string& label, float* value, float min, float max, int area, const std::string& format = "%.2f");
	Button* AddButton(const std::string& label, const std::string& button_text, ButtonCallback callback, void* this_ptr, int area);
	Hotkey* AddHotkey(const std::string& label, int* value, int area);
	DropdownWidget* AddDropdown(const std::string& label, XyVector<std::string>* options, int* value, int area);
	ColorPickerWidget* AddColorpicker(const std::string& label, XyColor* value, bool hasAlpha, int area);
	ColorPicker* AddColorpickerOverlay(XyColor* value, bool hasAlpha);

	ChildWindow* AddChildWindow(const std::string& label);

	// Other
	void SetIcon(const unsigned char* data, size_t size);

public:
	XyVector<Widget*> Widgets;
	float animation_selected = 0.f;
	float animation_hovered = 0.f;
	bool last_active = false;

protected:
	std::string Name;
	const int NumAreas = 0;
	float* PositionsAreas = nullptr;
	XyVec2* SizesAreas = nullptr;

	int Selected = -1;
	int Hovered = -1;

	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;


	float Scroll = 0.f, DesScroll = 0.f, ScrollSpeed = 0.f;

	XyTextureID* Icon = NULL;
};