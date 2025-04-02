#include "Label.h"
#include "../Vane.h"
#include "Sidebar.h"

void Label::Render(float x, float y, float w, int id, int* hovered, int* selected)
{
	if (!text_width_calculated)
	{
		text_width = Vane::renderer.CalcTextWidth(name.c_str());
		text_width_calculated = true;
	}

	Vane::renderer.AddText(XyVec2(x + (w - text_width) / 2.f, y + (Vane::Style::TabHeight - Vane::renderer.FontSize) / 2.f),
		Vane::Util::ConvColor(Vane::Style::Text), name.c_str());

	Vane::renderer.PushClipRect(XyVec2(Vane::x, y), XyVec2(Vane::x + Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * Sidebar::animation, y + Vane::Style::TabHeight), true);

	float xpos = x + (w - text_width) / 2.f;
	if (xpos < 2.f + Vane::x)
		xpos = 2.f + Vane::x;
	Vane::renderer.AddRectFilled(
		XyVec2(xpos, y + (Vane::Style::TabHeight - Vane::renderer.FontSize) / 2.f + Vane::renderer.FontSize),
		XyVec2(xpos + text_width, y + (Vane::Style::TabHeight - Vane::renderer.FontSize) / 2.f + Vane::renderer.FontSize + 2),
		Vane::Util::ConvColor(Vane::Style::Text));

	Vane::renderer.PopClipRect();
}

std::optional<long> Label::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected)
{
	return {};
}
