#pragma once
#include "../Widget.h"
#include <string>

typedef void(*ButtonCallback)(void* this_ptr);

class Button : public Widget {
public:
	Button(std::string label, std::string button_text, ButtonCallback callback, void* this_ptr) : strLabel(label), strButtonText(button_text), pCallback(callback), pThisPtr(this_ptr)
	{
		default_height = 26.f;
	}
	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) override;

private:
	ButtonCallback pCallback;
	void* pThisPtr;
	std::string strLabel;
	std::string strButtonText;
	float text_size = 0.f;
	bool text_size_calculated = false;
};