#pragma once
#include <optional>
#include <stdint.h>

#include "../Renderer/XyphraDef.h"
#include "ChildTab.h"

class Tab {
public:
	Tab(const std::string& name) : name(name)
	{
	}
	~Tab() = default;

	float animation = 0.f;
	float animation_hovered = 0.f;

	virtual void Render(float x, float y, float w, int id, int* hovered, int* selected);
	virtual std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected);

public:
	bool isTab = false;

protected:
	bool text_width_calculated = false;
	float text_width = 0.f;
	std::string name;
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;
};

class TabNormal : public Tab {
public:
	TabNormal(const std::string& name) : Tab(name)
	{
		isTab = true;
	}

	void Render(float x, float y, float w, int id, int* hovered, int* selected) override;
	std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected) override;

public:
	XyVector<ChildTab*> ChildTabs;

	ChildTab* AddChildTab(const std::string& name, int num_areas);
	ChildTab* GetLastChildTab();

protected:
	int SelectedChild = 0;
	bool last_active = false;
};