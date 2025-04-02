#pragma once
#include "../Widget.h"
#include <string>


class Slider : public Widget {
public:
	Slider(std::string label, float* value, float min, float max, std::string format) : pValue(value), strLabel(label), min(min), max(max), strFormat(format)
	{
		default_height = 32.f;
	}
	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) override;
	void InitAnimation() override;

private:
	float animation = 0.f;
	float min, max;
	std::string strLabel, strFormat;
	float* pValue;
};