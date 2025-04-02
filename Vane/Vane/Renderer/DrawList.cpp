#include "Xyphra.h"


void _Xyphra::SetCurrentFont(XyFont* font)
{
    XY_ASSERT(font && font->IsLoaded());    // Font Atlas not created. Did you call io.Fonts->GetTexDataAsRGBA32 / GetTexDataAsAlpha8 ?
    XY_ASSERT(font->Scale > 0.0f);
    Font = font;
    FontBaseSize = XyMax(1.0f, Font->FontSize * Font->Scale);
    FontSize = FontBaseSize;

    XyFontAtlas* atlas = Font->ContainerAtlas;
    shared_data.TexUvWhitePixel = atlas->TexUvWhitePixel;
    shared_data.TexUvLines = atlas->TexUvLines;
    shared_data.Font = Font;
    shared_data.FontSize = FontSize;
    shared_data.ShadowRectIds = &atlas->ShadowRectIds[0];
    shared_data.ShadowRectUvs = &atlas->ShadowRectUvs[0];
}

void _Xyphra::PushFont(XyFont* font)
{
    if (!font)
        font = Fonts.Fonts[0];
    SetCurrentFont(font);
    FontStack.push_back(font);
}

float _Xyphra::AddTextTest(const XyVec2& pos, XyU32 col, const char* text_begin, float pos_find, void* out, bool search_type)
{
    XY_ASSERT(out != NULL);

    return AddTextTest(NULL, 0.0f, pos, col, text_begin, pos_find, out, search_type);
}

