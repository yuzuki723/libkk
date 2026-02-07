//! ===================================================================
//! @file Direct3d.h
//! @details Direct3D関連.
//! 
//! @author yuzuki
//! ===================================================================
#pragma once

// Direct3Dのライブラリ使用明記.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <cstdint>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
namespace wrl = Microsoft::WRL;

namespace kk
{
namespace d3d
{
//! @brief Direct3Dクラス.
class Direct3d
{
public:
	//! @brief 初期化.
	//! @param handle_window ウィンドウハンドル.
	//! @param width 横幅.
	//! @param height 縦幅.
	//! @retval true 成功.
	//! @retval false 失敗.
	bool Setup(HWND handle_window, int32_t width, int32_t height);

private:
	wrl::ComPtr<ID3D11Device>			m_device = nullptr;				//! Direct3Dデバイス.
	wrl::ComPtr<ID3D11DeviceContext>	m_device_context = nullptr;		//! Direct3Dデバイスコンテキスト.
	wrl::ComPtr<IDXGISwapChain>			m_swap_chain = nullptr;			//! スワップチェイン.
	wrl::ComPtr<ID3D11RenderTargetView>	m_back_buffer_view = nullptr;	//! バックバッファのRTビュー.

/*--------------------以下、シングルトン定義--------------------*/
public:
	/*コピー、代入禁止*/
	Direct3d(const Direct3d&) = delete;
	Direct3d& operator=(const Direct3d&) = delete;

	//! @brief インスタンスを取得.
	//! @return インスタンス.
	static Direct3d& GetInstance() noexcept
	{
		static Direct3d instance;
		return instance;
	}

private:
	//! @brief コンストラクタ.
	Direct3d() = default;
	~Direct3d() = default;

};
inline Direct3d& GetD3D() { return Direct3d::GetInstance(); }

}
}
