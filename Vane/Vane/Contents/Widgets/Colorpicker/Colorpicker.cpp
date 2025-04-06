#include "colorpicker.h"
#include "../../../Vane.h"
#include <iostream>

void ColorPicker::loadFromHSV() 
{
	float r, g, b;
	Vane::renderer.ColorConvertHSVtoRGB(hsv.h, hsv.s, hsv.v, r, g, b);
	*pColor = XyColor(r, g, b, hasAlpha ? hsv.a : 1.f);
	oldColor = *pColor;

	// For the hue bar, get the color corresponding to the current hue at full saturation and value
	Vane::renderer.ColorConvertHSVtoRGB(hsv.h, 1.0f, 1.0f, r, g, b);
	hue = XyColor(r, g, b);
}

void ColorPicker::loadFromColor(XyColor color)
{
	float old_hue = hsv.h;
	Vane::renderer.ColorConvertRGBtoHSV(color.r, color.g, color.b, hsv.h, hsv.s, hsv.v);
	if (hsv.s == 0.f)
		hsv.h = old_hue;
	hsv.a = color.a;
}


void ColorPicker::Draw(const int id, int* opened)
{
	lastX = Vane::x + xPos;
	lastY = Vane::y + yPos;
	lastH = round(Vane::Style::ChildWindowWidth / 1.5f);


	// Animation
	animation = Vane::Util::Lerp(animation, id == *opened ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	if (animation < Vane::MinAlpha)
		return;

	space_offset = round(lastH / 12.f);
	square_size = round(lastH - space_offset * 2.f);
	bar_width = round(square_size / 8.5f);

	lastW = square_size + space_offset * (hasAlpha ? 4.f : 3.f) + bar_width * (hasAlpha ? 2.f : 1.f); // space for alpha and hue bar

	// Update Color
	if (oldColor != *pColor) 
	{
		loadFromColor(*pColor); // Will only be called if something else than this class updated the color
		loadFromHSV();
		oldColor = *pColor;
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

	Vane::renderer.PushClipRect(XyVec2(lastX - 50.f, lastY - 50.f), XyVec2(lastX + lastW + 50.f, lastY + lastH + 50.f)); // Remove Main Cliprect

	// Background
	Vane::renderer.AddShadowRect(
		XyVec2(lastX, lastY),
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::CWBackground, animation), Vane::Style::Rounding
	);

	Vane::renderer.AddRectFilled(
		XyVec2(lastX, lastY),
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::CWBackground, animation), Vane::Style::Rounding
	);

	// Border
	Vane::renderer.AddRect(
		XyVec2(lastX, lastY), 
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::Accent, animation), Vane::Style::Rounding, 0, Vane::Style::BorderSize);

	// Color Square
	Vane::renderer.AddRectFilledMultiColor(
		XyVec2(lastX + space_offset, lastY + space_offset), 
		XyVec2(lastX + space_offset + square_size, lastY + space_offset + square_size), 
		Vane::Util::ConvColor(XyColor(1.f, 1.f, 1.f), animation), Vane::Util::ConvColor(XyColor(hue.r, hue.g, hue.b), animation), 
		Vane::Util::ConvColor(XyColor(hue.r, hue.g, hue.b), animation), Vane::Util::ConvColor(XyColor(1.f, 1.f, 1.f), animation)
	);
	
	Vane::renderer.AddRectFilledMultiColor(
		XyVec2(lastX + space_offset, lastY + space_offset), 
		XyVec2(lastX + space_offset + square_size, lastY + space_offset + square_size), 
		0, 0, Vane::Util::ConvColor(XyColor(0.f, 0.f, 0.f), animation), Vane::Util::ConvColor(XyColor(0.f, 0.f, 0.f), animation)
	);

	Vane::renderer.AddCircleFilled(
		XyVec2(lastX + space_offset + square_size * hsv.s, lastY + space_offset + square_size * (1.f - hsv.v)),
		4.f, Vane::Util::ConvColor(Vane::Style::Highlight, animation)
	);

	// Hue bar
	constexpr const XyColor colors[7] = { XyColor(1.f, 0.f, 0.f), XyColor(1.f, 1.f, 0.f), XyColor(0.f, 1.f, 0.f), XyColor(0.f, 1.f, 1.f), XyColor(0.f,0.f, 1.f), XyColor(1.f, 0.f, 1.f), XyColor(1.f, 0.f, 0.f) };

	for (int i = 0; i < 6; ++i)
	{
		Vane::renderer.AddRectFilledMultiColor(
			XyVec2(lastX + lastH, lastY + space_offset + i * (square_size / 6.f)), 
			XyVec2(lastX + lastH + bar_width, lastY + space_offset + (i + 1) * (square_size / 6.f)), 
			Vane::Util::ConvColor(colors[i], animation), Vane::Util::ConvColor(colors[i], animation), Vane::Util::ConvColor(colors[i + 1], animation), Vane::Util::ConvColor(colors[i + 1], animation)
		);
	}

	Vane::renderer.AddRectFilled(
		XyVec2(lastX + lastH, lastY + space_offset + (square_size - 2.f) * hsv.h), 
		XyVec2(lastX + lastH + bar_width, lastY + space_offset + (square_size - 2.f) * hsv.h + 2.f), 
		Vane::Util::ConvColor(Vane::Style::Highlight, animation)
	);

	// Alpha bar
	if (!hasAlpha)
		return;

	float step = bar_width / 2.f;
	bool toggle = false;
	for (float y = 0.f; y < square_size; y += step)
	{
		float height = step;
		if (y + step > square_size)
			height = square_size - y;

		Vane::renderer.AddRectFilled(
			XyVec2(lastX + lastH + space_offset + bar_width, lastY + space_offset + y), 
			XyVec2(lastX + lastH + space_offset + bar_width + step, lastY + space_offset + y + height),
			toggle ? Vane::Util::ConvColor(XyColor(0.9f, 0.9f, 0.9f), animation) : Vane::Util::ConvColor(XyColor(0.4f, 0.4f, 0.4f), animation)
		);

		Vane::renderer.AddRectFilled(
			XyVec2(lastX + lastH + space_offset + bar_width + step, lastY + space_offset + y), 
			XyVec2(lastX + lastH + space_offset + bar_width + step * 2.f, lastY + space_offset + y + height),
			!toggle ? Vane::Util::ConvColor(XyColor(0.9f, 0.9f, 0.9f), animation) : Vane::Util::ConvColor(XyColor(0.4f, 0.4f, 0.4f), animation)
		);
		toggle = !toggle;
	}

	Vane::renderer.AddRectFilledMultiColor(
		XyVec2(lastX + lastH + space_offset + bar_width, lastY + space_offset), 
		XyVec2(lastX + lastH + space_offset + bar_width * 2.f, lastY + space_offset + square_size), 
		Vane::Util::ConvColor(XyColor(pColor->r, pColor->g, pColor->b), animation), Vane::Util::ConvColor(XyColor(pColor->r, pColor->g, pColor->b), animation), 0, 0
	);

	Vane::renderer.AddRectFilled(
		XyVec2(lastX + lastH + space_offset + bar_width, lastY + space_offset + (square_size - 2.f) * (1.f - hsv.a)), 
		XyVec2(lastX + lastH + space_offset + bar_width * 2.f, lastY + space_offset + (square_size - 2.f) * (1.f - hsv.a) + 2.f), 
		Vane::Util::ConvColor(Vane::Style::Highlight, animation)
	);

	Vane::renderer.PopClipRect();
}

