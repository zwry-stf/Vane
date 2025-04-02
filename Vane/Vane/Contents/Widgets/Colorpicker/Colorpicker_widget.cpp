#include "colorpicker.h"
#include "../../../Vane.h"
#include <thread>
#include <sstream>

void ColorPickerWidget::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) 
{
	lastX = x;
	lastY = y;
	lastW = w;
	lastH = default_height;

	// Animations
	animation_hovered = Vane::Util::Lerp(animation_hovered, id == *hovered && _hovered == 1 ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_disabled = Vane::Util::Lerp(animation_disabled, (disabled && (*disabled == !dis_inv)) ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	// Text
	label_width = Vane::renderer.AddText(
		XyVec2(x, y + (lastH - Vane::Style::TextSize) / 2),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Text, alpha), animation_disabled), (strLabel + ": ").c_str()
	) - x;

	if (!typing)
	{
		XyColor col = ((ColorPicker*)pOverlays->at(child_id))->getColor();
		
		bool hasAlpha = ((ColorPicker*)pOverlays->at(child_id))->hasAlpha;

		static constexpr const char hexChars[] = "0123456789ABCDEF";
		unsigned char r = std::min((unsigned char)255, (unsigned char)std::round(col.r * 255.f));
		unsigned char g = std::min((unsigned char)255, (unsigned char)std::round(col.g * 255.f));
		unsigned char b = std::min((unsigned char)255, (unsigned char)std::round(col.b * 255.f));
		unsigned char a = std::min((unsigned char)255, (unsigned char)std::round(col.a * 255.f));

		buffer[0] = '#';

		buffer[1] = hexChars[(r >> 4) & 0x0F];
		buffer[2] = hexChars[r & 0x0F];

		buffer[3] = hexChars[(g >> 4) & 0x0F];
		buffer[4] = hexChars[g & 0x0F];

		buffer[5] = hexChars[(b >> 4) & 0x0F];
		buffer[6] = hexChars[b & 0x0F];

		if (hasAlpha)
		{
			buffer[7] = hexChars[(a >> 4) & 0x0F];
			buffer[8] = hexChars[a & 0x0F];
		}

		str_length = hasAlpha ? 9 : 7;
	}

	if (typing) 
	{
		void* out = typing_pos != -1 ? (void*)&selected_char_begin : (void*)&typing_pos;
		color_width = Vane::renderer.AddTextTest(
			XyVec2(x + label_width, y + (lastH - Vane::Style::TextSize) / 2),
			typing ? 
				Vane::Util::DisableColor(Vane::Util::ConvColor(XyColor(0.8f, 0.8f, 0.8f), alpha), animation_disabled) :
				Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha), animation_disabled),
			buffer.c_str(), typing_pos == -1 ? (typed_pos + lastX + label_width) : (float)typing_pos, out, typing_pos == -1 ? 0 : 1
		) - x - label_width;

		if (Vane::renderer.data.curr_time - last_toggle_time > 500)
		{
			typing_toggle = !typing_toggle;
			last_toggle_time = Vane::renderer.data.curr_time;
		}
	}
	else 
	{
		color_width = Vane::renderer.AddText(
			XyVec2(x + label_width, y + (lastH - Vane::Style::TextSize) / 2),
			typing ? 
				Vane::Util::DisableColor(Vane::Util::ConvColor(XyColor(0.8f, 0.8f, 0.8f), alpha), animation_disabled) : 
				Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha), animation_disabled), buffer.c_str()
		) - x - label_width;
	}

	typing_animation = Vane::Util::Lerp(typing_animation, (typing_toggle && typing) ? 1.f : 0.f, Vane::Style::AnimationSpeed * 1.5f);

	if (typing_animation > 0.001f)
		Vane::renderer.AddRectFilled(
			XyVec2(selected_char_begin - 1.f , y + (lastH - Vane::Style::TextSize) / 2 - 1.f),
			XyVec2(selected_char_begin + 1.f, y + (lastH - Vane::Style::TextSize) / 2 + Vane::Style::TextSize + 1.f),
			Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha * typing_animation), animation_disabled));

	// Color
	const float color_height = lastH / 1.7f;
	const float spacing = (lastH - color_height) / 2.f;
	const float offset = animation_hovered * 1.5f;
	const float size = default_height + 3.f;

	Vane::renderer.AddRectFilled(
		XyVec2(x + w - size - 3.f - offset, y + spacing - offset),
		XyVec2(x + w - 3.f + offset, y + lastH - spacing + offset),
		Vane::Util::DisableColor(Vane::Util::ConvColor(((ColorPicker*)pOverlays->at(child_id))->getColorIA(), alpha), animation_disabled),
		Vane::Style::Rounding / 2.f + offset);
	
	Vane::renderer.AddRect(
		XyVec2(x + w - size - 3.f - offset, y + spacing - offset),
		XyVec2(x + w - 3.f + offset, y + lastH - spacing + offset),
		Vane::Util::DisableColor(Vane::Util::ConvColor(Vane::Style::Accent, alpha), animation_disabled),
		Vane::Style::Rounding / 2.f + offset, 0, Vane::Style::BorderSize);
}

