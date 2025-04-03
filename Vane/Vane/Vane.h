// Copyright (c) 2025 Jakob
// All rights reserved.
// Redistribution or modification of this code is prohibited without permission.

#pragma once
#include <stdint.h>
#include <optional>
#include <atomic>
#include <filesystem>
#include <mutex>


#include "Renderer/Xyphra.h"
#include "Renderer/api/d3dapi.h"
#include "Contents/Label.h"
#include "Contents/ConfigTab.h"
#include "Config/ConfigDrawable.h"

enum ItemIds
{
	Sidebar_Begin = 100,
	Topbar_Begin = 200,
	Tab_Begin = 300
};

class Vane {
public:
	inline static _Xyphra renderer;

	class Style {
	public:
		// Sizes						     
		inline static const float TextSize   = 18.f;
		inline static float Rounding         = 8.f;
		inline static float BorderSize       = 1.f;

		inline static float TabHeight        = 28.f;
		inline static float Spacing          = 8.f;

		inline static float TopBarHeight     = 48.f;
		inline static float ChildTabWidth    = 100.f;

		inline static float SideBarColl      = 68.f;
		inline static float SideBarWidth     = 140.f;

		inline static float ChildWindowWidth = 220.f;

		inline static float AnimationSpeed   = 8.f;

		// Colors						     
		inline static XyColor Background     = XyColor(0.1f, 0.1f, 0.1f, 0.65f);
		inline static XyColor Text           = XyColor(0xE0, 0xE8, 0xFF);
		inline static XyColor Icon           = XyColor(0xE0, 0xE8, 0xFF);
		inline static XyColor Accent         = XyColor(0xB2, 0xC6, 0xFF);
		inline static XyColor Accent2        = XyColor(0xA5, 0xA0, 0xBC, 0x75);
		inline static XyColor Highlight      = XyColor(1.f, 1.f, 1.f);
		inline static XyColor Disabled       = XyColor(0.5f, 0.5f, 0.5f);
	};

	class Data {
	public:
		inline static XyFont* Font         = NULL;

		inline static XyVec2 LastCursorPos = { 0.f, 0.f };
	};

	class Util {
	public:
		static float   Lerp(const float curr, const float target, float speed);
		static XyColor InterpCol(const XyColor& curr, const XyColor& target, const float t);
		static XyColor ConvColorEx(const XyColor& col, float animation);
		static XyColor ConvColor(const XyColor& col);
		static XyColor ConvColor(const XyColor& col, float animation);
		static XyColor DisableColor(const XyColor& col, float animation);
		static bool    LoadIconXy(const unsigned char* data, int size, XyTextureID* icon);
		static XyVec2  RotatePoint(XyVec2 point, XyVec2 center, float angle_rad);
	};

	class Background {
	public:
		static bool Init();
		static void Destroy();
		static void Render();

	public:
		struct ShaderConstants
		{
			XyVec2 iResolution;
			int BlurEnabled   = true;
			int BlurMenuOnly  = true;

			XyVec4 MenuPos;
			
			float iTime       = 0.f;
			float Animation;
			float NoiseScale  = 0.3f;
			float Rounding;

			float ShadowSize  = 20.f;
			float ShadowAlpha = 0.6f;

			float TURB_AMP    = 1.35f;
			float TURB_NUM    = 8.0;

			float TURB_SPEED  = 1.2;
			float TURB_FREQ   = 5.2;
			float TURB_EXP    = 1.25;
			float pad;

			XyColor BackgroundColor = XyColor(0.f, 0.f, 0.f, 0.3f);
			XyColor MenuBackgroundColor;
			XyColor AnimationColor  = XyColor(1.f, 1.f, 1.f, 0.2f);
		};

		inline static ConstantBuffer<ShaderConstants> ConstantBuffer;

	private:
		inline static ID3D11Texture2D* m_pOffscreenTexture      = nullptr;
		inline static ID3D11ShaderResourceView* m_pOffscreenSRV = nullptr;
		inline static ID3D11Texture2D* m_p1PassTexture          = nullptr;
		inline static ID3D11ShaderResourceView* m_p1PassSRV     = nullptr;
		inline static ID3D11RenderTargetView* m_p1PassRTV	    = nullptr;
		inline static ID3D11Texture2D* m_pBackBuffer            = nullptr;
		inline static ID3D11VertexShader* m_pQuadVS				= nullptr;
		inline static ID3D11PixelShader* m_pBlurPS				= nullptr;
		inline static ID3D11PixelShader* m_pBlurPSV				= nullptr;
		inline static ID3D11InputLayout* m_pInputLayout			= nullptr;
		inline static ID3D11Buffer* m_pQuadVB					= nullptr;
		inline static ID3D11Buffer* m_pQuadIB					= nullptr;
		inline static ID3D11SamplerState* m_pSamplerState		= nullptr;
		inline static ID3D11BlendState* m_pBlendState			= nullptr;
	};