float _Xyphra::AddTextTest(const XyFont* font, float font_size, const XyVec2& pos, XyU32 col, const char* text_begin, float pos_find, void* out, bool search_type, float wrap_width, const XyVec4* cpu_fine_clip_rect)
{
    if (!text_begin || text_begin == (const char*)0xFFFFFFFFFFFFFFFF || text_begin == (const char*)0xdddddddddddddddd)
        return pos.x;
    if ((col & XY_COL32_A_MASK) == 0)
        return pos.x;

    const char* text_end = text_begin + strlen(text_begin);

    // Init return values
    if (search_type) // by index
        *(float*)out = pos.x;
    else
        *(int*)out = 0;

    if (text_begin == text_end)
        return pos.x;

    // Pull default font/size from the shared XyDrawListSharedData instance
    if (font == NULL)
        font = Font;
    if (font_size == 0.0f)
        font_size = FontSize;

    XY_ASSERT(font->ContainerAtlas->TexID == _CmdHeader.TextureId);  // Use high-level PushFont() or low-level XyDrawList::PushTextureId() to change font.

    XyVec4 clip_rect = _CmdHeader.ClipRect;
    if (cpu_fine_clip_rect)
    {
        clip_rect.x = XyMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = XyMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = XyMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = XyMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    return font->RenderTextTest(font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL, pos_find, out, search_type);
}

float _Xyphra::AddText(const XyVec2& pos, XyU32 col, const char* text_begin, const char* text_end)
{
    return AddText(NULL, 0.0f, pos, col, text_begin, text_end);
}

float _Xyphra::AddText(const XyFont* font, float font_size, const XyVec2& pos, XyU32 col, const char* text_begin, const char* text_end, float wrap_width, const XyVec4* cpu_fine_clip_rect)
{
    if (!text_begin || text_begin == (const char*)0xFFFFFFFFFFFFFFFF || text_begin == (const char*)0xdddddddddddddddd)
        return pos.x;
    if ((col & XY_COL32_A_MASK) == 0)
        return pos.x;

    if (text_end == NULL)
        text_end = text_begin + strlen(text_begin);
    if (text_begin == text_end)
        return pos.x;

    // Pull default font/size from the shared XyDrawListSharedData instance
    if (font == NULL)
        font = Font;
    if (font_size == 0.0f)
        font_size = FontSize;

    XY_ASSERT(font->ContainerAtlas->TexID == _CmdHeader.TextureId);  // Use high-level PushFont() or low-level PushTextureId() to change font.

    XyVec4 clip_rect = _CmdHeader.ClipRect;
    if (cpu_fine_clip_rect)
    {
        clip_rect.x = XyMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = XyMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = XyMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = XyMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    return font->RenderText(font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}

void  _Xyphra::PopFont()
{
    FontStack.pop_back();
    SetCurrentFont(FontStack.empty() ? Fonts.Fonts[0] : FontStack.back());
}

void _Xyphra::AddDrawCmd()
{
    XyCmd draw_cmd;
    draw_cmd.ClipRect = _CmdHeader.ClipRect;    // Same as calling XyDrawCmd_HeaderCopy()
    draw_cmd.TextureId = _CmdHeader.TextureId;
    draw_cmd.VtxOffset = _CmdHeader.VtxOffset;
    draw_cmd.IdxOffset = IdxBuffer.Size;

    CmdBuffer.push_back(draw_cmd);
}

void _Xyphra::PrimReserve(int idx_count, int vtx_count)
{
    // Large mesh support (when enabled)
    XY_ASSERT(idx_count >= 0 && vtx_count >= 0);
    if ((_VtxCurrentIdx + vtx_count >= (1 << 16)) && (Flags & XyDrawListFlags_AllowVtxOffset))
    {
        // FIXME: In theory we should be testing that vtx_count <64k here.
        // In practice, RenderText() relies on reserving ahead for a worst case scenario so it is currently useful for us
        // to not make that check until we rework the text functions to handle clipping and large horizontal lines better.
        _CmdHeader.VtxOffset = VtxBuffer.Size;
        //_OnChangedVtxOffset();
        _VtxCurrentIdx = 0;
        XyCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
        //XY_ASSERT(curr_cmd->VtxOffset != _CmdHeader.VtxOffset); // See #3349
        if (curr_cmd->ElemCount != 0)
        {
            AddDrawCmd();
            return;
        }
        curr_cmd->VtxOffset = _CmdHeader.VtxOffset;
    }

    XyCmd* draw_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    draw_cmd->ElemCount += idx_count;

    int vtx_buffer_old_size = VtxBuffer.Size;
    VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
    _VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

    int idx_buffer_old_size = IdxBuffer.Size;
    IdxBuffer.resize(idx_buffer_old_size + idx_count);
    _IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

void _Xyphra::PrimUnreserve(int idx_count, int vtx_count)
{
    XyCmd* draw_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    draw_cmd->ElemCount -= idx_count;
    VtxBuffer.shrink(VtxBuffer.Size - vtx_count);
    IdxBuffer.shrink(IdxBuffer.Size - idx_count);
}

void _Xyphra::PrimRect(const XyVec2& a, const XyVec2& c, XyU32 col)
{
    XyVec2 b(c.x, a.y), d(a.x, c.y), uv(shared_data.TexUvWhitePixel);
    XyIndex idx = (XyIndex)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (XyIndex)(idx + 1); _IdxWritePtr[2] = (XyIndex)(idx + 2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (XyIndex)(idx + 2); _IdxWritePtr[5] = (XyIndex)(idx + 3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}


void _Xyphra::PrimRectUV(const XyVec2& a, const XyVec2& c, const XyVec2& uv_a, const XyVec2& uv_c, XyU32 col)
{
    XyVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
    XyIndex idx = (XyIndex)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (XyIndex)(idx + 1); _IdxWritePtr[2] = (XyIndex)(idx + 2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (XyIndex)(idx + 2); _IdxWritePtr[5] = (XyIndex)(idx + 3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void _Xyphra::PrimQuadUV(const XyVec2& a, const XyVec2& b, const XyVec2& c, const XyVec2& d, const XyVec2& uv_a, const XyVec2& uv_b, const XyVec2& uv_c, const XyVec2& uv_d, XyU32 col)
{
    XyIndex idx = (XyIndex)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (XyIndex)(idx + 1); _IdxWritePtr[2] = (XyIndex)(idx + 2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (XyIndex)(idx + 2); _IdxWritePtr[5] = (XyIndex)(idx + 3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void _Xyphra::AddConvexPolyFilled(const XyVec2* points, int points_count, XyU32 col)
{

    if (points_count < 3 || (col & XY_COL32_A_MASK) == 0)
        return;

    const XyVec2 uv = shared_data.TexUvWhitePixel;

    if (Flags & XyDrawListFlags_AntiAliasedFill)
    {
        // Anti-aliased Fill
        const float AA_SIZE = _FringeScale;
        const XyU32 col_trans = col & ~XY_COL32_A_MASK;
        const int idx_count = (points_count - 2) * 3 + points_count * 6;
        const int vtx_count = (points_count * 2);
        PrimReserve(idx_count, vtx_count);

        // Add indexes for fill
        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (XyIndex)(vtx_inner_idx); _IdxWritePtr[1] = (XyIndex)(vtx_inner_idx + ((i - 1) << 1)); _IdxWritePtr[2] = (XyIndex)(vtx_inner_idx + (i << 1));
            _IdxWritePtr += 3;
        }

        // Compute normals
        shared_data.TempBuffer.reserve_discard(points_count);
        XyVec2* temp_normals = shared_data.TempBuffer.Data;
        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const XyVec2& p0 = points[i0];
            const XyVec2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            XY_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            // Average normals
            const XyVec2& n0 = temp_normals[i0];
            const XyVec2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            XY_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            // Add vertices
            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x); _VtxWritePtr[0].pos.y = (points[i1].y - dm_y); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x); _VtxWritePtr[1].pos.y = (points[i1].y + dm_y); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
            _VtxWritePtr += 2;

            // Add indexes for fringes
            _IdxWritePtr[0] = (XyIndex)(vtx_inner_idx + (i1 << 1)); _IdxWritePtr[1] = (XyIndex)(vtx_inner_idx + (i0 << 1)); _IdxWritePtr[2] = (XyIndex)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[3] = (XyIndex)(vtx_outer_idx + (i0 << 1)); _IdxWritePtr[4] = (XyIndex)(vtx_outer_idx + (i1 << 1)); _IdxWritePtr[5] = (XyIndex)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (XyIndex)vtx_count;
    }
    else
    {
        // Non Anti-aliased Fill
        const int idx_count = (points_count - 2) * 3;
        const int vtx_count = points_count;
        PrimReserve(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr++;
        }
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (XyIndex)(_VtxCurrentIdx); _IdxWritePtr[1] = (XyIndex)(_VtxCurrentIdx + i - 1); _IdxWritePtr[2] = (XyIndex)(_VtxCurrentIdx + i);
            _IdxWritePtr += 3;
        }
        _VtxCurrentIdx += (XyIndex)vtx_count;
    }
}

void _Xyphra::AddPolyline(const XyVec2* points, int points_count, XyU32 col, XyDrawFlags flags, float thickness)
{

    if (points_count < 2 || (col & XY_COL32_A_MASK) == 0)
        return;

    const bool closed = (flags & XyDrawFlags_Closed) != 0;
    const XyVec2 opaque_uv = shared_data.TexUvWhitePixel;
    const int count = closed ? points_count : points_count - 1; // The number of line segments we need to draw
    const bool thick_line = (thickness > _FringeScale);

    if (Flags & XyDrawListFlags_AntiAliasedLines)
    {
        // Anti-aliased stroke
        const float AA_SIZE = _FringeScale;
        const XyU32 col_trans = col & ~XY_COL32_A_MASK;

        // Thicknesses <1.0 should behave like thickness 1.0
        thickness = XyMax(thickness, 1.0f);
        const int integer_thickness = (int)thickness;
        const float fractional_thickness = thickness - integer_thickness;

        // Do we want to draw this line using a texture?
        // - For now, only draw integer-width lines using textures to avoid issues with the way scaling occurs, could be improved.
        // - If AA_SIZE is not 1.0f we cannot use the texture path.
        const bool use_texture = (Flags & XyDrawListFlags_AntiAliasedLinesUseTex) && (integer_thickness < XY_DRAWLIST_TEX_LINES_WIDTH_MAX) && (fractional_thickness <= 0.00001f) && (AA_SIZE == 1.0f);

        // We should never hit this, because NewFrame() doesn't set XyDrawListFlags_AntiAliasedLinesUseTex unless XyFontAtlasFlags_NoBakedLines is off
        const int idx_count = use_texture ? (count * 6) : (thick_line ? count * 18 : count * 12);
        const int vtx_count = use_texture ? (points_count * 2) : (thick_line ? points_count * 4 : points_count * 3);
        PrimReserve(idx_count, vtx_count);

        // Temporary buffer
        // The first <points_count> items are normals at each line point, then after that there are either 2 or 4 temp points for each line point
        shared_data.TempBuffer.reserve_discard(points_count * ((use_texture || !thick_line) ? 3 : 5));
        XyVec2* temp_normals = shared_data.TempBuffer.Data;
        XyVec2* temp_points = temp_normals + points_count;

        // Calculate normals (tangents) for each line segment
        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            XY_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count - 1] = temp_normals[points_count - 2];

        // If we are drawing a one-pixel-wide line without a texture, or a textured line of any width, we only need 2 or 3 vertices per point
        if (use_texture || !thick_line)
        {
            // [PATH 1] Texture-based lines (thick or non-thick)
            // [PATH 2] Non texture-based lines (non-thick)

            // The width of the geometry we need to draw - this is essentially <thickness> pixels for the line itself, plus "one pixel" for AA.
            // - In the texture-based path, we don't use AA_SIZE here because the +1 is tied to the generated texture
            //   (see XyFontAtlasBuildRenderLinesTexData() function), and so alternate values won't work without changes to that code.
            // - In the non texture-based paths, we would allow AA_SIZE to potentially be != 1.0f with a patch (e.g. fringe_scale patch to
            //   allow scaling geometry while preserving one-screen-pixel AA fringe).
            const float half_draw_size = use_texture ? ((thickness * 0.5f) + 1) : AA_SIZE;

            // If line is not closed, the first and last points need to be generated differently as there are no normals to blend
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * half_draw_size;
                temp_points[1] = points[0] - temp_normals[0] * half_draw_size;
                temp_points[(points_count - 1) * 2 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * half_draw_size;
                temp_points[(points_count - 1) * 2 + 1] = points[points_count - 1] - temp_normals[points_count - 1] * half_draw_size;
            }

            // Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
            // This takes points n and n+1 and writes into n+1, with the first point in a closed line being generated from the final one (as n+1 wraps)
            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx; // Vertex index for start of line segment
            for (int i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
            {
                const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1; // i2 is the second point of the line segment
                const unsigned int idx2 = ((i1 + 1) == points_count) ? _VtxCurrentIdx : (idx1 + (use_texture ? 2 : 3)); // Vertex index for end of segment

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                XY_FIXNORMAL2F(dm_x, dm_y);
                dm_x *= half_draw_size; // dm_x, dm_y are offset to the outer edge of the AA area
                dm_y *= half_draw_size;

                // Add temporary vertexes for the outer edges
                XyVec2* out_vtx = &temp_points[i2 * 2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                if (use_texture)
                {
                    // Add indices for two triangles
                    _IdxWritePtr[0] = (XyIndex)(idx2 + 0); _IdxWritePtr[1] = (XyIndex)(idx1 + 0); _IdxWritePtr[2] = (XyIndex)(idx1 + 1); // Right tri
                    _IdxWritePtr[3] = (XyIndex)(idx2 + 1); _IdxWritePtr[4] = (XyIndex)(idx1 + 1); _IdxWritePtr[5] = (XyIndex)(idx2 + 0); // Left tri
                    _IdxWritePtr += 6;
                }
                else
                {
                    // Add indexes for four triangles
                    _IdxWritePtr[0] = (XyIndex)(idx2 + 0); _IdxWritePtr[1] = (XyIndex)(idx1 + 0); _IdxWritePtr[2] = (XyIndex)(idx1 + 2); // Right tri 1
                    _IdxWritePtr[3] = (XyIndex)(idx1 + 2); _IdxWritePtr[4] = (XyIndex)(idx2 + 2); _IdxWritePtr[5] = (XyIndex)(idx2 + 0); // Right tri 2
                    _IdxWritePtr[6] = (XyIndex)(idx2 + 1); _IdxWritePtr[7] = (XyIndex)(idx1 + 1); _IdxWritePtr[8] = (XyIndex)(idx1 + 0); // Left tri 1
                    _IdxWritePtr[9] = (XyIndex)(idx1 + 0); _IdxWritePtr[10] = (XyIndex)(idx2 + 0); _IdxWritePtr[11] = (XyIndex)(idx2 + 1); // Left tri 2
                    _IdxWritePtr += 12;
                }

                idx1 = idx2;
            }

            // Add vertexes for each point on the line
            if (use_texture)
            {
                // If we're using textures we only need to emit the left/right edge vertices
                XyVec4 tex_uvs = shared_data.TexUvLines[integer_thickness];
                /*if (fractional_thickness != 0.0f) // Currently always zero when use_texture==false!
                {
                    const XyVec4 tex_uvs_1 = _Xyphra::shared_data.TexUvLines[integer_thickness + 1];
                    tex_uvs.x = tex_uvs.x + (tex_uvs_1.x - tex_uvs.x) * fractional_thickness; // inlined XyLerp()
                    tex_uvs.y = tex_uvs.y + (tex_uvs_1.y - tex_uvs.y) * fractional_thickness;
                    tex_uvs.z = tex_uvs.z + (tex_uvs_1.z - tex_uvs.z) * fractional_thickness;
                    tex_uvs.w = tex_uvs.w + (tex_uvs_1.w - tex_uvs.w) * fractional_thickness;
                }*/
                XyVec2 tex_uv0(tex_uvs.x, tex_uvs.y);
                XyVec2 tex_uv1(tex_uvs.z, tex_uvs.w);
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = temp_points[i * 2 + 0]; _VtxWritePtr[0].uv = tex_uv0; _VtxWritePtr[0].col = col; // Left-side outer edge
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 1]; _VtxWritePtr[1].uv = tex_uv1; _VtxWritePtr[1].col = col; // Right-side outer edge
                    _VtxWritePtr += 2;
                }
            }
            else
            {
                // If we're not using a texture, we need the center vertex as well
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = points[i];              _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col;       // Center of line
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 0]; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col_trans; // Left-side outer edge
                    _VtxWritePtr[2].pos = temp_points[i * 2 + 1]; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col_trans; // Right-side outer edge
                    _VtxWritePtr += 3;
                }
            }
        }
        else
        {
            // [PATH 2] Non texture-based lines (thick): we need to draw the solid line core and thus require four vertices per point
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;

            // If line is not closed, the first and last points need to be generated differently as there are no normals to blend
            if (!closed)
            {
                const int points_last = points_count - 1;
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 0] = points[points_last] + temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 1] = points[points_last] + temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 2] = points[points_last] - temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 3] = points[points_last] - temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
            }

            // Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
            // This takes points n and n+1 and writes into n+1, with the first point in a closed line being generated from the final one (as n+1 wraps)
            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx; // Vertex index for start of line segment
            for (int i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
            {
                const int i2 = (i1 + 1) == points_count ? 0 : (i1 + 1); // i2 is the second point of the line segment
                const unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : (idx1 + 4); // Vertex index for end of segment

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                XY_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                // Add temporary vertices
                XyVec2* out_vtx = &temp_points[i2 * 4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                // Add indexes
                _IdxWritePtr[0] = (XyIndex)(idx2 + 1); _IdxWritePtr[1] = (XyIndex)(idx1 + 1); _IdxWritePtr[2] = (XyIndex)(idx1 + 2);
                _IdxWritePtr[3] = (XyIndex)(idx1 + 2); _IdxWritePtr[4] = (XyIndex)(idx2 + 2); _IdxWritePtr[5] = (XyIndex)(idx2 + 1);
                _IdxWritePtr[6] = (XyIndex)(idx2 + 1); _IdxWritePtr[7] = (XyIndex)(idx1 + 1); _IdxWritePtr[8] = (XyIndex)(idx1 + 0);
                _IdxWritePtr[9] = (XyIndex)(idx1 + 0); _IdxWritePtr[10] = (XyIndex)(idx2 + 0); _IdxWritePtr[11] = (XyIndex)(idx2 + 1);
                _IdxWritePtr[12] = (XyIndex)(idx2 + 2); _IdxWritePtr[13] = (XyIndex)(idx1 + 2); _IdxWritePtr[14] = (XyIndex)(idx1 + 3);
                _IdxWritePtr[15] = (XyIndex)(idx1 + 3); _IdxWritePtr[16] = (XyIndex)(idx2 + 3); _IdxWritePtr[17] = (XyIndex)(idx2 + 2);
                _IdxWritePtr += 18;

                idx1 = idx2;
            }

            // Add vertices
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = temp_points[i * 4 + 0]; _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col_trans;
                _VtxWritePtr[1].pos = temp_points[i * 4 + 1]; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col;
                _VtxWritePtr[2].pos = temp_points[i * 4 + 2]; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col;
                _VtxWritePtr[3].pos = temp_points[i * 4 + 3]; _VtxWritePtr[3].uv = opaque_uv; _VtxWritePtr[3].col = col_trans;
                _VtxWritePtr += 4;
            }
        }
        _VtxCurrentIdx += (XyIndex)vtx_count;
    }
    else
    {
        // [PATH 4] Non texture-based, Non anti-aliased lines
        const int idx_count = count * 6;
        const int vtx_count = count * 4;    // FIXME-OPT: Not sharing edges
        PrimReserve(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            const XyVec2& p1 = points[i1];
            const XyVec2& p2 = points[i2];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            XY_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            _VtxWritePtr[0].pos.x = p1.x + dy; _VtxWritePtr[0].pos.y = p1.y - dx; _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = p2.x + dy; _VtxWritePtr[1].pos.y = p2.y - dx; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col;
            _VtxWritePtr[2].pos.x = p2.x - dy; _VtxWritePtr[2].pos.y = p2.y + dx; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col;
            _VtxWritePtr[3].pos.x = p1.x - dy; _VtxWritePtr[3].pos.y = p1.y + dx; _VtxWritePtr[3].uv = opaque_uv; _VtxWritePtr[3].col = col;
            _VtxWritePtr += 4;

            _IdxWritePtr[0] = (XyIndex)(_VtxCurrentIdx); _IdxWritePtr[1] = (XyIndex)(_VtxCurrentIdx + 1); _IdxWritePtr[2] = (XyIndex)(_VtxCurrentIdx + 2);
            _IdxWritePtr[3] = (XyIndex)(_VtxCurrentIdx); _IdxWritePtr[4] = (XyIndex)(_VtxCurrentIdx + 2); _IdxWritePtr[5] = (XyIndex)(_VtxCurrentIdx + 3);
            _IdxWritePtr += 6;
            _VtxCurrentIdx += 4;
        }
    }
}

XyColor _Xyphra::ColorConvertU32ToFloat4(XyU32 in)
{
    float s = 1.0f / 255.0f;
    return XyColor(
        (float)((in >> XY_COL32_R_SHIFT) & 0xFF) * s,
        (float)((in >> XY_COL32_G_SHIFT) & 0xFF) * s,
        (float)((in >> XY_COL32_B_SHIFT) & 0xFF) * s,
        (float)((in >> XY_COL32_A_SHIFT) & 0xFF) * s);
}

XyU32 _Xyphra::ColorConvertFloat4ToU32(const XyVec4& in)
{
    XyU32 out;
    out = ((XyU32)XY_F32_TO_INT8_SAT(in.x)) << XY_COL32_R_SHIFT;
    out |= ((XyU32)XY_F32_TO_INT8_SAT(in.y)) << XY_COL32_G_SHIFT;
    out |= ((XyU32)XY_F32_TO_INT8_SAT(in.z)) << XY_COL32_B_SHIFT;
    out |= ((XyU32)XY_F32_TO_INT8_SAT(in.w)) << XY_COL32_A_SHIFT;
    return out;
}

#define XyCmd_HeaderSize                            (XY_OFFSETOF(XyCmd, VtxOffset) + sizeof(unsigned int))
#define XyCmd_HeaderCompare(CMD_LHS, CMD_RHS)       (memcmp(CMD_LHS, CMD_RHS, XyCmd_HeaderSize))    // Compare ClipRect, TextureId, VtxOffset
#define XyCmd_HeaderCopy(CMD_DST, CMD_SRC)          (memcpy(CMD_DST, CMD_SRC, XyCmd_HeaderSize))    // Copy ClipRect, TextureId, VtxOffset
#define XyCmd_AreSequentialIdxOffset(CMD_0, CMD_1)  (CMD_0->IdxOffset + CMD_0->ElemCount == CMD_1->IdxOffset)
void _Xyphra::PushClipRect(const XyVec2& cr_min, const XyVec2& cr_max, bool intersect_with_current_clip_rect)
{
    XyVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
    if (intersect_with_current_clip_rect)
    {
        XyVec4 current = _CmdHeader.ClipRect;
        if (cr.x < current.x) cr.x = current.x;
        if (cr.y < current.y) cr.y = current.y;
        if (cr.z > current.z) cr.z = current.z;
        if (cr.w > current.w) cr.w = current.w;
    }
    cr.z = XyMax(cr.x, cr.z);
    cr.w = XyMax(cr.y, cr.w);

    _ClipRectStack.push_back(cr);
    _CmdHeader.ClipRect = cr;
    _OnChangedClipRect();
}

void _Xyphra::PopClipRect()
{
    _ClipRectStack.pop_back();
    _CmdHeader.ClipRect = (_ClipRectStack.Size == 0) ? shared_data.ClipRectFullscreen : _ClipRectStack.Data[_ClipRectStack.Size - 1];
    _OnChangedClipRect();
}

static inline XyDrawFlags FixRectCornerFlags(XyDrawFlags flags)
{
    // Obsoleted in 1.82 (from February 2021)
    // Legacy Support for hard coded ~0 (used to be a suggested equivalent to XyDrawCornerFlags_All)
    //   ~0   --> XyDrawFlags_RoundCornersAll or 0
    if (flags == ~0)
        return XyDrawFlags_RoundCornersAll;

    // Legacy Support for hard coded 0x01 to 0x0F (matching 15 out of 16 old flags combinations)
    //   0x01 --> XyDrawFlags_RoundCornersTopLeft (VALUE 0x01 OVERLAPS XyDrawFlags_Closed but XyDrawFlags_Closed is never valid in this path!)
    //   0x02 --> XyDrawFlags_RoundCornersTopRight
    //   0x03 --> XyDrawFlags_RoundCornersTopLeft | XyDrawFlags_RoundCornersTopRight
    //   0x04 --> XyDrawFlags_RoundCornersBotLeft
    //   0x05 --> XyDrawFlags_RoundCornersTopLeft | XyDrawFlags_RoundCornersBotLeft
    //   ...
    //   0x0F --> XyDrawFlags_RoundCornersAll or 0
    // (See all values in XyDrawCornerFlags_)
    if (flags >= 0x01 && flags <= 0x0F)
        return (flags << 4);

    // We cannot support hard coded 0x00 with 'float rounding > 0.0f' --> replace with XyDrawFlags_RoundCornersNone or use 'float rounding = 0.0f'

    // If this triggers, please update your code replacing hardcoded values with new XyDrawFlags_RoundCorners* values.
    // Note that XyDrawFlags_Closed (== 0x01) is an invalid flag for AddRect(), AddRectFilled(), PathRect() etc...
    XY_ASSERT((flags & 0x0F) == 0 && "Misuse of legacy hardcoded XyDrawCornerFlags values!");

    if ((flags & XyDrawFlags_RoundCornersMask_) == 0)
        flags |= XyDrawFlags_RoundCornersAll;

    return flags;
}

int _Xyphra::_CalcCircleAutoSegmentCount(float radius) const
{
    // Automatic segment count
    const int radius_idx = (int)(radius + 0.999999f); // ceil to never reduce accuracy
    if (radius_idx < XY_ARRAYSIZE(shared_data.CircleSegmentCounts))
        return shared_data.CircleSegmentCounts[radius_idx]; // Use cached value
    else
        return XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, shared_data.CircleSegmentMaxError);
}

static int ClipPolygonShape(XyVec2* src_points, int num_src_points, XyVec2* dest_points, int allocated_dest_points, XyVec2 clip_rect_min, XyVec2 clip_rect_max)
{
    // Early-out with an empty result if clipping region is zero-sized
    if (clip_rect_max.x <= clip_rect_min.x || clip_rect_max.y <= clip_rect_min.y)
        return 0;

    // Early-out if there is no source geometry
    if (num_src_points < 3)
        return 0;

    // The four clip planes here are indexed as:
    // 0 = X-, 1 = X+, 2 = Y-, 3 = Y+
    XyU8* outflags[2]; // Double-buffered flags for each vertex indicating which of the four clip planes it is outside of
    outflags[0] = (XyU8*)alloca(2 * allocated_dest_points * sizeof(XyU8));
    outflags[1] = outflags[0] + allocated_dest_points;

    // Calculate initial outflags
    XyU8 outflags_anded = 0xFF;
    XyU8 outflags_ored = 0;
    for (int point_idx = 0; point_idx < num_src_points; point_idx++)
    {
        const XyVec2 pos = src_points[point_idx];
        const XyU8 point_outflags = (pos.x < clip_rect_min.x ? 1 : 0) | (pos.x > clip_rect_max.x ? 2 : 0) | (pos.y < clip_rect_min.y ? 4 : 0) | (pos.y > clip_rect_max.y ? 8 : 0);
        outflags[0][point_idx] = point_outflags; // Writing to buffer 0
        outflags_anded &= point_outflags;
        outflags_ored |= point_outflags;
    }
    if (outflags_anded != 0) // Entirely clipped by any one plane, so nothing remains
        return 0;

    if (outflags_ored == 0) // Entirely within bounds, so trivial accept
    {
        if (allocated_dest_points < num_src_points)
            return -1; // Not sure what the caller was thinking if this happens, but we should handle it gracefully

        memcpy(dest_points, src_points, num_src_points * sizeof(XyVec2));
        return num_src_points;
    }

    // Shape needs clipping
    XyVec2* clip_buf[2]; // Double-buffered work area
    clip_buf[0] = (XyVec2*)alloca(2 * allocated_dest_points * sizeof(XyVec2)); //-V630
    clip_buf[1] = clip_buf[0] + allocated_dest_points;

    memcpy(clip_buf[0], src_points, num_src_points * sizeof(XyVec2));
    int clip_buf_size = num_src_points; // Number of vertices currently in the clip buffer

    int read_buffer_idx = 0; // The index of the clip buffer/out-flags we are reading (0 or 1)

    for (int clip_plane = 0; clip_plane < 4; clip_plane++) // 0 = X-, 1 = X+, 2 = Y-, 3 = Y+
    {
        const int clip_plane_bit = 1 << clip_plane; // Bit mask for our current plane in out-flags
        if ((outflags_ored & clip_plane_bit) == 0)
            continue; // All vertices are inside this plane, so no need to clip

        XyVec2* read_vert = &clip_buf[read_buffer_idx][0];              // Clip buffer vertex we are currently reading
        XyVec2* write_vert = &clip_buf[1 - read_buffer_idx][0];         // Clip buffer vertex we are currently writing
        XyVec2* write_vert_end = write_vert + allocated_dest_points;    // End of the write buffer
        XyU8* read_outflags = &outflags[read_buffer_idx][0];            // Out-flag we are currently reading
        XyU8* write_outflags = &outflags[1 - read_buffer_idx][0];       // Out-flag we are currently writing

        // Keep track of the last vertex visited, initially the last in the list
        XyVec2* last_vert = &read_vert[clip_buf_size - 1];
        XyU8 last_outflags = read_outflags[clip_buf_size - 1];

        for (int vert = 0; vert < clip_buf_size; vert++)
        {
            XyU8 current_outflags = *(read_outflags++);
            bool out = (current_outflags & clip_plane_bit) != 0;
            if (((current_outflags ^ last_outflags) & clip_plane_bit) == 0) // We haven't crossed the clip plane
            {
                if (!out)
                {
                    // Emit vertex as-is
                    if (write_vert >= write_vert_end)
                        return -1; // Ran out of buffer space, so abort
                    *(write_vert++) = *read_vert;
                    *(write_outflags++) = current_outflags;
                }
            }
            else
            {
                // Emit a vertex at the intersection point
                float t = 0.0f;
                XyVec2 pos0 = *last_vert;
                XyVec2 pos1 = *read_vert;
                XyVec2 intersect_pos;
                switch (clip_plane)
                {
                case 0: t = (clip_rect_min.x - pos0.x) / (pos1.x - pos0.x); intersect_pos = XyVec2(clip_rect_min.x, pos0.y + ((pos1.y - pos0.y) * t)); break; // X-
                case 1: t = (clip_rect_max.x - pos0.x) / (pos1.x - pos0.x); intersect_pos = XyVec2(clip_rect_max.x, pos0.y + ((pos1.y - pos0.y) * t)); break; // X+
                case 2: t = (clip_rect_min.y - pos0.y) / (pos1.y - pos0.y); intersect_pos = XyVec2(pos0.x + ((pos1.x - pos0.x) * t), clip_rect_min.y); break; // Y-
                case 3: t = (clip_rect_max.y - pos0.y) / (pos1.y - pos0.y); intersect_pos = XyVec2(pos0.x + ((pos1.x - pos0.x) * t), clip_rect_max.y); break; // Y+
                }

                if (write_vert >= write_vert_end)
                    return -1; // Ran out of buffer space, so abort

                // Write new out-flags for the vertex we just emitted
                *(write_vert++) = intersect_pos;
                *(write_outflags++) = (intersect_pos.x < clip_rect_min.x ? 1 : 0) | (intersect_pos.x > clip_rect_max.x ? 2 : 0) | (intersect_pos.y < clip_rect_min.y ? 4 : 0) | (intersect_pos.y > clip_rect_max.y ? 8 : 0);

                if (!out)
                {
                    // When coming back in, also emit the actual vertex
                    if (write_vert >= write_vert_end)
                        return -1; // Ran out of buffer space, so abort
                    *(write_vert++) = *read_vert;
                    *(write_outflags++) = current_outflags;
                }

                last_outflags = current_outflags;
            }

            last_vert = read_vert;
            read_vert++; // Advance to next vertex
        }

        clip_buf_size = (int)(write_vert - &clip_buf[1 - read_buffer_idx][0]); // Update buffer size
        read_buffer_idx = 1 - read_buffer_idx; // Swap buffers
    }

    if (clip_buf_size < 3)
        return 0; // Nothing to return

    // Copy results to output buffer, removing any redundant vertices
    int num_out_verts = 0;
    XyVec2 last_vert = clip_buf[read_buffer_idx][clip_buf_size - 1];
    for (int i = 0; i < clip_buf_size; i++)
    {
        XyVec2 vert = clip_buf[read_buffer_idx][i];
        if (XyLengthSqr(vert - last_vert) <= 0.00001f)
            continue;
        dest_points[num_out_verts++] = vert;
        last_vert = vert;
    }

    // Return size (IF this is still a valid shape)
    return (num_out_verts > 2) ? num_out_verts : 0;
}

void _Xyphra::AddSubtractedRect(const XyVec2& a_min, const XyVec2& a_max, const XyVec2& a_min_uv, const XyVec2& a_max_uv, XyVec2* b_points, int num_b_points, XyU32 col)
{
    // Early out without drawing anything if A is zero-size
    if (a_min.x >= a_max.x || a_min.y >= a_max.y)
        return;

    // First clip B to A
    const int max_clipped_points = num_b_points + 4;
    XyVec2* clipped_b_points = (XyVec2*)alloca(max_clipped_points * sizeof(XyVec2)); //-V630
    const int num_clipped_points = ClipPolygonShape(b_points, num_b_points, clipped_b_points, max_clipped_points, a_min, a_max);
    XY_ASSERT(num_clipped_points >= 0); // -1 would indicate max_clipped_points was too small, which shouldn't happen

    b_points = clipped_b_points;
    num_b_points = num_clipped_points;

    if (num_clipped_points == 0)
    {
        // B is entirely outside A, so just draw A as-is
        this->PrimReserve(6, 4);
        this->PrimRectUV(a_min, a_max, a_min_uv, a_max_uv, col);
    }
    else
    {
        // We need to generate clipped geometry
        // To do this we walk the inner polygon and connect each edge to one of the four corners of our rectangle based on the quadrant their normal points at
        const int max_verts = num_b_points + 4; // Inner points plus the four corners
        const int max_indices = (num_b_points * 3) + (4 * 3); // Worst case is one triangle per inner edge and then four filler triangles
        this->PrimReserve(max_indices, max_verts);

        XyIndex* idx_write = this->_IdxWritePtr;
        XyVertex* vtx_write = this->_VtxWritePtr;
        XyIndex inner_idx = (XyIndex)this->_VtxCurrentIdx; // Starting index for inner vertices

        // Write inner vertices
        const XyVec2 pos_to_uv_scale = (a_max_uv - a_min_uv) / (a_max - a_min); // Guaranteed never to be a /0 because we check for zero-size A above
        const XyVec2 pos_to_uv_offset = (a_min_uv / pos_to_uv_scale) - a_min;

        // Helper that generates an interpolated UV based on position
#define LERP_UV(x_pos, y_pos) (XyVec2(((x_pos) + pos_to_uv_offset.x) * pos_to_uv_scale.x, ((y_pos) + pos_to_uv_offset.y) * pos_to_uv_scale.y))
        for (int i = 0; i < num_b_points; i++)
        {
            vtx_write[i].pos = b_points[i];
            vtx_write[i].uv = LERP_UV(b_points[i].x, b_points[i].y);
            vtx_write[i].col = col;
        }
#undef LERP_UV

        vtx_write += num_b_points;

        // Write outer vertices
        XyIndex outer_idx = (XyIndex)(inner_idx + num_b_points); // Starting index for outer vertices

        XyVec2 outer_verts[4];
        outer_verts[0] = XyVec2(a_min.x, a_min.y); // X- Y- (quadrant 0, top left)
        outer_verts[1] = XyVec2(a_max.x, a_min.y); // X+ Y- (quadrant 1, top right)
        outer_verts[2] = XyVec2(a_max.x, a_max.y); // X+ Y+ (quadrant 2, bottom right)
        outer_verts[3] = XyVec2(a_min.x, a_max.y); // X- Y+ (quadrant 3, bottom left)

        vtx_write[0].pos = outer_verts[0]; vtx_write[0].uv = XyVec2(a_min_uv.x, a_min_uv.y); vtx_write[0].col = col;
        vtx_write[1].pos = outer_verts[1]; vtx_write[1].uv = XyVec2(a_max_uv.x, a_min_uv.y); vtx_write[1].col = col;
        vtx_write[2].pos = outer_verts[2]; vtx_write[2].uv = XyVec2(a_max_uv.x, a_max_uv.y); vtx_write[2].col = col;
        vtx_write[3].pos = outer_verts[3]; vtx_write[3].uv = XyVec2(a_min_uv.x, a_max_uv.y); vtx_write[3].col = col;

        this->_VtxCurrentIdx += num_b_points + 4;
        this->_VtxWritePtr += num_b_points + 4;

        // Now walk the inner vertices in order
        XyVec2 last_inner_vert = b_points[num_b_points - 1];
        int last_inner_vert_idx = num_b_points - 1;
        int last_outer_vert_idx = -1;
        int first_outer_vert_idx = -1;

        // Triangle-area based check for degenerate triangles
        // Min area (0.1f) is doubled (* 2.0f) because we're calculating (area * 2) here
#define IS_DEGENERATE(a, b, c) (XyFabs((((a).x * ((b).y - (c).y)) + ((b).x * ((c).y - (a).y)) + ((c).x * ((a).y - (b).y)))) < (0.1f * 2.0f))

        // Check the winding order of the inner vertices using the sign of the triangle area, and set the outer vertex winding to match
        int outer_vertex_winding = (((b_points[0].x * (b_points[1].y - b_points[2].y)) + (b_points[1].x * (b_points[2].y - b_points[0].y)) + (b_points[2].x * (b_points[0].y - b_points[1].y))) < 0.0f) ? -1 : 1;
        for (int inner_vert_idx = 0; inner_vert_idx < num_b_points; inner_vert_idx++)
        {
            XyVec2 current_inner_vert = b_points[inner_vert_idx];

            // Calculate normal (not actually normalized, as for our purposes here it doesn't need to be)
            XyVec2 normal(current_inner_vert.y - last_inner_vert.y, -(current_inner_vert.x - last_inner_vert.x));

            // Calculate the outer vertex index based on the quadrant the normal points at (0=top left, 1=top right, 2=bottom right, 3=bottom left)
            int outer_vert_idx = (XyFabs(normal.x) > XyFabs(normal.y)) ? ((normal.x >= 0.0f) ? ((normal.y > 0.0f) ? 2 : 1) : ((normal.y > 0.0f) ? 3 : 0)) : ((normal.y >= 0.0f) ? ((normal.x > 0.0f) ? 2 : 3) : ((normal.x > 0.0f) ? 1 : 0));
            XyVec2 outer_vert = outer_verts[outer_vert_idx];

            // Write the main triangle (connecting the inner edge to the corner)
            if (!IS_DEGENERATE(last_inner_vert, current_inner_vert, outer_vert))
            {
                idx_write[0] = (XyIndex)(inner_idx + last_inner_vert_idx);
                idx_write[1] = (XyIndex)(inner_idx + inner_vert_idx);
                idx_write[2] = (XyIndex)(outer_idx + outer_vert_idx);
                idx_write += 3;
            }

            // We don't initially know which outer vertex we are going to start from, so set that here when processing the first inner vertex
            if (first_outer_vert_idx == -1)
            {
                first_outer_vert_idx = outer_vert_idx;
                last_outer_vert_idx = outer_vert_idx;
            }

            // Now walk the outer edge and write any filler triangles needed (connecting outer edges to the inner vertex)
            while (outer_vert_idx != last_outer_vert_idx)
            {
                int next_outer_vert_idx = (last_outer_vert_idx + outer_vertex_winding) & 3;
                if (!IS_DEGENERATE(outer_verts[last_outer_vert_idx], outer_verts[next_outer_vert_idx], last_inner_vert))
                {
                    idx_write[0] = (XyIndex)(outer_idx + last_outer_vert_idx);
                    idx_write[1] = (XyIndex)(outer_idx + next_outer_vert_idx);
                    idx_write[2] = (XyIndex)(inner_idx + last_inner_vert_idx);
                    idx_write += 3;
                }
                last_outer_vert_idx = next_outer_vert_idx;
            }

            last_inner_vert = current_inner_vert;
            last_inner_vert_idx = inner_vert_idx;
        }

        // Write remaining filler triangles for any un-traversed outer edges
        if (first_outer_vert_idx != -1)
        {
            while (first_outer_vert_idx != last_outer_vert_idx)
            {
                int next_outer_vert_idx = (last_outer_vert_idx + outer_vertex_winding) & 3;
                if (!IS_DEGENERATE(outer_verts[last_outer_vert_idx], outer_verts[next_outer_vert_idx], last_inner_vert))
                {
                    idx_write[0] = (XyIndex)(outer_idx + last_outer_vert_idx);
                    idx_write[1] = (XyIndex)(outer_idx + next_outer_vert_idx);
                    idx_write[2] = (XyIndex)(inner_idx + last_inner_vert_idx);
                    idx_write += 3;
                }
                last_outer_vert_idx = next_outer_vert_idx;
            }
        }
#undef IS_DEGENERATE

        int used_indices = (int)(idx_write - this->_IdxWritePtr);
        this->_IdxWritePtr = idx_write;
        this->PrimUnreserve(max_indices - used_indices, 0);
    }
}

void _Xyphra::AddSubtractedRect(const XyVec2& a_min, const XyVec2& a_max, const XyVec2& a_min_uv, const XyVec2& a_max_uv, XyVec2 b_min, XyVec2 b_max, XyU32 col)
{
    // Early out without drawing anything if A is zero-size
    if (a_min.x >= a_max.x || a_min.y >= a_max.y)
        return;

    // Early out without drawing anything if B covers A entirely
    if (a_min.x >= b_min.x && a_max.x <= b_max.x && a_min.y >= b_min.y && a_max.y <= b_max.y)
        return;

    // First clip the extents of B to A
    b_min = XyMax(b_min, a_min);
    b_max = XyMin(b_max, a_max);
    if (b_min.x >= b_max.x || b_min.y >= b_max.y)
    {
        // B is entirely outside A, so just draw A as-is
        this->PrimReserve(6, 4);
        this->PrimRectUV(a_min, a_max, a_min_uv, a_max_uv, col);
        return;
    }

    // Otherwise we need to emit (up to) four quads to cover the visible area...
    // Our layout looks like this (numbers are vertex indices, letters are quads):
    //
    // 0---8------9-----1
    // |   |  B   |     |
    // +   4------5     +
    // | A |xxxxxx|  C  |
    // |   |xxxxxx|     |
    // +   7------6     +
    // |   |  D   |     |
    // 3---11-----10----2

    const int max_verts = 12;
    const int max_indices = 6 * 4; // At most four quads
    this->PrimReserve(max_indices, max_verts);

    XyIndex* idx_write = this->_IdxWritePtr;
    XyVertex* vtx_write = this->_VtxWritePtr;
    XyIndex idx = (XyIndex)this->_VtxCurrentIdx;

    // Write vertices
    vtx_write[0].pos = XyVec2(a_min.x, a_min.y); vtx_write[0].uv = XyVec2(a_min_uv.x, a_min_uv.y); vtx_write[0].col = col;
    vtx_write[1].pos = XyVec2(a_max.x, a_min.y); vtx_write[1].uv = XyVec2(a_max_uv.x, a_min_uv.y); vtx_write[1].col = col;
    vtx_write[2].pos = XyVec2(a_max.x, a_max.y); vtx_write[2].uv = XyVec2(a_max_uv.x, a_max_uv.y); vtx_write[2].col = col;
    vtx_write[3].pos = XyVec2(a_min.x, a_max.y); vtx_write[3].uv = XyVec2(a_min_uv.x, a_max_uv.y); vtx_write[3].col = col;

    const XyVec2 pos_to_uv_scale = (a_max_uv - a_min_uv) / (a_max - a_min); // Guaranteed never to be a /0 because we check for zero-size A above
    const XyVec2 pos_to_uv_offset = (a_min_uv / pos_to_uv_scale) - a_min;

    // Helper that generates an interpolated UV based on position
#define LERP_UV(x_pos, y_pos) (XyVec2(((x_pos) + pos_to_uv_offset.x) * pos_to_uv_scale.x, ((y_pos) + pos_to_uv_offset.y) * pos_to_uv_scale.y))
    vtx_write[4].pos = XyVec2(b_min.x, b_min.y); vtx_write[4].uv = LERP_UV(b_min.x, b_min.y); vtx_write[4].col = col;
    vtx_write[5].pos = XyVec2(b_max.x, b_min.y); vtx_write[5].uv = LERP_UV(b_max.x, b_min.y); vtx_write[5].col = col;
    vtx_write[6].pos = XyVec2(b_max.x, b_max.y); vtx_write[6].uv = LERP_UV(b_max.x, b_max.y); vtx_write[6].col = col;
    vtx_write[7].pos = XyVec2(b_min.x, b_max.y); vtx_write[7].uv = LERP_UV(b_min.x, b_max.y); vtx_write[7].col = col;
    vtx_write[8].pos = XyVec2(b_min.x, a_min.y); vtx_write[8].uv = LERP_UV(b_min.x, a_min.y); vtx_write[8].col = col;
    vtx_write[9].pos = XyVec2(b_max.x, a_min.y); vtx_write[9].uv = LERP_UV(b_max.x, a_min.y); vtx_write[9].col = col;
    vtx_write[10].pos = XyVec2(b_max.x, a_max.y); vtx_write[10].uv = LERP_UV(b_max.x, a_max.y); vtx_write[10].col = col;
    vtx_write[11].pos = XyVec2(b_min.x, a_max.y); vtx_write[11].uv = LERP_UV(b_min.x, a_max.y); vtx_write[11].col = col;
#undef LERP_UV
    this->_VtxWritePtr += 12;
    this->_VtxCurrentIdx += 12;

    // Write indices for each quad (if it is visible)
    if (b_min.x > a_min.x) // A
    {
        idx_write[0] = (XyIndex)(idx + 0); idx_write[1] = (XyIndex)(idx + 8); idx_write[2] = (XyIndex)(idx + 11);
        idx_write[3] = (XyIndex)(idx + 0); idx_write[4] = (XyIndex)(idx + 11); idx_write[5] = (XyIndex)(idx + 3);
        idx_write += 6;
    }
    if (b_min.y > a_min.y) // B
    {
        idx_write[0] = (XyIndex)(idx + 8); idx_write[1] = (XyIndex)(idx + 9); idx_write[2] = (XyIndex)(idx + 5);
        idx_write[3] = (XyIndex)(idx + 8); idx_write[4] = (XyIndex)(idx + 5); idx_write[5] = (XyIndex)(idx + 4);
        idx_write += 6;
    }
    if (a_max.x > b_max.x) // C
    {
        idx_write[0] = (XyIndex)(idx + 9); idx_write[1] = (XyIndex)(idx + 1); idx_write[2] = (XyIndex)(idx + 2);
        idx_write[3] = (XyIndex)(idx + 9); idx_write[4] = (XyIndex)(idx + 2); idx_write[5] = (XyIndex)(idx + 10);
        idx_write += 6;
    }
    if (a_max.y > b_max.y) // D
    {
        idx_write[0] = (XyIndex)(idx + 7); idx_write[1] = (XyIndex)(idx + 6); idx_write[2] = (XyIndex)(idx + 10);
        idx_write[3] = (XyIndex)(idx + 7); idx_write[4] = (XyIndex)(idx + 10); idx_write[5] = (XyIndex)(idx + 11);
        idx_write += 6;
    }

    const int used_indices = (int)(idx_write - this->_IdxWritePtr);
    this->_IdxWritePtr = idx_write;
    this->PrimUnreserve(max_indices - used_indices, 0);
}

void _Xyphra::AddShadowRect(const XyVec2& obj_min, const XyVec2& obj_max, XyU32 shadow_col, float obj_rounding, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags)
{
    if ((shadow_col & XY_COL32_A_MASK) == 0)
        return;


    PathRect(obj_min, obj_max, obj_rounding, flags);
    AddShadowConvexPoly(_Path.Data, _Path.Size, shadow_col, shadow_thickness, shadow_offset, flags);
    _Path.Size = 0;
    return;

    XyVec2* inner_rect_points = NULL; // Points that make up the shape of the inner rectangle (used when it has rounded corners)
    int inner_rect_points_count = 0;

    // Generate a path describing the inner rectangle and copy it to our buffer
    const bool is_filled = (flags & XyDrawFlags_ShadowCutOutShapeBackground) == 0;
    const bool is_rounded = (obj_rounding > 0.0f) && ((flags & XyDrawFlags_RoundCornersMask_) != XyDrawFlags_RoundCornersNone); // Do we have rounded corners?
    if (is_rounded && !is_filled)
    {
        XY_ASSERT(_Path.Size == 0);
        PathRect(obj_min, obj_max, obj_rounding, flags);
        inner_rect_points_count = _Path.Size;
        inner_rect_points = (XyVec2*)alloca(inner_rect_points_count * sizeof(XyVec2)); //-V630
        memcpy(inner_rect_points, _Path.Data, inner_rect_points_count * sizeof(XyVec2));
        _Path.Size = 0;
    }

    if (is_filled)
        PrimReserve(6 * 9, 4 * 9); // Reserve space for adding unclipped chunks

    // Draw the relevant chunks of the texture (the texture is split into a 3x3 grid)
    // FIXME-OPT: Might make sense to optimize/unroll for the fast paths (filled or not rounded)
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            const int uv_index = x + (y + y + y); // y*3 formatted so as to ensure the compiler avoids an actual multiply
            const XyVec4 uvs = shared_data.ShadowRectUvs[uv_index];

            XyVec2 draw_min, draw_max;
            switch (x)
            {
            case 0: draw_min.x = obj_min.x - shadow_thickness; draw_max.x = obj_min.x; break;
            case 1: draw_min.x = obj_min.x; draw_max.x = obj_max.x; break;
            case 2: draw_min.x = obj_max.x; draw_max.x = obj_max.x + shadow_thickness; break;
            }
            switch (y)
            {
            case 0: draw_min.y = obj_min.y - shadow_thickness; draw_max.y = obj_min.y; break;
            case 1: draw_min.y = obj_min.y; draw_max.y = obj_max.y; break;
            case 2: draw_min.y = obj_max.y; draw_max.y = obj_max.y + shadow_thickness; break;
            }

            XyVec2 uv_min(uvs.x, uvs.y);
            XyVec2 uv_max(uvs.z, uvs.w);
            if (is_filled)
                PrimRectUV(draw_min + shadow_offset, draw_max + shadow_offset, uv_min, uv_max, shadow_col); // No clipping path (draw entire shadow)
            else if (is_rounded)
                AddSubtractedRect(draw_min + shadow_offset, draw_max + shadow_offset, uv_min, uv_max, inner_rect_points, inner_rect_points_count, shadow_col); // Complex path for rounded rectangles
            else
                AddSubtractedRect(draw_min + shadow_offset, draw_max + shadow_offset, uv_min, uv_max, obj_min, obj_max, shadow_col); // Simple fast path for non-rounded rectangles
        }
    }
}