void ColorPickerWidget::stopTyping()
{
	// Parse Buffer
	int offset = 0;
	if (strncmp(buffer.c_str(), "0x", 2) == 0)
		offset = 2;
	if (strncmp(buffer.c_str(), "#", 1) == 0)
		offset = 1;

	unsigned char r = 0, g = 0, b = 0, a = 0;

	bool valid = true;
	for (int i = offset; i < 8 + offset; i+=2)
	{
		unsigned char hex1 = buffer[i];
		unsigned char hex2 = buffer[i + 1];

		if (hex1 == 0 || hex2 == 0)
		{
			if (i < 6 + offset)
				valid = false;
			else
				a = 255;
			break;
		}

		hex1 = std::toupper(hex1);
		hex2 = std::toupper(hex2);

		auto hexCharToValue = [](char c) -> unsigned char {
			c = std::toupper(c); // Normalize to uppercase for consistency
			if (c >= '0' && c <= '9') return c - '0';
			if (c >= 'A' && c <= 'F') return c - 'A' + 10;
			return 0; // Should not reach here with valid input
			};

		uint8_t highNibble = hexCharToValue(hex1);
		uint8_t lowNibble = hexCharToValue(hex2);

		// Combine the nibbles into a single byte
		uint8_t combinedByte = (highNibble << 4) | lowNibble;
		switch (i - offset) 
		{
		case 0: r = combinedByte; break;
		case 2: g = combinedByte; break;
		case 4: b = combinedByte; break;
		case 6: a = combinedByte; break;
		}
	}

	if (!valid)
	{
		typing = false;
		return;
	}

	((ColorPicker*)pOverlays->at(child_id))->setColor(XyColor(r, g, b, a));

	typing = false;
}

std::optional<long> ColorPickerWidget::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) 
{
	if (disabled && (*disabled == !dis_inv))
	{
		if (*selected == id)
			*selected = -1;
		if (*hovered == id)
			*hovered = -1;
		if (typing)
			stopTyping();
		return {};
	}

	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_MOUSEMOVE) 
	{
		if (isInRect(mouseX, mouseY, lastX + label_width, lastY + (lastH - Vane::Style::TextSize) / 2, color_width + 2.f, Vane::Style::TextSize))
		{
			Vane::Cursor::current = Vane::Cursor::text;
			_hovered = 0;
			*hovered = id;
			return S_OK;
		}
		else if (*hovered == id && _hovered == 0)
		{
			*hovered = -1;
			return S_OK;
		}
	}

	if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(mouseX, mouseY, lastX + label_width, lastY + (lastH - Vane::Style::TextSize) / 2, color_width + 2.f, Vane::Style::TextSize))
		{
			typed_pos = mouseX - lastX - label_width + 1.5f;
			typing_pos = -1;

			last_toggle_time = GetTickCount64();
			typing_toggle = true;

			typing = true;
			return S_OK;
		}
		else if (typing) 
		{
			stopTyping();
			return S_OK;
		}
	}

	if (typing && msg == WM_CHAR && str_length <= 9)
	{ // 0x00000000
		if ((wParam >= '0' && wParam <= '9') || (wParam >= 'A' && wParam <= 'F') || (wParam >= 'a' && wParam <= 'f'))
		{
			buffer.insert(typing_pos, (const char*)&wParam, 1);
			if (typing_pos < str_length)
				typing_pos++;

			return S_OK;
		}
	}

	if (typing && msg == WM_KEYDOWN) 
	{
		if (wParam == VK_LEFT)
		{
			if (typing_pos > 0)
			{
				typing_pos--;

				last_toggle_time = GetTickCount64();
				typing_toggle = true;
			}
		}
		else if (wParam == VK_RIGHT)
		{
			if (typing_pos < str_length)
			{
				typing_pos++;

				last_toggle_time = GetTickCount64();
				typing_toggle = true;
			}
		}
		else if (wParam == VK_BACK)
		{
			if (typing_pos > 0)
			{
				typing_pos--;

				buffer.erase(typing_pos, 1);
			}
		}
		else if (wParam == VK_DELETE)
		{
			if (typing_pos < str_length)
			{
				buffer.erase(typing_pos, 1);
			}
		}
		else if (wParam == VK_RETURN || wParam == VK_ESCAPE)
		{
			stopTyping();
		}
		return S_OK;
	}

	if (msg == WM_LBUTTONDOWN || msg == WM_MOUSEMOVE) 
	{
		if (typing)
			return S_OK;

		const float size = default_height + 3.f;
		const float color_height = lastH / 1.7f;
		const float spacing = (lastH - color_height) / 2.f;
		if (isInRect(mouseX, mouseY, lastX + lastW - size - 3.f, lastY + spacing, size, lastH - spacing * 2.f))
		{
			if (msg == WM_MOUSEMOVE) 
			{
				_hovered = 1;
				*hovered = id;
				Vane::Cursor::current = Vane::Cursor::hand;
			}
			else if (msg == WM_LBUTTONDOWN) 
			{
				pOverlays->at(child_id)->SetPos(lastX + lastW - size - 3.f + size / 2.f - Vane::x, lastY + spacing + (lastH - spacing * 2.f) / 2.f - Vane::y);
				*opened = child_id;
			}
			return S_OK;
		}
		else if (msg == WM_MOUSEMOVE && *hovered == id && _hovered == 1) 
		{
			*hovered = -1;
		}
	}
	return {};
}