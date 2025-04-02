#include "Xyphra.h"
#include "imstb_truetype.h"

static float DistanceFromRectangle(const XyVec2& sample_pos, const XyVec2& rect_min, const XyVec2& rect_max)
{
    XyVec2 rect_centre = (rect_min + rect_max) * 0.5f;
    XyVec2 rect_half_size = (rect_max - rect_min) * 0.5f;
    XyVec2 local_sample_pos = sample_pos - rect_centre;
    XyVec2 axis_dist = XyVec2(XyFabs(local_sample_pos.x), XyFabs(local_sample_pos.y)) - rect_half_size;
    float out_dist = XyLength(XyVec2(XyMax(axis_dist.x, 0.0f), XyMax(axis_dist.y, 0.0f)), 0.00001f);
    float in_dist = XyMin(XyMax(axis_dist.x, axis_dist.y), 0.0f);
    return out_dist + in_dist;
}

static void UnpackBitVectorToFlatIndexList(const XyBitVector* in, XyVector<int>* out)
{
    XY_ASSERT(sizeof(in->Storage.Data[0]) == sizeof(int));
    const XyU32* it_begin = in->Storage.begin();
    const XyU32* it_end = in->Storage.end();
    for (const XyU32* it = it_begin; it < it_end; it++)
        if (XyU32 entries_32 = *it)
            for (XyU32 bit_n = 0; bit_n < 32; bit_n++)
                if (entries_32 & ((XyU32)1 << bit_n))
                    out->push_back((int)(((it - it_begin) << 5) + bit_n));
}


void   BuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_brighten_factor)
{
    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned int value = (unsigned int)((float)i * in_brighten_factor);
        out_table[i] = value > 255 ? 255 : (value & 0xFF);
    }
}

void    BuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride)
{
    unsigned char* data = pixels + x + y * stride;
    for (int j = h; j > 0; j--, data += stride - w)
        for (int i = w; i > 0; i--, data++)
            *data = table[*data];
}

const int FONT_ATLAS_DEFAULT_TEX_DATA_W = 122; // Actual texture will be 2 times that + 1 spacing.
const int FONT_ATLAS_DEFAULT_TEX_DATA_H = 27;
static const char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[FONT_ATLAS_DEFAULT_TEX_DATA_W * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] =
{
    "..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX-     XX          - XX       XX "
    "..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X-    X..X         -X..X     X..X"
    "---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X-    X..X         -X...X   X...X"
    "X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X-    X..X         - X...X X...X "
    "XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X-    X..X         -  X...X...X  "
    "X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X-    X..XXX       -   X.....X   "
    "X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX-    X..X..XXX    -    X...X    "
    "X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      -    X..X..X..XX  -     X.X     "
    "X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       -    X..X..X..X.X -    X...X    "
    "X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        -XXX X..X..X..X..X-   X.....X   "
    "X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         -X..XX........X..X-  X...X...X  "
    "X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          -X...X...........X- X...X X...X "
    "X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           - X..............X-X...X   X...X"
    "X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            -  X.............X-X..X     X..X"
    "X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           -  X.............X- XX       XX "
    "X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          -   X............X--------------"
    "X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          -   X...........X -             "
    "X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       -------------------------------------    X..........X -             "
    "X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           -    X..........X -             "
    "XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           -     X........X  -             "
    "      X..X  -       -  X...X  -         X...X         -  X..X           X..X  -           -     X........X  -             "
    "       XX   -       -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           -     XXXXXXXXXX  -             "
    "-------------       -    X    -           X           -X.....................X-           -------------------             "
    "                    ----------------------------------- X...XXXXXXXXXXXXX...X -                                           "
    "                                                      -  X..X           X..X  -                                           "
    "                                                      -   X.X           X.X   -                                           "
    "                                                      -    XX           XX    -                                           "
};

void XyFontAtlas::BuildRender8bppRectFromString(int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value)
{
    XY_ASSERT(x >= 0 && x + w <= this->TexWidth);
    XY_ASSERT(y >= 0 && y + h <= this->TexHeight);
    unsigned char* out_pixel = this->TexPixelsAlpha8 + x + (y * this->TexWidth);
    for (int off_y = 0; off_y < h; off_y++, out_pixel += this->TexWidth, in_str += w)
        for (int off_x = 0; off_x < w; off_x++)
            out_pixel[off_x] = (in_str[off_x] == in_marker_char) ? in_marker_pixel_value : 0x00;
}

void XyFontAtlas::BuildRender32bppRectFromString(int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value)
{
    XY_ASSERT(x >= 0 && x + w <= this->TexWidth);
    XY_ASSERT(y >= 0 && y + h <= this->TexHeight);
    unsigned int* out_pixel = this->TexPixelsRGBA32 + x + (y * this->TexWidth);
    for (int off_y = 0; off_y < h; off_y++, out_pixel += this->TexWidth, in_str += w)
        for (int off_x = 0; off_x < w; off_x++)
            out_pixel[off_x] = (in_str[off_x] == in_marker_char) ? in_marker_pixel_value : XY_COL32_BLACK_TRANS;
}

