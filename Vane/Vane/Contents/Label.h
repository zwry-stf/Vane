#pragma once
#include "Tab.h"

class Label : public Tab {
public:
	Label(const std::string& name) : Tab(name)
	{
	}

	void Render(float x, float y, float w, int id, int* hovered, int* selected) override;
	std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected) override;
};