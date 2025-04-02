#include "ConfigTab.h"
#include "../Vane.h"
#include "Sidebar.h"

void TabConfig::Render(float x, float y, float w, int id, int* hovered, int* selected)
{
	Tab::Render(x, y, w, id, hovered, selected);

	bool active = id == Vane::SelectedTab;
	if (last_active != active)
	{
		last_active = active;

		if (active)
		{
			Vane::Config::FetchConfigs();

			for (auto& cfg : Vane::Config::configs)
			{
				cfg.InitAnimation();
			}

			ScrollSpeed = DesScroll = Scroll = 0.f;
		}
	}

	if (id != Vane::SelectedTab)
		return;

	float xpos = Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * Sidebar::animation;
	float highest = (Vane::Config::configs.size() - 1) * (ConfigDrawable::default_height + Vane::Style::Spacing);

	// Scrolling
	DesScroll -= ScrollSpeed * Vane::renderer.data.delta_time * Vane::Style::AnimationSpeed * 2.5f;
	ScrollSpeed = Vane::Util::Lerp(ScrollSpeed, 0.f, Vane::Style::AnimationSpeed * 2.f);

	if (DesScroll < 0.f)
		DesScroll = Vane::Util::Lerp(DesScroll, 0.f, Vane::Style::AnimationSpeed * 2.3f);

	if (DesScroll > highest)
		DesScroll = Vane::Util::Lerp(DesScroll, highest, Vane::Style::AnimationSpeed * 2.3f);

	Scroll = Vane::Util::Lerp(Scroll, DesScroll, Vane::Style::AnimationSpeed * 2.f);

	float ypos = Vane::Style::Spacing * 2.f - Scroll;

	// Render
	Vane::renderer.PushClipRect(XyVec2(Vane::x + xpos, Vane::y + Vane::Style::BorderSize),
		XyVec2(Vane::x + Vane::w - Vane::Style::BorderSize, Vane::y + Vane::h - Vane::Style::BorderSize), true);

	xpos += Vane::Style::Spacing * 2.f;

	std::lock_guard<std::mutex> lock(Vane::Config::mutex);

	for (int i = 0; i < Vane::Config::configs.size(); i++)
	{
		Vane::Config::configs[i].Render(xpos + Vane::x, Vane::y + ypos, Vane::w - xpos - Vane::Style::Spacing * 2.f, animation, Tab_Begin + i, hovered, selected);

		ypos += ConfigDrawable::default_height + Vane::Style::Spacing;
	}

	ypos -= Vane::Style::Spacing;
	const float add_button_size = ConfigDrawable::default_height * 0.5f;
	last_add_renderpos = XyVec4(xpos + Vane::x + Vane::w / 2.f - xpos / 2.f - Vane::Style::Spacing - add_button_size / 2.f,
		Vane::y + ypos + (ConfigDrawable::default_height - add_button_size) / 2.f,
		xpos + Vane::x + Vane::w / 2.f - xpos / 2.f - Vane::Style::Spacing + add_button_size / 2.f,
		Vane::y + ypos + (ConfigDrawable::default_height + add_button_size) / 2.f);

	animation_add_hovered = Vane::Util::Lerp(animation_add_hovered, *hovered == Tab_Begin + 99 ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_add_selected = Vane::Util::Lerp(animation_add_selected, *selected == Tab_Begin + 99 ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	Vane::renderer.AddRectFilled(XyVec2(last_add_renderpos.x, last_add_renderpos.y),
		XyVec2(last_add_renderpos.z, last_add_renderpos.w),
		Vane::Util::ConvColor(
			Vane::Util::InterpCol(Vane::Style::Accent2, XyColor(0.5f, 0.5f, 0.5f, Vane::Style::Accent2.a), animation_add_selected * 0.3f), animation * (animation_add_hovered * 0.3f + animation_add_selected * 0.4f)));

	XyColor color = Vane::Util::ConvColor(
		Vane::Util::InterpCol(Vane::Style::Accent, Vane::Style::Accent2, animation_add_selected * 0.3f + animation_add_hovered * 0.2f));
	Vane::renderer.AddRect(XyVec2(last_add_renderpos.x + 1.f, last_add_renderpos.y + 1.f),
		XyVec2(last_add_renderpos.z - 1.f, last_add_renderpos.w - 1.f),
		color, 0, 2.f);

	Vane::renderer.AddRectFilled(XyVec2(last_add_renderpos.x + add_button_size / 2.f - 1.f, last_add_renderpos.y + 4.f),
		XyVec2(last_add_renderpos.x + add_button_size / 2.f + 1.f, last_add_renderpos.y + add_button_size - 4.f),
		color);

	Vane::renderer.AddRectFilled(XyVec2(last_add_renderpos.x + 4.f, last_add_renderpos.y + add_button_size / 2.f - 1.f),
		XyVec2(last_add_renderpos.x + add_button_size - 4.f, last_add_renderpos.y + add_button_size / 2.f + 1.f),
		color);
	//Vane::renderer.AddImage(Vane::Icons::IconAddCfg, XyVec2(last_add_renderpos.x, last_add_renderpos.y),
	//	XyVec2(last_add_renderpos.z, last_add_renderpos.w),
	//	XyVec2(0, 0), XyVec2(1, 1), Vane::Util::ConvColor(Vane::Style::Accent, animation));


	Vane::renderer.PopClipRect();
}

std::optional<HRESULT> TabConfig::WndProc(UINT msg, WPARAM wParam, LPARAM lParam, int id, int* hovered, int* selected)
{
	auto ret = Tab::WndProc(msg, wParam, lParam, id, hovered, selected);
	if (ret.has_value())
		return ret.value();


	if (id != Vane::SelectedTab)
		return {};

	for (int i = 0; i < Vane::Config::configs.size(); i++)
	{
		auto ret = Vane::Config::configs[i].WndProc(msg, wParam, lParam, Tab_Begin + i, hovered, selected);
		if (ret.has_value())
			return ret.value();
	}

	float x = (float)(int16_t)LOWORD(lParam);
	float y = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_MOUSEMOVE)
	{
		if (isInRect2(x, y, last_add_renderpos.x, last_add_renderpos.y, last_add_renderpos.z, last_add_renderpos.w))
		{
			*hovered = Tab_Begin + 99;
			Vane::Cursor::current = Vane::Cursor::hand;
			return S_OK;
		}
		else if (*hovered == Tab_Begin + 99)
		{
			*hovered = -1;
		}
	}

	else if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect2(x, y, last_add_renderpos.x, last_add_renderpos.y, last_add_renderpos.z, last_add_renderpos.w))
		{
			*selected = Tab_Begin + 99;
			return S_OK;
		}
	}

	else if (msg == WM_LBUTTONUP && *selected == Tab_Begin + 99)
	{
		*selected = -1;

		if (isInRect2(x, y, last_add_renderpos.x, last_add_renderpos.y, last_add_renderpos.z, last_add_renderpos.w))
		{
			Vane::Config::Create();
			return S_OK;
		}
	}

	else if (msg == WM_MOUSEWHEEL)
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

	return {};
}