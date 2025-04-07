#include "ChildTab.h"
#include "../Vane.h"
#include "Sidebar.h"



float ChildTab::Render(float x, float y, float alpha, int id, int* hovered, int* selected)
{
	animation_selected = Vane::Util::Lerp(animation_selected, *selected == id ? 1.f : 0.f, Vane::Style::AnimationSpeed / 1.5f);
	animation_hovered = Vane::Util::Lerp(animation_hovered, *hovered == id ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	float offset = -animation_hovered * Vane::Style::Spacing / 6.f + animation_selected * Vane::Style::Spacing / 8.f;

	lastX = x + Vane::Style::Spacing + offset;
	lastY = y + Vane::Style::Spacing / 2.f + offset;
	lastH = Vane::Style::TopBarHeight - Vane::Style::Spacing - offset * 2.f;
	lastW = lastH + (Vane::Style::ChildTabWidth - lastH) * animation_selected - offset * 2.f;

	// Background
	Vane::renderer.AddRectFilled(
		XyVec2(lastX, lastY), XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Util::InterpCol(Vane::Style::Background, XyColor(1.f, 1.f, 1.f), animation_hovered * 0.25f + animation_selected * 0.1f), 0.5f * alpha),
		Vane::Style::Rounding / 2.f);

	// Icon
	if (Icon && *Icon)
	{
		Vane::renderer.AddImage(
			*Icon,
			XyVec2(lastX + Vane::Style::BorderSize, lastY + Vane::Style::BorderSize), XyVec2(lastX + lastH - Vane::Style::BorderSize, lastY + lastH - Vane::Style::BorderSize),
			XyVec2(0, 0), XyVec2(1, 1),
			Vane::Util::ConvColor(Vane::Style::Icon, alpha));
	}

	// Text
	if (lastW > lastH)
	{
		Vane::renderer.PushClipRect(XyVec2(lastX + lastH, lastY), XyVec2(lastX + lastW, lastY + lastH), true);

		Vane::renderer.AddText(
			XyVec2(lastX + lastH + 1.f, lastY + (lastH - Vane::renderer.FontSize) / 2.f),
			Vane::Util::ConvColor(Vane::Style::Text, alpha), Name.c_str());

		Vane::renderer.PopClipRect();
	}

	// Border
	Vane::renderer.AddRect(
		XyVec2(lastX, lastY), XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Util::InterpCol(Vane::Style::Accent, Vane::Style::Accent2, animation_selected * 0.7f), alpha),
		Vane::Style::Rounding / 2.f, 0, Vane::Style::BorderSize);

	bool active = id == *selected;
	
	if (active != last_active)
	{
		last_active = active;

		if (active)
		{
			for (auto& widget : Widgets)
			{
				if (widget->endGroup || widget->isTitle) // Little Optimization
					continue;

				widget->InitAnimation();
			}

			ScrollSpeed = DesScroll = Scroll = 0.f;
		}
	}

	if (!active)
		return lastW + Vane::Style::Spacing;

	//alpha *= animation_selected;

	// Widgets
	float posX = Vane::x + Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * Sidebar::animation + Vane::Style::Spacing;
	float full_width = Vane::w - (Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * Sidebar::animation) - Vane::Style::Spacing * 2.f;
	float area_width = full_width / NumAreas;

	// Find Highest
	float highest = 0.f;
	for (int i = 0; i < NumAreas; i++)
	{
		PositionsAreas[i] = 0.f;
	}

	bool endGroupLast = false;
	float last_height = 0.f;
	for (int i = 0; i < Widgets.Size; i++)
	{
		bool endGroup = Widgets[i]->endGroup;
		if (endGroup)
			PositionsAreas[Widgets[i]->area] += Vane::Style::Spacing;
		else if (Widgets[i]->isTitle)
			PositionsAreas[Widgets[i]->area] += Widgets[i]->default_height / 2.f;
		else
			PositionsAreas[Widgets[i]->area] += Widgets[i]->default_height + Vane::Style::Spacing;

		if (PositionsAreas[Widgets[i]->area] > highest)
		{
			endGroupLast = endGroup;
			if (!endGroup)
				last_height = Widgets[i]->default_height;
			highest = PositionsAreas[Widgets[i]->area];
		}
	}

	highest += Vane::Style::Spacing * 2.f;
	if (endGroupLast)
		highest -= Vane::Style::Spacing;

	highest -= last_height + Vane::Style::Spacing;

	// Scrolling
	DesScroll -= ScrollSpeed * Vane::renderer.data.delta_time * Vane::Style::AnimationSpeed * 2.5f;
	ScrollSpeed = Vane::Util::Lerp(ScrollSpeed, 0.f, Vane::Style::AnimationSpeed * 2.f);

	if (DesScroll < 0.f)
		DesScroll = Vane::Util::Lerp(DesScroll, 0.f, Vane::Style::AnimationSpeed * 2.3f);

	if (DesScroll > highest)
		DesScroll = Vane::Util::Lerp(DesScroll, highest, Vane::Style::AnimationSpeed * 2.3f);

	Scroll = Vane::Util::Lerp(Scroll, DesScroll, Vane::Style::AnimationSpeed * 2.f);

	// Render
	Vane::renderer.PushClipRect(XyVec2(posX - Vane::Style::Spacing, Vane::y + Vane::Style::TopBarHeight + Vane::Style::BorderSize),
		XyVec2(Vane::x + Vane::w - Vane::Style::BorderSize,Vane::y + Vane::h - Vane::Style::BorderSize), true);

	// Reset positions
	for (int i = 0; i < NumAreas; i++)
	{
		PositionsAreas[i] = Vane::y + Vane::Style::TopBarHeight + Vane::Style::Spacing * 2.f - Scroll;
		SizesAreas[i].x = PositionsAreas[i];
	}

	// Areas
	for (int i = 0; i < Widgets.Size; i++)
	{
		if (!Widgets[i]->endGroup)
		{
			if (Widgets[i]->isTitle)
				PositionsAreas[Widgets[i]->area] += Widgets[i]->default_height / 2.f;
			else
				PositionsAreas[Widgets[i]->area] += Widgets[i]->default_height + Vane::Style::Spacing;
			continue;
		}

		SizesAreas[Widgets[i]->area].y = PositionsAreas[Widgets[i]->area];
		if (abs(SizesAreas[Widgets[i]->area].x - SizesAreas[Widgets[i]->area].y) <= 1.f)
			continue;

		if (SizesAreas[Widgets[i]->area].y > Vane::y + Vane::Style::TopBarHeight)
			Vane::renderer.AddRectFilled(
				XyVec2(posX + area_width * Widgets[i]->area + Vane::Style::Spacing - Vane::Style::Spacing / 3.f, SizesAreas[Widgets[i]->area].x),
				XyVec2(posX + area_width * Widgets[i]->area + area_width - Vane::Style::Spacing + Vane::Style::Spacing / 3.f, SizesAreas[Widgets[i]->area].y),
				Vane::Util::ConvColor(Vane::Style::Background, alpha * 0.85f),
				Vane::Style::Rounding / 2.f);

		SizesAreas[Widgets[i]->area].x = SizesAreas[Widgets[i]->area].y + Vane::Style::Spacing;

		PositionsAreas[Widgets[i]->area] += Vane::Style::Spacing;
	}

	// End Areas
	for (int i = 0; i < NumAreas; i++)
	{
		SizesAreas[i].y = PositionsAreas[i];

		if (abs(SizesAreas[i].x - SizesAreas[i].y) <= 1.f)
			continue;

		if (SizesAreas[i].y < Vane::y + Vane::Style::TopBarHeight)
			continue;

		Vane::renderer.AddRectFilled(
			XyVec2(posX + area_width * i + Vane::Style::Spacing - Vane::Style::Spacing / 3.f, SizesAreas[i].x),
			XyVec2(posX + area_width * i + area_width - Vane::Style::Spacing + Vane::Style::Spacing / 3.f, SizesAreas[i].y),
			Vane::Util::ConvColor(Vane::Style::Background, alpha * 0.85f),
			Vane::Style::Rounding / 2.f);
	}

	// Reset positions
	for (int i = 0; i < NumAreas; i++)
	{
		PositionsAreas[i] = Vane::y + Vane::Style::TopBarHeight + Vane::Style::Spacing * 2.f - Scroll;
	}

	// Render
	for (int i = 0; i < Widgets.Size; i++)
	{
		if (Widgets[i]->endGroup)
		{
			PositionsAreas[Widgets[i]->area] += Vane::Style::Spacing;
			continue;
		}
		else if (Widgets[i]->isTitle)
		{

			if (PositionsAreas[Widgets[i]->area] + Widgets[i]->default_height / 2.f > Vane::y + Vane::Style::TopBarHeight)
				Widgets[i]->Draw(posX + area_width * Widgets[i]->area + Vane::Style::Spacing, PositionsAreas[Widgets[i]->area] - Widgets[i]->default_height / 2.f, 
					area_width - Vane::Style::Spacing * 2.f, alpha, i, &Hovered, &Selected, &Vane::OpenedOverlay);

			PositionsAreas[Widgets[i]->area] += Widgets[i]->default_height / 2.f;
			continue;
		}

		if (PositionsAreas[Widgets[i]->area] + Widgets[i]->default_height > Vane::y + Vane::Style::TopBarHeight)
			Widgets[i]->Draw(posX + area_width * Widgets[i]->area + Vane::Style::Spacing, PositionsAreas[Widgets[i]->area], 
				area_width - Vane::Style::Spacing * 2.f, alpha, i, &Hovered, &Selected, &Vane::OpenedOverlay);

		PositionsAreas[Widgets[i]->area] += Widgets[i]->default_height + Vane::Style::Spacing;
	}

	Vane::renderer.PopClipRect();

	return lastW + Vane::Style::Spacing;
}

