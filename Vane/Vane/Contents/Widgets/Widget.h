#pragma once
#include <stdint.h>
#include <optional>
#include "../../Renderer/XyphraDef.h"

class Overlay;
class Widget {
public:
	Widget() = default;
	~Widget() = default;

	virtual void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened)
	{
	}
	virtual std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* hovered, int* selected, int* opened)
	{
		return {};
	}

	virtual void InitAnimation()
	{
		if (disabled)
			animation_disabled = *disabled ? 1.f : 0.f;

		animation_hovered = 0.f;
		animation_selected = 0.f;
	}

	void SetDisabled(bool* value)
	{
		disabled = value;
		dis_inv = false;
	}
	void SetDisabledInv(bool* value)
	{
		disabled = value;
		dis_inv = true;
	}

public:
	float default_height = 0.f;
	int area = 0; // The area of the tab the widget will be in
	bool endGroup = false; // end a group of widgets
	bool isTitle = false;
	int child_id = -1;

	XyVector<Overlay*>* pOverlays = nullptr;

protected:
	bool* disabled = 0;
	bool dis_inv = false;
	float animation_disabled = 0.f;

	float animation_hovered = 0.f;
	float animation_selected = 0.f;

	float lastX = 0, lastY = 0, lastW = 0, lastH = 0;
};

class GroupTitle : public Widget {
public:
	GroupTitle(const std::string& title) : strLabel(title)
	{
		isTitle = true;
		default_height = 20.f;
	}

	void Draw(const float x, const float y, const float w, const float alpha, const int id, int* hovered, int* selected, int* opened) override;

	bool text_width_calculated = false;
	float text_width = 0.f;
	std::string strLabel = "";
};

class Overlay {
public:
	Overlay() = default;
	~Overlay() = default;

	virtual void Draw(const int id, int* opened)
	{
	}
	virtual std::optional<long> WndProc(const uint32_t msg, const uint64_t wParam, int64_t lParam, const int id, int* opened)
	{
		return {};
	}

	void SetPos(float x, float y)
	{ 
		xPos = roundf(x); 
		yPos = roundf(y);
		pos_changed = true;
	} // Should be called by parent

public:
	bool isColorPicker = false;
	bool isChildWindow = false;
	float animation = 0.f;
	float lastX = 0, lastY = 0, lastW = 0, lastH = 0;

protected:
	float Height = 0.f;
	float xPos = 0.f, yPos = 0.f;
	bool pos_changed = false;
};