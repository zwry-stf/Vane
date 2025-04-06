#pragma once
#include "../Widget.h"
#include <string>


class Checkbox : public Widget {
public:
	Checkbox(std::string label, bool* value) : pValue(value), strLabel(label)
	{
		default_height = 26.f;
	}
	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) override;
	void InitAnimation() override;

	// We return Checkbox* so we can modify this class further in one line of code
	Checkbox* LinkTo(Overlay* overlay);

private:
	float animation_hovered_child = 0.f;
	std::string strLabel;
	bool* pValue;
	bool selected_type = false; // 0 = value, 1 = child
};