	class Cursor {
	public:
		inline static std::atomic<HCURSOR> actual = NULL;
		inline static HCURSOR current			  = NULL;

		inline static HCURSOR arrow				  = NULL;
		inline static HCURSOR hand				  = NULL;
		inline static HCURSOR text				  = NULL;
		inline static HCURSOR size				  = NULL;
		inline static HCURSOR sizev				  = NULL;
		inline static HCURSOR sizeall			  = NULL;

		static bool Init();
		static void Set();
	};

	class Icons {
	public:
		inline static XyTextureID ChildIcon   = NULL;
		inline static XyTextureID SidebarIcon = NULL;
		inline static XyTextureID IconDelete  = NULL;
		inline static XyTextureID IconLoad    = NULL;
		inline static XyTextureID IconSave    = NULL;


		static bool Init();
		static void Destroy();

		struct IconData {
			IconData(const unsigned char* Data, size_t Size, XyTextureID* Texture) : data(Data), data_size(Size), texture(Texture)
			{
			}

			const unsigned char* data = 0;
			size_t data_size = 0;
			XyTextureID* texture;
		};

		inline static std::vector<IconData> icons;
	};

	class Errors {
	private:
		inline static std::string error = "";

	public:
		inline static void Clear()
		{ 
			error = "";
		}
		inline static void Set(const std::string& err)
		{
			error = err; 
		}
		inline static const std::string& Get()
		{ 
			return error;
		}
	};

	class Config {
	public:
		static bool Init(const std::string& projName);

		static bool Save();
		static bool Load();

		static bool SaveLastFile();
		static void SetCurr();

		static bool FetchConfigs();
		static bool Create();
		static bool Remove();
		static bool Rename(const std::string& new_name);

		struct IModule {
			virtual void Reset() = 0;
			virtual bool Load(const char* s_name, const unsigned char* data) = 0;
			virtual void Save(XyVector<unsigned char>& buffer) const = 0;
		};

		template<typename T>
		struct Module : public IModule {
			Module(const std::string& name, T* data) : value(data), name(name), default_value(*data)
			{
			}

			std::string name;
			T* value;
			T default_value;

			void Reset() override
			{
				*value = default_value;
			}

			bool Load(const char* s_name, const unsigned char* rawData) override
			{
				if (strcmp(s_name, name.c_str()) == 0)
				{
					std::memcpy(value, rawData, sizeof(T));
					return true;
				}
				return false;
			}

			void Save(XyVector<unsigned char>& buffer) const override
			{
				unsigned char nameLen = static_cast<unsigned char>(name.size());
				buffer.push_back(nameLen);
				for (char c : name)
				{
					buffer.push_back(static_cast<unsigned char>(c));
				}

				buffer.push_back(static_cast<unsigned char>(sizeof(T)));

				unsigned char* raw = reinterpret_cast<unsigned char*>(value);
				for (size_t i = 0; i < sizeof(T); i++)
				{
					buffer.push_back(raw[i]);
				}
			}

		};

		template<typename T>
		static void AddModule(T* data, const std::string& path)
		{
			XY_ASSERT(!Vane::Initialized && "Add Modules before initializing");

			module_configs.push_back(new Module<T>(path, data));
		}

	public:
		inline static std::filesystem::path config_path;
		inline static std::string curr_file;
		inline static std::mutex mutex;

		// Configs
		inline static int selected_config = 0;
		inline static std::vector<ConfigDrawable> configs;

		// Modules
		inline static std::vector<IModule*> module_configs;
	};

public:
	static bool Init(IDXGISwapChain* swapchain, const XyVec2 menuSize = XyVec2(800.f, 550.f));
	static void Destroy();

	static bool Render();
	static std::optional<long> WndProc(uint32_t msg, uint64_t wParam, int64_t lParam);

	// Add Widgets
	static TabNormal* AddTab(const std::string& text);
	static void AddLabel(const std::string& text);
	static void AddConfigTab(const std::string& text);
	static void AddDefaultThemeTab();

	static TabNormal* LastTab();

public:
	inline static bool open = true;
	inline static float animation = 0.f;

	inline static bool Initialized = false;
	inline static bool RenderingStarted = false;

	inline static float x = -1.f;
	inline static float y = -1.f;
	inline static float w = 0.f;
	inline static float h = 0.f;

	inline static XyVector<Tab*> Tabs;
	inline static int SelectedTab = -1;

	inline static constexpr float MinAlpha = 1.f / 255.f;

public:
	// Overlays
	inline static int OpenedOverlay = -1;
	inline static XyVector<Overlay*> Overlays;

private:
	inline static int Hovered = -1;
	inline static int Selected = -1;

	inline static bool Moving = false;
	inline static XyVec2 MovingOff;
};


#define isInRect(posx, posy, x, y, w, h) (posx >= x && posx <= x + w && posy >= y && posy <= y + h)
#define isInRect2(posx, posy, x, y, x2, y2) (posx >= x && posx <= x2 && posy >= y && posy <= y2)