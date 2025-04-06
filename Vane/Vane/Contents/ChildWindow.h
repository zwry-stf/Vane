#pragma once
#include "Widgets/Widget.h"
#include <string>
#include "Widgets/Checkbox/Checkbox.h"
#include "Widgets/Slider/Slider.h"
#include "Widgets/Button/Button.h"
#include "Widgets/Hotkey/Hotkey.h"
#include "Widgets/Dropdown/Dropdown.h"
#include "Widgets/Colorpicker/Colorpicker.h"

class ChildWindow : public Overlay {
public:
	ChildWindow(std::string label) : Label(label)
	{
		isChildWindow = true;
	}

	void Draw(const int id, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened) override;

	// Add Widgets
	Checkbox* AddCheckbox(const std::string& label, bool* value);
	Slider* AddSlider(const std::string& label, float* value, float min, float max, const std::string& format = "%.2f");
	Button* AddButton(const std::string& label, const std::string& button_text, ButtonCallback callback, void* this_ptr);
	Hotkey* AddHotkey(const std::string& label, int* value);
	DropdownWidget* AddDropdown(const std::string& label, XyVector<std::string>* options, int* value);
	ColorPickerWidget* AddColorpicker(const std::string& label, XyColor* value, bool hasAlpha);
	ColorPicker* AddColorpickerOverlay(XyColor* value, bool hasAlpha);

private:
	std::string Label;
	XyVector<Widget*> Widgets;
	XyVector<Overlay*> Overlays;
	float WidgetPos = 0.f;
	float LastChildWindowSize = 0.f;
	float height = 0.f;

private:
	int Selected = -1;
	int Hovered = -1;
	int Opened = -1;
};