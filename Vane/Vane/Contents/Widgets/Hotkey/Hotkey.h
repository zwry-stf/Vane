#pragma once
#include "../Widget.h"
#include <string>


class Hotkey : public Widget {
public:
	Hotkey(std::string label, int* value) : pValue(value), strLabel(label) 
	{
		default_height = 26.f;
	}
	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) override;
	void InitAnimation() override;

private:
	std::string strLabel;
	int* pValue;
	int old_value = 0;
	float text_size = 0.f;
	char buffer[64] = { 0 };

	// Animation
	uint64_t last_tick = 0;
	float animation_typing = 0.f;
	bool ticking = false;
};