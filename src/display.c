/* nuklear - 1.32.0 - public domain */
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#pragma comment(lib, "comdlg32.lib")

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 750

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_INDEX_BUFFER 128 * 1024

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION

#include "../lib/nuklear.h"
#include "../lib/nuklear_d3d11.h"
#include "../lib/inv.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "nk_windows.c"
#include "overview.c"
#include "style.c"

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

static IDXGISwapChain *swap_chain;
static ID3D11Device *device;
static ID3D11DeviceContext *context;
static ID3D11RenderTargetView *rt_view;

static void
set_swap_chain_size(int width, int height)
{
  ID3D11Texture2D *back_buffer;
  D3D11_RENDER_TARGET_VIEW_DESC desc;
  HRESULT hr;

  if (rt_view)
    ID3D11RenderTargetView_Release(rt_view);

  ID3D11DeviceContext_OMSetRenderTargets(context, 0, NULL, NULL);

  hr = IDXGISwapChain_ResizeBuffers(swap_chain, 0, width, height, DXGI_FORMAT_UNKNOWN, 0);
  if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET || hr == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
  {
    /* to recover from this, you'll need to recreate device and all the resources */
    MessageBoxW(NULL, L"DXGI device is removed or reset!", L"Error", 0);
    exit(0);
  }
  assert(SUCCEEDED(hr));

  memset(&desc, 0, sizeof(desc));
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

  hr = IDXGISwapChain_GetBuffer(swap_chain, 0, &IID_ID3D11Texture2D, (void **)&back_buffer);
  assert(SUCCEEDED(hr));

  hr = ID3D11Device_CreateRenderTargetView(device, (ID3D11Resource *)back_buffer, &desc, &rt_view);
  assert(SUCCEEDED(hr));

  ID3D11Texture2D_Release(back_buffer);
}

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_CREATE:
  {
    /*    
    Window Menu -

    HMENU hMenu, hSubMenu;
    hMenu = CreateMenu();

    hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, 9001, "E&xit");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

    hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, 9002, "&Go");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

    SetMenu(wnd, hMenu); 
    */

    HICON hIcon, hIconSm;

    hIcon = LoadImage(NULL, "assets/inv_icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if(hIcon)
      SendMessage(wnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    else
      MessageBox(wnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);

    hIconSm = LoadImage(NULL, "assets/inv_icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    if(hIconSm)
      SendMessage(wnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
    else
      MessageBox(wnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  case WM_SIZE:
    if (swap_chain)
    {
      int height = HIWORD(lparam);
      int width  = LOWORD(lparam);
      set_swap_chain_size(width, height);
      nk_d3d11_resize(context, width, height);
    }
    break;
  }

  if (nk_d3d11_handle_event(wnd, msg, wparam, lparam))
    return 0;

  return DefWindowProcW(wnd, msg, wparam, lparam);
}


// Currently this only opens browser, grabs file, and prints filename
// TODO - something with the file
static void open_file_browser(HWND wnd) {
  OPENFILENAME ofn; // common dialog box structure
  char szFile[260]; // buffer for file name
  HANDLE hf;        // file handle

  // Initialize OPENFILENAME
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = wnd;
  ofn.lpstrFile = szFile;
  // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
  // use the contents of szFile to initialize itself.
  ofn.lpstrFile[0] = '\0'; // filename buffer
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "Inventory\0*.INV\0All\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  // Display the Open dialog box.
  if (GetOpenFileName(&ofn) == TRUE)
    hf = CreateFile(ofn.lpstrFile,
                    GENERIC_READ,
                    0,
                    (LPSECURITY_ATTRIBUTES)NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    (HANDLE)NULL);

  printf("%s", ofn.lpstrFile);
}

// static int window_init_status = 0;

  int main(void)
  {
    struct nk_context *ctx;
    struct nk_colorf bg;

    WNDCLASSW wc;
    HWND wnd;
    HRESULT hr;

    RECT rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exstyle = WS_EX_APPWINDOW;

    D3D_FEATURE_LEVEL feature_level;
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;

    int running = 1;

    /* Win32 */
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(0);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"NuklearWindowClass";

    if (!RegisterClassW(&wc))
    {
      MessageBox(NULL, "Window Registration Failed!", "Error!",
                 MB_ICONEXCLAMATION | MB_OK);
      return 0;
    }

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);

    // Initialize Windows OS window
    wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"inventorio", style | WS_VISIBLE,
                          CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
                          NULL, NULL, wc.hInstance, NULL);

    /* D3D11 setup */
    memset(&swap_chain_desc, 0, sizeof(swap_chain_desc));
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = wnd;
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;
    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
                                             NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc,
                                             &swap_chain, &device, &feature_level, &context)))
    {
      /* if hardware device fails, then try WARP high-performance
           software rasterizer, this is useful for RDP sessions */
      hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP,
                                         NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc,
                                         &swap_chain, &device, &feature_level, &context);
      assert(SUCCEEDED(hr));
    }
    set_swap_chain_size(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* GUI */
    ctx = nk_d3d11_init(device, WINDOW_WIDTH, WINDOW_HEIGHT, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);
    {
      struct nk_font_atlas *atlas;
      nk_d3d11_font_stash_begin(&atlas);
      nk_d3d11_font_stash_end();
    }
    set_style(ctx, THEME_RED);

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (running)
    {
      /* Input */
      MSG msg;
      nk_input_begin(ctx);
      while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
      {
        if (msg.message == WM_QUIT)
          running = 0;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
      nk_input_end(ctx);

/* GUI */

// Creates windows and hides all except main menu
init_gui_windows(ctx);

/* Main Menu - Entry Point */
#define MMENU_BTN_WIDTH (int)WINDOW_WIDTH * 0.25
#define MMENU_PAD_WIDTH (int)MMENU_BTN_WIDTH * 0.08

      static const float ratio[] = {MMENU_PAD_WIDTH, MMENU_BTN_WIDTH, MMENU_PAD_WIDTH};

#define MMENU_WIDTH (float)(ratio[0] + ratio[1] + ratio[2] + 35)
#define MMENU_HEIGHT MMENU_WIDTH - 100

#define MMENU_OFFSET_X ((float)WINDOW_WIDTH / 2) - ((float)MMENU_WIDTH / 2)
#define MMENU_OFFSET_Y ((float)WINDOW_HEIGHT / 2) - ((float)MMENU_HEIGHT * 1.8)

      if (nk_begin(ctx, "Main Menu", nk_rect(MMENU_OFFSET_X, MMENU_OFFSET_Y, MMENU_WIDTH, MMENU_HEIGHT),
                   NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE |
                       NK_WINDOW_TITLE))
      {
        nk_layout_row(ctx, NK_STATIC, 40, 3, ratio);
        nk_label(ctx, "", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "Open Inventory"))
        {
          open_file_browser(wnd); // find .inv file to load
        }
        nk_label(ctx, "", NK_TEXT_RIGHT);

        nk_label(ctx, "", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "Connect to Inventory"))
        {
        }
        nk_label(ctx, "", NK_TEXT_RIGHT);

        nk_label(ctx, "", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "New Inventory"))
        {
          show_window(ctx, "New Inventory");
        }
        nk_label(ctx, "", NK_TEXT_RIGHT);

        nk_label(ctx, "", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "Settings"))
        {
        }
        nk_label(ctx, "", NK_TEXT_RIGHT);
      }
      nk_end(ctx);

      // NK Overview Window showing most ui elements must include file at top  
      // overview(ctx);

      /* Draw */
      ID3D11DeviceContext_ClearRenderTargetView(context, rt_view, &bg.r);
      ID3D11DeviceContext_OMSetRenderTargets(context, 1, &rt_view, NULL);
      nk_d3d11_render(context, NK_ANTI_ALIASING_ON);
      hr = IDXGISwapChain_Present(swap_chain, 1, 0);
      if (hr == DXGI_ERROR_DEVICE_RESET || hr == DXGI_ERROR_DEVICE_REMOVED)
      {
        /* to recover from this, you'll need to recreate device and all the resources */
        MessageBoxW(NULL, L"D3D11 device is lost or removed!", L"Error", 0);
        break;
      }
      else if (hr == DXGI_STATUS_OCCLUDED)
      {
        /* window is not visible, so vsync won't work. Let's sleep a bit to reduce CPU usage */
        Sleep(10);
      }
      assert(SUCCEEDED(hr));
    }

    ID3D11DeviceContext_ClearState(context);
    nk_d3d11_shutdown();
    ID3D11RenderTargetView_Release(rt_view);
    ID3D11DeviceContext_Release(context);
    ID3D11Device_Release(device);
    IDXGISwapChain_Release(swap_chain);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
  }
