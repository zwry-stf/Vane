#include "ConfigDrawable.h"
#include "../Vane.h"

enum TypeButton {
	Type_Delete = 1,
	Type_Load = 2,
	Type_Save = 3
};

void ConfigDrawable::Render(float x, float y, float w, float alpha, int id, int* hovered, int* selected)
{
	animation_hovered = Vane::Util::Lerp(animation_hovered, id == *hovered, Vane::Style::AnimationSpeed);
	animation_selected = Vane::Util::Lerp(animation_selected, id - Tab_Begin == Vane::Config::selected_config, Vane::Style::AnimationSpeed);

	float offset = animation_hovered * default_height / 30.f + animation_selected * default_height / 50.f;
	lastX = x - offset;
	lastY = y - offset;
	lastW = w + offset * 2.f;
	lastH = default_height + offset * 2.f;

	animation_delete_hovered = Vane::Util::Lerp(animation_delete_hovered, id == *hovered && hovered_button == Type_Delete, Vane::Style::AnimationSpeed);
	animation_delete_selected = Vane::Util::Lerp(animation_delete_selected, id == *selected && selected_button == Type_Delete, Vane::Style::AnimationSpeed);

	animation_load_hovered = Vane::Util::Lerp(animation_load_hovered, id == *hovered && hovered_button == Type_Load, Vane::Style::AnimationSpeed);
	animation_load_selected = Vane::Util::Lerp(animation_load_selected, id == *selected && selected_button == Type_Load, Vane::Style::AnimationSpeed);

	animation_save_hovered = Vane::Util::Lerp(animation_save_hovered, id == *hovered && hovered_button == Type_Save, Vane::Style::AnimationSpeed);
	animation_save_selected = Vane::Util::Lerp(animation_save_selected, id == *selected && selected_button == Type_Save, Vane::Style::AnimationSpeed);

	// Background
	Vane::renderer.AddRectFilled(XyVec2(lastX, lastY), XyVec2(lastX + lastW, lastY + lastH), Vane::Util::ConvColor(Vane::Style::Background, 0.35f * alpha), Vane::Style::Rounding);

	// Border
	Vane::renderer.AddRect(XyVec2(lastX, lastY), XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Util::InterpCol(Vane::Style::Accent, Vane::Style::Accent2, animation_selected * 0.7f), alpha), Vane::Style::Rounding, 0, Vane::Style::BorderSize);

	// Text
	//Vane::renderer.AddText(XyVec2(lastX + 5.f, lastY + (lastH - Vane::Style::TextSize) / 2.f), Vane::Util::ConvColor(Vane::Style::Text), name.c_str());

	if (Vane::renderer.data.curr_time - last_blink > 500)
	{
		last_blink = Vane::renderer.data.curr_time;
		blink_toggle = !blink_toggle;
	}
	blinking_animation = Vane::Util::Lerp(blinking_animation, blink_toggle && typing ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	float out_pos = 0.f;
	if (pos_to_get > 0.f)
	{
		last_string_width = Vane::renderer.AddTextTest(XyVec2(lastX + 5.f, lastY + (lastH - Vane::Style::TextSize) / 2.f), Vane::Util::ConvColor(Vane::Style::Text, alpha), typing ? typing_buffer.c_str() : name.c_str(),
			pos_to_get + lastX + 5.f, &typing_pos, 0);

		pos_to_get = -1.f;
	}
	else
	{
		last_string_width = Vane::renderer.AddTextTest(XyVec2(lastX + 5.f, lastY + (lastH - Vane::Style::TextSize) / 2.f), Vane::Util::ConvColor(Vane::Style::Text, alpha), typing ? typing_buffer.c_str() : name.c_str(),
			typing_pos, &out_pos, 1);
	}

	last_string_width -= lastX + 5.f;
	if (last_string_width < 0.f)
		last_string_width = 0.f;

	if (blinking_animation * Vane::Animation > Vane::MinAlpha)
	{
		Vane::renderer.AddRectFilled(XyVec2(out_pos, lastY + (lastH - Vane::Style::TextSize) / 2.f),
			XyVec2(out_pos + 2.f, lastY + (lastH + Vane::Style::TextSize) / 2.f), Vane::Util::ConvColor(Vane::Style::Accent, blinking_animation * alpha));
	}

	// Buttons
	float button_size = default_height / 3.f;
	float button_spacing = button_size / 3.f;
	offset = Vane::Style::Rounding - (Vane::Style::Rounding) / sqrtf(2.0f);

	// Delete (Top Right)
	Vane::renderer.AddImage(Vane::Icons::IconDelete,
		XyVec2(lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - 2.f - offset, lastY + Vane::Style::BorderSize + 2.f + offset),
		XyVec2(lastX + lastW - Vane::Style::BorderSize - 2.f - offset, lastY + button_size / 1.8f + Vane::Style::BorderSize + 2.f + offset),
		XyVec2(0, 0), XyVec2(1, 1), Vane::Util::ConvColor(Vane::Style::Icon, animation_hovered * alpha));

	Vane::renderer.AddRectFilled(
		XyVec2(lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - 2.f - offset, lastY + Vane::Style::BorderSize + 2.f + offset),
		XyVec2(lastX + lastW - Vane::Style::BorderSize - 2.f - offset, lastY + button_size / 1.8f + Vane::Style::BorderSize + 2.f + offset),
		Vane::Util::ConvColor(Vane::Style::Accent2, (animation_delete_hovered * 0.6f + animation_delete_selected * 0.3f) * alpha));

	// Load (Top)
	Vane::renderer.AddImage(Vane::Icons::IconLoad,
		XyVec2(lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f - button_size),
		XyVec2(lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f),
		XyVec2(0, 0), XyVec2(1, 1), Vane::Util::ConvColor(Vane::Style::Icon, alpha));

	Vane::renderer.AddRectFilled(
		XyVec2(lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f - button_size),
		XyVec2(lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f),
		Vane::Util::ConvColor(Vane::Style::Accent2, (animation_load_hovered * 0.6f + animation_load_selected * 0.3f) * alpha));

	// Save (Bottom)
	Vane::renderer.AddImage(Vane::Icons::IconSave,
		XyVec2(lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f),
		XyVec2(lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f + button_size),
		XyVec2(0, 0), XyVec2(1, 1), Vane::Util::ConvColor(Vane::Style::Icon, alpha));

	Vane::renderer.AddRectFilled(
		XyVec2(lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f),
		XyVec2(lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f + button_size),
		Vane::Util::ConvColor(Vane::Style::Accent2, (animation_save_hovered * 0.6f + animation_save_selected * 0.3f) * alpha));
}