void _Xyphra::AddShadowCircle(const XyVec2& obj_center, float obj_radius, XyU32 shadow_col, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags, int num_segments)
{
    // Obtain segment count
    if (num_segments <= 0)
    {
        // Automatic segment count
        const int radius_idx = (int)obj_radius - 1;
        if (radius_idx < XY_ARRAYSIZE(shared_data.CircleSegmentCounts))
            num_segments = shared_data.CircleSegmentCounts[radius_idx]; // Use cached value
        else
            num_segments = XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(obj_radius, shared_data.CircleSegmentMaxError);
    }
    else
    {
        // Explicit segment count (still clamp to avoid drawing insanely tessellated shapes)
        num_segments = XyClamp(num_segments, 3, XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);
    }

    // Generate a path describing the inner circle and copy it to our buffer
    XY_ASSERT(_Path.Size == 0);
    const float a_max = (XY_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    if (num_segments == 12)
        PathArcToFast(obj_center, obj_radius, 0, 12 - 1);
    else
        PathArcTo(obj_center, obj_radius, 0.0f, a_max, num_segments - 1);

    // Draw the shadow using the convex shape code
    AddShadowConvexPoly(_Path.Data, _Path.Size, shadow_col, shadow_thickness, shadow_offset, flags);
    _Path.Size = 0;
}

void _Xyphra::AddShadowConvexPoly(const XyVec2* points, int points_count, XyU32 shadow_col, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags)
{
    const bool is_filled = (flags & XyDrawFlags_ShadowCutOutShapeBackground) == 0;
    XY_ASSERT((is_filled || (XyLengthSqr(shadow_offset) < 0.00001f)) && "Drawing circle/convex shape shadows with no center fill and an offset is not currently supported");
    XY_ASSERT(points_count >= 3);

    // Calculate poly vertex order
    const int vertex_winding = (((points[0].x * (points[1].y - points[2].y)) + (points[1].x * (points[2].y - points[0].y)) + (points[2].x * (points[0].y - points[1].y))) < 0.0f) ? -1 : 1;

    // If we're using anti-aliasing, then inset the shadow by 0.5 pixels to avoid unpleasant fringing artifacts
    const bool use_inset_distance = (Flags & XyDrawListFlags_AntiAliasedFill) && (!is_filled);
    const float inset_distance = 0.5f;

    const XyVec4 uvs = shared_data.ShadowRectUvs[9];

    int tex_width = 4; //shared_data.Font->ContainerAtlas->TexWidth;
    int tex_height = 4;// shared_data.Font->ContainerAtlas->TexHeight;
    float inv_tex_width = 1.0f / (float)tex_width;
    float inv_tex_height = 1.0f / (float)tex_height;

    XyVec2 solid_uv = XyVec2(uvs.z, uvs.w); // UV at the inside of an edge
    XyVec2 edge_uv = XyVec2(uvs.x, uvs.w); // UV at the outside of an edge

    XyVec2 solid_to_edge_delta_texels = edge_uv - solid_uv; // Delta between the solid/edge points in texel-space (we need this in pixels - or, to be more precise, to be at a 1:1 aspect ratio - for the rotation to work)
    solid_to_edge_delta_texels.x *= (float)tex_width;
    solid_to_edge_delta_texels.y *= (float)tex_height;

    // Our basic algorithm here is that we generate a straight section along each edge, and then either one or two curved corner triangles at the corners,
    // which use an appropriate chunk of the texture to generate a smooth curve.
    const int num_edges = points_count;

    // Normalize a vector
#define NORMALIZE(vec) ((vec) / XyLength((vec), 0.001f))

    const int required_stack_mem = (num_edges * sizeof(XyVec2)) + (num_edges * sizeof(float));
    XyU8* base_mem_for_normals_and_edges = (XyU8*)alloca(required_stack_mem);
    XyU8* mem_for_normals_and_edges = (XyU8*)base_mem_for_normals_and_edges;

    // Calculate edge normals
    XyVec2* edge_normals = (XyVec2*)(void*)mem_for_normals_and_edges;
    mem_for_normals_and_edges += num_edges * sizeof(XyVec2);

    for (int edge_index = 0; edge_index < num_edges; edge_index++)
    {
        XyVec2 edge_start = points[edge_index]; // No need to apply offset here because the normal is unaffected
        XyVec2 edge_end = points[(edge_index + 1) % num_edges];
        XyVec2 edge_normal = NORMALIZE(XyVec2(edge_end.y - edge_start.y, -(edge_end.x - edge_start.x)));
        edge_normals[edge_index] = edge_normal * (float)vertex_winding; // Flip normals for reverse winding
    }

    // Pre-calculate edge scales
    // We need to do this because we need the edge strips to have widths that match up with the corner sections, otherwise pixel cracking can occur along the boundaries
    float* edge_size_scales = (float*)(void*)mem_for_normals_and_edges;
    mem_for_normals_and_edges += num_edges * sizeof(float);

    {
        XyVec2 prev_edge_normal = edge_normals[num_edges - 1];
        for (int edge_index = 0; edge_index < num_edges; edge_index++)
        {
            XyVec2 edge_normal = edge_normals[edge_index];
            float cos_angle_coverage = XyDot(edge_normal, prev_edge_normal);

            if (cos_angle_coverage < 0.999999f)
            {
                // If we are covering more than 90 degrees we need an intermediate vertex to stop the required expansion tending towards infinity.
                // And thus the effective angle will be halved (matches the similar code in loop below)
                float angle_coverage = XyAcos(cos_angle_coverage);
                if (cos_angle_coverage <= 0.0f) // -V1051
                    angle_coverage *= 0.5f;
                edge_size_scales[edge_index] = 1.0f / XyCos(angle_coverage * 0.5f); // How much we need to expand our size by to avoid clipping the corner of the texture off
            }
            else
            {
                edge_size_scales[edge_index] = 1.0f; // No corner, thus default scale
            }

            prev_edge_normal = edge_normal;
        }
    }

    const int max_vertices = (4 + (3 * 2) + (is_filled ? 1 : 0)) * num_edges; // 4 vertices per edge plus 3*2 for potentially two corner triangles, plus one per vertex for fill
    const int max_indices = ((6 + (3 * 2)) * num_edges) + (is_filled ? ((num_edges - 2) * 3) : 0); // 2 tris per edge plus up to two corner triangles, plus fill triangles
    PrimReserve(max_indices, max_vertices);
    XyIndex* idx_write = _IdxWritePtr;
    XyVertex* vtx_write = _VtxWritePtr;
    XyIndex current_idx = (XyIndex)_VtxCurrentIdx;

    //XyVec2 previous_edge_start = points[0] + offset;
    XyVec2 prev_edge_normal = edge_normals[num_edges - 1];
    XyVec2 edge_start = points[0] + shadow_offset;

    if (use_inset_distance)
        edge_start -= NORMALIZE(edge_normals[0] + prev_edge_normal) * inset_distance;

    for (int edge_index = 0; edge_index < num_edges; edge_index++)
    {
        XyVec2 edge_end = points[(edge_index + 1) % num_edges] + shadow_offset;
        XyVec2 edge_normal = edge_normals[edge_index];
        const float size_scale_start = edge_size_scales[edge_index];
        const float size_scale_end = edge_size_scales[(edge_index + 1) % num_edges];

        if (use_inset_distance)
            edge_end -= NORMALIZE(edge_normals[(edge_index + 1) % num_edges] + edge_normal) * inset_distance;

        // Add corner section
        float cos_angle_coverage = XyDot(edge_normal, prev_edge_normal);
        if (cos_angle_coverage < 0.999999f) // Don't fill if the corner is actually straight
        {
            // If we are covering more than 90 degrees we need an intermediate vertex to stop the required expansion tending towards infinity.
            // And thus the effective angle has been halved (matches the similar code in loop above)
            int num_steps = (cos_angle_coverage <= 0.0f) ? 2 : 1;

            for (int step = 0; step < num_steps; step++)
            {
                if (num_steps > 1)
                {
                    if (step == 0)
                        edge_normal = NORMALIZE(edge_normal + prev_edge_normal); // Use half-way normal for first step
                    else
                        edge_normal = edge_normals[edge_index]; // Then use the "real" next edge normal for the second

                    cos_angle_coverage = XyDot(edge_normal, prev_edge_normal); // Recalculate angle
                }

                // Calculate UV for the section of the curved texture

                const float angle_coverage = XyAcos(cos_angle_coverage);
                const float sin_angle_coverage = XySin(angle_coverage);

                XyVec2 edge_delta = solid_to_edge_delta_texels;
                edge_delta *= size_scale_start;

                XyVec2 rotated_edge_delta = XyVec2((edge_delta.x * cos_angle_coverage) + (edge_delta.y * sin_angle_coverage), (edge_delta.x * sin_angle_coverage) + (edge_delta.y * cos_angle_coverage));

                // Convert from texels back into UV space
                edge_delta.x *= inv_tex_width;
                edge_delta.y *= inv_tex_height;
                rotated_edge_delta.x *= inv_tex_width;
                rotated_edge_delta.y *= inv_tex_height;

                XyVec2 expanded_edge_uv = solid_uv + edge_delta;
                XyVec2 other_edge_uv = solid_uv + rotated_edge_delta; // Rotated UV to encompass the necessary section of the curve

                float expanded_thickness = shadow_thickness * size_scale_start;

                // Add a triangle to fill the corner
                XyVec2 outer_edge_start = edge_start + (prev_edge_normal * expanded_thickness);
                XyVec2 outer_edge_end = edge_start + (edge_normal * expanded_thickness);

                vtx_write->pos = edge_start; vtx_write->col = shadow_col; vtx_write->uv = solid_uv; vtx_write++;
                vtx_write->pos = outer_edge_end;   vtx_write->col = shadow_col; vtx_write->uv = expanded_edge_uv; vtx_write++;
                vtx_write->pos = outer_edge_start; vtx_write->col = shadow_col; vtx_write->uv = other_edge_uv; vtx_write++;

                *(idx_write++) = current_idx;
                *(idx_write++) = current_idx + 1;
                *(idx_write++) = current_idx + 2;
                current_idx += 3;

                prev_edge_normal = edge_normal;
            }
        }

        // Add section along edge
        const float edge_length = XyLength(edge_end - edge_start, 0.0f);
        if (edge_length > 0.00001f) // Don't try and process degenerate edges
        {
            XyVec2 outer_edge_start = edge_start + (edge_normal * shadow_thickness * size_scale_start);
            XyVec2 outer_edge_end = edge_end + (edge_normal * shadow_thickness * size_scale_end);
            XyVec2 scaled_edge_uv_start = solid_uv + ((edge_uv - solid_uv) * size_scale_start);
            XyVec2 scaled_edge_uv_end = solid_uv + ((edge_uv - solid_uv) * size_scale_end);

            // Write vertices, inner first, then outer
            vtx_write->pos = edge_start; vtx_write->col = shadow_col; vtx_write->uv = solid_uv; vtx_write++;
            vtx_write->pos = edge_end; vtx_write->col = shadow_col; vtx_write->uv = solid_uv; vtx_write++;
            vtx_write->pos = outer_edge_end; vtx_write->col = shadow_col; vtx_write->uv = scaled_edge_uv_end; vtx_write++;
            vtx_write->pos = outer_edge_start; vtx_write->col = shadow_col; vtx_write->uv = scaled_edge_uv_start; vtx_write++;

            *(idx_write++) = current_idx;
            *(idx_write++) = current_idx + 1;
            *(idx_write++) = current_idx + 2;
            *(idx_write++) = current_idx;
            *(idx_write++) = current_idx + 2;
            *(idx_write++) = current_idx + 3;
            current_idx += 4;
        }

        edge_start = edge_end;
    }

    // Fill if requested
    if (is_filled)
    {
        // Add vertices
        for (int edge_index = 0; edge_index < num_edges; edge_index++)
        {
            vtx_write->pos = points[edge_index] + shadow_offset;
            vtx_write->col = shadow_col;
            vtx_write->uv = solid_uv;
            vtx_write++;
        }

        // Add triangles
        for (int edge_index = 2; edge_index < num_edges; edge_index++)
        {
            *(idx_write++) = current_idx;
            *(idx_write++) = (XyIndex)(current_idx + edge_index - 1);
            *(idx_write++) = (XyIndex)(current_idx + edge_index);
        }

        current_idx += (XyIndex)num_edges;
    }

    // Release any unused vertices/indices
    int used_indices = (int)(idx_write - _IdxWritePtr);
    int used_vertices = (int)(vtx_write - _VtxWritePtr);
    _IdxWritePtr = idx_write;
    _VtxWritePtr = vtx_write;
    _VtxCurrentIdx = current_idx;
    PrimUnreserve(max_indices - used_indices, max_vertices - used_vertices);
#undef NORMALIZE
}

void _Xyphra::AddShadowNGon(const XyVec2& obj_center, float obj_radius, XyU32 shadow_col, float shadow_thickness, const XyVec2& shadow_offset, XyDrawFlags flags, int obj_num_segments)
{
    XY_ASSERT(obj_num_segments != 0);
    AddShadowCircle(obj_center, obj_radius, shadow_col, shadow_thickness, shadow_offset, flags, obj_num_segments);
}

static XyVec2 XyBezierQuadraticCalc(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, float t)
{
    float u = 1.0f - t;
    float w1 = u * u;
    float w2 = 2 * u * t;
    float w3 = t * t;
    return XyVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x, w1 * p1.y + w2 * p2.y + w3 * p3.y);
}

