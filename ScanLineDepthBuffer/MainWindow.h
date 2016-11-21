﻿#pragma once

#include <string>
#include <Windows.h>
#include "BaseWindow.h"
#include "ObjModel.h"
#include "OffscreenBuffer.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
    MainWindow() : m_buffer(), m_objModel() { }
    PCWSTR ClassName() const { return L"MainWindow"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void OpenObjFile();

private:
    OffscreenBuffer m_buffer;
    ObjModel m_objModel;

};