void XyFontAtlas::BuildRenderDefaultTexData()
{
    XyFontAtlasCustomRect* r = this->GetCustomRectByIndex(this->PackIdMouseCursors);
    XY_ASSERT(r->IsPacked());

    const int w = this->TexWidth;
    if (!(this->Flags & XyFontAtlasFlags_NoMouseCursors))
    {
        // Render/copy pixels
        XY_ASSERT(r->Width == FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1 && r->Height == FONT_ATLAS_DEFAULT_TEX_DATA_H);
        const int x_for_white = r->X;
        const int x_for_black = r->X + FONT_ATLAS_DEFAULT_TEX_DATA_W + 1;
        if (this->TexPixelsAlpha8 != NULL)
        {
            BuildRender8bppRectFromString(x_for_white, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, '.', 0xFF);
            BuildRender8bppRectFromString(x_for_black, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, 'X', 0xFF);
        }
        else
        {
            BuildRender32bppRectFromString(x_for_white, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, '.', XY_COL32_WHITE);
            BuildRender32bppRectFromString(x_for_black, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, 'X', XY_COL32_WHITE);
        }
    }
    else
    {
        // Render 4 white pixels
        XY_ASSERT(r->Width == 2 && r->Height == 2);
        const int offset = (int)r->X + (int)r->Y * w;
        if (this->TexPixelsAlpha8 != NULL)
        {
            this->TexPixelsAlpha8[offset] = this->TexPixelsAlpha8[offset + 1] = this->TexPixelsAlpha8[offset + w] = this->TexPixelsAlpha8[offset + w + 1] = 0xFF;
        }
        else
        {
            this->TexPixelsRGBA32[offset] = this->TexPixelsRGBA32[offset + 1] = this->TexPixelsRGBA32[offset + w] = this->TexPixelsRGBA32[offset + w + 1] = XY_COL32_WHITE;
        }
    }
    this->TexUvWhitePixel = XyVec2((r->X + 0.5f) * this->TexUvScale.x, (r->Y + 0.5f) * this->TexUvScale.y);
}

