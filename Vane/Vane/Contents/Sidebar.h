#pragma once
#include <optional>
#include <stdint.h>

#include "../Renderer/XyphraDef.h"

class Sidebar {
public:
	inline static float animation = 0.f;
	inline static float animation_hovered = 0.f;
	inline static bool open = false;

	static void Render(int* hovered, int* selected);
	static std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int* hovered, int* selected);

	enum ItemIds
	{
		ID_Icon = 0
	};
};