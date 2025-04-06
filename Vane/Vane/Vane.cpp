#include "Vane.h"
#include "Res/fonts.hpp"
#include "Res/icons.hpp"
#include "Contents/Sidebar.h"
#include <d3dx11.h>
#include <string>

bool Vane::Init(IDXGISwapChain* swapchain, const XyVec2 menuSize)
{
	w = menuSize.x;
	h = menuSize.y;

	if (Initialized)
	{
		renderer.destroy();
		if (!renderer.init(swapchain))
		{
			Vane::Errors::Set("Failed to initialize Renderer");
			return false;
		}

		Icons::Destroy();
		if (!Icons::Init())
		{
			Vane::Errors::Set("Failed to load Icons");
			return false;
		}

		Vane::Background::Destroy();
		if (!Vane::Background::Init())
		{
			std::string old = Vane::Errors::Get();
			Vane::Errors::Set("Failed to initialize screen background ( \"" + old + "\" )");
		    return false;
		}

		x = (renderer.data.display_size.x - w) / 2.f;
		y = (renderer.data.display_size.y - h) / 2.f;

		return true;
	}

	if (!Config::Init("Vane Example"))
	{
		Vane::Errors::Set("Failed to initialize Config");
		return false;
	}

	renderer.shared_data.InitialFlags = (XyDrawListFlags)(XyDrawListFlags_AntiAliasedLines | XyDrawListFlags_AntiAliasedFill);

	// Create Fonts
	XyVector<XyWchar> ranges;
	XyFontGlyphRangesBuilder builder;
	builder.AddRanges(renderer.Fonts.GetGlyphRangesDefault());
	builder.AddRanges(renderer.Fonts.GetGlyphRangesJapanese());
	builder.AddRanges(renderer.Fonts.GetGlyphRangesCyrillic());
	builder.BuildRanges(&ranges);

	XyFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;
	cfg.GlyphOffset.y = -1.f;
	cfg.OversampleH = 2;
	Data::Font = renderer.Fonts.AddFontFromMemoryTTF((void*)Fonts::_acNotoSansJP_Medium, sizeof(Fonts::_acNotoSansJP_Medium), Style::TextSize, &cfg, ranges.Data);
	if (!Data::Font)
	{
		Vane::Errors::Set("Failed to load Default Font");
		return false;
	}

	// Build Fonts
	if (!renderer.Fonts.Build())
	{
		Vane::Errors::Set("Failed to build Fonts");
		return false;
	}
	if (!renderer.init(swapchain))
	{
		Vane::Errors::Set("Failed to initialize Renderer");
		return false;
	}

	renderer.data.display_size = XyVec2((float)renderer.g_backBufferDesc.Width, (float)renderer.g_backBufferDesc.Height);

	if (!Cursor::Init())
	{
		Vane::Errors::Set("Failed to load Cursors");
		return false;
	}

	if (!Background::Init())
	{
		std::string old = Vane::Errors::Get();
		Vane::Errors::Set("Failed to initialize screen background ( \"" + old + "\" )");
		return false;
	}

	if (!Icons::Init())
	{
		Vane::Errors::Set("Failed to load Icons");
		return false;
	}

	x = (renderer.data.display_size.x - w) / 2.f;
	y = (renderer.data.display_size.y - h) / 2.f;

	Initialized = true;

	return true;
}

void Vane::Destroy()
{
	renderer.destroy();
}

