#pragma once
#include "Tab.h"

class TabConfig : public Tab {
public:
	TabConfig(const std::string& name) : Tab(name)
	{
		isTab = true;
	}

	void Render(float x, float y, float w, int id, int* hovered, int* selected) override;
	std::optional<HRESULT> WndProc(UINT msg, WPARAM wParam, LPARAM lParam, int id, int* hovered, int* selected) override;

public:

protected:
	float Scroll = 0.f, DesScroll = 0.f, ScrollSpeed = 0.f;

	XyVec4 last_add_renderpos;
	float animation_add_hovered = 0.f;
	float animation_add_selected = 0.f;

private:
	bool last_active = false;
};