static void PathBezierCubicCurveToCasteljau(XyVector<XyVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
{
    float dx = x4 - x1;
    float dy = y4 - y1;
    float d2 = (x2 - x4) * dy - (y2 - y4) * dx;
    float d3 = (x3 - x4) * dy - (y3 - y4) * dx;
    d2 = (d2 >= 0) ? d2 : -d2;
    d3 = (d3 >= 0) ? d3 : -d3;
    if ((d2 + d3) * (d2 + d3) < tess_tol * (dx * dx + dy * dy))
    {
        path->push_back(XyVec2(x4, y4));
    }
    else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x34 = (x3 + x4) * 0.5f, y34 = (y3 + y4) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
        float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
        PathBezierCubicCurveToCasteljau(path, x1, y1, x12, y12, x123, y123, x1234, y1234, tess_tol, level + 1);
        PathBezierCubicCurveToCasteljau(path, x1234, y1234, x234, y234, x34, y34, x4, y4, tess_tol, level + 1);
    }
}


static void PathBezierQuadraticCurveToCasteljau(XyVector<XyVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float tess_tol, int level)
{
    float dx = x3 - x1, dy = y3 - y1;
    float det = (x2 - x3) * dy - (y2 - y3) * dx;
    if (det * det * 4.0f < tess_tol * (dx * dx + dy * dy))
    {
        path->push_back(XyVec2(x3, y3));
    }
    else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        PathBezierQuadraticCurveToCasteljau(path, x1, y1, x12, y12, x123, y123, tess_tol, level + 1);
        PathBezierQuadraticCurveToCasteljau(path, x123, y123, x23, y23, x3, y3, tess_tol, level + 1);
    }
}


