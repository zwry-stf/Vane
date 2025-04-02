#pragma once
#include "XyphraDef.h"
#include <d3d11.h>

class _Xyphra {
public:
	_Xyphra();
	bool init(IDXGISwapChain* pSwapChain);
	void destroy();

	bool start_frame(bool update_frame_time = true);
	void end_frame();

public:
	UData data = {};
	XyDrawListSharedData shared_data = {};
	DX11Data dx11 = {};

private:
	BData bg = {};

private:
	bool create_dx11_resources();
	void setup_render_state();

public:
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	ID3D11RenderTargetView* g_pd3dRenderTargetView = nullptr;
	D3D11_TEXTURE2D_DESC g_backBufferDesc = {};
	HWND g_hWnd = nullptr;

	// Text Rendering
public:
	XyFontAtlas Fonts;

	// DrawList Code
public:
	// This is what you have to render
	XyVector<XyCmd>         CmdBuffer;          // Draw commands. Typically 1 command = 1 GPU draw call, unless the command is a callback.
	XyVector<XyIndex>       IdxBuffer;          // Index buffer. Each command consume ImDrawCmd::ElemCount of those
	XyVector<XyVertex>      VtxBuffer;          // Vertex buffer.
	XyDrawListFlags         Flags;              // Flags, you may poke into these to adjust anti-aliasing settings per-primitive.

	// [Internal, used while building lists]
	unsigned int            _VtxCurrentIdx;     // [Internal] generally == VtxBuffer.Size unless we are past 64K vertices, in which case this gets reset to 0.
	XyVertex* _VtxWritePtr;       // [Internal] point within VtxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
	XyIndex* _IdxWritePtr;       // [Internal] point within IdxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
	XyVector<XyVec4>        _ClipRectStack;     // [Internal]
	XyVector<XyTextureID>   _TextureIdStack;    // [Internal]
	XyVector<XyVec2>        _Path;              // [Internal] current path building
	XyCmdHeader             _CmdHeader;         // [Internal] template of active commands. Fields should match those of CmdBuffer.back().
	float                   _FringeScale;       // [Internal] anti-alias fringe is scaled by this value, this helps to keep things sharp while zooming at vertex buffer content

private:
	void AddDrawCmd();
	void _OnChangedClipRect();
	void _OnChangedTextureID();

public:

	// Drawing Functions
	void  PrimReserve(int idx_count, int vtx_count);
	void  PrimUnreserve(int idx_count, int vtx_count);
	void  PrimRect(const XyVec2& a, const XyVec2& b, XyU32 col);      // Axis aligned rectangle (composed of two triangles)
	void  PrimQuadUV(const XyVec2& a, const XyVec2& b, const XyVec2& c, const XyVec2& d, const XyVec2& uv_a, const XyVec2& uv_b, const XyVec2& uv_c, const XyVec2& uv_d, XyU32 col);
	void  PrimRectUV(const XyVec2& a, const XyVec2& c, const XyVec2& uv_a, const XyVec2& uv_c, XyU32 col);
	inline    void  PrimWriteVtx(const XyVec2& pos, const XyVec2& uv, XyU32 col) { _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
	inline    void  PrimWriteIdx(XyIndex idx) { *_IdxWritePtr = idx; _IdxWritePtr++; }
	inline    void  PrimVtx(const XyVec2& pos, const XyVec2& uv, XyU32 col) { PrimWriteIdx((XyIndex)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); } // Write vertex with unique index

	void  AddConvexPolyFilled(const XyVec2* points, int num_points, XyU32 col);
	void  AddPolyline(const XyVec2* points, int num_points, XyU32 col, XyDrawFlags flags, float thickness);

	XyColor       ColorConvertU32ToFloat4(XyU32 in);
	XyU32         ColorConvertFloat4ToU32(const XyVec4& in);
	void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
	void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

	/// Clip Rect
	void PushClipRect(const XyVec2& clip_rect_min, const XyVec2& clip_rect_max, bool intersect_with_current_clip_rect = false);
	void PopClipRect();

	/// Path
	inline void  PathClear() { _Path.Size = 0; }
	inline void  PathLineTo(const XyVec2& pos) { _Path.push_back(pos); }
	inline void  PathLineToMergeDuplicate(const XyVec2& pos) { if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0) _Path.push_back(pos); }
	inline void  PathFillConvex(XyU32 col) { AddConvexPolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0; }
	inline void  PathStroke(XyU32 col, XyDrawFlags flags = XyDrawFlags_None, float thickness = 1.0f) { AddPolyline(_Path.Data, _Path.Size, col, flags, thickness); _Path.Size = 0; }
	void         PathArcTo(const XyVec2& center, float radius, float a_min, float a_max, int num_segments = 0);
	void         PathArcToFast(const XyVec2& center, float radius, int a_min_of_12, int a_max_of_12);                // Use precomputed angles for a 12 steps circle
	void         PathBezierCubicCurveTo(const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, int num_segments = 0); // Cubic Bezier (4 control points)
	void         PathBezierQuadraticCurveTo(const XyVec2& p2, const XyVec2& p3, int num_segments = 0);               // Quadratic Bezier (3 control points)
	void         PathRect(const XyVec2& rect_min, const XyVec2& rect_max, float rounding = 0.0f, XyDrawFlags flags = XyDrawFlags_None);

