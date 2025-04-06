#pragma once
#include <math.h>
#include <cstring>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <chrono>
#include <d3d11.h>

typedef unsigned int        XyU32;  // 32-bit unsigned integer (often used to store packed colors)
typedef void* XyTextureID;          // Default: store a pointer or an integer fitting in a pointer (most renderer backends are ok with that)

typedef signed char         XyS8;   // 8-bit signed integer
typedef unsigned char       XyU8;   // 8-bit unsigned integer
typedef signed short        XyS16;  // 16-bit signed integer
typedef unsigned short      XyU16;  // 16-bit unsigned integer
typedef signed int          XyS32;  // 32-bit signed integer == int
typedef unsigned int        XyU32;  // 32-bit unsigned integer (often used to store packed colors)
typedef signed   long long  XyS64;  // 64-bit signed integer
typedef unsigned long long  XyU64;  // 64-bit unsigned integer

typedef unsigned short XyWchar16;   // A single decoded U16 character/code point. We encode them as multi bytes UTF-8 when used in strings.
typedef unsigned int XyWchar32;     // A single decoded U32 character/code point. We encode them as multi bytes UTF-8 when used in strings.
typedef XyWchar16 XyWchar;

class XyVec2 {
public:
    float x, y;

    /// 
    constexpr XyVec2() : x(0), y(0) {};
    constexpr XyVec2(float x, float y) : x(x), y(y) {};

    XyVec2 operator+(const XyVec2& other) const { return XyVec2(x + other.x, y + other.y); }
    XyVec2 operator+=(const XyVec2& other) { x += other.x; y += other.y; return *this; }
    XyVec2 operator-(const XyVec2& other) const { return XyVec2(x - other.x, y - other.y); }
    XyVec2 operator-=(const XyVec2& other) { x -= other.x; y -= other.y; return *this; }
    XyVec2 operator*(const XyVec2& other) const { return XyVec2(x * other.x, y * other.y); }
    XyVec2 operator*(const float other) const { return XyVec2(x * other, y * other); }
    XyVec2 operator*=(const XyVec2& other) { x *= other.x; y *= other.y; return *this; }
    XyVec2 operator*=(const float other) { x *= other; y *= other; return *this; }
    XyVec2 operator/(const XyVec2& other) const { return XyVec2(x / other.x, y / other.y); }
    XyVec2 operator/(const float other) const { return XyVec2(x / other, y / other); }
    XyVec2 operator/=(const XyVec2& other) { x /= other.x; y /= other.y; return *this; }
    XyVec2 operator/=(const float other) { x /= other; y /= other; return *this; }
};

class XyVec4 {
public:
    float x, y, z, w;

    /// 
    XyVec4() : x(0), y(0), z(0), w(0) {};
    XyVec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

