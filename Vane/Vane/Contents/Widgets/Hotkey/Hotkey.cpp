#include "hotkey.h"
#include "../../../Vane.h"

void Hotkey::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened)
{
	lastX = x;
	lastY = y;
	lastW = w;
	lastH = default_height;

	// Animations
	animation_hovered = Vane::Util::Lerp(animation_hovered, id == *hovered ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_disabled = Vane::Util::Lerp(animation_disabled, (disabled && (*disabled == !dis_inv)) ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	if (alpha < Vane::MinAlpha)
		return;

	// Text
	Vane::renderer.AddText(
		XyVec2(x, y + (lastH - Vane::Style::TextSize) / 2),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled), strLabel.c_str());


	const float size = w / 2.2f;
	//

	// Background
	Vane::renderer.AddRectFilled(
		XyVec2(x + w - size - 3.f, y + 2.f),
		XyVec2(x + w - 3.f, y + lastH - 2.f),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent2, alpha), animation_disabled), Vane::Style::Rounding / 2.f);

	// Border
	Vane::renderer.AddRect(
		XyVec2(x + w - size - 3.f, y + 2.f),
		XyVec2(x + w - 3.f, y + lastH - 2.f),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * (0.5f + animation_hovered * 0.5f)), animation_disabled), Vane::Style::Rounding / 2.f, 0, Vane::Style::BorderSize);

	// Text
	Vane::renderer.PushClipRect(
		XyVec2(x + w - size - 3.f, y + 2.f),
		XyVec2(x + w - 3.f, y + lastH - 2.f), true
	);

	if (old_value != *pValue || text_size == 0.f) 
	{
		int scan_code = MapVirtualKey(*pValue, MAPVK_VK_TO_VSC);

		if (GetKeyNameTextA(scan_code << 16, buffer, sizeof(buffer)) == 0)
		{
			memcpy(buffer, "none", 5);
		}
		text_size = Vane::renderer.CalcTextWidth(buffer);

		old_value = *pValue;
	}
	// Get Key Name
	if (text_size > 0.f)
		Vane::renderer.AddText(
			XyVec2(x + w - size / 2.f - 3.f - text_size / 2.f, y + 2.f + (lastH - 4.f - Vane::Style::TextSize) / 2.f),
			Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled),
			buffer);

	// Typing
	if (Vane::renderer.data.curr_time - last_tick > 500)
	{
		last_tick = Vane::renderer.data.curr_time;
		ticking = !ticking;
	}

	animation_typing = Vane::Util::Lerp(animation_typing, (ticking && *selected == id) ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	Vane::renderer.AddRectFilled(
		XyVec2(x + w - size / 2.f - 3.f - text_size / 2.f, y + 2.f + (lastH - 4.f - Vane::Style::TextSize) / 2.f + Vane::Style::TextSize),
		XyVec2(x + w - size / 2.f - 3.f + text_size / 2.f, y + 2.f + (lastH - 4.f - Vane::Style::TextSize) / 2.f + Vane::Style::TextSize + 2.f),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * animation_typing), animation_disabled));

	Vane::renderer.PopClipRect();
}

std::optional<long> Hotkey::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened)
{
	if (disabled && (*disabled == !dis_inv))
	{
		if (*selected == id)
			*selected = -1;
		if (*hovered == id)
			*hovered = -1;
		return {};
	}

	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

	if (*selected == id && wParam == SC_KEYMENU && (lParam >> 16) <= 0)
	{
		*pValue = VK_LMENU;
		*selected = -1;
		return S_OK;
	}

	if (*selected == id && msg == WM_KEYDOWN)
	{
		if (wParam == VK_BACK)
			*pValue = 0;
		else if (wParam != VK_ESCAPE)
			*pValue = (int)wParam;
		*selected = -1;
		return S_OK;
	}

	if (msg == WM_LBUTTONDOWN || msg == WM_MOUSEMOVE)
	{
		const float size = lastW / 2.2f;
		if (isInRect(mouseX, mouseY, lastX + lastW - size - 3.f, lastY + 2.f, size, lastH - 4.f))
		{
			if (msg == WM_MOUSEMOVE)
			{
				*hovered = id;
				Vane::Cursor::current = Vane::Cursor::text;
			}
			else if (msg == WM_LBUTTONDOWN)
			{
				ticking = true;
				*hovered = id;
				*selected = id;
			}
			return S_OK;
		}
		else if (msg == WM_MOUSEMOVE && *hovered == id)
		{
			*hovered = -1;
		}
		else if (msg == WM_LBUTTONDOWN && *selected == id)
		{
			*selected = -1;
		}
	}
	return {};
}

void Hotkey::InitAnimation()
{
	Widget::InitAnimation();

	animation_typing = 0.f;
}
