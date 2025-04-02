#pragma once
#include "../Widget.h"
#include <string>

class Dropdown : public Overlay {
public:
	Dropdown(XyVector<std::string>* options, int* selected_option) : pOptions(options), pSelected(selected_option)
	{
	}

	void Draw(const int id, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened) override;

public:
	XyVector<std::string>* pOptions;
	XyVector<std::pair<float, float>> animations = { { 0.f, 0.f }, { 0.f, 0.f } }; //first selected, second hover
	int* pSelected;
	float Width = 0.f;

	int hovered = -1;
};

class DropdownWidget : public Widget {
public:
	DropdownWidget(const std::string& label) : strLabel(label) 
	{
		default_height = 26.f;
	};

	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;
	std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened) override;
private:
	std::string strLabel;
};