bool Vane::Render()
{
	renderer.g_pd3dDeviceContext->OMSetRenderTargets(1, &renderer.g_pd3dRenderTargetView, nullptr);

	// Mouse Input
	{
		// Reset hovered state
		Vane::Cursor::current = Vane::Cursor::arrow;
		Hovered = -1;

		POINT pt;
		if (!GetCursorPos(&pt))
		{
			XY_ASSERT(false && "GetCursorPos failed"); // should never happen
			return false;
		}

		HWND hwndUnderCursor = WindowFromPoint(pt);
		if (hwndUnderCursor != renderer.g_hWnd && !Moving)
		{
			if (!ScreenToClient(renderer.g_hWnd, &pt))
			{
				XY_ASSERT(false && "ScreenToClient failed"); // should never happen
				return false;
			}

			if (pt.x < 0 || pt.x > renderer.data.display_size.x || pt.y < 0 || pt.y > renderer.data.display_size.y) // correctly give out of screen input
			{
				Data::LastCursorPos = { (float)pt.x, (float)pt.y };
				WndProc(WM_MOUSEMOVE, 0, MAKELPARAM((int16_t)pt.x, (int16_t)pt.y));
			}
			else
			{
				Data::LastCursorPos = { FLT_MAX, FLT_MAX };
				WndProc(WM_MOUSEMOVE, 0, MAKELPARAM((int16_t)0x7FFF, (int16_t)0x7FFF)); // if window over our window
			}
		}
		else
		{
			if (!ScreenToClient(renderer.g_hWnd, &pt))
			{
				XY_ASSERT(false && "ScreenToClient failed"); // should never happen
				return false;
			}

			Data::LastCursorPos = { (float)pt.x, (float)pt.y };
			WndProc(WM_MOUSEMOVE, 0, MAKELPARAM((int16_t)pt.x, (int16_t)pt.y));
		}
	}

	Animation = Util::Lerp(Animation, IsOpen ? 1.f : 0.f, Style::AnimationSpeed);

	{
		static XyVec2 old_size = { 0.f, 0.f };

		if (old_size.x != renderer.data.display_size.x || old_size.y != renderer.data.display_size.y)
		{
			if (x < 0.f || y < 0.f)
			{
				x = (renderer.data.display_size.x - w) / 2.f;
				y = (renderer.data.display_size.y - h) / 2.f;
			}
			else
			{
				if (x + w > renderer.data.display_size.x)
					x = renderer.data.display_size.x - w;
				if (y + h > renderer.data.display_size.y)
					y = renderer.data.display_size.y - h;
				if (x < 0.f)
					x = 0.f;
				if (y < 0.f)
					y = 0.f;
			}

			old_size = renderer.data.display_size;
		}
	}

	// Start Frame
	if (!renderer.start_frame())
	{
		XY_ASSERT(false && "Failed to start frame");
		return false;
	}

	RenderingStarted = true;

	// make sure window not out of screen
	if (x > renderer.data.display_size.x - 100.f)
		x = renderer.data.display_size.x - 100.f;
	else if (x < -w + 100.f)
		x = -w + 100.f;
	if (y > renderer.data.display_size.y - 100.f)
		y = renderer.data.display_size.y - 100.f;
	else if (y < -h + 100.f)
		y = -h + 100.f;

	x = roundf(x);
	y = roundf(y);

	if (Animation < MinAlpha)
		goto skipRender;

	// Background
	Background::Render();

	// Sidebar
	Sidebar::Render(&Hovered, &Selected);

	// Border
	renderer.AddRect(XyVec2(x, y), XyVec2(x + w, y + h), Util::ConvColor(Style::Accent), Style::Rounding, 0, Style::BorderSize);

	// Dim Background
	{
		float highest = 0.f;
		for (auto& overlay : Overlays)
		{
			if (overlay->animation > highest)
				highest = overlay->animation;
		}

		if (highest > MinAlpha)
			renderer.AddRectFilled(XyVec2(x, y), XyVec2(x + w, y + h), Util::ConvColor(XyColor(0.f, 0.f, 0.f, 0.3f), highest), Style::Rounding);
	}

	// Render Overlays
	for (int i = 0; i < Overlays.Size; i++)
	{
		Overlays[i]->Draw(i, &OpenedOverlay);
	}
skipRender:

	//#ifdef _DEBUG
#if (1)
	{
		/// Fps
		static float fps_full = 0.f;
		static int counted = 0;
		static int64_t last_update = 0;
		static float curr_fps = 0.f;
		float fps = 1.f / renderer.data.delta_time;

		fps_full += fps;
		counted++;

		if (renderer.data.curr_time - last_update >= 1000)
		{
			last_update = renderer.data.curr_time;
			curr_fps = fps_full / counted;
			fps_full = 0.f;
			counted = 0;
		}
		renderer.AddText(XyVec2(2.f, 2.f), Style::Text, std::to_string((int)curr_fps).c_str());
	}
#endif

	renderer.end_frame();

	// Cursor
	Cursor::actual.store(Cursor::current);

	return true;
}

std::optional<long> Vane::WndProc(uint32_t msg, uint64_t wParam, int64_t lParam)
{
	float mouseX = (float)(int16_t)LOWORD(lParam);
	float mouseY = (float)(int16_t)HIWORD(lParam);

	if (msg == WM_KEYDOWN)
	{
		if (wParam == VK_INSERT)
		{
			IsOpen = !IsOpen;
			// Enable/Disable Mouse here
			return S_OK;
		}
	}

	if (!IsOpen)
		return {};

	if (msg == WM_MOUSEWHEEL)
	{
		POINT point;
		point.x = (LONG)mouseX;
		point.y = (LONG)mouseY;

		// Scroll tab
		ScreenToClient(Vane::renderer.g_hWnd, &point);

		lParam = MAKELPARAM((int16_t)point.x, (int16_t)point.y);
	}

	if (OpenedOverlay != -1)
	{
		auto ret = Overlays[OpenedOverlay]->WndProc(msg, wParam, lParam, OpenedOverlay, &OpenedOverlay);
		if (ret.has_value())
			return ret;
	}

	auto ret = Sidebar::WndProc(msg, wParam, lParam, &Hovered, &Selected);
	if (ret.has_value())
		return ret.value();

	if (msg == WM_LBUTTONDOWN)
	{
		if (isInRect(mouseX, mouseY, x, y, w, h))
		{
			MovingOff = XyVec2(mouseX - x, mouseY - y);
			Moving = true;
			return S_OK;
		}
	}

	else if (msg == WM_MOUSEMOVE)
	{
		if (Moving)
		{
			x = mouseX - MovingOff.x;
			y = mouseY - MovingOff.y;

			if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
				Moving = false;
			return S_OK;
		}

		return S_OK; // Dont let input through
	}

	else if (msg == WM_LBUTTONUP)
	{
		if (Moving)
		{
			Moving = false;
			return S_OK;
		}

		return S_OK; // Dont let input through
	}

	else if (msg == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE)
		{
			IsOpen = false;
			// Disable Mouse here
			return S_OK;
		}

		return S_OK; // Dont let input through
	}

	return {};
}