static XyVec2 XyBezierCubicCalc(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, float t)
{
    float u = 1.0f - t;
    float w1 = u * u * u;
    float w2 = 3 * u * u * t;
    float w3 = 3 * u * t * t;
    float w4 = t * t * t;
    return XyVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x, w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y);
}

void _Xyphra::_PathArcToFastEx(const XyVec2& center, float radius, int a_min_sample, int a_max_sample, int a_step)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }

    // Calculate arc auto segment step size
    if (a_step <= 0)
        a_step = XY_DRAWLIST_ARCFAST_SAMPLE_MAX / _CalcCircleAutoSegmentCount(radius);

    // Make sure we never do steps larger than one quarter of the circle
    a_step = XyClamp(a_step, 1, XY_DRAWLIST_ARCFAST_TABLE_SIZE / 4);

    const int sample_range = XyAbs(a_max_sample - a_min_sample);
    const int a_next_step = a_step;

    int samples = sample_range + 1;
    bool extra_max_sample = false;
    if (a_step > 1)
    {
        samples = sample_range / a_step + 1;
        const int overstep = sample_range % a_step;

        if (overstep > 0)
        {
            extra_max_sample = true;
            samples++;

            // When we have overstep to avoid awkwardly looking one long line and one tiny one at the end,
            // distribute first step range evenly between them by reducing first step size.
            if (sample_range > 0)
                a_step -= (a_step - overstep) / 2;
        }
    }

    _Path.resize(_Path.Size + samples);
    XyVec2* out_ptr = _Path.Data + (_Path.Size - samples);

    int sample_index = a_min_sample;
    if (sample_index < 0 || sample_index >= XY_DRAWLIST_ARCFAST_SAMPLE_MAX)
    {
        sample_index = sample_index % XY_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (sample_index < 0)
            sample_index += XY_DRAWLIST_ARCFAST_SAMPLE_MAX;
    }

    if (a_max_sample >= a_min_sample)
    {
        for (int a = a_min_sample; a <= a_max_sample; a += a_step, sample_index += a_step, a_step = a_next_step)
        {
            // a_step is clamped to XY_DRAWLIST_ARCFAST_SAMPLE_MAX, so we have guaranteed that it will not wrap over range twice or more
            if (sample_index >= XY_DRAWLIST_ARCFAST_SAMPLE_MAX)
                sample_index -= XY_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const XyVec2 s = shared_data.ArcFastVtx[sample_index];
            out_ptr->x = center.x + s.x * radius;
            out_ptr->y = center.y + s.y * radius;
            out_ptr++;
        }
    }
    else
    {
        for (int a = a_min_sample; a >= a_max_sample; a -= a_step, sample_index -= a_step, a_step = a_next_step)
        {
            // a_step is clamped to XY_DRAWLIST_ARCFAST_SAMPLE_MAX, so we have guaranteed that it will not wrap over range twice or more
            if (sample_index < 0)
                sample_index += XY_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const XyVec2 s = shared_data.ArcFastVtx[sample_index];
            out_ptr->x = center.x + s.x * radius;
            out_ptr->y = center.y + s.y * radius;
            out_ptr++;
        }
    }

    if (extra_max_sample)
    {
        int normalized_max_sample = a_max_sample % XY_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (normalized_max_sample < 0)
            normalized_max_sample += XY_DRAWLIST_ARCFAST_SAMPLE_MAX;

        const XyVec2 s = shared_data.ArcFastVtx[normalized_max_sample];
        out_ptr->x = center.x + s.x * radius;
        out_ptr->y = center.y + s.y * radius;
        out_ptr++;
    }
}


