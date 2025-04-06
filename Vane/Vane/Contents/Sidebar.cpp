#include "Sidebar.h"
#include "../Vane.h"


void DrawRotatingImage(XyTextureID texture, XyVec2 pos, XyVec2 size, float t)
{
    float angle = t * (XY_PI / 2.0f); // Convert t (0 to 1) into 0 to 90 degrees

    XyVec2 center = XyVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);

    // Original corners before rotation
    XyVec2 corners[4] = {
        {pos.x, pos.y},                        // Top-left
        {pos.x + size.x, pos.y},               // Top-right
        {pos.x + size.x, pos.y + size.y},      // Bottom-right
        {pos.x, pos.y + size.y}                // Bottom-left
    };

    // Rotate corners around the center
    XyVec2 rotatedCorners[4];
    for (int i = 0; i < 4; i++)
    {
        rotatedCorners[i] = Vane::Util::RotatePoint(corners[i], center, angle);
    }

    // Draw the rotated image
    Vane::renderer.AddImageQuad(texture,
        rotatedCorners[0], rotatedCorners[1], rotatedCorners[2], rotatedCorners[3],
        XyVec2(0, 0), XyVec2(1, 0), XyVec2(1, 1), XyVec2(0, 1), Vane::Util::ConvColor(XyColor(1.f, 1.f, 1.f))
    );
}

void Sidebar::Render(int* hovered, int* selected)
{
    animation = Vane::Util::Lerp(animation, open ? 1.f : 0.f, Vane::Style::AnimationSpeed);
    animation_hovered = Vane::Util::Lerp(animation_hovered, *hovered == Sidebar_Begin + ID_Icon ? 1.f : 0.f, Vane::Style::AnimationSpeed);

    float width = Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * animation;

    // Background
    Vane::renderer.AddRectFilled(
        XyVec2(Vane::x, Vane::y), 
        XyVec2(Vane::x + width, Vane::y + Vane::h), 
        Vane::Util::ConvColor(Vane::Style::Background, 0.1f + animation * 0.15f),
        Vane::Style::Rounding, XyDrawCornerFlags_Left);

	// Border
	Vane::renderer.AddRectFilled(
        XyVec2(Vane::x + width - Vane::Style::BorderSize, Vane::y + Vane::Style::BorderSize),
        XyVec2(Vane::x + width, Vane::y + Vane::h - Vane::Style::BorderSize),
        Vane::Util::ConvColor(Vane::Style::Accent));

	// Icon
    float offset = Vane::Style::BorderSize + Vane::Style::Spacing / 2.f - 
        animation_hovered * Vane::Style::Spacing / 4.f +
        ((0.5f - abs(animation - 0.5f)) * Vane::Style::Spacing / 1.f);

	XyVec2 center = { Vane::x + Vane::Style::SideBarColl / 2.f, Vane::y + Vane::Style::SideBarColl / 2.f };

    DrawRotatingImage(Vane::Icons::SidebarIcon,
        XyVec2(Vane::x + offset, Vane::y + offset),
        XyVec2(Vane::Style::SideBarColl - offset * 2, Vane::Style::SideBarColl - offset * 2), animation);

    // Tabs
    float pos = Vane::y + Vane::Style::SideBarColl + Vane::Style::BorderSize;
	for (int i = 0; i < Vane::Tabs.Size; i++)
	{
        Vane::Tabs[i]->Render(Vane::x + Vane::Style::BorderSize + Vane::Style::Spacing / 2.f, pos, width - Vane::Style::BorderSize * 2 - Vane::Style::Spacing, i + Sidebar_Begin + 10, hovered, selected);
		pos += Vane::Style::TabHeight + Vane::Style::Spacing;
	}
}

std::optional<long> Sidebar::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int* hovered, int* selected)
{
    float x = (float)(int16_t)LOWORD(lParam);
    float y = (float)(int16_t)HIWORD(lParam);

    if (msg == WM_MOUSEMOVE)
    {
        if (*selected == Sidebar_Begin + ID_Icon)
        {
            *hovered = Sidebar_Begin + ID_Icon;
            Vane::Cursor::current = Vane::Cursor::hand;
            return S_OK;
        }
        else if (isInRect2(x, y, Vane::x + Vane::Style::BorderSize, Vane::y + Vane::Style::BorderSize, Vane::x + Vane::Style::SideBarColl - Vane::Style::BorderSize, Vane::y + Vane::Style::SideBarColl - Vane::Style::BorderSize))
        {
            *hovered = Sidebar_Begin + ID_Icon;
            Vane::Cursor::current = Vane::Cursor::hand;
            return S_OK;
        }
        else if (open && x > (Vane::x + Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * animation))
        {
			return S_OK; // Block input if open
        }
    }

    else if (msg == WM_LBUTTONDOWN)
    {
        float width = Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * animation;

        if (isInRect2(x, y, Vane::x + Vane::Style::BorderSize, Vane::y + Vane::Style::BorderSize, Vane::x + Vane::Style::SideBarColl - Vane::Style::BorderSize, Vane::y + Vane::Style::SideBarColl - Vane::Style::BorderSize))
        {
            *selected = Sidebar_Begin + ID_Icon;
            return S_OK;
        }

        else if (x > Vane::x + width && open)
        {
            open = false;
			return S_OK;
        }
    }

    else if (msg == WM_LBUTTONUP && *selected == Sidebar_Begin + ID_Icon)
    {
        if (isInRect2(x, y, Vane::x + Vane::Style::BorderSize, Vane::y + Vane::Style::BorderSize, Vane::x + Vane::Style::SideBarColl - Vane::Style::BorderSize, Vane::y + Vane::Style::SideBarColl - Vane::Style::BorderSize))
        {
            open = !open;
        }

        *selected = -1;
        return S_OK;
    }

    else if (msg == WM_MOUSEWHEEL && open)
    {
        if (x > (Vane::x + Vane::Style::SideBarColl + (Vane::Style::SideBarWidth - Vane::Style::SideBarColl) * animation))
            return S_OK;
    }

    // Tabs
    for (int i = 0; i < Vane::Tabs.Size; i++)
    {
        auto ret = Vane::Tabs[i]->WndProc(msg, wParam, lParam, i + Sidebar_Begin + 10, hovered, selected);
        if (ret.has_value())
			return ret.value();
    }

    return {};
}