TabNormal* Vane::LastTab()
{
	XY_ASSERT(!Tabs.empty() && Tabs.back()->isTab);
	return (TabNormal*)Tabs.back();
}

float Vane::Util::Lerp(const float curr, const float target, float speed)
{
	speed *= renderer.data.delta_time;
	float ret = curr + (target - curr) * speed;
	if (curr <= target && ret > target)
		ret = target; // Limit so it does not jump back and forth
	else if (curr > target && ret < target)
		ret = target;
	return ret;
}

XyColor Vane::Util::InterpCol(const XyColor& curr, const XyColor& target, const float t)
{
	float r = curr.r + (target.r - curr.r) * t;
	float g = curr.g + (target.g - curr.g) * t;
	float b = curr.b + (target.b - curr.b) * t;
	float a = curr.a + (target.a - curr.a) * t;
	return XyColor(r, g, b, a);
}

XyColor Vane::Util::ConvColorEx(const XyColor& col, float animation)
{
	return XyColor(col.r, col.g, col.b, col.a * animation);
}

XyColor Vane::Util::ConvColor(const XyColor& col)
{
	return ConvColorEx(col, Animation);
}

XyColor Vane::Util::ConvColor(const XyColor& col, float _animation)
{
	return ConvColorEx(col, Animation * _animation);
}

XyColor Vane::Util::DisableColor(const XyColor& col, float t)
{
	if (t < MinAlpha)
		return col;

	XyColor out = {
	 col.r + (Style::Disabled.r - col.r) * t,
	 col.g + (Style::Disabled.g - col.g) * t,
	 col.b + (Style::Disabled.b - col.b) * t,
	 col.a
	};

	return out;
}

bool Vane::Util::LoadIconXy(const unsigned char* data, int size, XyTextureID* out)
{
	D3DX11_IMAGE_LOAD_INFO info;
	if (FAILED(
		D3DX11CreateShaderResourceViewFromMemory(renderer.g_pd3dDevice, data, size, &info, nullptr, (ID3D11ShaderResourceView**)out, nullptr)))
		return false;

	return true;
}


XyVec2 Vane::Util::RotatePoint(XyVec2 point, XyVec2 center, float angle_rad)
{
	float s = sinf(angle_rad);
	float c = cosf(angle_rad);
	point.x -= center.x;
	point.y -= center.y;
	float newX = point.x * c - point.y * s;
	float newY = point.x * s + point.y * c;
	return XyVec2(newX + center.x, newY + center.y);
}

bool Vane::Cursor::Init()
{
	arrow = LoadCursorA(NULL, IDC_ARROW);
	hand = LoadCursorA(NULL, IDC_HAND);
	text = LoadCursorA(NULL, IDC_IBEAM);
	size = LoadCursorA(NULL, IDC_SIZEWE);
	sizev = LoadCursorA(NULL, IDC_SIZENS);
	sizeall = LoadCursorA(NULL, IDC_SIZEALL);

	current = arrow;
	actual.store(arrow);

	return arrow && hand && text && size && sizev && sizeall;
}

void Vane::Cursor::Set()
{
	SetCursor(actual.load());
}

/// Icons

#define LOAD_ICON(icon, data) if (!Util::LoadIconXy(data, sizeof(data), &icon)) \
	return false

bool Vane::Icons::Init()
{
	LOAD_ICON(SidebarIcon, _Icons::_acicon_128x128);
	LOAD_ICON(ChildIcon, _Icons::_acchild);
	LOAD_ICON(IconDelete, _Icons::_acdelete);
	LOAD_ICON(IconLoad, _Icons::_acload);
	LOAD_ICON(IconSave, _Icons::_acsave);

	for (auto& icon : icons)
	{
		if (!Util::LoadIconXy(icon.data, icon.data_size, icon.texture))
			return false;
	}

	return true;
}