void _Xyphra::_PathArcToN(const XyVec2& center, float radius, float a_min, float a_max, int num_segments)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }

    // Note that we are adding a point at both a_min and a_max.
    // If you are trying to draw a full closed circle you don't want the overlapping points!
    _Path.reserve(_Path.Size + (num_segments + 1));
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        _Path.push_back(XyVec2(center.x + XyCos(a) * radius, center.y + XySin(a) * radius));
    }
}

void _Xyphra::PathArcTo(const XyVec2& center, float radius, float a_min, float a_max, int num_segments)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }

    if (num_segments > 0)
    {
        _PathArcToN(center, radius, a_min, a_max, num_segments);
        return;
    }

    // Automatic segment count
    if (radius <= shared_data.ArcFastRadiusCutoff)
    {
        const bool a_is_reverse = a_max < a_min;

        // We are going to use precomputed values for mid samples.
        // Determine first and last sample in lookup table that belong to the arc.
        const float a_min_sample_f = XY_DRAWLIST_ARCFAST_SAMPLE_MAX * a_min / (XY_PI * 2.0f);
        const float a_max_sample_f = XY_DRAWLIST_ARCFAST_SAMPLE_MAX * a_max / (XY_PI * 2.0f);

        const int a_min_sample = a_is_reverse ? (int)XyFloorSigned(a_min_sample_f) : (int)XyCeil(a_min_sample_f);
        const int a_max_sample = a_is_reverse ? (int)XyCeil(a_max_sample_f) : (int)XyFloorSigned(a_max_sample_f);
        const int a_mid_samples = a_is_reverse ? XyMax(a_min_sample - a_max_sample, 0) : XyMax(a_max_sample - a_min_sample, 0);

        const float a_min_segment_angle = a_min_sample * XY_PI * 2.0f / XY_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const float a_max_segment_angle = a_max_sample * XY_PI * 2.0f / XY_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const bool a_emit_start = XyAbs(a_min_segment_angle - a_min) >= 1e-5f;
        const bool a_emit_end = XyAbs(a_max - a_max_segment_angle) >= 1e-5f;

        _Path.reserve(_Path.Size + (a_mid_samples + 1 + (a_emit_start ? 1 : 0) + (a_emit_end ? 1 : 0)));
        if (a_emit_start)
            _Path.push_back(XyVec2(center.x + XyCos(a_min) * radius, center.y + XySin(a_min) * radius));
        if (a_mid_samples > 0)
            _PathArcToFastEx(center, radius, a_min_sample, a_max_sample, 0);
        if (a_emit_end)
            _Path.push_back(XyVec2(center.x + XyCos(a_max) * radius, center.y + XySin(a_max) * radius));
    }
    else
    {
        const float arc_length = XyAbs(a_max - a_min);
        const int circle_segment_count = _CalcCircleAutoSegmentCount(radius);
        const int arc_segment_count = XyMax((int)XyCeil(circle_segment_count * arc_length / (XY_PI * 2.0f)), (int)(2.0f * XY_PI / arc_length));
        _PathArcToN(center, radius, a_min, a_max, arc_segment_count);
    }
}

