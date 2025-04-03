#include "ChildWindow.h"
#include "../Vane.h"

void ChildWindow::Draw(const int id, int* opened)
{
	lastX = Vane::x + xPos;
	lastY = Vane::y + yPos;
	lastW = Vane::Style::ChildWindowWidth;
	lastH = Height;

	// Animation
	animation = Vane::Util::Lerp(animation, id == *opened ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	if (animation < Vane::MinAlpha)
		return;

	if (!size_init)
	{
		Height = 0.f;
		for (int i = 0; i < Widgets.size(); i++)
		{
			Height += Widgets[i]->default_height + Vane::Style::Spacing;
		}

		Height -= Vane::Style::Spacing;
		Height += Vane::Style::TextSize + 25.f;

		lastH = Height;
	}

	if (pos_changed)
	{
		pos_changed = false;
		if (lastX + lastW > Vane::renderer.data.display_size.x)
		{
			lastX -= lastW;
			xPos -= lastW;
		}
		if (lastY + lastH > Vane::renderer.data.display_size.y)
		{
			lastY -= lastH;
			yPos -= lastH;
		}
	}


	// Background
	Vane::renderer.AddShadowRect(
		XyVec2(lastX, lastY),
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::Background, animation), Vane::Style::Rounding
	);

	Vane::renderer.AddRectFilled(
		XyVec2(lastX, lastY),
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::Background, animation), Vane::Style::Rounding
	);

	// Text
	Vane::renderer.AddText(
		XyVec2(lastX + std::max<float>(2.f, Vane::Style::Rounding / 2.f), lastY + 2.f),
		Vane::Util::ConvColor(Vane::Style::Text, animation), Label.c_str()
	);

	// Top Line
	Vane::renderer.AddRectFilled(
		XyVec2(lastX + Vane::Style::BorderSize, lastY + Vane::Style::TextSize + 5.f),
		XyVec2(lastX + lastW - Vane::Style::BorderSize, lastY + Vane::Style::TextSize + 5.f + Vane::Style::BorderSize),
		Vane::Util::ConvColor(Vane::Style::Accent, 0.5f * animation));

	// Render Widgets
	WidgetPos = lastY + Vane::Style::TextSize + 15.f;

	for (int i = 0; i < Widgets.size(); i++)
	{
		Widgets[i]->Draw(
			lastX + 10.f, WidgetPos, lastW - 20.f, animation,
			i, &this->Hovered, &this->Selected, &this->Opened);

		WidgetPos += Widgets[i]->default_height + Vane::Style::Spacing;
	}


	// Border
	Vane::renderer.AddRect(
		XyVec2(lastX, lastY),
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::Accent, 0.5f * animation), Vane::Style::Rounding, 0, Vane::Style::BorderSize);

	// Overlays
	for (int i = 0; i < Overlays.Size; i++)
	{
		Overlays[i]->Draw(i, &Opened);
	}
}

std::optional<long> ChildWindow::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened)
{
	float mouseX = (float)LOWORD(lParam);
	float mouseY = (float)HIWORD(lParam);

	if (Opened != -1)
	{
		auto ret = Overlays[Opened]->WndProc(msg, wParam, lParam, Opened, &Opened);
		if (ret.has_value())
			return ret;
	}

	if (msg == WM_MOUSEMOVE)
		Hovered = -1;

	for (int i = 0; i < Widgets.size(); i++)
	{
		if (Widgets[i]->WndProc(msg, wParam, lParam, i, &this->Hovered, &this->Selected, &this->Opened))
			return S_OK;
	}

	if (msg == WM_LBUTTONDOWN && !isInRect(mouseX, mouseY, lastX, lastY, lastW, lastH)) 
	{
		*opened = -1;
		return S_OK;
	}

	if (msg == WM_MOUSEMOVE && !isInRect(mouseX, mouseY, lastX, lastY, lastW, lastH) && this->Selected == -1 && this->Hovered == -1 && this->Opened == -1) 
	{
		return S_OK;
	}

	return (msg == WM_LBUTTONDOWN || msg == WM_MOUSEMOVE || msg == WM_LBUTTONUP || msg == WM_MOUSEWHEEL) ? S_OK : std::optional<long>();
}

Checkbox* ChildWindow::AddCheckbox(const std::string& label, bool* value)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Checkbox{ label, value });

	Widgets.back()->pOverlays = &Overlays;

	return (Checkbox*)Widgets.back();
}

Slider* ChildWindow::AddSlider(const std::string& label, float* value, float min, float max, const std::string& format)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Slider{ label, value, min, max, format });

	Widgets.back()->pOverlays = &Overlays;

	return (Slider*)Widgets.back();
}

Button* ChildWindow::AddButton(const std::string& label, const std::string& button_text, ButtonCallback callback, void* this_ptr)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Button{ label, button_text, callback, this_ptr });

	Widgets.back()->pOverlays = &Overlays;

	return (Button*)Widgets.back();
}

Hotkey* ChildWindow::AddHotkey(const std::string& label, int* value)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Hotkey{ label, value });

	Widgets.back()->pOverlays = &Overlays;

	return (Hotkey*)Widgets.back();
}

DropdownWidget* ChildWindow::AddDropdown(const std::string& label, XyVector<std::string>* options, int* value)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	// Create Overlay
	Overlays.push_back(new Dropdown{ options, value });

	Widgets.push_back(new DropdownWidget{ label });

	Widgets.back()->pOverlays = &Overlays;
	Widgets.back()->child_id = Overlays.Size - 1;

	return (DropdownWidget*)Widgets.back();
}

ColorPickerWidget* ChildWindow::AddColorpicker(const std::string& label, XyColor* value, bool hasAlpha)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	// Create Overlay
	Overlays.push_back(new ColorPicker{ value, hasAlpha });

	Widgets.push_back(new ColorPickerWidget{ label });

	Widgets.back()->pOverlays = &Overlays;
	Widgets.back()->child_id = Overlays.Size - 1;

	return (ColorPickerWidget*)Widgets.back();
}

ColorPicker* ChildWindow::AddColorpickerOverlay(XyColor* value, bool hasAlpha)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Overlays.push_back(new ColorPicker{ value, hasAlpha });

	return (ColorPicker*)Overlays.back();
}