std::optional<long> ChildTab::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected)
{
	float x = (float)(int16_t)LOWORD(lParam);
	float y = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(x, y, lastX, lastY, lastW, lastH))
		{
			*selected = id;
			return S_OK;
		}
	}

	else if (msg == WM_MOUSEMOVE)
	{
		if (isInRect(x, y, lastX, lastY, lastW, lastH))
		{
			*hovered = id;
			Vane::Cursor::current = Vane::Cursor::hand;
			return S_OK;
		}
	}

	if (id != *selected)
		return {};

	// Scroll
	if (msg == WM_MOUSEWHEEL)
	{
		float posX = Vane::x + Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * Sidebar::animation;

		if (isInRect2(x, y, posX, Vane::y + Vane::Style::TopBarHeight, Vane::x + Vane::w, Vane::y + Vane::h))
		{
			ScrollSpeed += GET_WHEEL_DELTA_WPARAM(wParam);
			constexpr float MAX_SCROLL_SPEED = 140.f;
			if (ScrollSpeed > MAX_SCROLL_SPEED) 
				ScrollSpeed = MAX_SCROLL_SPEED;
			else if (ScrollSpeed < -MAX_SCROLL_SPEED)
				ScrollSpeed = -MAX_SCROLL_SPEED;
			return S_OK;
		}
	}

	// Widgets
	int hovered_temp = -1; // Prevent visual (small) issues when multithreading
	for (int i = 0; i < Widgets.Size; i++)
	{
		if (Widgets[i]->endGroup || Widgets[i]->isTitle)
			continue;

		auto ret = Widgets[i]->WndProc(msg, wParam, lParam, i, &hovered_temp, &Selected, &Vane::OpenedOverlay);
		if (ret.has_value())
		{
			Hovered = hovered_temp;
			return ret;
		}
	}
	Hovered = hovered_temp;

	return {};
}