void _Xyphra::PathArcToFast(const XyVec2& center, float radius, int a_min_of_12, int a_max_of_12)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }
    _PathArcToFastEx(center, radius, a_min_of_12 * XY_DRAWLIST_ARCFAST_SAMPLE_MAX / 12, a_max_of_12 * XY_DRAWLIST_ARCFAST_SAMPLE_MAX / 12, 0);
}

void _Xyphra::PathBezierCubicCurveTo(const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, int num_segments)
{
    XyVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        XY_ASSERT(shared_data.CurveTessellationTol > 0.0f);
        PathBezierCubicCurveToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, shared_data.CurveTessellationTol, 0); // Auto-tessellated
    }
    else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
            _Path.push_back(XyBezierCubicCalc(p1, p2, p3, p4, t_step * i_step));
    }
}

void _Xyphra::PathBezierQuadraticCurveTo(const XyVec2& p2, const XyVec2& p3, int num_segments)
{
    XyVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        XY_ASSERT(shared_data.CurveTessellationTol > 0.0f);
        PathBezierQuadraticCurveToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, shared_data.CurveTessellationTol, 0);// Auto-tessellated
    }
    else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
            _Path.push_back(XyBezierQuadraticCalc(p1, p2, p3, t_step * i_step));
    }
}

void _Xyphra::PathRect(const XyVec2& a, const XyVec2& b, float rounding, XyDrawFlags flags)
{
    flags = FixRectCornerFlags(flags);
    rounding = XyMin(rounding, XyFabs(b.x - a.x) * (((flags & XyDrawFlags_RoundCornersTop) == XyDrawFlags_RoundCornersTop) || ((flags & XyDrawFlags_RoundCornersBottom) == XyDrawFlags_RoundCornersBottom) ? 0.5f : 1.0f) - 1.0f);
    rounding = XyMin(rounding, XyFabs(b.y - a.y) * (((flags & XyDrawFlags_RoundCornersLeft) == XyDrawFlags_RoundCornersLeft) || ((flags & XyDrawFlags_RoundCornersRight) == XyDrawFlags_RoundCornersRight) ? 0.5f : 1.0f) - 1.0f);

    if (rounding < 0.5f || (flags & XyDrawFlags_RoundCornersMask_) == XyDrawFlags_RoundCornersNone)
    {
        PathLineTo(a);
        PathLineTo(XyVec2(b.x, a.y));
        PathLineTo(b);
        PathLineTo(XyVec2(a.x, b.y));
    }
    else
    {
        const float rounding_tl = (flags & XyDrawFlags_RoundCornersTopLeft) ? rounding : 0.0f;
        const float rounding_tr = (flags & XyDrawFlags_RoundCornersTopRight) ? rounding : 0.0f;
        const float rounding_br = (flags & XyDrawFlags_RoundCornersBottomRight) ? rounding : 0.0f;
        const float rounding_bl = (flags & XyDrawFlags_RoundCornersBottomLeft) ? rounding : 0.0f;
        PathArcToFast(XyVec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
        PathArcToFast(XyVec2(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
        PathArcToFast(XyVec2(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
        PathArcToFast(XyVec2(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
    }
}

void _Xyphra::_OnChangedClipRect()
{
    XyCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &_CmdHeader.ClipRect, sizeof(XyVec4)) != 0)
    {
        AddDrawCmd();
        return;
    }

    // Try to merge with previous command if it matches, else use current command
    XyCmd* prev_cmd = curr_cmd - 1;
    if (curr_cmd->ElemCount == 0 && CmdBuffer.Size > 1 && XyCmd_HeaderCompare(&_CmdHeader, prev_cmd) == 0 && XyCmd_AreSequentialIdxOffset(prev_cmd, curr_cmd))
    {
        CmdBuffer.pop_back();
        return;
    }

    curr_cmd->ClipRect = _CmdHeader.ClipRect;
}

void _Xyphra::_OnChangedTextureID()
{
    // If current command is used with different settings we need to add a new command
    XyCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != _CmdHeader.TextureId)
    {
        AddDrawCmd();
        return;
    }

    // Try to merge with previous command if it matches, else use current command
    XyCmd* prev_cmd = curr_cmd - 1;
    if (curr_cmd->ElemCount == 0 && CmdBuffer.Size > 1 && XyCmd_HeaderCompare(&_CmdHeader, prev_cmd) == 0 && XyCmd_AreSequentialIdxOffset(prev_cmd, curr_cmd))
    {
        CmdBuffer.pop_back();
        return;
    }

    curr_cmd->TextureId = _CmdHeader.TextureId;
}


void _Xyphra::PushTextureID(XyTextureID texture_id)
{
    _TextureIdStack.push_back(texture_id);
    _CmdHeader.TextureId = texture_id;
    _OnChangedTextureID();
}

void _Xyphra::PopTextureID()
{
    _TextureIdStack.pop_back();
    _CmdHeader.TextureId = (_TextureIdStack.Size == 0) ? (XyTextureID)NULL : _TextureIdStack.Data[_TextureIdStack.Size - 1];
    _OnChangedTextureID();
}


void _Xyphra::AddLine(const XyVec2& p1, const XyVec2& p2, XyU32 col, float thickness)
{
    PathLineTo(p1 + XyVec2(0.5f, 0.5f));
    PathLineTo(p2 + XyVec2(0.5f, 0.5f));
    PathStroke(col, 0, thickness);
}

void _Xyphra::AddLineMultiColor(const XyVec2& a, const XyVec2& b, XyU32 col_a, XyU32 col_b, float thickness)
{
    // Compute the difference vector and its length.
    XyVec2 diff = XyVec2(b.x - a.x, b.y - a.y);
    float len = sqrtf(diff.x * diff.x + diff.y * diff.y);
    if (len < 0.0001f)
        return; // avoid division by zero

    // Compute a perpendicular vector (normalized) scaled by half the thickness.
    XyVec2 perp = XyVec2(-diff.y, diff.x);
    perp.x /= len;
    perp.y /= len;
    perp.x *= thickness * 0.5f;
    perp.y *= thickness * 0.5f;

    // Define the four corners of the quad.
    // v0 and v3 correspond to the start point (a) and v1 and v2 to the end point (b)
    XyVec2 v0 = XyVec2(a.x + perp.x, a.y + perp.y); // start, top side
    XyVec2 v1 = XyVec2(b.x + perp.x, b.y + perp.y); // end, top side
    XyVec2 v2 = XyVec2(b.x - perp.x, b.y - perp.y); // end, bottom side
    XyVec2 v3 = XyVec2(a.x - perp.x, a.y - perp.y); // start, bottom side

    // Use the low-level API to build a convex quad with two triangles.
    // Reserve 6 indices (2 triangles) and 4 vertices.
    PrimReserve(6, 4);
    PrimWriteIdx((XyIndex)0);
    PrimWriteIdx((XyIndex)1);
    PrimWriteIdx((XyIndex)2);
    PrimWriteIdx((XyIndex)2);
    PrimWriteIdx((XyIndex)3);
    PrimWriteIdx((XyIndex)0);

    // Write the four vertices.
    // Assign col_a to the start vertices and col_b to the end vertices.
    PrimWriteVtx(v0, XyVec2(0, 0), col_a);
    PrimWriteVtx(v1, XyVec2(0, 0), col_b);
    PrimWriteVtx(v2, XyVec2(0, 0), col_b);
    PrimWriteVtx(v3, XyVec2(0, 0), col_a);
}

void _Xyphra::AddRect(const XyVec2& p_min, const XyVec2& p_max, XyU32 col, float rounding, XyDrawFlags flags, float thickness)
{
    if (Flags & XyDrawListFlags_AntiAliasedLines)
        PathRect(p_min + XyVec2(0.50f, 0.50f), p_max - XyVec2(0.50f, 0.50f), rounding, flags);
    else
        PathRect(p_min + XyVec2(0.50f, 0.50f), p_max - XyVec2(0.49f, 0.49f), rounding, flags); // Better looking lower-right corner and rounded non-AA shapes.
    PathStroke(col, XyDrawFlags_Closed, thickness);
}

void _Xyphra::AddRectFilled(const XyVec2& p_min, const XyVec2& p_max, XyU32 col, float rounding, XyDrawFlags flags)
{
    if (rounding < 0.5f || (flags & XyDrawFlags_RoundCornersMask_) == XyDrawFlags_RoundCornersNone)
    {
        PrimReserve(6, 4);
        PrimRect(p_min, p_max, col);
    }
    else
    {
        PathRect(p_min, p_max, rounding, flags);
        PathFillConvex(col);
    }
}

void _Xyphra::AddRectFilledMultiColor(const XyVec2& p_min, const XyVec2& p_max, XyU32 col_upr_left, XyU32 col_upr_right, XyU32 col_bot_right, XyU32 col_bot_left)
{
    const XyVec2 uv = shared_data.TexUvWhitePixel;
    PrimReserve(6, 4);
    PrimWriteIdx((XyIndex)(_VtxCurrentIdx)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 1)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 2));
    PrimWriteIdx((XyIndex)(_VtxCurrentIdx)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 2)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 3));
    PrimWriteVtx(p_min, uv, col_upr_left);
    PrimWriteVtx(XyVec2(p_max.x, p_min.y), uv, col_upr_right);
    PrimWriteVtx(p_max, uv, col_bot_right);
    PrimWriteVtx(XyVec2(p_min.x, p_max.y), uv, col_bot_left);
}

