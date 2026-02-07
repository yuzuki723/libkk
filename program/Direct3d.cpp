//! ===================================================================
//! @file Direct3d.cpp
//! @details Direct3D関連.
//! 
//! @author yuzuki
//! ===================================================================
#include <cstdio>
#include <windows.h>
#include "defined.h"
#include "Direct3d.h"

#include <crtdbg.h>

namespace kk
{
#define KK_PRINT(fmt, ...) { char buf[256]; sprintf_s(buf, fmt, __VA_ARGS__); OutputDebugStringA(buf); }

namespace d3d
{
	//! @brief 初期化.
	//! @param handle_window ウィンドウハンドル.
	//! @param width 横幅.
	//! @param height 縦幅.
	//! @retval true 成功.
	//! @retval false 失敗.
	bool Direct3d::Setup(HWND handle_window, int32_t width, int32_t height)
	{
		// ファクトリー作成.
		// ビデオ・グラフィックス設定の列挙や指定に使用.
		wrl::ComPtr<IDXGIFactory1> factory;
		if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
		{
			KK_PRINT("Failed to create DXGIFactory");
			return false;
		}

		// デバイス生成.
		// 主にリソース生成時に使用.
		UINT creation_flags = 0;

#ifdef _DEBUG
		// Direct3Dでバック有効化.
		creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// デバイスとデバイスコンテキストを作成.
		D3D_FEATURE_LEVEL future_level;
		if (FAILED(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creation_flags,
			feature_levels,
			_countof(feature_levels),
			D3D11_SDK_VERSION,
			&m_device,
			&future_level,
			&m_device_context
		)))
		{
			KK_PRINT("Failed to create D3D11Device");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC sc_desc = {};											//! スワップチェーンの設定データ.
		sc_desc.BufferDesc.Width = width;											//! 横幅.
		sc_desc.BufferDesc.Height = height;											//! 縦幅.
		sc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						//! バッファ形式.
		sc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//! 表示順はドライバに任せる.
		sc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//! 拡大方法もドライバに任せる.
		sc_desc.BufferDesc.RefreshRate.Numerator = 0;								//! リフレッシュレート.
		sc_desc.BufferDesc.RefreshRate.Denominator = 1;								//! リフレッシュレート.
		sc_desc.SampleDesc.Count = 1;												//! MSAAは使用しない.
		sc_desc.SampleDesc.Quality = 0;												//! MSAAは使用しない.
		sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						//! バッファの使用方法.
		sc_desc.BufferCount = 2;													//! バッファの数.
		sc_desc.OutputWindow = handle_window;
		sc_desc.Windowed = true;													//! ウィンドウモード.
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;							//! フィリップモデルの場合はバッファカウントは2以上.
		sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;						//! モードスイッチを許可.

		// スワップチェイン作成.
		if (FAILED(factory->CreateSwapChain(m_device.Get(), &sc_desc, &m_swap_chain)))
		{
			KK_PRINT("Failed to create SwapChain");
			return false;
		}

		// スワップチェインからバックバッファリソース取得.
		wrl::ComPtr<ID3D11Texture2D> back_buffer;
		if (FAILED(m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))))
		{
			KK_PRINT("Failed to acquire the back buffer");
			return false;
		}

		// バックバッファリソース用のレンダーターゲットビューを作成.
		D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc = {};
		render_target_view_desc.Format = sc_desc.BufferDesc.Format;
		render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		if (FAILED(m_device->CreateRenderTargetView(back_buffer.Get(), &render_target_view_desc, &m_back_buffer_view)))
		{
			KK_PRINT("ailed to create Render Target View");
			return false;
		}

		// バックバッファをレンダーターゲットとしてセット.
		m_device_context->OMSetRenderTargets(1, m_back_buffer_view.GetAddressOf(), nullptr);

		// ビューポート設定.
		D3D11_VIEWPORT viewport =
		{
			0.0f,						//! 左端X座標.
			0.0f,						//! 左端Y座標.
			static_cast<float>(width),	//! 横幅.
			static_cast<float>(height),	//! 縦幅.
			0.0f,						//! 最小深度.
			1.0f						//! 最大深度.
		};
		m_device_context->RSSetViewports(1, &viewport);

		return true;
	}
}
}