	/// Rendering
	void  AddLine(const XyVec2& p1, const XyVec2& p2, XyU32 col, float thickness = 1.0f);
	void  AddLineMultiColor(const XyVec2& a, const XyVec2& b, XyU32 col_a, XyU32 col_b, float thickness = 1.f);
	void  AddRect(const XyVec2& p_min, const XyVec2& p_max, XyU32 col, float rounding = 0.0f, XyDrawFlags flags = 0, float thickness = 1.0f);   // a: upper-left, b: lower-right (== upper-left + size)
	void  AddRectFilled(const XyVec2& p_min, const XyVec2& p_max, XyU32 col, float rounding = 0.0f, XyDrawFlags flags = 0);                     // a: upper-left, b: lower-right (== upper-left + size)
	void  AddRectFilledMultiColor(const XyVec2& p_min, const XyVec2& p_max, XyU32 col_upr_left, XyU32 col_upr_right, XyU32 col_bot_right, XyU32 col_bot_left);
	void  AddRectFilledMultiColorRounded(const XyVec2& p_min, const XyVec2& p_max, XyU32 col_upr_left, XyU32 col_upr_right, XyU32 col_bot_right, XyU32 col_bot_left, float rounding = 0, XyDrawFlags rounding_corners = 0);
	void  AddQuad(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, XyU32 col, float thickness = 1.0f);
	void  AddQuadFilled(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, XyU32 col);
	void  AddTriangle(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, XyU32 col, float thickness = 1.0f);
	void  AddTriangleFilled(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, XyU32 col);
	void  AddTriangleFilledMultiColor(const XyVec2& a, const XyVec2& b, const XyVec2& c, XyU32 aCol, XyU32 bCol, XyU32 cCol);
	void  AddCircle(const XyVec2& center, float radius, XyU32 col, int num_segments = 0, float thickness = 1.0f);
	void  AddCircleFilled(const XyVec2& center, float radius, XyU32 col, int num_segments = 0);
	void  AddNgon(const XyVec2& center, float radius, XyU32 col, int num_segments, float thickness = 1.0f);
	void  AddNgonFilled(const XyVec2& center, float radius, XyU32 col, int num_segments);

	void  AddImage(XyTextureID user_texture_id, const XyVec2& p_min, const XyVec2& p_max, const XyVec2& uv_min = XyVec2(0, 0), const XyVec2& uv_max = XyVec2(1, 1), XyU32 col = XY_COL32_WHITE);
	void  AddImageQuad(XyTextureID user_texture_id, const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, const XyVec2& uv1 = XyVec2(0, 0), const XyVec2& uv2 = XyVec2(1, 0), const XyVec2& uv3 = XyVec2(1, 1), const XyVec2& uv4 = XyVec2(0, 1), XyU32 col = XY_COL32_WHITE);
	void  AddImageRounded(XyTextureID user_texture_id, const XyVec2& p_min, const XyVec2& p_max, const XyVec2& uv_min, const XyVec2& uv_max, XyU32 col, float rounding, XyDrawFlags flags = 0);
	void  ShadeVertsLinearUV(int vert_start_idx, int vert_end_idx, const XyVec2& a, const XyVec2& b, const XyVec2& uv_a, const XyVec2& uv_b, bool clamp);

private:
	void _PathArcToN(const XyVec2& center, float radius, float a_min, float a_max, int num_segments);
	void _PathArcToFastEx(const XyVec2& center, float radius, int a_min_sample, int a_max_sample, int a_step);
	int  _CalcCircleAutoSegmentCount(float radius) const;

public:
	/// Shadows
	void  AddSubtractedRect(const XyVec2& a_min, const XyVec2& a_max, const XyVec2& a_min_uv, const XyVec2& a_max_uv, XyVec2* b_points, int num_b_points, XyU32 col);
	void  AddSubtractedRect(const XyVec2& a_min, const XyVec2& a_max, const XyVec2& a_min_uv, const XyVec2& a_max_uv, XyVec2 b_min, XyVec2 b_max, XyU32 col);
	void  AddShadowRect(const XyVec2& obj_min, const XyVec2& obj_max, XyU32 shadow_col, float obj_rounding = 0.f, float shadow_thickness = 50.f, const XyVec2& shadow_offset = XyVec2(0.f, 0.f), XyDrawFlags flags = 0);
	void  AddShadowCircle(const XyVec2& obj_center, float obj_radius, XyU32 shadow_col, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags = 0, int obj_num_segments = 12);
	void  AddShadowConvexPoly(const XyVec2* points, int points_count, XyU32 shadow_col, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags = 0);
	void  AddShadowNGon(const XyVec2& obj_center, float obj_radius, XyU32 shadow_col, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags, int obj_num_segments);

	/// Fonts
	 // search_type 0: by pos, 1: by index
	float AddTextTest(const XyVec2& pos, XyU32 col, const char* text_begin,
		float pos_find, void* out, bool search_type);
	float AddTextTest(const XyFont* font, float font_size, const XyVec2& pos, XyU32 col, const char* text_begin,
		float pos_find, void* out, bool search_type, float wrap_width = 0.0f, const XyVec4* cpu_fine_clip_rect = NULL);
	float AddText(const XyVec2& pos, XyU32 col, const char* text_begin, const char* text_end = NULL);
	float AddText(const XyFont* font, float font_size, const XyVec2& pos, XyU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const XyVec4* cpu_fine_clip_rect = NULL);
	float CalcTextWidth(const char* text, const char* text_end = 0, float wrap_width = 0.f);

	void PopFont();
	void SetCurrentFont(XyFont* font);
	void PushFont(XyFont* font);
	void PushTextureID(XyTextureID texture_id);
	void PopTextureID();

private:
	XyVector<XyFont*>       FontStack;

public:
	XyFont* Font;                               // (Shortcut) == FontStack.empty() ? IO.Font : FontStack.back()
	float                   FontSize;                           // (Shortcut) == FontBaseSize * g.CurrentWindow->FontWindowScale == window->FontSize(). Text height for current window.
	float                   FontBaseSize;                       // (Shortcut) == IO.FontGlobalScale * Font->Scale * Font->FontSize. Base text height.

};