void XyFontAtlas::BuildRenderLinesTexData()
{
    if (this->Flags & XyFontAtlasFlags_NoBakedLines)
        return;

    // This generates a triangular shape in the texture, with the various line widths stacked on top of each other to allow interpolation between them
    XyFontAtlasCustomRect* r = this->GetCustomRectByIndex(this->PackIdLines);
    XY_ASSERT(r->IsPacked());
    for (unsigned int n = 0; n < XY_DRAWLIST_TEX_LINES_WIDTH_MAX + 1; n++) // +1 because of the zero-width row
    {
        // Each line consists of at least two empty pixels at the ends, with a line of solid pixels in the middle
        unsigned int y = n;
        unsigned int line_width = n;
        unsigned int pad_left = (r->Width - line_width) / 2;
        unsigned int pad_right = r->Width - (pad_left + line_width);

        // Write each slice
        XY_ASSERT(pad_left + line_width + pad_right == r->Width && y < r->Height); // Make sure we're inside the texture bounds before we start writing pixels
        if (this->TexPixelsAlpha8 != NULL)
        {
            unsigned char* write_ptr = &this->TexPixelsAlpha8[r->X + ((r->Y + y) * this->TexWidth)];
            for (unsigned int i = 0; i < pad_left; i++)
                *(write_ptr + i) = 0x00;

            for (unsigned int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = 0xFF;

            for (unsigned int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = 0x00;
        }
        else
        {
            unsigned int* write_ptr = &this->TexPixelsRGBA32[r->X + ((r->Y + y) * this->TexWidth)];
            for (unsigned int i = 0; i < pad_left; i++)
                *(write_ptr + i) = XY_COL32(255, 255, 255, 0);

            for (unsigned int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = XY_COL32_WHITE;

            for (unsigned int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = XY_COL32(255, 255, 255, 0);
        }

        // Calculate UVs for this line
        XyVec2 uv0 = XyVec2((float)(r->X + pad_left - 1), (float)(r->Y + y)) * this->TexUvScale;
        XyVec2 uv1 = XyVec2((float)(r->X + pad_left + line_width + 1), (float)(r->Y + y + 1)) * this->TexUvScale;
        float half_v = (uv0.y + uv1.y) * 0.5f; // Calculate a constant V in the middle of the row to avoid sampling artifacts
        this->TexUvLines[n] = XyVec4(uv0.x, half_v, uv1.x, half_v);
    }
}

static void GaussianBlurPass(float* src, float* dest, int size, bool horizontal)
{
    // See http://dev.theomader.com/gaussian-kernel-calculator/
    const float coefficients[] = { 0.0f, 0.0f, 0.000003f, 0.000229f, 0.005977f, 0.060598f, 0.24173f, 0.382925f, 0.24173f, 0.060598f, 0.005977f, 0.000229f, 0.000003f, 0.0f, 0.0f };
    const int kernel_size = XY_ARRAYSIZE(coefficients);
    const int sample_step = horizontal ? 1 : size;

    float* read_ptr = src;
    float* write_ptr = dest;
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
        {
            float result = 0.0f;
            int current_offset = (horizontal ? x : y) - ((kernel_size - 1) >> 1);
            float* sample_ptr = read_ptr - (((kernel_size - 1) >> 1) * sample_step);
            for (int j = 0; j < kernel_size; j++)
            {
                if (current_offset >= 0 && current_offset < size)
                    result += (*sample_ptr) * coefficients[j];
                current_offset++;
                sample_ptr += sample_step;
            }
            read_ptr++;
            *(write_ptr++) = result;
        }
}

void GaussianBlur(float* data, int size)
{
    // Do two passes, one from data into temp and then the second back to data again
    float* temp = (float*)alloca(size * size * sizeof(float));
    GaussianBlurPass(data, temp, size, true);
    GaussianBlurPass(temp, data, size, false);
}

static float DistanceFromPoint(const XyVec2& sample_pos, const XyVec2& point)
{
    return XyLength(sample_pos - point, 0.0f);
}

void XyFontAtlas::BuildRenderShadowTexData()
{
    XY_ASSERT(this->TexPixelsAlpha8 != NULL || this->TexPixelsRGBA32 != NULL);
    XY_ASSERT(this->ShadowRectIds[0] >= 0 && this->ShadowRectIds[1] >= 0);

    // Because of the blur, we have to generate the full 3x3 texture here, and then we chop that down to just the 2x2 section we need later.
    // 'size' correspond to the our 3x3 size, whereas 'shadow_tex_size' correspond to our 2x2 version where duplicate mirrored corners are not stored.
    const XyFontAtlasShadowTexConfig* shadow_cfg = &this->ShadowTexConfig;

    // The rectangular shadow texture
    {
        const int size = shadow_cfg->TexCornerSize + shadow_cfg->TexEdgeSize + shadow_cfg->TexCornerSize;
        const int corner_size = shadow_cfg->TexCornerSize;
        const int edge_size = shadow_cfg->TexEdgeSize;

        // The bounds of the rectangle we are generating the shadow from
        const XyVec2 shadow_rect_min((float)corner_size, (float)corner_size);
        const XyVec2 shadow_rect_max((float)(corner_size + edge_size), (float)(corner_size + edge_size));

        // Remove the padding we added
        XyFontAtlasCustomRect r = this->CustomRects[this->ShadowRectIds[0]];
        const int padding = shadow_cfg->GetRectTexPadding();
        r.X += (unsigned short)padding;
        r.Y += (unsigned short)padding;
        r.Width -= (unsigned short)padding * 2;
        r.Height -= (unsigned short)padding * 2;

        // Generate distance field
        // We draw the actual texture content by evaluating the distance field for the inner rectangle
        float* tex_data = (float*)alloca(size * size * sizeof(float));
        for (int y = 0; y < size; y++)
            for (int x = 0; x < size; x++)
            {
                float dist = DistanceFromRectangle(XyVec2((float)x, (float)y), shadow_rect_min, shadow_rect_max);
                float alpha = 1.0f - XyMin(XyMax(dist + shadow_cfg->TexDistanceFieldOffset, 0.0f) / XyMax((float)shadow_cfg->TexCornerSize + shadow_cfg->TexDistanceFieldOffset, 0.001f), 1.0f);
                alpha = XyPow(alpha, shadow_cfg->TexFalloffPower);  // Apply power curve to give a nicer falloff
                tex_data[x + (y * size)] = alpha;
            }

        // Blur
        if (shadow_cfg->TexBlur)
            GaussianBlur(tex_data, size);

        // Copy to texture, truncating to the actual required texture size (the bottom/right of the source data is chopped off, as we don't need it - see below). The truncated size is essentially the top 2x2 of our data, plus a little bit of padding for sampling.
        const int tex_w = this->TexWidth;
        const int shadow_tex_size = shadow_cfg->CalcRectTexSize();
        for (int y = 0; y < shadow_tex_size; y++)
            for (int x = 0; x < shadow_tex_size; x++)
            {
                const unsigned int offset = (const unsigned int)(r.X + x) + (int)(r.Y + y) * tex_w;
                const float alpha_f = tex_data[x + (y * size)];
                const unsigned char alpha_8 = (unsigned char)(0xFF * alpha_f);
                if (this->TexPixelsAlpha8)
                    this->TexPixelsAlpha8[offset] = alpha_8;
                else
                    this->TexPixelsRGBA32[offset] = XY_COL32(255, 255, 255, alpha_8);
            }

        // Generate UVs for each of the nine sections, which are arranged in a 3x3 grid starting from 0 in the top-left and going across then down
        for (int i = 0; i < 9; i++)
        {
            // The third row/column of the 3x3 grid are generated by flipping the appropriate chunks of the upper 2x2 grid.
            bool flip_h = false; // Do we need to flip the UVs horizontally?
            bool flip_v = false; // Do we need to flip the UVs vertically?

            XyFontAtlasCustomRect sub_rect = r;
            switch (i % 3)
            {
            case 0: sub_rect.Width = (unsigned short)corner_size; break;
            case 1: sub_rect.X += (unsigned short)corner_size; sub_rect.Width = (unsigned short)edge_size; break;
            case 2: sub_rect.Width = (unsigned short)corner_size; flip_h = true; break;
            }

            switch (i / 3)
            {
            case 0: sub_rect.Height = (unsigned short)corner_size; break;
            case 1: sub_rect.Y += (unsigned short)corner_size; sub_rect.Height = (unsigned short)edge_size; break;
            case 2: sub_rect.Height = (unsigned short)corner_size; flip_v = true; break;
            }

            XyVec2 uv0, uv1;
            this->CalcCustomRectUV(&sub_rect, &uv0, &uv1);
            this->ShadowRectUvs[i] = XyVec4(flip_h ? uv1.x : uv0.x, flip_v ? uv1.y : uv0.y, flip_h ? uv0.x : uv1.x, flip_v ? uv0.y : uv1.y);
        }
    }

    // The convex shape shadow texture
    {
        const int size = shadow_cfg->TexCornerSize * 2;
        const int padding = shadow_cfg->GetConvexTexPadding();

        // Generate distance field
        // We draw the actual texture content by evaluating the distance field for the distance from a center point
        XyFontAtlasCustomRect r = this->CustomRects[this->ShadowRectIds[1]];
        XyVec2 center_point((float)size * 0.5f, (float)size * 0.5f);
        float* tex_data = (float*)alloca(size * size * sizeof(float));
        for (int y = 0; y < size; y++)
            for (int x = 0; x < size; x++)
            {
                float dist = DistanceFromPoint(XyVec2((float)x, (float)y), center_point);
                float alpha = 1.0f - XyMin(XyMax((float)dist + shadow_cfg->TexDistanceFieldOffset, 0.0f) / XyMax((float)shadow_cfg->TexCornerSize + shadow_cfg->TexDistanceFieldOffset, 0.001f), 1.0f);
                alpha = XyPow(alpha, shadow_cfg->TexFalloffPower);  // Apply power curve to give a nicer falloff
                tex_data[x + (y * size)] = alpha;
            }

        // Blur
        if (shadow_cfg->TexBlur)
            GaussianBlur(tex_data, size);

        // Copy to texture, truncating to the actual required texture size (the bottom/right of the source data is chopped off, as we don't need it - see below)
        // We push the data down and right by the amount we padded the top of the texture (see CalcConvexTexWidth/CalcConvexTexHeight) for details
        const int padded_size = (int)((float)shadow_cfg->TexCornerSize / XyCos(XY_PI * 0.25f));
        const int src_x_offset = padding + (padded_size - shadow_cfg->TexCornerSize);
        const int src_y_offset = padding + (padded_size - shadow_cfg->TexCornerSize);

        const int tex_width = shadow_cfg->CalcConvexTexWidth();
        const int tex_height = shadow_cfg->CalcConvexTexHeight();
        const int tex_w = this->TexWidth;
        for (int y = 0; y < tex_height; y++)
            for (int x = 0; x < tex_width; x++)
            {
                const int src_x = XyClamp(x - src_x_offset, 0, size - 1);
                const int src_y = XyClamp(y - src_y_offset, 0, size - 1);
                const float alpha_f = tex_data[src_x + (src_y * size)];
                const unsigned char alpha_8 = (unsigned char)(0xFF * alpha_f);
                const unsigned int offset = (const unsigned int)(r.X + x) + (int)(r.Y + y) * tex_w;
                if (this->TexPixelsAlpha8)
                    this->TexPixelsAlpha8[offset] = alpha_8;
                else
                    this->TexPixelsRGBA32[offset] = XY_COL32(255, 255, 255, alpha_8);
            }

        // Remove the padding we added
        r.X += (unsigned short)padding;
        r.Y += (unsigned short)padding;
        r.Width = (unsigned short)(tex_width - (padding * 2));
        r.Height = (unsigned short)(tex_height - (padding * 2));

        // Generate UVs
        XyVec2 uv0, uv1;
        this->CalcCustomRectUV(&r, &uv0, &uv1);
        this->ShadowRectUvs[9] = XyVec4(uv0.x, uv0.y, uv1.x, uv1.y);
    }
}

void XyFontAtlas::BuildFinish()
{
    // Render into our custom data blocks
    XY_ASSERT(this->TexPixelsAlpha8 != NULL || this->TexPixelsRGBA32 != NULL);
    BuildRenderDefaultTexData();
    BuildRenderLinesTexData();
    BuildRenderShadowTexData();

    // Register custom rectangle glyphs
    for (int i = 0; i < this->CustomRects.Size; i++)
    {
        const XyFontAtlasCustomRect* r = &this->CustomRects[i];
        if (r->Font == NULL || r->GlyphID == 0)
            continue;

        // Will ignore XyFontConfig settings: GlyphMinAdvanceX, GlyphMinAdvanceY, GlyphExtraSpacing, PixelSnapH
        XY_ASSERT(r->Font->ContainerAtlas == this);
        XyVec2 uv0, uv1;
        this->CalcCustomRectUV(r, &uv0, &uv1);
        r->Font->AddGlyph(NULL, (XyWchar)r->GlyphID, r->GlyphOffset.x, r->GlyphOffset.y, r->GlyphOffset.x + r->Width, r->GlyphOffset.y + r->Height, uv0.x, uv0.y, uv1.x, uv1.y, r->GlyphAdvanceX);
    }

    // Build all fonts lookup tables
    for (int i = 0; i < this->Fonts.Size; i++)
        if (this->Fonts[i]->DirtyLookupTables)
            this->Fonts[i]->BuildLookupTable();

    this->TexReady = true;
}

void XyFontAtlas::BuildSetupFont(XyFont* font, XyFontConfig* font_config, float ascent, float descent)
{
    if (!font_config->MergeMode)
    {
        font->ClearOutputData();
        font->FontSize = font_config->SizePixels;
        font->ConfigData = font_config;
        font->ConfigDataCount = 0;
        font->ContainerAtlas = this;
        font->Ascent = ascent;
        font->Descent = descent;
    }
    font->ConfigDataCount++;
}

void XyFontAtlas::BuildPackCustomRects(void* stbrp_context_opaque)
{
    stbrp_context* pack_context = (stbrp_context*)stbrp_context_opaque;
    XY_ASSERT(pack_context != NULL);

    XyVector<XyFontAtlasCustomRect>& user_rects = this->CustomRects;
    XY_ASSERT(user_rects.Size >= 1); // We expect at least the default custom rects to be registered, else something went wrong.
#ifdef __GNUC__
    if (user_rects.Size < 1) { __builtin_unreachable(); } // Workaround for GCC bug if XY_ASSERT() is defined to conditionally throw (see #5343)
#endif

    XyVector<stbrp_rect> pack_rects;
    pack_rects.resize(user_rects.Size);
    memset(pack_rects.Data, 0, (size_t)pack_rects.size_in_bytes());
    for (int i = 0; i < user_rects.Size; i++)
    {
        pack_rects[i].w = user_rects[i].Width;
        pack_rects[i].h = user_rects[i].Height;
    }
    stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.Size);
    for (int i = 0; i < pack_rects.Size; i++)
        if (pack_rects[i].was_packed)
        {
            user_rects[i].X = (unsigned short)pack_rects[i].x;
            user_rects[i].Y = (unsigned short)pack_rects[i].y;
            XY_ASSERT(pack_rects[i].w == user_rects[i].Width && pack_rects[i].h == user_rects[i].Height);
            this->TexHeight = XyMax(this->TexHeight, pack_rects[i].y + pack_rects[i].h);
        }
}

void XyFontAtlas::BuildRegisterShadowCustomRects()
{
    if (this->ShadowRectIds[0] >= 0)
        return;

    // ShadowRectIds[0] is the rectangle for rectangular shadows
    // ShadowRectIds[1] is the rectangle for convex shadows

    // The actual size we want to reserve, including padding
    const XyFontAtlasShadowTexConfig* shadow_cfg = &this->ShadowTexConfig;
    int effective_size = shadow_cfg->CalcRectTexSize() + shadow_cfg->GetRectTexPadding();
    this->ShadowRectIds[0] = this->AddCustomRectRegular(effective_size, effective_size);
    this->ShadowRectIds[1] = this->AddCustomRectRegular(shadow_cfg->CalcConvexTexWidth() + shadow_cfg->GetConvexTexPadding(), shadow_cfg->CalcConvexTexHeight() + shadow_cfg->GetConvexTexPadding());
}

void XyFontAtlas::BuildInit()
{
    // Register texture region for mouse cursors or standard white pixels
    if (this->PackIdMouseCursors < 0)
    {
        if (!(this->Flags & XyFontAtlasFlags_NoMouseCursors))
            this->PackIdMouseCursors = this->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1, FONT_ATLAS_DEFAULT_TEX_DATA_H);
        else
            this->PackIdMouseCursors = this->AddCustomRectRegular(2, 2);
    }

    // Register texture region for thick lines
    // The +2 here is to give space for the end caps, whilst height +1 is to accommodate the fact we have a zero-width row
    if (this->PackIdLines < 0)
    {
        if (!(this->Flags & XyFontAtlasFlags_NoBakedLines))
            this->PackIdLines = this->AddCustomRectRegular(XY_DRAWLIST_TEX_LINES_WIDTH_MAX + 2, XY_DRAWLIST_TEX_LINES_WIDTH_MAX + 1);
    }

    BuildRegisterShadowCustomRects();
}


struct XyFontBuildSrcData
{
    stbtt_fontinfo      FontInfo;
    stbtt_pack_range    PackRange;          // Hold the list of codepoints to pack (essentially points to Codepoints.Data)
    stbrp_rect* Rects;              // Rectangle to pack. We first fill in their size and the packer will give us their position.
    stbtt_packedchar* PackedChars;        // Output glyphs
    const XyWchar* SrcRanges;          // Ranges as requested by user (user is allowed to request too much, e.g. 0x0020..0xFFFF)
    int                 DstIndex;           // Index into atlas->Fonts[] and dst_tmp_array[]
    int                 GlyphsHighest;      // Highest requested codepoint
    int                 GlyphsCount;        // Glyph count (excluding missing glyphs and glyphs already set by an earlier source font)
private:
    int ___pad;
public:
    XyBitVector         GlyphsSet;          // Glyph bit map (random access, 1-bit per codepoint. This will be a maximum of 8KB)
    XyVector<int>       GlyphsList;         // Glyph codepoints list (flattened version of GlyphsSet)
};

// Temporary data for one destination ImFont* (multiple source fonts can be merged into one destination ImFont)
struct XyFontBuildDstData
{
    int                 SrcCount;           // Number of source fonts targeting this destination font.
    int                 GlyphsHighest;
    int                 GlyphsCount;
private:
    int ___pad;
public:
    XyBitVector         GlyphsSet;          // This is used to resolve collision when multiple sources are merged into a same destination font.
};

bool XyFontAtlas::BuildWithStbTruetype()
{
    XY_ASSERT(this->ConfigData.Size > 0);

    BuildInit();

    // Clear atlas
    this->TexID = (XyTextureID)NULL;
    this->TexWidth = this->TexHeight = 0;
    this->TexUvScale = XyVec2(0.0f, 0.0f);
    this->TexUvWhitePixel = XyVec2(0.0f, 0.0f);
    this->ClearTexData();

    // Temporary storage for building
    XyVector<XyFontBuildSrcData> src_tmp_array;
    XyVector<XyFontBuildDstData> dst_tmp_array;
    src_tmp_array.resize(this->ConfigData.Size);
    dst_tmp_array.resize(this->Fonts.Size);
    memset(src_tmp_array.Data, 0, (size_t)src_tmp_array.size_in_bytes());
    memset(dst_tmp_array.Data, 0, (size_t)dst_tmp_array.size_in_bytes());

    // 1. Initialize font loading structure, check font data validity
    for (int src_i = 0; src_i < this->ConfigData.Size; src_i++)
    {
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        XyFontConfig& cfg = this->ConfigData[src_i];
        XY_ASSERT(cfg.DstFont && (!cfg.DstFont->IsLoaded() || cfg.DstFont->ContainerAtlas == this));

        // Find index from cfg.DstFont (we allow the user to set cfg.DstFont. Also it makes casual debugging nicer than when storing indices)
        src_tmp.DstIndex = -1;
        for (int output_i = 0; output_i < this->Fonts.Size && src_tmp.DstIndex == -1; output_i++)
            if (cfg.DstFont == this->Fonts[output_i])
                src_tmp.DstIndex = output_i;
        if (src_tmp.DstIndex == -1)
        {
            XY_ASSERT(src_tmp.DstIndex != -1); // cfg.DstFont not pointing within this->Fonts[] array?
            return false;
        }
        // Initialize helper structure for font loading and verify that the TTF/OTF data is correct
        const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData, cfg.FontNo);
        XY_ASSERT(font_offset >= 0 && "FontData is incorrect, or FontNo cannot be found.");
        if (!stbtt_InitFont(&src_tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
            return false;

        // Measure highest codepoints
        XyFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.SrcRanges = cfg.GlyphRanges ? cfg.GlyphRanges : this->GetGlyphRangesDefault();
        for (const XyWchar* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
        {
            // Check for valid range. This may also help detect *some* dangling pointers, because a common
            // user error is to setup XyFontConfig::GlyphRanges with a pointer to data that isn't persistent.
            XY_ASSERT(src_range[0] <= src_range[1]);
            src_tmp.GlyphsHighest = XyMax(src_tmp.GlyphsHighest, (int)src_range[1]);
        }
        dst_tmp.SrcCount++;
        dst_tmp.GlyphsHighest = XyMax(dst_tmp.GlyphsHighest, src_tmp.GlyphsHighest);
    }

    // 2. For every requested codepoint, check for their presence in the font data, and handle redundancy or overlaps between source fonts to avoid unused glyphs.
    int total_glyphs_count = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        XyFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.GlyphsSet.Create(src_tmp.GlyphsHighest + 1);
        if (dst_tmp.GlyphsSet.Storage.empty())
            dst_tmp.GlyphsSet.Create(dst_tmp.GlyphsHighest + 1);

        for (const XyWchar* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
            for (unsigned int codepoint = src_range[0]; codepoint <= src_range[1]; codepoint++)
            {
                if (dst_tmp.GlyphsSet.TestBit((int)codepoint))    // Don't overwrite existing glyphs. We could make this an option for MergeMode (e.g. MergeOverwrite==true)
                    continue;
                if (!stbtt_FindGlyphIndex(&src_tmp.FontInfo, (int)codepoint))    // It is actually in the font?
                    continue;

                // Add to avail set/counters
                src_tmp.GlyphsCount++;
                dst_tmp.GlyphsCount++;
                src_tmp.GlyphsSet.SetBit((int)codepoint);
                dst_tmp.GlyphsSet.SetBit((int)codepoint);
                total_glyphs_count++;
            }
    }

    // 3. Unpack our bit map into a flat list (we now have all the Unicode points that we know are requested _and_ available _and_ not overlapping another)
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        src_tmp.GlyphsList.reserve(src_tmp.GlyphsCount);
        UnpackBitVectorToFlatIndexList(&src_tmp.GlyphsSet, &src_tmp.GlyphsList);
        src_tmp.GlyphsSet.Clear();
        XY_ASSERT(src_tmp.GlyphsList.Size == src_tmp.GlyphsCount);
    }
    for (int dst_i = 0; dst_i < dst_tmp_array.Size; dst_i++)
        dst_tmp_array[dst_i].GlyphsSet.Clear();
    dst_tmp_array.clear();

    // Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
    // (We technically don't need to zero-clear buf_rects, but let's do it for the sake of sanity)
    XyVector<stbrp_rect> buf_rects;
    XyVector<stbtt_packedchar> buf_packedchars;
    buf_rects.resize(total_glyphs_count);
    buf_packedchars.resize(total_glyphs_count);
    memset(buf_rects.Data, 0, (size_t)buf_rects.size_in_bytes());
    memset(buf_packedchars.Data, 0, (size_t)buf_packedchars.size_in_bytes());

    // 4. Gather glyphs sizes so we can pack them in our virtual canvas.
    int total_surface = 0;
    int buf_rects_out_n = 0;
    int buf_packedchars_out_n = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        src_tmp.Rects = &buf_rects[buf_rects_out_n];
        src_tmp.PackedChars = &buf_packedchars[buf_packedchars_out_n];
        buf_rects_out_n += src_tmp.GlyphsCount;
        buf_packedchars_out_n += src_tmp.GlyphsCount;

        // Convert our ranges in the format stb_truetype wants
        XyFontConfig& cfg = this->ConfigData[src_i];
        src_tmp.PackRange.font_size = cfg.SizePixels;
        src_tmp.PackRange.first_unicode_codepoint_in_range = 0;
        src_tmp.PackRange.array_of_unicode_codepoints = src_tmp.GlyphsList.Data;
        src_tmp.PackRange.num_chars = src_tmp.GlyphsList.Size;
        src_tmp.PackRange.chardata_for_range = src_tmp.PackedChars;
        src_tmp.PackRange.h_oversample = (unsigned char)cfg.OversampleH;
        src_tmp.PackRange.v_oversample = (unsigned char)cfg.OversampleV;

        // Gather the sizes of all rectangles we will need to pack (this loop is based on stbtt_PackFontRangesGatherRects)
        const float scale = (cfg.SizePixels > 0) ? stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels) : stbtt_ScaleForMappingEmToPixels(&src_tmp.FontInfo, -cfg.SizePixels);
        const int padding = this->TexGlyphPadding;
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsList.Size; glyph_i++)
        {
            int x0, y0, x1, y1;
            const int glyph_index_in_font = stbtt_FindGlyphIndex(&src_tmp.FontInfo, src_tmp.GlyphsList[glyph_i]);
            XY_ASSERT(glyph_index_in_font != 0);
            stbtt_GetGlyphBitmapBoxSubpixel(&src_tmp.FontInfo, glyph_index_in_font, scale * (float)cfg.OversampleH, scale * (float)cfg.OversampleV, 0, 0, &x0, &y0, &x1, &y1);
            src_tmp.Rects[glyph_i].w = (stbrp_coord)(x1 - x0 + padding + cfg.OversampleH - 1);
            src_tmp.Rects[glyph_i].h = (stbrp_coord)(y1 - y0 + padding + cfg.OversampleV - 1);
            total_surface += src_tmp.Rects[glyph_i].w * src_tmp.Rects[glyph_i].h;
        }
    }

    // We need a width for the skyline algorithm, any width!
    // The exact width doesn't really matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
    // User can override TexDesiredWidth and TexGlyphPadding if they wish, otherwise we use a simple heuristic to select the width based on expected surface.
    const float surface_sqrt = XySqrt((float)total_surface) + 1;
    this->TexHeight = 0;
    if (this->TexDesiredWidth > 0)
        this->TexWidth = this->TexDesiredWidth;
    else
        this->TexWidth = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

    // 5. Start packing
    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    const int TEX_HEIGHT_MAX = 1024 * 32;
    stbtt_pack_context spc = {};
    stbtt_PackBegin(&spc, NULL, this->TexWidth, TEX_HEIGHT_MAX, 0, this->TexGlyphPadding, NULL);
    BuildPackCustomRects(spc.pack_info);

    // 6. Pack each source font. No rendering yet, we are working with rectangles in an infinitely tall texture at this point.
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbrp_pack_rects((stbrp_context*)spc.pack_info, src_tmp.Rects, src_tmp.GlyphsCount);

        // Extend texture height and mark missing glyphs as non-packed so we won't render them.
        // FIXME: We are not handling packing failure here (would happen if we got off TEX_HEIGHT_MAX or if a single if larger than TexWidth?)
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
            if (src_tmp.Rects[glyph_i].was_packed)
                this->TexHeight = XyMax(this->TexHeight, src_tmp.Rects[glyph_i].y + src_tmp.Rects[glyph_i].h);
    }

    // 7. Allocate texture
    this->TexHeight = (this->Flags & XyFontAtlasFlags_NoPowerOfTwoHeight) ? (this->TexHeight + 1) : XyUpperPowerOfTwo(this->TexHeight);
    this->TexUvScale = XyVec2(1.0f / (float)this->TexWidth, 1.0f / (float)this->TexHeight);
    this->TexPixelsAlpha8 = (unsigned char*)malloc((size_t)(this->TexWidth * this->TexHeight));
    memset(this->TexPixelsAlpha8, 0, (size_t)(this->TexWidth * this->TexHeight));
    spc.pixels = this->TexPixelsAlpha8;
    spc.height = this->TexHeight;

    // 8. Render/rasterize font characters into the texture
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        XyFontConfig& cfg = this->ConfigData[src_i];
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbtt_PackFontRangesRenderIntoRects(&spc, &src_tmp.FontInfo, &src_tmp.PackRange, 1, src_tmp.Rects);

        // Apply multiply operator
        if (cfg.RasterizerMultiply != 1.0f)
        {
            unsigned char multiply_table[256];
            BuildMultiplyCalcLookupTable(multiply_table, cfg.RasterizerMultiply);
            stbrp_rect* r = &src_tmp.Rects[0];
            for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++, r++)
                if (r->was_packed)
                    BuildMultiplyRectAlpha8(multiply_table, this->TexPixelsAlpha8, r->x, r->y, r->w, r->h, this->TexWidth * 1);
        }
        src_tmp.Rects = NULL;
    }

    // End packing
    stbtt_PackEnd(&spc);
    buf_rects.clear();

    // 9. Setup XyFont and glyphs for runtime
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        // When merging fonts with MergeMode=true:
        // - We can have multiple input fonts writing into a same destination font.
        // - dst_font->ConfigData is != from cfg which is our source configuration.
        XyFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        XyFontConfig& cfg = this->ConfigData[src_i];
        XyFont* dst_font = cfg.DstFont;

        const float font_scale = stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&src_tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

        const float ascent = XyFloor((float)unscaled_ascent * font_scale + ((unscaled_ascent > 0) ? +1.f : -1.f));
        const float descent = XyFloor((float)unscaled_descent * font_scale + ((unscaled_descent > 0) ? +1.f : -1.f));
        BuildSetupFont(dst_font, &cfg, ascent, descent);
        const float font_off_x = cfg.GlyphOffset.x;
        const float font_off_y = cfg.GlyphOffset.y + XY_ROUND(dst_font->Ascent);

        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
        {
            // Register glyph
            const int codepoint = src_tmp.GlyphsList[glyph_i];
            const stbtt_packedchar& pc = src_tmp.PackedChars[glyph_i];
            stbtt_aligned_quad q;
            float unused_x = 0.0f, unused_y = 0.0f;
            stbtt_GetPackedQuad(src_tmp.PackedChars, this->TexWidth, this->TexHeight, glyph_i, &unused_x, &unused_y, &q, 0);
            dst_font->AddGlyph(&cfg, (XyWchar)codepoint, q.x0 + font_off_x, q.y0 + font_off_y, q.x1 + font_off_x, q.y1 + font_off_y, q.s0, q.t0, q.s1, q.t1, pc.xadvance);
        }
    }

    // Cleanup
    src_tmp_array.clear_destruct();

    BuildFinish();
    return true;
}