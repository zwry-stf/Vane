#include "slider.h"
#include "../../../Vane.h"

void Slider::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened)
{
	lastX = x;
	lastY = y;
	lastW = w;
	lastH = default_height;

	// Animations
	animation_selected = Vane::Util::Lerp(animation_selected, *selected == id ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_hovered = Vane::Util::Lerp(animation_hovered, id == *hovered ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_disabled = Vane::Util::Lerp(animation_disabled, (disabled && (*disabled == !dis_inv)) ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	float value = (*pValue - min) / (max - min);
	if (value < 0.f) value = 0.f;
	if (value > 1.f) value = 1.f;
	animation = Vane::Util::Lerp(animation, value, Vane::Style::AnimationSpeed);

	if (alpha < Vane::MinAlpha)
		return;

	// Text
	char buffer[96];  // Large enough for most labels and values

	int length = std::snprintf(buffer, sizeof(buffer), "%s: %s", strLabel.c_str(), strFormat.c_str());

	std::snprintf(buffer + length - strFormat.length(), sizeof(buffer) - length + strFormat.length(), strFormat.c_str(), *pValue);

	Vane::renderer.AddText(XyVec2(x, y), 
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled), buffer);

	// Line
	const float circle_width = default_height - Vane::Style::TextSize - 4.f;
	Vane::renderer.AddRectFilled(
		XyVec2(x + circle_width / 2, y + Vane::Style::TextSize + (lastH - Vane::Style::TextSize) / 2 - circle_width / 5.f),
		XyVec2(x + w - circle_width / 2, y + Vane::Style::TextSize + (lastH - Vane::Style::TextSize) / 2 + circle_width / 5.f), 
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent2, alpha), animation_disabled), std::min(Vane::Style::Rounding / 2.5f, circle_width / 5.f - 1.f));

	Vane::renderer.AddRect(
		XyVec2(x + circle_width / 2, y + Vane::Style::TextSize + (lastH - Vane::Style::TextSize) / 2 - circle_width / 5.f), 
		XyVec2(x + w - circle_width / 2, y + Vane::Style::TextSize + (lastH - Vane::Style::TextSize) / 2 + circle_width / 5.f), 
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * (0.5f + animation_hovered * 0.5f)), animation_disabled), std::min(Vane::Style::Rounding / 2.5f, circle_width / 5.f - 1.f), 0, Vane::Style::BorderSize);

	// Circle
	Vane::renderer.AddCircleFilled(
		XyVec2(x + circle_width / 2 + (lastW - circle_width) * animation, y + Vane::Style::TextSize + (lastH - Vane::Style::TextSize) / 2),
		circle_width / 2, Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Highlight, alpha), animation_disabled));
	
	Vane::renderer.AddCircle(
		XyVec2(x + circle_width / 2 + (lastW - circle_width) * animation, y + Vane::Style::TextSize + (lastH - Vane::Style::TextSize) / 2),
		circle_width / 2 + 0.7f, Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * animation_selected * 0.7f), animation_disabled));
}

std::optional<long>  Slider::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened)
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

	if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(mouseX, mouseY, lastX, lastY + Vane::Style::TextSize, lastW, lastH - Vane::Style::TextSize))
		{
			*selected = id;
			return S_OK;
		}
	}

	else if (msg == WM_LBUTTONUP)
	{
		if (*selected == id) 
		{
			*selected = -1;
			return S_OK;
		}
	}

	else if (msg == WM_MOUSEMOVE) 
	{
		if (*selected == id)
		{
			float circle_width = default_height - Vane::Style::TextSize - 4.f;
			float value = (mouseX - lastX - circle_width/2) / (lastW - circle_width);
			if (value < 0.f) value = 0.f;
			if (value > 1.f) value = 1.f;
			*pValue = min + value * (max - min);

			Vane::Cursor::current = Vane::Cursor::size;

			return S_OK;
		}
		else if (isInRect(mouseX, mouseY, lastX, lastY + Vane::Style::TextSize, lastW, lastH - Vane::Style::TextSize))
		{
			*hovered = id;
			Vane::Cursor::current = Vane::Cursor::size;
			return S_OK;
		}
		else if (*hovered == id)
		{
			*hovered = -1;
		}
	}
	return {};
}

void Slider::InitAnimation()
{
	Widget::InitAnimation();

	animation_selected = 0.f;
	float value = (*pValue - min) / (max - min);
	if (value < 0.f) value = 0.f;
	if (value > 1.f) value = 1.f;
	animation = value;
}