void _Xyphra::AddRectFilledMultiColorRounded(const XyVec2& p_min, const XyVec2& p_max, XyU32 col_upr_left, XyU32 col_upr_right, XyU32 col_bot_right, XyU32 col_bot_left, float rounding, XyDrawFlags flags)
{
    flags = FixRectCornerFlags(flags);
    rounding = XyMin(rounding, XyFabs(p_max.x - p_min.x) * (((flags & XyDrawCornerFlags_Top) == XyDrawCornerFlags_Top) || ((flags & XyDrawCornerFlags_Bot) == XyDrawCornerFlags_Bot) ? 0.5f : 1.0f) - 1.0f);
    rounding = XyMin(rounding, XyFabs(p_max.y - p_min.y) * (((flags & XyDrawCornerFlags_Left) == XyDrawCornerFlags_Left) || ((flags & XyDrawCornerFlags_Right) == XyDrawCornerFlags_Right) ? 0.5f : 1.0f) - 1.0f);

    // https://github.com/ocornut/imgui/issues/3710#issuecomment-758555966
    if (rounding > 0.0f)
    {
        const int size_before = VtxBuffer.Size;
        AddRectFilled(p_min, p_max, XY_COL32_WHITE, rounding, flags);
        const int size_after = VtxBuffer.Size;

        for (int i = size_before; i < size_after; i++)
        {
            XyVertex* vert = VtxBuffer.Data + i;

            XyVec4 upr_left = ColorConvertU32ToFloat4(col_upr_left);
            XyVec4 bot_left = ColorConvertU32ToFloat4(col_bot_left);
            XyVec4 up_right = ColorConvertU32ToFloat4(col_upr_right);
            XyVec4 bot_right = ColorConvertU32ToFloat4(col_bot_right);

            float X = XyClamp((vert->pos.x - p_min.x) / (p_max.x - p_min.x), 0.0f, 1.0f);

            // 4 colors - 8 deltas

            float r1 = upr_left.x + (up_right.x - upr_left.x) * X;
            float r2 = bot_left.x + (bot_right.x - bot_left.x) * X;

            float g1 = upr_left.y + (up_right.y - upr_left.y) * X;
            float g2 = bot_left.y + (bot_right.y - bot_left.y) * X;

            float b1 = upr_left.z + (up_right.z - upr_left.z) * X;
            float b2 = bot_left.z + (bot_right.z - bot_left.z) * X;

            float a1 = upr_left.w + (up_right.w - upr_left.w) * X;
            float a2 = bot_left.w + (bot_right.w - bot_left.w) * X;


            float Y = XyClamp((vert->pos.y - p_min.y) / (p_max.y - p_min.y), 0.0f, 1.0f);
            float r = r1 + (r2 - r1) * Y;
            float g = g1 + (g2 - g1) * Y;
            float b = b1 + (b2 - b1) * Y;
            float a = a1 + (a2 - a1) * Y;
            XyVec4 RGBA(r, g, b, a);

            RGBA = RGBA * ColorConvertU32ToFloat4(vert->col);

            vert->col = XyColor(RGBA);
        }
        return;
    }

    const XyVec2 uv = shared_data.TexUvWhitePixel;
    PrimReserve(6, 4);
    PrimWriteIdx((XyIndex)(_VtxCurrentIdx)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 1)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 2));
    PrimWriteIdx((XyIndex)(_VtxCurrentIdx)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 2)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 3));
    PrimWriteVtx(p_min, uv, col_upr_left);
    PrimWriteVtx(XyVec2(p_max.x, p_min.y), uv, col_upr_right);
    PrimWriteVtx(p_max, uv, col_bot_right);
    PrimWriteVtx(XyVec2(p_min.x, p_max.y), uv, col_bot_left);
}

void _Xyphra::AddQuad(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, XyU32 col, float thickness)
{
    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathLineTo(p4);
    PathStroke(col, XyDrawFlags_Closed, thickness);
}

void _Xyphra::AddQuadFilled(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, XyU32 col)
{
    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathLineTo(p4);
    PathFillConvex(col);
}

void _Xyphra::AddTriangle(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, XyU32 col, float thickness)
{
    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathStroke(col, XyDrawFlags_Closed, thickness);
}

void _Xyphra::AddTriangleFilled(const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, XyU32 col)
{
    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathFillConvex(col);
}


void _Xyphra::AddTriangleFilledMultiColor(const XyVec2& a, const XyVec2& b, const XyVec2& c, XyU32 aCol, XyU32 bCol, XyU32 cCol)
{
    const XyVec2 uv = shared_data.TexUvWhitePixel;
    PrimReserve(3, 3);
    PrimWriteIdx((XyIndex)(_VtxCurrentIdx)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 1)); PrimWriteIdx((XyIndex)(_VtxCurrentIdx + 2));
    PrimWriteVtx(a, uv, aCol);
    PrimWriteVtx(b, uv, bCol);
    PrimWriteVtx(c, uv, cCol);
}

void _Xyphra::AddCircle(const XyVec2& center, float radius, XyU32 col, int num_segments, float thickness)
{
    if (radius < 0.5f)
        return;

    if (num_segments <= 0)
    {
        // Use arc with automatic segment count
        _PathArcToFastEx(center, radius - 0.5f, 0, XY_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
        _Path.Size--;
    }
    else
    {
        // Explicit segment count (still clamp to avoid drawing insanely tessellated shapes)
        num_segments = XyClamp(num_segments, 3, XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);

        // Because we are filling a closed shape we remove 1 from the count of segments/points
        const float a_max = (XY_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        PathArcTo(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
    }

    PathStroke(col, XyDrawFlags_Closed, thickness);
}

void _Xyphra::AddCircleFilled(const XyVec2& center, float radius, XyU32 col, int num_segments)
{
    if (radius < 0.5f)
        return;

    if (num_segments <= 0)
    {
        // Use arc with automatic segment count
        _PathArcToFastEx(center, radius, 0, XY_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
        _Path.Size--;
    }
    else
    {
        // Explicit segment count (still clamp to avoid drawing insanely tessellated shapes)
        num_segments = XyClamp(num_segments, 3, XY_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);

        // Because we are filling a closed shape we remove 1 from the count of segments/points
        const float a_max = (XY_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    }

    PathFillConvex(col);
}

void _Xyphra::AddNgon(const XyVec2& center, float radius, XyU32 col, int num_segments, float thickness)
{
    if (num_segments <= 2)
        return;

    // Because we are filling a closed shape we remove 1 from the count of segments/points
    const float a_max = (XY_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
    PathStroke(col, XyDrawFlags_Closed, thickness);
}

void _Xyphra::AddNgonFilled(const XyVec2& center, float radius, XyU32 col, int num_segments)
{
    if (num_segments <= 2)
        return;

    // Because we are filling a closed shape we remove 1 from the count of segments/points
    const float a_max = (XY_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    PathFillConvex(col);
}

void _Xyphra::AddImage(XyTextureID user_texture_id, const XyVec2& p_min, const XyVec2& p_max, const XyVec2& uv_min, const XyVec2& uv_max, XyU32 col)
{
    const bool push_texture_id = user_texture_id != _CmdHeader.TextureId;
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimRectUV(p_min, p_max, uv_min, uv_max, col);

    if (push_texture_id)
        PopTextureID();
}

void _Xyphra::AddImageQuad(XyTextureID user_texture_id, const XyVec2& p1, const XyVec2& p2, const XyVec2& p3, const XyVec2& p4, const XyVec2& uv1, const XyVec2& uv2, const XyVec2& uv3, const XyVec2& uv4, XyU32 col)
{
    const bool push_texture_id = user_texture_id != _CmdHeader.TextureId;
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimQuadUV(p1, p2, p3, p4, uv1, uv2, uv3, uv4, col);

    if (push_texture_id)
        PopTextureID();
}

void _Xyphra::AddImageRounded(XyTextureID user_texture_id, const XyVec2& p_min, const XyVec2& p_max, const XyVec2& uv_min, const XyVec2& uv_max, XyU32 col, float rounding, XyDrawFlags flags)
{
    flags = FixRectCornerFlags(flags);
    if (rounding < 0.5f || (flags & XyDrawFlags_RoundCornersMask_) == XyDrawFlags_RoundCornersNone)
    {
        AddImage(user_texture_id, p_min, p_max, uv_min, uv_max, col);
        return;
    }

    const bool push_texture_id = user_texture_id != _CmdHeader.TextureId;
    if (push_texture_id)
        PushTextureID(user_texture_id);

    int vert_start_idx = VtxBuffer.Size;
    PathRect(p_min, p_max, rounding, flags);
    PathFillConvex(col);
    int vert_end_idx = VtxBuffer.Size;
    ShadeVertsLinearUV(vert_start_idx, vert_end_idx, p_min, p_max, uv_min, uv_max, true);

    if (push_texture_id)
        PopTextureID();
}

void _Xyphra::ShadeVertsLinearUV(int vert_start_idx, int vert_end_idx, const XyVec2& a, const XyVec2& b, const XyVec2& uv_a, const XyVec2& uv_b, bool clamp)
{
    const XyVec2 size = b - a;
    const XyVec2 uv_size = uv_b - uv_a;
    const XyVec2 scale = XyVec2(
        size.x != 0.0f ? (uv_size.x / size.x) : 0.0f,
        size.y != 0.0f ? (uv_size.y / size.y) : 0.0f);

    XyVertex* vert_start = VtxBuffer.Data + vert_start_idx;
    XyVertex* vert_end = VtxBuffer.Data + vert_end_idx;
    if (clamp)
    {
        const XyVec2 min = XyMin(uv_a, uv_b);
        const XyVec2 max = XyMax(uv_a, uv_b);
        for (XyVertex* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = XyClamp(uv_a + XyMul(XyVec2(vertex->pos.x, vertex->pos.y) - a, scale), min, max);
    }
    else
    {
        for (XyVertex* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = uv_a + XyMul(XyVec2(vertex->pos.x, vertex->pos.y) - a, scale);
    }
}

float _Xyphra::CalcTextWidth(const char* text, const char* text_end, float wrap_width)
{
    const char* text_display_end;
    text_display_end = text_end;

    const float font_size = FontSize;
    if (text == text_display_end)
        return 0.f;
    float text_size = Font->CalcTextWidthA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL).x;

    //text_size = ceilf(text_size);

    text_size = XY_FLOOR(text_size + 0.99999f);

    return text_size;
}

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void _Xyphra::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if (g < b)
    {
        XySwap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        XySwap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = XyFabs(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void _Xyphra::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f)
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = XyFmod(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}