std::optional<HRESULT> ConfigDrawable::WndProc(UINT msg, WPARAM wParam, LPARAM lParam, int id, int* hovered, int* selected)
{
	if (typing && *selected != id)
		StopTyping();

	float x = (float)(int16_t)LOWORD(lParam);
	float y = (float)(int16_t)HIWORD(lParam);

	if (typing && msg == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE || wParam == VK_RETURN)
		{
			StopTyping();
			*selected = -1;
		}
		else if (wParam == VK_LEFT)
		{
			if (typing_pos > 0)
				typing_pos--;

			blink_toggle = false;
			last_blink = 0;
		}
		else if (wParam == VK_RIGHT)
		{
			if (typing_pos < typing_buffer.length())
				typing_pos++;

			blink_toggle = false;
			last_blink = 0;
		}
		else if (wParam == VK_BACK)
		{
			if (typing_pos > 0)
			{
				typing_pos--;
				typing_buffer.erase(typing_buffer.begin() + typing_pos);
			}

			blink_toggle = false;
			last_blink = 0;
		}

		return S_OK;
	}

	else if (typing && msg == WM_CHAR && typing_buffer.length() < 20)
	{
		// https://www.torsten-horn.de/techdocs/ascii.htm
		if ((wParam >= '\'' && wParam <= '9') ||
			(wParam >= 'A' && wParam <= 'Z') ||
			(wParam >= 'a' && wParam <= 'z') ||
			wParam == '_' || wParam == ' ')
		{
			typing_buffer.insert(typing_pos, (const char*)&wParam, 1);
			typing_pos++;

			blink_toggle = false;
			last_blink = 0;
		}

		return S_OK;
	}

	else if (typing && msg == WM_LBUTTONDOWN)
	{
		if (isInRect(x, y, lastX + 3.f, lastY + (lastH - Vane::Style::TextSize) / 2.f, last_string_width + 7.f, Vane::Style::TextSize)) // On Text
		{
			pos_to_get = x - lastX - 5.f;
			return S_OK;
		}
		else
		{
			*selected = -1;
			StopTyping();
			return false;
		}

		return S_OK;
	}

	if (msg == WM_MOUSEMOVE)
	{
		if (typing)
			return S_OK;

		if (*selected != id && *selected != -1)
			return {};

		if (isInRect(x, y, lastX, lastY, lastW, lastH))
		{
			*hovered = id;

			// Check if on button
			float button_size = default_height / 3.f;
			float button_spacing = button_size / 3.f;
			float offset = Vane::Style::Rounding - (Vane::Style::Rounding) / sqrtf(2.0f);

			if (isInRect(x, y, lastX + 3.f, lastY + (lastH - Vane::Style::TextSize) / 2.f, last_string_width + 7.f, Vane::Style::TextSize)) // On Text
			{
				Vane::Cursor::current = Vane::Cursor::text;
			}
			else if (isInRect(x, y, lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - 2.f - offset, lastY + Vane::Style::BorderSize + 2.f + offset, button_size / 1.8f, button_size / 1.8f)) // Delete
			{
				hovered_button = Type_Delete;
				Vane::Cursor::current = Vane::Cursor::hand;
			}
			else if (isInRect(x, y, lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f - button_size, button_size, button_size)) // Load
			{
				hovered_button = Type_Load;
				Vane::Cursor::current = Vane::Cursor::hand;
			}
			else if (isInRect(x, y, lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f, button_size, button_size)) // Save
			{
				hovered_button = Type_Save;
				Vane::Cursor::current = Vane::Cursor::hand;
			}
			else
				hovered_button = -1;
			return S_OK;
		}
		else if (*hovered == id)
		{
			*hovered = -1;
		}
	}

	else if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(x, y, lastX, lastY, lastW, lastH))
		{
			*selected = id;
			if (Vane::Config::selected_config != id - Tab_Begin)
			{
				Vane::Config::selected_config = id - Tab_Begin;
				Vane::Config::SetCurr();
			}

			// Check if on button
			float button_size = default_height / 3.f;
			float button_spacing = button_size / 3.f;
			float offset = Vane::Style::Rounding - (Vane::Style::Rounding) / sqrtf(2.0f);

			if (isInRect(x, y, lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - 2.f - offset, lastY + Vane::Style::BorderSize + 2.f + offset, button_size / 1.8f, button_size / 1.8f)) // Delete
			{
				selected_button = Type_Delete;
			}
			else if (isInRect(x, y, lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f - button_size, button_size, button_size)) // Load
			{
				selected_button = Type_Load;
			}
			else if (isInRect(x, y, lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f, button_size, button_size)) // Save
			{
				selected_button = Type_Save;
			}
			else
				selected_button = -1;

			// Check if on text
			if (isInRect(x, y, lastX + 3.f, lastY + (lastH - Vane::Style::TextSize) / 2.f, last_string_width + 7.f, Vane::Style::TextSize)) // On Text
			{
				pos_to_get = x - lastX - 4.f;
				*selected = id;
				StartTyping();
			}

			return S_OK;
		}
	}

	else if (msg == WM_LBUTTONUP)
	{
		if (typing)
			return S_OK;

		if (*selected == id)
		{
			*selected = -1;

			// Check if on button
			float button_size = default_height / 3.f;
			float button_spacing = button_size / 3.f;
			float offset = Vane::Style::Rounding - (Vane::Style::Rounding) / sqrtf(2.0f);

			if (isInRect(x, y, lastX + lastW - button_size / 1.8f - Vane::Style::BorderSize - 2.f - offset, lastY + Vane::Style::BorderSize + 2.f + offset, button_size / 1.8f, button_size / 1.8f)) // Delete
			{
				Vane::Config::Remove();
			}
			else if (isInRect(x, y, lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f - button_spacing / 2.f - button_size, button_size, button_size) && selected_button == Type_Load) // Load
			{
				Vane::Config::Load();
			}
			else if (isInRect(x, y, lastX + lastW - button_size - button_size / 1.8f - Vane::Style::BorderSize - button_spacing, lastY + lastH / 2.f + button_spacing / 2.f, button_size, button_size) && selected_button == Type_Save) // Save
			{
				Vane::Config::Save();
			}

			selected_button = -1;
			return S_OK;
		}
	}

	return {};
}

void ConfigDrawable::InitAnimation()
{
	animation_hovered = 0.f;
	animation_selected = 0.f;
	animation_delete_hovered = 0.f;
	animation_delete_selected = 0.f;
	animation_load_hovered = 0.f;
	animation_load_selected = 0.f;
	animation_save_hovered = 0.f;
	animation_save_selected = 0.f;
	blinking_animation = 0.f;
}

void ConfigDrawable::StartTyping()
{
	XY_ASSERT(name.length() > 4);
	typing_buffer = name.substr(0, name.length() - 4);

	typing = true;
}

void ConfigDrawable::StopTyping()
{
	Vane::Config::Rename(typing_buffer + ".cfg");
	typing = false;
}
