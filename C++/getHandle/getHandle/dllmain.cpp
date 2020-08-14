﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <thread>
#include <dwmapi.h>
#include <algorithm>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32")
//#define TEST


using namespace std;

volatile bool isStart = false;
volatile bool isRunning = true;
int x;
int y;
volatile long width;
volatile long height;
struct handle_data {
    unsigned long process_id;
    HWND window_handle;
};

void getTopWindow(HWND& hwnd);
void getWindowRect(HWND& hwnd, LPRECT lprect);
bool isExplorerWindow(HWND& hwnd);
DWORD getExplorerID();
void _start();
bool isFileChooserWindow(HWND& hwnd);
extern "C" __declspec(dllexport) bool isDialogNotExist();
extern "C" __declspec(dllexport) void start();
extern "C" __declspec(dllexport) void stop();
extern "C" __declspec(dllexport) bool is_explorer_at_top();
extern "C" __declspec(dllexport) long getX();
extern "C" __declspec(dllexport) long getY();
extern "C" __declspec(dllexport) long getWidth();
extern "C" __declspec(dllexport) long getHeight();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport) bool isDialogNotExist()
{
    HWND hd = GetDesktopWindow();        //得到桌面窗口
    hd = GetWindow(hd, GW_CHILD);        //得到屏幕上第一个子窗口
    int num = 1;
    while (hd != NULL)                    //循环得到所有的子窗口
    {
        if (IsWindowVisible(hd))
        {
            if (isFileChooserWindow(hd) || isExplorerWindow(hd))
            {
                return false;
            }
        }
        hd = GetNextWindow(hd, GW_HWNDNEXT);
    }
    return true;
}

void getTopWindow(HWND& hwnd)
{
    hwnd = ::GetForegroundWindow();
}

void getWindowRect(HWND& hwnd, LPRECT lprect)
{
    GetWindowRect(hwnd, lprect);
}

bool isFileChooserWindow(HWND& hwnd)
{
    char title[100];
    GetClassNameA(hwnd, title, 100);
    string WindowClassName(title);
    transform(WindowClassName.begin(), WindowClassName.end(), WindowClassName.begin(), ::tolower);
    return WindowClassName.find("#32770") != string::npos ||
        WindowClassName.find("dialog") != string::npos;
}

bool isExplorerWindow(HWND& hwnd)
{
    if (IsWindowEnabled(hwnd))
    {
        char className[200];
        GetClassNameA(hwnd, className, 200);
        string WindowClassName(className);
        transform(WindowClassName.begin(), WindowClassName.end(), WindowClassName.begin(), ::tolower);
        return (WindowClassName.find("cabinet") != string::npos);
    }
    else
    {
        return false;
    }
}

__declspec(dllexport) void start()
{
    thread t(_start);
    t.detach();
}

__declspec(dllexport) void stop()
{
    isRunning = false;
}

__declspec(dllexport) bool is_explorer_at_top()
{
    return isStart;
}

__declspec(dllexport) long getX()
{
    return x;
}

__declspec(dllexport) long getY()
{
    return y;
}

__declspec(dllexport) long getWidth()
{
    return width;
}

__declspec(dllexport) long getHeight()
{
    return height;
}

void _start()
{
    HWND hwnd;
    RECT windowRect;
    while (isRunning)
    {
        getTopWindow(hwnd);
        if (isExplorerWindow(hwnd) || isFileChooserWindow(hwnd))
        {
            getWindowRect(hwnd, &windowRect);
            x = windowRect.left;
            y = windowRect.top;
            width = windowRect.right - windowRect.left;
            height = windowRect.bottom - windowRect.top;
            if (height < 200 || width < 200 || x < 50 || y < 50) {
                isStart = false;
            }
            else {
                isStart = true;
            }
        }
        else 
        {
            isStart = false;
        }
        if (isStart)
        {
            Sleep(10);
        } 
        else
        {

            Sleep(300);
        }
    }
}


#ifdef TEST
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    start();
    while (true)
    {
        /*if (isStart)
        {
            int xPos = getX();
            int yPos = getY();
            int width = getWidth();
            int height = getHeight();
        }*/
        if (!isDialogNotExist())
        {
            Sleep(50);
        }
        Sleep(10);
    }
}
#endif