#define MOVING_SQUARE 0
#define MOVING_HUEBAR 1
#define MOVING_ALPHA 2

#define clamp(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

std::optional<long> ColorPicker::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened)
{
	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_LBUTTONDOWN && !isInRect(mouseX, mouseY, lastX, lastY, lastW, lastH))
	{
		*opened = -1;
		return S_OK;
	}

	if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(mouseX, mouseY, lastX + space_offset, lastY + space_offset, square_size, square_size))
		{
			Vane::Cursor::current = Vane::Cursor::sizeall;
			moving = MOVING_SQUARE;
		}
		else if (isInRect(mouseX, mouseY, lastX + space_offset * 2.f + square_size, lastY + space_offset, bar_width, square_size))
		{
			Vane::Cursor::current = Vane::Cursor::sizev;
			moving = MOVING_HUEBAR;
		}
		else if (hasAlpha && isInRect(mouseX, mouseY, lastX + space_offset * 3.f + square_size + bar_width, lastY + space_offset, bar_width, square_size)) 
		{
			Vane::Cursor::current = Vane::Cursor::sizev;
			moving = MOVING_ALPHA;
		}
		return S_OK;
	}

	else if (msg == WM_LBUTTONUP)
	{
		if (moving != -1)
		{
			moving = -1;
			return S_OK;
		}
	}

	else if (msg == WM_MOUSEMOVE)
	{
		if (moving != -1)
		{
			if (moving == MOVING_SQUARE) 
			{
				Vane::Cursor::current = Vane::Cursor::sizeall;
				hsv.s = clamp((mouseX - lastX - space_offset) / square_size, 0.f, 1.f);
				hsv.v = 1.f - clamp((mouseY - lastY - space_offset) / square_size, 0.f, 1.f);
			}

			else if (moving == MOVING_HUEBAR) 
			{
				Vane::Cursor::current = Vane::Cursor::sizev;
				hsv.h = clamp((mouseY - lastY - space_offset) / square_size, 0.f, 1.f) * 0.999f;
			}

			else if (moving == MOVING_ALPHA)
			{
				Vane::Cursor::current = Vane::Cursor::sizev;
				hsv.a = 1.f - clamp((mouseY - lastY - space_offset) / square_size, 0.f, 1.f);
			}
			loadFromHSV();
		}

		else {
			if (isInRect(mouseX, mouseY, lastX + space_offset, lastY + space_offset, square_size, square_size))
			{
				Vane::Cursor::current = Vane::Cursor::sizeall;
			}
			else if (isInRect(mouseX, mouseY, lastX + space_offset * 2.f + square_size, lastY + space_offset, bar_width, square_size)) 
			{
				Vane::Cursor::current = Vane::Cursor::sizev;
			}
			else if (hasAlpha && isInRect(mouseX, mouseY, lastX + space_offset * 3.f + square_size + bar_width, lastY + space_offset, bar_width, square_size)) 
			{
				Vane::Cursor::current = Vane::Cursor::sizev;
			}
		}
	}

	return (msg == WM_MOUSEMOVE || msg == WM_LBUTTONUP || msg == WM_MOUSEWHEEL) ? S_OK : std::optional<long>();
}