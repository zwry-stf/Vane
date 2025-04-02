#include "dropdown.h"
#include "../../../Vane.h"


void Dropdown::Draw(const int id, int* opened)
{
	lastX = Vane::x + xPos;
	lastY = Vane::y + yPos;
	lastW = Width;

	// Animation
	animation = Vane::Util::Lerp(animation, id == *opened ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	if (animation < Vane::MinAlpha)
		return;

	const float rowHeight = Vane::w / 42.f;

	lastH = pOptions->Size * rowHeight * animation;

	if (pos_changed)
	{
		pos_changed = false;
		if (lastY + lastH > Vane::renderer.data.display_size.y)
		{
			lastY -= lastH;
			yPos -= lastH;
		}
	}

	// Background
	Vane::renderer.AddRectFilled(
		XyVec2(lastX, lastY), 
		XyVec2(lastX + lastW, lastY + lastH), 
		Vane::Util::ConvColor(Vane::Style::Background, animation), Vane::Style::Rounding
	);

	// Border
	Vane::renderer.AddRect(
		XyVec2(lastX, lastY), 
		XyVec2(lastX + lastW, lastY + lastH),
		Vane::Util::ConvColor(Vane::Style::Accent, animation), Vane::Style::Rounding, 0, Vane::Style::BorderSize);

	// Items
	Vane::renderer.PushClipRect(
		XyVec2(lastX, lastY), 
		XyVec2(lastX + lastW, lastY + lastH)
	);


	if (pOptions->Size > animations.Size)
	{
		int oldSize = animations.Size;
		animations.resize(pOptions->Size);
		memset(animations.Data + oldSize, 0, (animations.Size - oldSize) * sizeof(std::pair<float, float>));
	}

	for (int i = 0; i < pOptions->Size; i++)
	{
		animations[i].first = Vane::Util::Lerp(animations[i].first, i == hovered ? 1.f : 0.f, Vane::Style::AnimationSpeed);
		animations[i].second = Vane::Util::Lerp(animations[i].second, i == *pSelected ? 1.f : 0.f, Vane::Style::AnimationSpeed);

		Vane::renderer.AddText(
			XyVec2(lastX + 3.f + std::min(1.f, animations[i].first + animations[i].second * 0.5f) * lastW / 24.f, lastY + i * rowHeight), 
			Vane::Util::ConvColor(Vane::Util::InterpCol(Vane::Style::Text, Vane::Style::Accent, animations[i].second * 0.6f), 
				animation * (0.7f + animations[i].first * 0.3f)), pOptions->at(i).c_str()
		);
	}

	Vane::renderer.PopClipRect();
}	

std::optional<long> Dropdown::WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened)
{
	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_LBUTTONDOWN || msg == WM_MOUSEMOVE) 
	{
		if (!isInRect(mouseX, mouseY, lastX, lastY, lastW, lastH))
		{
			hovered = -1;
			if (msg == WM_LBUTTONDOWN) 
			{
				*opened = -1;
			}
			return S_OK;
		}

		const float rowHeight = Vane::w / 42.f;
		int selected = (int)((mouseY - lastY) / rowHeight);
		if (selected < 0) 
		{
			*opened = -1;
			return S_OK;
		}
		if (selected >= pOptions->Size)
		{
			*opened = -1;
			return S_OK;
		}

		if (msg == WM_LBUTTONDOWN)
		{
			*pSelected = selected;
		}
		else if (msg == WM_MOUSEMOVE)
		{
			hovered = selected;
			Vane::Cursor::current = Vane::Cursor::hand;
		}
		return S_OK;
	}

	return (msg == WM_MOUSEMOVE || msg == WM_LBUTTONUP || msg == WM_MOUSEWHEEL) ? S_OK : std::optional<long>();
}