void ChildTab::EndGroup(int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Widget());

	Widgets.back()->endGroup = true;
	Widgets.back()->area = area;
}

void ChildTab::AddGroupTitle(const std::string& title, int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widget* lastWidgetInArea = nullptr;
	for (auto& widget : Widgets)
	{
		if (widget->area == area)
			lastWidgetInArea = widget;
	}

	XY_ASSERT(!lastWidgetInArea || lastWidgetInArea->endGroup && "Must end/begin group first, call EndGroup()");

	Widgets.push_back(new GroupTitle{ title });

	Widgets.back()->area = area;
}

Checkbox* ChildTab::AddCheckbox(const std::string& label, bool* value, int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Checkbox{ label, value });

	Widgets.back()->area = area;
	Widgets.back()->pOverlays = &Vane::Overlays;

	return (Checkbox*)Widgets.back();
}

Slider* ChildTab::AddSlider(const std::string& label, float* value, float min, float max, int area, const std::string& format)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Slider{ label, value, min, max, format });

	Widgets.back()->area = area;
	Widgets.back()->pOverlays = &Vane::Overlays;

	return (Slider*)Widgets.back();
}

Button* ChildTab::AddButton(const std::string& label, const std::string& button_text, ButtonCallback callback, void* this_ptr, int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Button{ label, button_text, callback, this_ptr });

	Widgets.back()->area = area;
	Widgets.back()->pOverlays = &Vane::Overlays;

	return (Button*)Widgets.back();
}

Hotkey* ChildTab::AddHotkey(const std::string& label, int* value, int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Widgets.push_back(new Hotkey{ label, value });

	Widgets.back()->area = area;
	Widgets.back()->pOverlays = &Vane::Overlays;

	return (Hotkey*)Widgets.back();
}

DropdownWidget* ChildTab::AddDropdown(const std::string& label, XyVector<std::string>* options, int* value, int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	// Create Overlay
	Vane::Overlays.push_back(new Dropdown{ options, value });

	Widgets.push_back(new DropdownWidget{ label });

	Widgets.back()->area = area;
	Widgets.back()->pOverlays = &Vane::Overlays;
	Widgets.back()->child_id = Vane::Overlays.Size - 1;

	return (DropdownWidget*)Widgets.back();
}

ColorPickerWidget* ChildTab::AddColorpicker(const std::string& label, XyColor* value, bool hasAlpha, int area)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	// Create Overlay
	Vane::Overlays.push_back(new ColorPicker{ value, hasAlpha });

	Widgets.push_back(new ColorPickerWidget{ label });

	Widgets.back()->area = area;
	Widgets.back()->pOverlays = &Vane::Overlays;
	Widgets.back()->child_id = Vane::Overlays.Size - 1;

	return (ColorPickerWidget*)Widgets.back();
}

ColorPicker* ChildTab::AddColorpickerOverlay(XyColor* value, bool hasAlpha)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Vane::Overlays.push_back(new ColorPicker{ value, hasAlpha });

	return (ColorPicker*)Vane::Overlays.back();
}

ChildWindow* ChildTab::AddChildWindow(const std::string& label)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Vane::Overlays.push_back(new ChildWindow{ label });

	return (ChildWindow*)Vane::Overlays.back();
}

void ChildTab::SetIcon(const unsigned char* data, size_t size)
{
	XY_ASSERT(!Vane::RenderingStarted && !Vane::Initialized);

	Icon = new XyTextureID;
	*Icon = 0;

	Vane::Icons::icons.emplace_back(data, size, Icon);
}