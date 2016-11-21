﻿#include <cstdlib>  // std::abort
#include <cassert>
#include "OffscreenBuffer.h"
#include "DebugPrint.h"

void OffscreenBuffer::Resize(INT32 width, INT32 height)
{
    if (m_memory)
    {
        VirtualFree(m_memory, 0, MEM_RELEASE);
        m_memory = NULL;
    }

    m_width = width;
    m_height = height;
    m_pitch = width * BYTES_PER_PIXEL;

    m_info.bmiHeader.biSize = sizeof(m_info.bmiHeader);
    m_info.bmiHeader.biWidth = m_width;
    m_info.bmiHeader.biHeight = -m_height;
    m_info.bmiHeader.biPlanes = 1;
    m_info.bmiHeader.biBitCount = 32;
    m_info.bmiHeader.biCompression = BI_RGB;

    SIZE_T bitmapMemorySize = m_width * m_height * BYTES_PER_PIXEL;
    m_memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    if (!m_memory && width != 0 && height != 0)
    {
        DebugPrint(L"VirtualAlloc Failed.");
        std::abort();
    }

    if (m_memory)
    {
        // TODO(jaege): temporary
        Render();
    }
}

void OffscreenBuffer::Render()
{
    // TODO(jaege): Below code is just for fun, I will implement scan-line
    //              z-buffer algorithm here in future.

    UINT8 *row = (UINT8 *)m_memory;
    for (int y = 0; y < m_height; ++y)
    {
        UINT32 *pixel = (UINT32 *)row;
        for (int x = 0; x < m_width; ++x)
        {
            *pixel++ = (((UINT8)(x)) << 16) |  // Red
                       (((UINT8)(y)) << 8) |  // Green
                       (UINT8)(x + y);  // Blue
        }
        row += m_pitch;
    }

    int j = GetHeight() / 2;
    for (int i = GetWidth() / 2; i < GetWidth(); ++i)
    {
        SetPixel(i, j++%GetHeight(), Color::RandomColor());
    }

    std::vector<Color> color(GetWidth(), Color::RandomColor());
    for (int i = GetHeight() / 5; i < GetHeight() * 2 / 5; ++i)
    {
        SetRow(i, color);
    }
}

void OffscreenBuffer::SetPixel(INT32 x, INT32 y, const Color & color)
{
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
    UINT32 *pixel = (UINT32 *)((UINT8 *)m_memory +
                               x * BYTES_PER_PIXEL + y * m_pitch);
    UINT32 *pixel2 = (UINT32 *)m_memory + x + y * m_width;
    *pixel = color.GetColor();
}

void OffscreenBuffer::SetRow(INT32 y, const std::vector<Color>& row)
{
    assert(y >= 0 && y < m_height && row.size() == m_width);
    UINT32 *pixel = (UINT32 *)((UINT8 *)m_memory + y * m_pitch);
    for (const Color &c : row)
        *pixel++ = c.GetColor();
}

void OffscreenBuffer::OnPaint(HDC hdc, LONG width, LONG height)
{ 
    StretchDIBits(hdc, 0, 0, width, height, 0, 0, m_width, m_height,
                  m_memory, &m_info, DIB_RGB_COLORS, SRCCOPY);
}