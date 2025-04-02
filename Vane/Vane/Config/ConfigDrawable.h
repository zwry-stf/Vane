#pragma once
#include <string>
#include <optional>

class ConfigDrawable {
public:
	ConfigDrawable(const std::string& _name) : name(_name)
	{
	}

	void Render(float x, float y, float w, float alpha, int id, int* hovered, int* selected);
	std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam, int id, int* hovered, int* selected);
	void InitAnimation();

	void StartTyping();
	void StopTyping();

public:
	std::string name;
	static inline float default_height = 64.f;

private:
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;

	float animation_hovered = 0.f;
	float animation_selected = 0.f;

	float animation_delete_hovered = 0.f;
	float animation_delete_selected = 0.f;

	float animation_load_hovered = 0.f;
	float animation_load_selected = 0.f;

	float animation_save_hovered = 0.f;
	float animation_save_selected = 0.f;

	int hovered_button = -1;
	int selected_button = -1;

private:
	// Typing stuff
	std::string typing_buffer;

	bool typing = false;
	int typing_pos = 0;
	float pos_to_get = -1.f;


	int64_t last_blink = 0;
	float blinking_animation = 0.f;
	bool blink_toggle = false;

	float last_string_width = 0.f;
};