    XyVec4 operator+(const XyVec4& other) const { return XyVec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    XyVec4 operator+=(const XyVec4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
    XyVec4 operator-(const XyVec4& other) const { return XyVec4(x - other.x, y - other.y, z - other.z, w - other.w); }
    XyVec4 operator-=(const XyVec4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
    XyVec4 operator*(const XyVec4& other) const { return XyVec4(x * other.x, y * other.y, z * other.z, w * other.w); }
    XyVec4 operator*=(const XyVec4& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
    XyVec4 operator/(const XyVec4& other) const { return XyVec4(x / other.x, y / other.y, z / other.z, w / other.w); }
    XyVec4 operator/=(const XyVec4& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
};

typedef XyVec4 XyRect;


#define XY_COL32_R_SHIFT    0
#define XY_COL32_G_SHIFT    8
#define XY_COL32_B_SHIFT    16
#define XY_COL32_A_SHIFT    24
#define XY_COL32_A_MASK     0xFF000000
#define XY_COL32(R,G,B,A)    (((XyU32)(A)<<XY_COL32_A_SHIFT) | ((XyU32)(B)<<XY_COL32_B_SHIFT) | ((XyU32)(G)<<XY_COL32_G_SHIFT) | ((XyU32)(R)<<XY_COL32_R_SHIFT))
#define XY_COL32_WHITE       XY_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define XY_COL32_BLACK       XY_COL32(0,0,0,255)        // Opaque black
#define XY_COL32_BLACK_TRANS XY_COL32(0,0,0,0)          // Transparent black = 0x00000000
static inline float  XySaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
#define XY_F32_TO_INT8_SAT(_VAL)        ((int)(XySaturate(_VAL) * 255.0f + 0.5f))               // Saturated, always output 0..255

class XyColor {
public:
    float r, g, b, a;

    /// 
    constexpr __forceinline XyColor() : r(0), g(0), b(0), a(0) {};
    constexpr __forceinline XyColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
    constexpr __forceinline XyColor(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {};
    constexpr __forceinline XyColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r((float)r / 255.f), g((float)g / 255.f), b((float)b / 255.f), a((float)a / 255.f) {};
    constexpr __forceinline XyColor(unsigned char r, unsigned char g, unsigned char b) : r((float)r / 255.f), g((float)g / 255.f), b((float)b / 255.f), a(1.f) {};
    constexpr __forceinline XyColor(int r, int g, int b, int a) : r((float)r / 255.f), g((float)g / 255.f), b((float)b / 255.f), a((float)a / 255.f) {};
    constexpr __forceinline XyColor(int r, int g, int b) : r((float)r / 255.f), g((float)g / 255.f), b((float)b / 255.f), a(1.f) {};
    constexpr __forceinline XyColor(const XyVec4& v) : r(v.x), g(v.y), b(v.z), a(v.w) {}

    operator XyRect() const { return XyRect(r, g, b, a); }
    operator float* () { return &r; }
    operator const float* () const { return &r; }

    bool operator!=(const XyColor& other) const { return r != other.r || g != other.g || b != other.b || a != other.a; }
    bool operator==(const XyColor& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }

    inline operator XyU32() const {
        XyU32 out;
        out = ((XyU32)XY_F32_TO_INT8_SAT(r)) << XY_COL32_R_SHIFT;
        out |= ((XyU32)XY_F32_TO_INT8_SAT(g)) << XY_COL32_G_SHIFT;
        out |= ((XyU32)XY_F32_TO_INT8_SAT(b)) << XY_COL32_B_SHIFT;
        out |= ((XyU32)XY_F32_TO_INT8_SAT(a)) << XY_COL32_A_SHIFT;
        return out;
    }
};

#define XY_FLOOR(_VAL)                  ((float)(int)(_VAL))                                    // ImFloor() is not inlined in MSVC debug builds
#define XY_ROUND(_VAL)                  ((float)(int)((_VAL) + 0.5f))                           //


#ifndef XY_ASSERT
#include <assert.h>
#define XY_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h
#endif
#define XY_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!
#define XY_UNUSED(_VAR)             ((void)(_VAR))                              // Used to silence "unused variable warnings". Often useful as asserts may be stripped out from final builds.
#define XY_OFFSETOF(_TYPE,_MEMBER)  offsetof(_TYPE, _MEMBER)                    // Offset of _MEMBER within _TYPE. Standardized as offsetof() in C++11


template<typename T>
struct XyVector
{
    int                 Size;
    int                 Capacity;
    T* Data;

    // Provide standard typedefs but we don't use them ourselves.
    typedef T                   value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;

    // Constructors, destructor
    inline XyVector() { Size = Capacity = 0; Data = NULL; }
    inline XyVector(const XyVector<T>& src) { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline XyVector<T>& operator=(const XyVector<T>& src) { clear(); resize(src.Size); if (src.Data) memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~XyVector() { if (Data) free(Data); } // Xyportant: does not destruct anything

    inline XyVector(std::initializer_list<T> init) : Size(0), Capacity(0), Data(nullptr)
    {
        reserve((int)init.size());
        for (const auto& item : init)
            push_back(item);
    }

    inline void         clear() { if (Data) { Size = Capacity = 0; free(Data); Data = NULL; } }  // Xyportant: does not destruct anything
    inline void         clear_delete()
    {
        for (int n = 0; n < Size; n++)
        {
            if (Data[n])
                free(Data[n]);
        }
        clear();
    }     // Xyportant: never called automatically! always explicit.
    inline void         clear_destruct() { for (int n = 0; n < Size; n++) Data[n].~T(); clear(); }           // Xyportant: never called automatically! always explicit.

    inline bool         empty() const { return Size == 0; }
    inline int          size() const { return Size; }
    inline int          size_in_bytes() const { return Size * (int)sizeof(T); }
    inline int          max_size() const { return 0x7FFFFFFF / (int)sizeof(T); }
    inline int          capacity() const { return Capacity; }
    inline T& operator[](int i) { XY_ASSERT(i >= 0 && i < Size); return Data[i]; }
    inline const T& operator[](int i) const { XY_ASSERT(i >= 0 && i < Size); return Data[i]; }
    inline T& at(int i) { XY_ASSERT(i >= 0 && i < Size); return Data[i]; }
	inline const T& at(int i) const { XY_ASSERT(i >= 0 && i < Size); return Data[i]; }

    inline T* begin() { return Data; }
    inline const T* begin() const { return Data; }
    inline T* end() { return Data + Size; }
    inline const T* end() const { return Data + Size; }
    inline T& front() { XY_ASSERT(Size > 0); return Data[0]; }
    inline const T& front() const { XY_ASSERT(Size > 0); return Data[0]; }
    inline T& back() { XY_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const T& back() const { XY_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void         swap(XyVector<T>& rhs) { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size, const T& v) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         shrink(int new_size) { XY_ASSERT(new_size <= Size); Size = new_size; } // Resize a vector to a smaller size, guaranteed not to cause a reallocation
    inline void         reserve(int new_capacity) 
    {
        if (new_capacity <= Capacity) 
            return;

        T* new_data = (T*)malloc((size_t)new_capacity * sizeof(T));
        if (Data) 
        { 
            memcpy(new_data, Data, (size_t)Size * sizeof(T)); 
            free(Data);
        } 
        Data = new_data; 
        Capacity = new_capacity;
    }
    inline void         reserve_discard(int new_capacity) { if (new_capacity <= Capacity) return; if (Data) free(Data); Data = (T*)malloc((size_t)new_capacity * sizeof(T)); Capacity = new_capacity; }

    // NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the XyVector data itself! e.g. v.push_back(v[10]) is forbidden.
    inline void         push_back(const T& v) { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         pop_back() { XY_ASSERT(Size > 0); Size--; }
    inline void         push_front(const T& v) { if (Size == 0) push_back(v); else insert(Data, v); }
    inline T* erase(const T* it) { XY_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
    inline T* erase(const T* it, const T* it_last) { XY_ASSERT(it >= Data && it < Data + Size && it_last >= it && it_last <= Data + Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - (size_t)count) * sizeof(T)); Size -= (int)count; return Data + off; }
    inline T* erase_unsorted(const T* it) { XY_ASSERT(it >= Data && it < Data + Size);  const ptrdiff_t off = it - Data; if (it < Data + Size - 1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
    inline T* insert(const T* it, const T& v) { XY_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const T& v) const { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline T* find(const T& v) { T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline const T* find(const T& v) const { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline bool         find_erase(const T& v) { const T* it = find(v); if (it < Data + Size) { erase(it); return true; } return false; }
    inline bool         find_erase_unsorted(const T& v) { const T* it = find(v); if (it < Data + Size) { erase_unsorted(it); return true; } return false; }
    inline int          index_from_ptr(const T* it) const { XY_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; return (int)off; }
};



class UData {
public:
    XyVec2       display_size;
    int64_t      curr_time;
    float        delta_time;


    UData()
    {
        display_size = XyVec2(0, 0);
        delta_time = 0.f;
        curr_time = GetTickCount64();
    }
};

class BData {
public:
    std::chrono::steady_clock::time_point time;

    BData()
    {
        time = std::chrono::steady_clock::now();
    }
};


struct DX11Data
{
    ID3D11Buffer* pVB;
    ID3D11Buffer* pIB;
    ID3D11VertexShader* pVertexShader;
    ID3D11InputLayout* pInputLayout;
    ID3D11Buffer* pVertexConstantBuffer;
    ID3D11PixelShader* pPixelShader;
    ID3D11SamplerState* pFontSampler;
    ID3D11ShaderResourceView* pFontTextureView;
    ID3D11RasterizerState* pRasterizerState;
    ID3D11BlendState* pBlendState;
    ID3D11DepthStencilState* pDepthStencilState;
    int                         VertexBufferSize;
    int                         IndexBufferSize;

    DX11Data() { memset((void*)this, 0, sizeof(*this)); VertexBufferSize = 5000; IndexBufferSize = 10000; }

    void clear();
};


typedef unsigned short      XyIndex;   // Default: 16-bit (for maximum compatibility with renderer backends)
struct XyVertex {
    XyVec2 pos;
    XyVec2 uv;
    XyU32 col;
};

struct VERTEX_CONSTANT_BUFFER_DX11
{
	float   mvp[4][4];
};

struct XyCmd
{
    XyVec4          ClipRect;           // 4*4  // Clipping rectangle (x1, y1, x2, y2). Subtract XyDrawData->DisplayPos to get clipping rectangle in "viewport" coordinates
    XyTextureID     TextureId;          // 4-8  // User-provided texture ID. Set by user in XyfontAtlas::SetTexID() for fonts or passed to Xyage*() functions. Ignore if never using images or multiple fonts atlas.
    unsigned int    VtxOffset;          // 4    // Start offset in vertex buffer. XyGuiBackendFlags_RendererHasVtxOffset: always 0, otherwise may be >0 to support meshes larger than 64K vertices with 16-bit indices.
    unsigned int    IdxOffset;          // 4    // Start offset in index buffer.
    unsigned int    ElemCount;          // 4    // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee XyDrawList's vtx_buffer[] array, indices in idx_buffer[].

    XyCmd() { memset(this, 0, sizeof(*this)); } // Also ensure our padding fields are zeroed

    // Since 1.83: returns XyTextureID associated with this draw call. Warning: DO NOT assume this is always same as 'TextureId' (we will change this function for an upcoming feature)
    inline XyTextureID GetTexID() const { return TextureId; }
};

struct XyCmdHeader
{
    XyVec4          ClipRect;
    XyTextureID     TextureId;
    unsigned int    VtxOffset;
};

typedef int XyDrawListFlags;
enum XyDrawListFlags_
{
    XyDrawListFlags_None = 0,
    XyDrawListFlags_AntiAliasedLines = 1 << 0,  // Enable anti-aliased lines/borders (*2 the number of triangles for 1.0f wide line or lines thin enough to be drawn using textures, otherwise *3 the number of triangles)
    XyDrawListFlags_AntiAliasedLinesUseTex = 1 << 1,  // Enable anti-aliased lines/borders using textures when possible. Require backend to render with bilinear filtering (NOT point/nearest filtering).
    XyDrawListFlags_AntiAliasedFill = 1 << 2,  // Enable anti-aliased edge around filled shapes (rounded rectangles, circles).
    XyDrawListFlags_AllowVtxOffset = 1 << 3,  // Can emit 'VtxOffset > 0' to allow large meshes. Set when 'ImGuiBackendFlags_RendererHasVtxOffset' is enabled.
};

#define XY_PI                           3.14159265358979323846f
#define XY_ROUNDUP_TO_EVEN(_V)                                  ((((_V) + 1) / 2) * 2)
#define XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     0
#define XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     2048
#define XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    XyClamp(XY_ROUNDUP_TO_EVEN((int)XyCeil(XY_PI / XyAcos(1 - XyMin((_MAXERROR), (_RAD)) / (_RAD)))), XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

// Raw equation from XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC rewritten for 'r' and 'error'.
#define XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N,_MAXERROR)    ((_MAXERROR) / (1 - XyCos(XY_PI / XyMax((float)(_N), XY_PI))))
#define XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_ERROR(_N,_RAD)     ((1 - XyCos(XY_PI / XyMax((float)(_N), XY_PI))) / (_RAD))

// XyDrawList: Lookup table size for adaptive arc drawing, cover full circle.
#ifndef XY_DRAWLIST_ARCFAST_TABLE_SIZE
#define XY_DRAWLIST_ARCFAST_TABLE_SIZE                          48 // Number of samples in lookup table.
#endif
#define XY_DRAWLIST_ARCFAST_SAMPLE_MAX                          XY_DRAWLIST_ARCFAST_TABLE_SIZE // Sample index _PathArcToFastEx() for 360 angle.

// Helper Functions
#pragma region HelperFunctions
#include <xmmintrin.h>
#define XyFabs(X)           fabsf(X)
#define XySqrt(X)           sqrtf(X)
#define XyFmod(X, Y)        fmodf((X), (Y))
#define XyCos(X)            cosf(X)
#define XySin(X)            sinf(X)
#define XyAcos(X)           acosf(X)
#define XyAtan2(Y, X)       atan2f((Y), (X))
#define XyAtof(STR)         atof(STR)
//#define XyFloorStd(X)     floorf(X)           // We use our own, see XyFloor() and XyFloorSigned()
#define XyCeil(X)           ceilf(X)
static inline float  XyPow(float x, float y) { return powf(x, y); }          // DragBehaviorT/SliderBehaviorT uses XyPow with either float/double and need the precision
static inline double XyPow(double x, double y) { return pow(x, y); }
static inline float  XyLog(float x) { return logf(x); }             // DragBehaviorT/SliderBehaviorT uses XyLog with either float/double and need the precision
static inline double XyLog(double x) { return log(x); }
static inline int    XyAbs(int x) { return x < 0 ? -x : x; }
static inline float  XyAbs(float x) { return fabsf(x); }
static inline double XyAbs(double x) { return fabs(x); }
static inline float  XySign(float x) { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f; } // Sign operator - returns -1, 0 or 1 based on sign of argument
static inline double XySign(double x) { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0; }
static inline float  XyRsqrt(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
static inline double XyRsqrt(double x) { return 1.0 / sqrt(x); }
// - XyMin/ImMax/ImClamp/ImLerp/ImSwap are used by widgets which support variety of types: signed/unsigned int/long long float/double
// (Exceptionally using templates here but we could also redefine them for those types)
template<typename T> static inline T XyMin(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T XyMax(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T XyClamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T XyLerp(T a, T b, float t) { return (T)(a + (b - a) * t); }
template<typename T> static inline void XySwap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template<typename T> static inline T XyAddClampOverflow(T a, T b, T mn, T mx) { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> static inline T XySubClampOverflow(T a, T b, T mn, T mx) { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }
// - Misc maths helpers
static inline XyVec2 XyMin(const XyVec2& lhs, const XyVec2& rhs) { return XyVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
static inline XyVec2 XyMax(const XyVec2& lhs, const XyVec2& rhs) { return XyVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
static inline XyVec2 XyClamp(const XyVec2& v, const XyVec2& mn, XyVec2 mx) { return XyVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
static inline XyVec2 XyLerp(const XyVec2& a, const XyVec2& b, float t) { return XyVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
static inline XyVec2 XyLerp(const XyVec2& a, const XyVec2& b, const XyVec2& t) { return XyVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline XyVec4 XyLerp(const XyVec4& a, const XyVec4& b, float t) { return XyVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
static inline float  XyLengthSqr(const XyVec2& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
static inline float  XyLengthSqr(const XyVec4& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
static inline float  XyLength(const XyVec2& lhs, float fail_value) { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return XySqrt(d); return fail_value; }
static inline float  XyInvLength(const XyVec2& lhs, float fail_value) { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return XyRsqrt(d); return fail_value; }
static inline float  XyFloor(float f) { return (float)(int)(f); }
static inline float  XyFloorSigned(float f) { return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); } // Decent replacement for floorf()
static inline XyVec2 XyFloor(const XyVec2& v) { return XyVec2((float)(int)(v.x), (float)(int)(v.y)); }
static inline XyVec2 XyFloorSigned(const XyVec2& v) { return XyVec2(XyFloorSigned(v.x), XyFloorSigned(v.y)); }
static inline int    XyModPositive(int a, int b) { return (a + b) % b; }
static inline float  XyDot(const XyVec2& a, const XyVec2& b) { return a.x * b.x + a.y * b.y; }
static inline XyVec2 XyRotate(const XyVec2& v, float cos_a, float sin_a) { return XyVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
static inline float  XyLinearSweep(float current, float target, float speed) { if (current < target) return XyMin(current + speed, target); if (current > target) return XyMax(current - speed, target); return current; }
static inline XyVec2 XyMul(const XyVec2& lhs, const XyVec2& rhs) { return XyVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline bool   XyIsFloatAboveGuaranteedIntegerPrecision(float f) { return f <= -16777216 || f >= 16777216; }
static inline float  XyExponentialMovingAverage(float avg, float sample, int n) { avg -= avg / (float)n; avg += sample / (float)n; return avg; }


#define XY_NORMALIZE2F_OVER_ZERO(VX,VY)     { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = XyRsqrt(d2); VX *= inv_len; VY *= inv_len; } } (void)0
#define XY_FIXNORMAL2F_MAX_INVLEN2          100.0f // 500.0f (see #4053, #3366)
#define XY_FIXNORMAL2F(VX,VY)               { float d2 = VX*VX + VY*VY; if (d2 > 0.000001f) { float inv_len2 = 1.0f / d2; if (inv_len2 > XY_FIXNORMAL2F_MAX_INVLEN2) inv_len2 = XY_FIXNORMAL2F_MAX_INVLEN2; VX *= inv_len2; VY *= inv_len2; } } (void)0

#define XY_DRAWLIST_TEX_LINES_WIDTH_MAX (63)

#pragma endregion

class XyFont;
struct XyDrawListSharedData
{
    XyVec2          TexUvWhitePixel;            // UV of white pixel in the atlas
    XyFont* Font;                       // Current/default font (optional, for simplified AddText overload)
    float           FontSize;                   // Current/default font size (optional, for simplified AddText overload)

    float           CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo()
    float           CircleSegmentMaxError;      // Number of circle segments to use per pixel of radius for AddCircle() etc
    XyVec4          ClipRectFullscreen;         // Value for PushClipRectFullscreen()
    XyDrawListFlags InitialFlags;               // Initial flags at the beginning of the frame (it is possible to alter flags on a per-drawlist basis afterwards)

    // [Internal] Temp write buffer
    XyVector<XyVec2> TempBuffer;

    // [Internal] Lookup tables
    XyVec2          ArcFastVtx[XY_DRAWLIST_ARCFAST_TABLE_SIZE]; // Sample points on the quarter of the circle.
    float           ArcFastRadiusCutoff;                        // Cutoff radius after which arc drawing will fallback to slower PathArcTo()
    XyU8            CircleSegmentCounts[64];    // Precomputed segment count for given radius before we calculate it dynamically (to avoid calculation overhead)
    const XyVec4* TexUvLines;                 // UV of anti-aliased lines in the atlas

    int* ShadowRectIds;              // IDs of rects for shadow texture (2 entries)
    const XyVec4* ShadowRectUvs;              // UV coordinates for shadow texture (10 entries)

    XyDrawListSharedData()
    {
        memset(this, 0, sizeof(*this));
        for (int i = 0; i < XY_ARRAYSIZE(ArcFastVtx); i++)
        {
            const float a = ((float)i * 2 * XY_PI) / (float)XY_ARRAYSIZE(ArcFastVtx);
            ArcFastVtx[i] = XyVec2(XyCos(a), XySin(a));
        }
        ArcFastRadiusCutoff = XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(XY_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);
    }

    void SetCircleTessellationMaxError(float max_error)
    {
        if (CircleSegmentMaxError == max_error)
            return;

        XY_ASSERT(max_error > 0.0f);
        CircleSegmentMaxError = max_error;
        for (int i = 0; i < XY_ARRAYSIZE(CircleSegmentCounts); i++)
        {
            const float radius = (float)i;
            CircleSegmentCounts[i] = (XyU8)((i > 0) ? XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, CircleSegmentMaxError) : XY_DRAWLIST_ARCFAST_SAMPLE_MAX);
        }
        ArcFastRadiusCutoff = XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(XY_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);
    }
};

typedef int XyDrawFlags;
enum XyDrawFlags_
{
    XyDrawFlags_None = 0,
    XyDrawFlags_Closed = 1 << 0, // PathStroke(), AddPolyline(): specify that shape should be closed (Xyportant: this is always == 1 for legacy reason)
    XyDrawFlags_RoundCornersTopLeft = 1 << 4, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-left corner only (when rounding > 0.0f, we default to all corners). Was 0x01.
    XyDrawFlags_RoundCornersTopRight = 1 << 5, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-right corner only (when rounding > 0.0f, we default to all corners). Was 0x02.
    XyDrawFlags_RoundCornersBottomLeft = 1 << 6, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-left corner only (when rounding > 0.0f, we default to all corners). Was 0x04.
    XyDrawFlags_RoundCornersBottomRight = 1 << 7, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-right corner only (when rounding > 0.0f, we default to all corners). Wax 0x08.
    XyDrawFlags_RoundCornersNone = 1 << 8, // AddRect(), AddRectFilled(), PathRect(): disable rounding on all corners (when rounding > 0.0f). This is NOT zero, NOT an implicit flag!
    XyDrawFlags_RoundCornersTop = XyDrawFlags_RoundCornersTopLeft | XyDrawFlags_RoundCornersTopRight,
    XyDrawFlags_RoundCornersBottom = XyDrawFlags_RoundCornersBottomLeft | XyDrawFlags_RoundCornersBottomRight,
    XyDrawFlags_RoundCornersLeft = XyDrawFlags_RoundCornersBottomLeft | XyDrawFlags_RoundCornersTopLeft,
    XyDrawFlags_RoundCornersRight = XyDrawFlags_RoundCornersBottomRight | XyDrawFlags_RoundCornersTopRight,
    XyDrawFlags_RoundCornersAll = XyDrawFlags_RoundCornersTopLeft | XyDrawFlags_RoundCornersTopRight | XyDrawFlags_RoundCornersBottomLeft | XyDrawFlags_RoundCornersBottomRight,
    XyDrawFlags_RoundCornersDefault_ = XyDrawFlags_RoundCornersAll, // Default to ALL corners if none of the _RoundCornersXX flags are specified.
    XyDrawFlags_RoundCornersMask_ = XyDrawFlags_RoundCornersAll | XyDrawFlags_RoundCornersNone,
    XyDrawFlags_ShadowCutOutShapeBackground = 1 << 9, // Do not render the shadow shape under the objects to be shadowed to save on fill-rate or facilitate blending. Slower on CPU.
};

typedef XyDrawFlags XyDrawCornerFlags;
enum XyDrawCornerFlags_
{
    XyDrawCornerFlags_None = XyDrawFlags_RoundCornersNone,         // Was == 0 prior to 1.82, this is now == XyDrawFlags_RoundCornersNone which is != 0 and not implicit
    XyDrawCornerFlags_TopLeft = XyDrawFlags_RoundCornersTopLeft,      // Was == 0x01 (1 << 0) prior to 1.82. Order matches XyDrawFlags_NoRoundCorner* flag (we exploit this internally).
    XyDrawCornerFlags_TopRight = XyDrawFlags_RoundCornersTopRight,     // Was == 0x02 (1 << 1) prior to 1.82.
    XyDrawCornerFlags_BotLeft = XyDrawFlags_RoundCornersBottomLeft,   // Was == 0x04 (1 << 2) prior to 1.82.
    XyDrawCornerFlags_BotRight = XyDrawFlags_RoundCornersBottomRight,  // Was == 0x08 (1 << 3) prior to 1.82.
    XyDrawCornerFlags_All = XyDrawFlags_RoundCornersAll,          // Was == 0x0F prior to 1.82
    XyDrawCornerFlags_Top = XyDrawCornerFlags_TopLeft | XyDrawCornerFlags_TopRight,
    XyDrawCornerFlags_Bot = XyDrawCornerFlags_BotLeft | XyDrawCornerFlags_BotRight,
    XyDrawCornerFlags_Left = XyDrawCornerFlags_TopLeft | XyDrawCornerFlags_BotLeft,
    XyDrawCornerFlags_Right = XyDrawCornerFlags_TopRight | XyDrawCornerFlags_BotRight,
};

class XyFont;

struct XyFontConfig
{
    void* FontData;               //          // TTF/OTF data
    int             FontDataSize;           //          // TTF/OTF data size
    bool            FontDataOwnedByAtlas;   // true     // TTF/OTF data ownership taken by the container XyFontAtlas (will delete memory itself).
    int             FontNo;                 // 0        // Index of font within TTF/OTF file
    float           SizePixels;             //          // Size in pixels for rasterizer (more or less maps to the resulting font height).
    int             OversampleH;            // 3        // Rasterize at higher quality for sub-pixel positioning. Note the difference between 2 and 3 is minimal so you can reduce this to 2 to save memory. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
    int             OversampleV;            // 1        // Rasterize at higher quality for sub-pixel positioning. This is not really useful as we don't use sub-pixel positions on the Y axis.
    bool            PixelSnapH;             // false    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
    XyVec2          GlyphExtraSpacing;      // 0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
    XyVec2          GlyphOffset;            // 0, 0     // Offset all glyphs from this font input.
    const XyWchar*  GlyphRanges;            // NULL     // THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE. Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list).
    float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
    float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
    bool            MergeMode;              // false    // Merge into previous XyFont, so you can combine multiple inputs font into one XyFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
    float           RasterizerMultiply;     // 1.0f     // Brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable.
    XyWchar         EllipsisChar;           // -1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.

    // [Internal]
    char            Name[40];               // Name (strictly to ease debugging)
    XyFont* DstFont;

    XyFontConfig()
    {
        memset(this, 0, sizeof(*this));
        FontDataOwnedByAtlas = true;
        OversampleH = 3; // FIXME: 2 may be a better default?
        OversampleV = 1;
        GlyphMaxAdvanceX = FLT_MAX;
        RasterizerMultiply = 1.0f;
        EllipsisChar = (XyWchar)-1;
    }
};

struct XyFontAtlasCustomRect
{
    unsigned short  Width, Height;  // Input    // Desired rectangle dimension
    unsigned short  X, Y;           // Output   // Packed position in Atlas
    unsigned int    GlyphID;        // Input    // For custom font glyphs only (ID < 0x110000)
    float           GlyphAdvanceX;  // Input    // For custom font glyphs only: glyph xadvance
    XyVec2          GlyphOffset;    // Input    // For custom font glyphs only: glyph display offset
    XyFont* Font;           // Input    // For custom font glyphs only: target font
    XyFontAtlasCustomRect() { Width = Height = 0; X = Y = 0xFFFF; GlyphID = 0; GlyphAdvanceX = 0.0f; GlyphOffset = XyVec2(0, 0); Font = NULL; }
    bool IsPacked() const { return X != 0xFFFF; }
};


typedef int XyFontAtlasFlags;       // -> enum XyFontAtlasFlags_     // Flags: for XyFontAtlas build
enum XyFontAtlasFlags_
{
    XyFontAtlasFlags_None = 0,
    XyFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don't round the height to next power of two
    XyFontAtlasFlags_NoMouseCursors = 1 << 1,   // Don't build software mouse cursors into the atlas (save a little texture memory)
    XyFontAtlasFlags_NoBakedLines = 1 << 2,   // Don't build thick line textures into the atlas (save a little texture memory, allow support for point/nearest filtering). The AntiAliasedLinesUseTex features uses them, otherwise they will be rendered using polygons (more expensive for CPU/GPU).
};

struct XyFontAtlasShadowTexConfig
{
    int     TexCornerSize;          // Size of the corner areas.
    int     TexEdgeSize;            // Size of the edge areas (and by extension the center). Changing this is normally unnecessary.
    float   TexFalloffPower;        // The power factor for the shadow falloff curve.
    float   TexDistanceFieldOffset; // How much to offset the distance field by (allows over/under-shadowing, potentially useful for accommodating rounded corners on the "casting" shape).
    bool    TexBlur;                // Do we want to Gaussian blur the shadow texture?

    inline XyFontAtlasShadowTexConfig() { memset(this, 0, sizeof(*this)); }
    void    SetupDefaults()
    {
        TexCornerSize = 16;
        TexEdgeSize = 1;
        TexFalloffPower = 4.8f;
        TexDistanceFieldOffset = 3.8f;
        TexBlur = true;
    }
    int     GetRectTexPadding() const { return 2; }                                                   // Number of pixels of padding to add to the rectangular texture to avoid sampling artifacts at the edges.
    int     CalcRectTexSize() const { return TexCornerSize + TexEdgeSize + GetRectTexPadding(); }   // The size of the texture area required for the actual 2x2 rectangle shadow texture (after the redundant corners have been removed). Padding is required here to avoid sampling artifacts at the edge adjoining the removed corners.    int     CalcConvexTexWidth() const;                                                                // The width of the texture area required for the convex shape shadow texture.
    int     GetConvexTexPadding() const { return 8; }                                                   // Number of pixels of padding to add to the convex shape texture to avoid sampling artifacts at the edges. This also acts as padding for the expanded corner triangles.
    int     CalcConvexTexWidth() const                                                                  // The width of the texture area required for the convex shape shadow texture.
    {
        // We have to pad the texture enough that we don't go off the edges when we expand the corner triangles
        return (int)(((float)TexCornerSize / XyCos(XY_PI * 0.25f)) + (float)(GetConvexTexPadding() * 2));
    }
    int     CalcConvexTexHeight() const                                                                 // The height of the texture area required for the convex shape shadow texture.
    {
        return CalcConvexTexWidth();
    }
};

struct XyFontGlyph
{
    unsigned int    Colored : 1;        // Flag to indicate glyph is colored and should generally ignore tinting (make it usable with no shift on little-endian as this is used in loops)
    unsigned int    Visible : 1;        // Flag to indicate glyph has no visible pixels (e.g. space). Allow early out when rendering.
    unsigned int    Codepoint : 30;     // 0x0000..0x10FFFF
    float           AdvanceX;           // Distance to next character (= data from font + ImFontConfig::GlyphExtraSpacing.x baked in)
    float           X0, Y0, X1, Y1;     // Glyph corners
    float           U0, V0, U1, V1;     // Texture coordinates
};
#define XY_UNICODE_CODEPOINT_MAX     0xFFFF     // Maximum Unicode code point supported by this build.
#define XY_UNICODE_CODEPOINT_INVALID 0xFFFD     // Invalid Unicode code point (standard value).

class XyFontAtlas;
class _Xyphra;
class XyFont {
public:
    // Parent
    _Xyphra* instance = nullptr;

    // Members: Hot ~20/24 bytes (for CalcTextWidth)
    XyVector<float>             IndexAdvanceX;      // 12-16 // out //            // Sparse. Glyphs->AdvanceX in a directly indexable way (cache-friendly for CalcTextWidth functions which only this this info, and are often bottleneck in large UI).
    float                       FallbackAdvanceX;   // 4     // out // = FallbackGlyph->AdvanceX
    float                       FontSize;           // 4     // in  //            // Height of characters/line, set during loading (don't change after loading)

    // Members: Hot ~28/40 bytes (for CalcTextWidth + render loop)
    XyVector<XyWchar>           IndexLookup;        // 12-16 // out //            // Sparse. Index glyphs by Unicode code-point.
    XyVector<XyFontGlyph>       Glyphs;             // 12-16 // out //            // All glyphs.
    const XyFontGlyph*          FallbackGlyph;      // 4-8   // out // = FindGlyph(FontFallbackChar)

    // Members: Cold ~32/40 bytes
    XyFontAtlas*                ContainerAtlas;     // 4-8   // out //            // What we has been loaded into
    const XyFontConfig*         ConfigData;         // 4-8   // in  //            // Pointer within ContainerAtlas->ConfigData
    short                       ConfigDataCount;    // 2     // in  // ~ 1        // Number of XyFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one XyFont.
    XyWchar                     FallbackChar;       // 2     // out // = FFFD/'?' // Character used if a glyph isn't found.
    XyWchar                     EllipsisChar;       // 2     // out // = '...'/'.'// Character used for ellipsis rendering.
    short                       EllipsisCharCount;  // 1     // out // 1 or 3
    float                       EllipsisWidth;      // 4     // out               // Width
    float                       EllipsisCharStep;   // 4     // out               // Step between characters when EllipsisCount > 0
    bool                        DirtyLookupTables;  // 1     // out //
    float                       Scale;              // 4     // in  // = 1.f      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
    float                       Ascent, Descent;    // 4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
    int                         MetricsTotalSurface;// 4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)
    XyU8                        Used4kPagesMap[(XY_UNICODE_CODEPOINT_MAX+1)/4096/8]; // 2 bytes if XyWchar=ImWchar16, 34 bytes if XyWchar==ImWchar32. Store 1-bit for each block of 4K codepoints that has one active glyph. This is mainly used to facilitate iterations across all used codepoints.

    // Methods
    XyFont(_Xyphra* instance);
    ~XyFont();
    const XyFontGlyph*FindGlyph(XyWchar c) const;
    const XyFontGlyph*FindGlyphNoFallback(XyWchar c) const;
    float                       GetCharAdvance(XyWchar c) const     { return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; }
    bool                        IsLoaded() const                    { return ContainerAtlas != NULL; }
    const char*                 GetDebugName() const                { return ConfigData ? ConfigData->Name : "<unknown>"; }

    // 'max_width' stops rendering after a certain width (could be turned into a 2d size). FLT_MAX to disable.
    // 'wrap_width' enable automatic word-wrapping across multiple lines to fit into given width. 0.0f to disable.
    XyVec2            CalcTextWidthA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8
    const char*       CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
    void              RenderChar(float size, const XyVec2& pos, XyU32 col, XyWchar c) const;
    float             RenderTextTest(float size, const XyVec2& pos, XyU32 col, const XyVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false, 
                                                float pos_find = 0, void* out = 0, bool search_type = 0) const;
    float             RenderText(float size, const XyVec2& pos, XyU32 col, const XyVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

    // [Internal] Don't use!
    void              BuildLookupTable();
    void              ClearOutputData();
    void              GrowIndex(int new_size);
    void              AddGlyph(const XyFontConfig* src_cfg, XyWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
    void              AddRemapChar(XyWchar dst, XyWchar src, bool overwrite_dst = true); // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.
    void              SetGlyphVisible(XyWchar c, bool visible);
    bool              IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);
};

template<int BITCOUNT, int OFFSET = 0>
struct XyBitArray
{
    XyU32           Storage[(BITCOUNT + 31) >> 5];
    XyBitArray() { ClearAllBits(); }
    void            ClearAllBits() { memset(Storage, 0, sizeof(Storage)); }
    void            SetAllBits() { memset(Storage, 255, sizeof(Storage)); }
    bool            TestBit(int n) const { n += OFFSET; XY_ASSERT(n >= 0 && n < BITCOUNT); return XY_BITARRAY_TESTBIT(Storage, n); }
    void            SetBit(int n) { n += OFFSET; XY_ASSERT(n >= 0 && n < BITCOUNT); XyBitArraySetBit(Storage, n); }
    void            ClearBit(int n) { n += OFFSET; XY_ASSERT(n >= 0 && n < BITCOUNT); XyBitArrayClearBit(Storage, n); }
    void            SetBitRange(int n, int n2) { n += OFFSET; n2 += OFFSET; XY_ASSERT(n >= 0 && n < BITCOUNT && n2 > n && n2 <= BITCOUNT); XyBitArraySetBitRange(Storage, n, n2); } // Works on range [n..n2)
    bool            operator[](int n) const { n += OFFSET; XY_ASSERT(n >= 0 && n < BITCOUNT); return XY_BITARRAY_TESTBIT(Storage, n); }
};

// Helper: XyBitArray
#define         XY_BITARRAY_TESTBIT(_ARRAY, _N)                 ((_ARRAY[(_N) >> 5] & ((XyU32)1 << ((_N) & 31))) != 0) // Macro version of XyBitArrayTestBit(): ensure args have side-effect or are costly!
#define         XY_BITARRAY_CLEARBIT(_ARRAY, _N)                ((_ARRAY[(_N) >> 5] &= ~((XyU32)1 << ((_N) & 31))))    // Macro version of XyBitArrayClearBit(): ensure args have side-effect or are costly!
inline size_t   XyBitArrayGetStorageSizeInBytes(int bitcount) { return (size_t)((bitcount + 31) >> 5) << 2; }
inline void     XyBitArrayClearAllBits(XyU32* arr, int bitcount) { memset(arr, 0, XyBitArrayGetStorageSizeInBytes(bitcount)); }
inline bool     XyBitArrayTestBit(const XyU32* arr, int n) { XyU32 mask = (XyU32)1 << (n & 31); return (arr[n >> 5] & mask) != 0; }
inline void     XyBitArrayClearBit(XyU32* arr, int n) { XyU32 mask = (XyU32)1 << (n & 31); arr[n >> 5] &= ~mask; }
inline void     XyBitArraySetBit(XyU32* arr, int n) { XyU32 mask = (XyU32)1 << (n & 31); arr[n >> 5] |= mask; }
inline void     XyBitArraySetBitRange(XyU32* arr, int n, int n2) // Works on range [n..n2)
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        XyU32 mask = (XyU32)(((XyU64)1 << b_mod) - 1) & ~(XyU32)(((XyU64)1 << a_mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

struct XyBitVector
{
    XyVector<XyU32> Storage;
    void            Create(int sz) { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear() { Storage.clear(); }
    bool            TestBit(int n) const { XY_ASSERT(n < (Storage.Size << 5)); return XY_BITARRAY_TESTBIT(Storage.Data, n); }
    void            SetBit(int n) { XY_ASSERT(n < (Storage.Size << 5)); XyBitArraySetBit(Storage.Data, n); }
    void            ClearBit(int n) { XY_ASSERT(n < (Storage.Size << 5)); XyBitArrayClearBit(Storage.Data, n); }
};

class XyFontAtlas {
public:
    _Xyphra* instance;

    XyFontAtlas(_Xyphra* instance);
    ~XyFontAtlas();
    XyFont* AddFont(const XyFontConfig* font_cfg);
    XyFont* AddFontDefault(const XyFontConfig* font_cfg = NULL);
    XyFont* AddFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const XyFontConfig* font_cfg = NULL, const XyWchar* glyph_ranges = NULL); // Note: Transfer ownership of 'ttf_data' to XyFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed.
    XyFont* AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const XyFontConfig* font_cfg = NULL, const XyWchar* glyph_ranges = NULL); // 'compressed_font_data' still owned by caller. Compress with binary_to_compressed_c.cpp.
    XyFont* AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const XyFontConfig* font_cfg = NULL, const XyWchar* glyph_ranges = NULL);              // 'compressed_font_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 parameter.
    void              ClearInputData();           // Clear input data (all XyFontConfig structures including sizes, TTF data, glyph ranges, etc.) = all the data used to build the texture and fonts.
    void              ClearTexData();             // Clear output texture data (CPU side). Saves RAM once the texture has been copied to graphics memory.
    void              ClearFonts();               // Clear output font data (glyphs storage, UV coordinates).
    void              Clear();                    // Clear all input and output.

    // Build atlas, retrieve pixel data.
    // User is in charge of copying the pixels into graphics memory (e.g. create a texture with your engine). Then store your texture handle with SetTexID().
    // The pitch is always = Width * BytesPerPixels (1 or 4)
    // Building in RGBA32 format is provided for convenience and compatibility, but note that unless you manually manipulate or copy color data into
    // the texture (e.g. when using the AddCustomRect*** api), then the RGB pixels emitted will always be white (~75% of memory/bandwidth waste.
    bool              Build();                    // Build pixels data. This is called automatically for you by the GetTexData*** functions.
    void              GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 1 byte per-pixel
    void              GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 4 bytes-per-pixel
    bool                        IsBuilt() const { return Fonts.Size > 0 && TexReady; } // Bit ambiguous: used to detect when user didn't build texture but effectively we should check TexID != 0 except that would be backend dependent...
    void                        SetTexID(XyTextureID id) { TexID = id; }

    //-------------------------------------------
    // Glyph Ranges
    //-------------------------------------------

    // Helpers to retrieve list of common Unicode ranges (2 value per range, values are inclusive, zero-terminated list)
    // NB: Make sure that your string are UTF-8 and NOT in your local code page.
    // Read https://github.com/ocornut/imgui/blob/master/docs/FONTS.md/#about-utf-8-encoding for details.
    // NB: Consider using XyFontGlyphRangesBuilder to build glyph ranges from textual data.
    const XyWchar* GetGlyphRangesDefault();                // Basic Latin, Extended Latin
    const XyWchar* GetGlyphRangesGreek();                  // Default + Greek and Coptic
    const XyWchar* GetGlyphRangesKorean();                 // Default + Korean characters
    const XyWchar* GetGlyphRangesJapanese();               // Default + Hiragana, Katakana, Half-Width, Selection of 2999 Ideographs
    const XyWchar* GetGlyphRangesChineseFull();            // Default + Half-Width + Japanese Hiragana/Katakana + full set of about 21000 CJK Unified Ideographs
    const XyWchar* GetGlyphRangesChineseSimplifiedCommon();// Default + Half-Width + Japanese Hiragana/Katakana + set of 2500 CJK Unified Ideographs for common simplified Chinese
    const XyWchar* GetGlyphRangesCyrillic();               // Default + about 400 Cyrillic characters
    const XyWchar* GetGlyphRangesThai();                   // Default + Thai characters
    const XyWchar* GetGlyphRangesVietnamese();             // Default + Vietnamese characters

    //-------------------------------------------
    // [BETA] Custom Rectangles/Glyphs API
    //-------------------------------------------

    // You can request arbitrary rectangles to be packed into the atlas, for your own purposes.
    // - After calling Build(), you can query the rectangle position and render your pixels.
    // - If you render colored output, set 'atlas->TexPixelsUseColors = true' as this may help some backends decide of prefered texture format.
    // - You can also request your rectangles to be mapped as font glyph (given a font + Unicode point),
    //   so you can render e.g. custom colorful icons and use them as regular glyphs.
    // - Read docs/FONTS.md for more details about using colorful icons.
    // - Note: this API may be redesigned later in order to support multi-monitor varying DPI settings.
    int               AddCustomRectRegular(int width, int height);
    int               AddCustomRectFontGlyph(XyFont* font, XyWchar id, int width, int height, float advance_x, const XyVec2& offset = XyVec2(0, 0));
    XyFontAtlasCustomRect* GetCustomRectByIndex(int index) { XY_ASSERT(index >= 0); return &CustomRects[index]; }

    // [Internal]
    void              CalcCustomRectUV(const XyFontAtlasCustomRect* rect, XyVec2* out_uv_min, XyVec2* out_uv_max) const;

    //-------------------------------------------
    // Members
    //-------------------------------------------

    XyFontAtlasFlags            Flags;              // Build flags (see XyFontAtlasFlags_)
    XyTextureID                 TexID;              // User data to refer to the texture once it has been uploaded to user's graphic systems. It is passed back to you during rendering via the XyDrawCmd structure.
    int                         TexDesiredWidth;    // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
    int                         TexGlyphPadding;    // Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0 (will also need to set AntiAliasedLinesUseTex = false).
    bool                        Locked;             // Marked as Locked by XyGui::NewFrame() so attempt to modify the atlas will assert.
    void* UserData;           // Store your own atlas related user-data (if e.g. you have multiple font atlas).

    // [Internal]
    // NB: Access texture data via GetTexData*() calls! Which will setup a default font for you.
    bool                        TexReady;           // Set when texture was built matching current font input
    bool                        TexPixelsUseColors; // Tell whether our texture data is known to use colors (rather than just alpha channel), in order to help backend select a format.
    unsigned char* TexPixelsAlpha8;    // 1 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight
    unsigned int* TexPixelsRGBA32;    // 4 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
    int                         TexWidth;           // Texture width calculated during Build().
    int                         TexHeight;          // Texture height calculated during Build().
    XyVec2                      TexUvScale;         // = (1.0f/TexWidth, 1.0f/TexHeight)
    XyVec2                      TexUvWhitePixel;    // Texture coordinates to a white pixel
    XyVector<XyFont*>           Fonts;              // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling XyGui::NewFrame(), use XyGui::PushFont()/PopFont() to change the current font.
    XyVector<XyFontAtlasCustomRect> CustomRects;    // Rectangles for packing custom texture data into the atlas.
    XyVector<XyFontConfig>      ConfigData;         // Configuration data
    XyVec4                      TexUvLines[XY_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];  // UVs for baked anti-aliased lines

    // [Internal] Font builder
    unsigned int                FontBuilderFlags;   // Shared flags (for all fonts) for custom font builder. THIS IS BUILD IMPLEMENTATION DEPENDENT. Per-font override is also available in XyFontConfig.

    // [Internal] Packing data
    int                         PackIdMouseCursors; // Custom texture rectangle ID for white pixel and mouse cursors
    int                         PackIdLines;        // Custom texture rectangle ID for baked anti-aliased lines

    // [Internal] Shadow data
    int                         ShadowRectIds[2];   // IDs of rect for shadow textures
    XyVec4                      ShadowRectUvs[10];  // UV coordinates for shadow textures, 9 for the rectangle shadows and the final entry for the convex shape shadows
    XyFontAtlasShadowTexConfig  ShadowTexConfig;    // Shadow texture baking config

    // [Obsolete]
    //typedef XyFontAtlasCustomRect    CustomRect;         // OBSOLETED in 1.72+
    //typedef XyFontGlyphRangesBuilder GlyphRangesBuilder; // OBSOLETED in 1.67+

    void BuildInit();
    void BuildPackCustomRects(void* stbrp_context_opaque);
    void BuildSetupFont(XyFont* font, XyFontConfig* font_config, float ascent, float descent);
    void BuildFinish();

    void BuildRenderDefaultTexData();
    void BuildRenderLinesTexData();
    void BuildRenderShadowTexData();
    void BuildRegisterShadowCustomRects();
    void BuildRender8bppRectFromString(int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value);
    void BuildRender32bppRectFromString(int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value);

    bool BuildWithStbTruetype();
};


static inline bool      XyIsPowerOfTwo(int v) { return v != 0 && (v & (v - 1)) == 0; }
static inline bool      XyIsPowerOfTwo(XyU64 v) { return v != 0 && (v & (v - 1)) == 0; }
static inline int       XyUpperPowerOfTwo(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }


inline int XyFormatString(char* buf, size_t buf_size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int w = _vsnprintf_s(buf, buf_size, _TRUNCATE, fmt, args);
    va_end(args);
    if (buf == NULL)
        return w;
    if (w == -1 || w >= (int)buf_size)
        w = (int)buf_size - 1;
    buf[w] = 0;
    return w;
}

static inline char      XyToUpper(char c) { return (c >= 'a' && c <= 'z') ? c &= ~32 : c; }
static inline bool      XyCharIsBlankA(char c) { return c == ' ' || c == '\t'; }
static inline bool      XyCharIsBlankW(unsigned int c) { return c == ' ' || c == '\t' || c == 0x3000; }


inline int XyTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)               // read one character. return input UTF-8 bytes count
{
    static const char lengths[32] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
    static const int masks[] = { 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
    static const uint32_t mins[] = { 0x400000, 0, 0x80, 0x800, 0x10000 };
    static const int shiftc[] = { 0, 18, 12, 6, 0 };
    static const int shifte[] = { 0, 6, 4, 2, 0 };
    int len = lengths[*(const unsigned char*)in_text >> 3];
    int wanted = len + (len ? 0 : 1);

    if (in_text_end == NULL)
        in_text_end = in_text + wanted; // Max length, nulls will be taken into account.

    // Copy at most 'len' bytes, stop copying at 0 or past in_text_end. Branch predictor does a good job here,
    // so it is fast even with excessive branching.
    unsigned char s[4];
    s[0] = in_text + 0 < in_text_end ? in_text[0] : 0;
    s[1] = in_text + 1 < in_text_end ? in_text[1] : 0;
    s[2] = in_text + 2 < in_text_end ? in_text[2] : 0;
    s[3] = in_text + 3 < in_text_end ? in_text[3] : 0;

    // Assume a four-byte character and load four bytes. Unused bits are shifted out.
    *out_char = (uint32_t)(s[0] & masks[len]) << 18;
    *out_char |= (uint32_t)(s[1] & 0x3f) << 12;
    *out_char |= (uint32_t)(s[2] & 0x3f) << 6;
    *out_char |= (uint32_t)(s[3] & 0x3f) << 0;
    *out_char >>= shiftc[len];

    // Accumulate the various error conditions.
    int e = 0;
    e = (*out_char < mins[len]) << 6; // non-canonical encoding
    e |= ((*out_char >> 11) == 0x1b) << 7;  // surrogate half?
    e |= (*out_char > XY_UNICODE_CODEPOINT_MAX) << 8;  // out of range?
    e |= (s[1] & 0xc0) >> 2;
    e |= (s[2] & 0xc0) >> 4;
    e |= (s[3]) >> 6;
    e ^= 0x2a; // top two bits of each tail byte correct?
    e >>= shifte[len];

    if (e)
    {
        // No bytes are consumed when *in_text == 0 || in_text == in_text_end.
        // One byte is consumed in case of invalid first byte of in_text.
        // All available bytes (at most `len` bytes) are consumed on incomplete/invalid second to last bytes.
        // Invalid or incomplete input may consume less bytes than wanted, therefore every byte has to be inspected in s.
        wanted = XyMin(wanted, !!s[0] + !!s[1] + !!s[2] + !!s[3]);
        *out_char = XY_UNICODE_CODEPOINT_INVALID;
    }

    return wanted;
}

struct XyFontGlyphRangesBuilder
{
    XyVector<XyU32> UsedChars;            // Store 1-bit per Unicode code point (0=unused, 1=used)

    XyFontGlyphRangesBuilder() { Clear(); }
    inline void     Clear() { int size_in_bytes = (XY_UNICODE_CODEPOINT_MAX + 1) / 8; UsedChars.resize(size_in_bytes / (int)sizeof(XyU32)); memset(UsedChars.Data, 0, (size_t)size_in_bytes); }
    inline bool     GetBit(size_t n) const { int off = (int)(n >> 5); XyU32 mask = 1u << (n & 31); return (UsedChars[off] & mask) != 0; }  // Get bit n in the array
    inline void     SetBit(size_t n) { int off = (int)(n >> 5); XyU32 mask = 1u << (n & 31); UsedChars[off] |= mask; }               // Set bit n in the array
    inline void     AddChar(XyWchar c) { SetBit(c); }                      // Add character
    void  AddText(const char* text, const char* text_end = NULL);     // Add string (each character of the UTF-8 string are added)
    void  AddRanges(const XyWchar* ranges);                           // Add ranges, e.g. builder.AddRanges(ImFontAtlas::GetGlyphRangesDefault()) to force add all of ASCII/Latin+Ext
    void  BuildRanges(XyVector<XyWchar>* out_ranges);                 // Output new ranges
};