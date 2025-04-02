#include "Widget.h"
#include "../../Vane.h"

void GroupTitle::Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened)
{
	if (!text_width_calculated)
	{
		text_width = Vane::renderer.CalcTextWidth(strLabel.c_str());
		text_width_calculated = true;
	}

	float offset = Vane::Style::Spacing * 3.f;

	// Right Line
	Vane::renderer.AddRectFilledMultiColor(
		XyVec2(x + w - offset, y + default_height / 2.f),
		XyVec2(x + w, y + default_height / 2.f + 1.f),
		Vane::Util::ConvColor(Vane::Style::Accent, alpha), // Top Left
		Vane::Util::ConvColorEx(Vane::Style::Accent, 0.f), // Top Right
		Vane::Util::ConvColorEx(Vane::Style::Accent, 0.f), // Bottom Right
		Vane::Util::ConvColor(Vane::Style::Accent, alpha)); // Bottom Left

	// Left Line
	Vane::renderer.AddRectFilledMultiColor(
		XyVec2(x + w - offset * 2.f - text_width, y + default_height / 2.f),
		XyVec2(x + w - offset - text_width, y + default_height / 2.f + 1.f),
		Vane::Util::ConvColorEx(Vane::Style::Accent, 0.f), // Top Left
		Vane::Util::ConvColor(Vane::Style::Accent, alpha), // Top Right
		Vane::Util::ConvColor(Vane::Style::Accent, alpha), // Bottom Right
		Vane::Util::ConvColorEx(Vane::Style::Accent, 0.f)); // Bottom Left

	// Text
	Vane::renderer.AddText(
		XyVec2(x + w - offset - text_width, y + (default_height - Vane::Style::TextSize) / 2.f),
		Vane::Util::ConvColor(Vane::Style::Text, alpha), strLabel.c_str());
}