#define RELEASE_ICON(icon)  if (icon) \
		((ID3D11ShaderResourceView*)icon)->Release()

void Vane::Icons::Destroy()
{
	RELEASE_ICON(SidebarIcon);
	RELEASE_ICON(ChildIcon);
	RELEASE_ICON(IconDelete);
	RELEASE_ICON(IconLoad);
	RELEASE_ICON(IconSave);

	for (auto& icon : icons)
	{
		RELEASE_ICON(*icon.texture);
		*icon.texture = 0;
	}
}

/// Widgets
TabNormal* Vane::AddTab(const std::string& text)
{
	XY_ASSERT(!RenderingStarted && !Initialized);

	Tabs.push_back(new TabNormal(text));

	if (SelectedTab == -1)
		SelectedTab = Tabs.Size - 1 + Sidebar_Begin + 10;

	return (TabNormal*)Tabs.back();
}

void Vane::AddLabel(const std::string& text)
{
	XY_ASSERT(!RenderingStarted && !Initialized);

	Tabs.push_back(new Label(text));
}

void Vane::AddConfigTab(const std::string& text)
{
	XY_ASSERT(!RenderingStarted && !Initialized);

	Tabs.push_back(new TabConfig(text));

	if (SelectedTab == -1)
		SelectedTab = Tabs.Size - 1 + Sidebar_Begin + 10;
}

void Vane::AddDefaultThemeTab()
{
	XY_ASSERT(!RenderingStarted && !Initialized);

	TabNormal* tab = AddTab("Theme");

	ChildTab* child = tab->AddChildTab("Menu", 2);
	child->SetIcon(_Icons::_acTheme, sizeof(_Icons::_acTheme));

	child->AddGroupTitle("Colors", 0);

	child->AddColorpicker("Background", &Style::Background, true, 0);

	child->AddColorpicker("Child Window Background", &Style::CWBackground, true, 0);

	child->AddColorpicker("Accent", &Style::Accent, true, 0);

	child->AddColorpicker("Accent 2", &Style::Accent2, true, 0);

	child->AddColorpicker("Highlight", &Style::Highlight, true, 0);

	child->AddColorpicker("Text", &Style::Text, true, 0);

	child->AddColorpicker("Icon", &Style::Icon, true, 0);

	child->EndGroup(0);

	child->AddGroupTitle("Background", 0);

	auto child_window = child->AddChildWindow("Blur");

	child_window->AddCheckbox("Blur Menu Only", (bool*)&Background::ConstantBuffer->BlurMenuOnly)->SetDisabledInv((bool*)&Background::ConstantBuffer->BlurEnabled);

	child_window->AddSlider("Radius", &Background::BlurRadius, 5.f, 50.f, "%.1f");

	child_window->AddCheckbox("High Quality", &Background::HighQuality);

	child->AddCheckbox("Blur", (bool*)&Background::ConstantBuffer->BlurEnabled, 0)->LinkTo(child_window);

	child->AddSlider("Noise", &Background::ConstantBuffer->NoiseScale, 0.f, 0.8f, 0, "%.2f");

	child->AddColorpicker("Dim", &Background::ConstantBuffer->BackgroundColor, true, 0);

	child->EndGroup(0);

	child->AddGroupTitle("Values", 1);

	child->AddSlider("Animation Speed", &Style::AnimationSpeed, 3.f, 20.f, 1, "%.1f");

	child->AddSlider("Rounding", &Style::Rounding, 0.f, 20.f, 1, "%.1f");

	child->AddSlider("Child Window Size", &Style::ChildWindowWidth, 100.f, 400.f, 1, "%.1f");

	child->AddSlider("Shadow Size", &Background::ConstantBuffer->ShadowSize, 2.f, 100.f, 1, "%.1f");

	child->AddSlider("Shadow Alpha", &Background::ConstantBuffer->ShadowAlpha, 0.f, 1.f, 1, "%.2f");

	child->EndGroup(1);

	child->AddGroupTitle("Menu Background", 1);

	child->AddColorpicker("Color", &Background::ConstantBuffer->AnimationColor, true, 1);

	child->AddSlider("Amplitude", &Background::ConstantBuffer->TURB_AMP, 0.05f, 2.f, 1, "%.2f");

	child->AddSlider("Number", &Background::ConstantBuffer->TURB_NUM, 1.f, 20.f, 1, "%.0f");

	child->AddSlider("Speed", &Background::ConstantBuffer->TURB_SPEED, 0.05f, 2.f, 1, "%.2f");

	child->AddSlider("Frequency", &Background::ConstantBuffer->TURB_FREQ, 0.5f, 10.f, 1, "%.2f");

	child->AddSlider("Exponent", &Background::ConstantBuffer->TURB_EXP, 0.5f, 10.f, 1, "%.2f");

	child->EndGroup(1);
}