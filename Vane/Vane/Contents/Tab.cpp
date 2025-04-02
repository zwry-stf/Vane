#include "Tab.h"
#include "../Vane.h"
#include "Sidebar.h"

void Tab::Render(float x, float y, float w, int id, int* hovered, int* selected)
{
	if (!text_width_calculated)
	{
		text_width = Vane::renderer.CalcTextWidth(name.c_str());
		text_width_calculated = true;
	}

	lastX = x;
	lastY = y;
	lastW = w;
	lastH = Vane::Style::TabHeight;

	animation = Vane::Util::Lerp(animation, Vane::SelectedTab == id ? 1.f : 0.f, Vane::Style::AnimationSpeed);
	animation_hovered = Vane::Util::Lerp(animation_hovered, *hovered == id ? 1.f : 0.f, Vane::Style::AnimationSpeed);

	// Background
	Vane::renderer.AddRectFilled(XyVec2(x, y), XyVec2(x + w, lastY + lastH),
		Vane::Util::ConvColor(Vane::Util::InterpCol(Vane::Style::Background, XyColor(1.f, 1.f, 1.f), animation_hovered * 0.25f + animation * 0.1f), 0.5f), Vane::Style::Rounding / 2.f);

	// Border
	Vane::renderer.AddRect(XyVec2(x, y), XyVec2(x + w, y + lastH),
		Vane::Util::ConvColor(Vane::Util::InterpCol(Vane::Style::Accent, Vane::Style::Accent2, animation)), Vane::Style::Rounding / 2.f, 0, Vane::Style::BorderSize);

	// Text
	Vane::renderer.PushClipRect(XyVec2(x, y), XyVec2(x + w, y + Vane::Style::TabHeight), true);

	Vane::renderer.AddText(XyVec2(x + (w - text_width) / 2.f, y + (Vane::Style::TabHeight - Vane::renderer.FontSize) / 2.f), Vane::Util::ConvColor(Vane::Style::Text), name.c_str());

	Vane::renderer.PopClipRect();
}

std::optional<long> Tab::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected)
{
	float x = (float)(int16_t)LOWORD(lParam);
	float y = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_MOUSEMOVE)
	{
		if (isInRect(x, y, lastX, lastY, lastW, lastH))
		{
			*hovered = id;
			Vane::Cursor::current = Vane::Cursor::hand;
			return S_OK;
		}
		else if (*hovered == id)
		{
			*hovered = -1;
			return {};
		}
	}

	else if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(x, y, lastX, lastY, lastW, lastH))
		{
			Vane::SelectedTab = id;
			return S_OK;
		}
	}
	return {};
}

ChildTab* TabNormal::AddChildTab(const std::string& name, int num_areas)
{
	ChildTabs.push_back(new ChildTab{ name, num_areas });

	return ChildTabs.back();
}

ChildTab* TabNormal::GetLastChildTab()
{
	XY_ASSERT(ChildTabs.Size > 0);

	return ChildTabs.back();
}


void TabNormal::Render(float x, float y, float w, int id, int* hovered, int* selected)
{
	Tab::Render(x, y, w, id, hovered, selected);
	// Child Tabs

	bool active = id == Vane::SelectedTab;
	if (active != last_active)
	{
		last_active = active;

		if (active)
		{
			for (int i = 0; i < ChildTabs.Size; i++)
			{
				ChildTabs[i]->animation_selected = ChildTabs[i]->animation_hovered = 0.f;
			}
		}
		else
		{
			for (int i = 0; i < ChildTabs.Size; i++)
			{
				ChildTabs[i]->last_active = false;;
			}
		}
	}

	if (!active)
		return;

	// Border
	float xPos = Vane::x + Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * Sidebar::animation;

	Vane::renderer.AddRectFilled(
		XyVec2(xPos, Vane::y + Vane::Style::TopBarHeight),
		XyVec2(Vane::x + Vane::w - Vane::Style::BorderSize, Vane::y + Vane::Style::TopBarHeight + Vane::Style::BorderSize),
		Vane::Util::ConvColor(Vane::Style::Accent));

	for (int i = 0; i < ChildTabs.Size; i++)
	{
		xPos += ChildTabs[i]->Render(xPos, Vane::y, animation * Vane::animation, i, hovered, &SelectedChild);
	}
}

std::optional<long> TabNormal::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected)
{
	auto ret = Tab::WndProc(msg, wParam, lParam, id, hovered, selected);
	if (ret.has_value())
		return ret.value();

	if (id != Vane::SelectedTab)
		return {};

	for (int i = 0; i < ChildTabs.Size; i++)
	{
		auto ret = ChildTabs[i]->WndProc(msg, wParam, lParam, i, hovered, &SelectedChild);
		if (ret.has_value())
			return ret;
	}

	return {};
}