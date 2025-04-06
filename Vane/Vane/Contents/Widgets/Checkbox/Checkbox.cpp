#include "checkbox.h"
#include "../../../Vane.h"
#include "../colorpicker/colorpicker.h"

#define HOVER_OFF_CHILD 100000

void Checkbox::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened)
{
	lastX = x;
	lastY = y;
	lastW = w;
	lastH = default_height;

	// Animations
	animation_selected = Vane::Util::Lerp(animation_selected, *pValue ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_hovered = Vane::Util::Lerp(animation_hovered, id == *hovered ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_hovered_child = Vane::Util::Lerp(animation_hovered_child, id + HOVER_OFF_CHILD == *hovered ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_disabled = Vane::Util::Lerp(animation_disabled, (disabled && (*disabled == !dis_inv)) ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	if (alpha < Vane::MinAlpha)
		return;

	// Text
	Vane::renderer.AddText(
		XyVec2(x, y + (lastH - Vane::Style::TextSize) / 2), 
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled), strLabel.c_str()
	);

	float size = default_height + 3.f;

	// Child
	if (child_id != -1)
	{
		const float offset = animation_hovered_child * 1.5f;
		if (pOverlays->at(child_id)->isColorPicker)
		{
			Vane::renderer.AddRectFilled(
				XyVec2(x + w - size - 7.f - lastH + size / 2.5f - offset, y + size / 5.f - offset),
				XyVec2(x + w - size - 7.f + offset, y + lastH - size / 5.f + offset),
				Vane::Util::DisableColor(Vane::Util::ConvColor(((ColorPicker*)pOverlays->at(child_id))->getColorIA(), alpha * (0.5f + animation_hovered_child * 0.5f)), animation_disabled),
				Vane::Style::Rounding / 4.f
			);

			Vane::renderer.AddRect(
				XyVec2(x + w - size - 7.f - lastH + size / 2.5f - offset, y + size / 5.f - offset),
				XyVec2(x + w - size - 7.f + offset, y + lastH - size / 5.f + offset),
				Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * (0.5f + animation_hovered_child * 0.5f)), animation_disabled),
				Vane::Style::Rounding / 4.f, 0, Vane::Style::BorderSize
			);
		}
		else if (pOverlays->at(child_id)->isChildWindow) 
		{
			Vane::renderer.AddImage(Vane::Icons::ChildIcon,
				XyVec2(x + w - size - 7.f - lastH + size / 2.5f - offset, y + size / 5.f - offset),
				XyVec2(x + w - size - 7.f + offset, y + lastH - size / 5.f + offset),
				XyVec2(0, 0), XyVec2(1, 1),
				Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Highlight, alpha * (0.5f + animation_hovered_child * 0.5f)), animation_disabled)
			);
		}
	}

	// Toggle
	Vane::renderer.AddRectFilled(XyVec2(
		x + w - size - 3.f,
		y + size / 5.f
	), XyVec2(
		x + w - 3.f,
		y + lastH - size / 5.f
	), Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent2, alpha), animation_disabled), Vane::Style::Rounding
	);

	Vane::renderer.AddRect(XyVec2(
		x + w - size - 3.f,
		y + size / 5.f
	), XyVec2(
		x + w - 3.f,
		y + lastH - size / 5.f
	), Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * (0.5f + animation_hovered * 0.5f)), animation_disabled), Vane::Style::Rounding, 0, Vane::Style::BorderSize
	);

	float animation_value = 0.5f - abs(0.5f - animation_selected);

	float height = lastH - size / 2.5f;
	Vane::renderer.AddRectFilled(XyVec2(
		x + w - size - 2.f + height * animation_selected + animation_value,
		y + size / 5.f + 1.f + animation_value
	), XyVec2(
		x + w - size - 4.f + height + height * animation_selected - animation_value,
		y + lastH - size / 5.f - 1.f - animation_value
	), Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Highlight, alpha), animation_disabled), Vane::Style::Rounding
	);
}

std::optional<long> Checkbox::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened)
{
	if (disabled && (*disabled == !dis_inv))
	{
		if (*selected == id)
			*selected = -1;
		return {};
	}

	if (*selected != -1 && *selected != id)
		return {};

	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_LBUTTONDOWN)
	{
		float size = default_height + 3.f;
		if (isInRect(mouseX, mouseY, lastX + lastW - size - 3.f, lastY + size / 5.f, size, lastH - size / 2.5f))
		{
			*selected = id;
			selected_type = false;
			return S_OK;
		}
		// Child
		if (child_id != -1 && isInRect(mouseX, mouseY, lastX + lastW - size - 7.f - lastH + size / 2.5f, lastY + size / 5.f, lastH - size / 2.5f, lastH - size / 2.5f))
		{
			*selected = id;
			selected_type = true;
			return S_OK;
		}
	}

	else if (msg == WM_LBUTTONUP && *selected == id)
	{
		*selected = -1;

		float size = default_height + 3.f;
		if (isInRect(mouseX, mouseY, lastX + lastW - size - 3.f, lastY + size / 5.f, size, lastH - size / 2.5f) && selected_type == false)
		{
			*pValue = !*pValue;
		}
		// Child
		else if (child_id != -1 && isInRect(mouseX, mouseY, lastX + lastW - size - 7.f - lastH + size / 2.5f, lastY + size / 5.f, lastH - size / 2.5f, lastH - size / 2.5f) && selected_type == true)
		{
			pOverlays->at(child_id)->SetPos(lastX + lastW - size - 7.f - size / 5.f - Vane::x, lastY + size / 2 - Vane::y);
			*opened = child_id;
		}

		return S_OK;
	}

	else if (msg == WM_MOUSEMOVE) 
	{
		if (*selected == id)
		{
			*hovered = id;
			Vane::Cursor::current = Vane::Cursor::hand;
			return S_OK;
		}
		float size = default_height + 3.f;
		if (isInRect(mouseX, mouseY, lastX + lastW - size - 3.f, lastY + size / 5.f, size, lastH - size / 2.5f))
		{
			*hovered = id;
			Vane::Cursor::current = Vane::Cursor::hand;
			return S_OK;
		}
		else if (child_id != -1 && isInRect(mouseX, mouseY, lastX + lastW - size - 7.f - lastH + size / 2.5f, lastY + size / 5.f, lastH - size / 2.5f, lastH - size / 2.5f)) 
		{
			*hovered = id + HOVER_OFF_CHILD;
			Vane::Cursor::current = Vane::Cursor::hand;
			return S_OK;
		}
	}
	return {};
}

void Checkbox::InitAnimation()
{
	Widget::InitAnimation();

	animation_selected = *pValue ? 1.f : 0.f;
	animation_hovered_child = 0.f;
}

Checkbox* Checkbox::LinkTo(Overlay* overlay)
{
	child_id = pOverlays->size() - 1;

	return this;
}
