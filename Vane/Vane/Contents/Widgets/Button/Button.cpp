#include "Button.h"
#include "../../../Vane.h"

void Button::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened)
{
	lastX = x;
	lastY = y;
	lastW = w;
	lastH = default_height;

	// Animations
	animation_selected = Vane::Util::Lerp(animation_selected, *selected == id ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_hovered = Vane::Util::Lerp(animation_hovered, *hovered == id ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_disabled = Vane::Util::Lerp(animation_disabled, (disabled && (*disabled == !dis_inv)) ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	if (alpha < Vane::MinAlpha)
		return;

	// Text
	Vane::renderer.AddText(
		XyVec2(x, y + (lastH - Vane::Style::TextSize) / 2),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled), strLabel.c_str()
	);


	const float size = w / 2.2f;
	//

	float offset_x = default_height / 15.f * animation_hovered * (1.f - animation_selected * 0.5f);
	float offset_y = offset_x * ((lastH - 4.f) / size);

	// Background
	Vane::renderer.AddRectFilled(
		XyVec2(x + w - size - 3.f - offset_x, y + 2.f - offset_y),
		XyVec2(x + w - 3.f + offset_x, y + lastH - 2.f + offset_y),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent2, alpha), animation_disabled), Vane::Style::Rounding / 2.f
	);

	// Border
	Vane::renderer.AddRect(
		XyVec2(x + w - size - 3.f - offset_x, y + 2.f - offset_y),
		XyVec2(x + w - 3.f + offset_x, y + lastH - 2.f + offset_y),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * (0.5f + animation_selected * 0.5f)), animation_disabled), Vane::Style::Rounding / 2.f, 0, Vane::Style::BorderSize);

	// Text
	Vane::renderer.PushClipRect(
		XyVec2(x + w - size - 3.f - offset_x, y + 2.f - offset_y),
		XyVec2(x + w - 3.f + offset_x, y + lastH - 2.f + offset_y), true
	);

	if (!text_size_calculated)
	{
		text_size = Vane::renderer.CalcTextWidth(strButtonText.c_str());
		text_size_calculated = true;
	}

	Vane::renderer.AddText(
		XyVec2(x + w - size / 2.f - 3.f - text_size / 2.f, y + 2.f + (lastH - 4.f - Vane::Style::TextSize) / 2.f),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled),
		strButtonText.c_str()
	);

	Vane::renderer.PopClipRect();
}

std::optional<long> Button::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened)
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

	if (*selected == id && msg == WM_LBUTTONUP)
	{
		*selected = -1;
		pCallback(pThisPtr);
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
				Vane::Cursor::current = Vane::Cursor::hand;
			}
			else if (msg == WM_LBUTTONDOWN)
			{
				*hovered = id;
				*selected = id;
			}
			return S_OK;
		}
		else if (msg == WM_MOUSEMOVE && *hovered == id)
		{
			*hovered = -1;
		}
	}
	return {};
}