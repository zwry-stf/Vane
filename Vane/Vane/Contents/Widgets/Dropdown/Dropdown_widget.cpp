#include "dropdown.h"
#include "../../../Vane.h"

void DropdownWidget::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) 
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

	int _selected = *((Dropdown*)pOverlays->at(child_id))->pSelected;
	if (_selected >= ((Dropdown*)pOverlays->at(child_id))->pOptions->size())
		_selected = 0;

	Vane::renderer.AddText(
		XyVec2(x + w - size, y + 2.f + (lastH - 4.f - Vane::Style::TextSize) / 2.f), 
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled),
		((Dropdown*)pOverlays->at(child_id))->pOptions->at(_selected).c_str());

	Vane::renderer.PopClipRect();
}

std::optional<long> DropdownWidget::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened)
{
	if (disabled && (*disabled == !dis_inv))
	{
		if (*hovered == id)
			*hovered = -1;
		return {};
	}

	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

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
				pOverlays->at(child_id)->SetPos(lastX + lastW - size - 3.f - Vane::x, lastY + lastH - 2.f - Vane::y);
				((Dropdown*)pOverlays->at(child_id))->Width = size;
				*opened = child_id;
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