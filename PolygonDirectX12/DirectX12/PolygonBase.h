#pragma once
#include"Device.h"
#include <DirectXMath.h>
#include<vector>

struct Vertex {
    DirectX::XMFLOAT3 pos;
    //解決法はSV＿VertexID
};

struct VertexData {
    std::vector<Vertex> vertex_vec{};
    std::vector<UINT> index_vec{};
};

class PolygonBase
{
public:
    //----------------------------------------------------------------------------------------------------

    //@brief    === ポリゴン作成関数 ===
    //@param    vertex_data 頂点データ配列
    //@return   ポリゴン作成の可否
    [[nodiscard]] bool create_polygon(const VertexData& vertex_data)noexcept;

    //@brief    === ポリゴン描画関数 ===
    //@param    list    コマンドリスト
    void draw_polygon(ID3D12GraphicsCommandList* list)noexcept;

    //----------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------

    VertexData polygon_{};
    Microsoft::WRL::ComPtr<ID3D12Resource> vertex_buffer_{};      /// 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> index_buffer_{};       /// インデックスバッファ
    D3D12_VERTEX_BUFFER_VIEW               vertex_buffer_view_{};  /// 頂点バッファビュー
    D3D12_INDEX_BUFFER_VIEW                index_buffer_view_{};   /// インデックスバッファビュー

    //----------------------------------------------------------------------------------------------------

    //@brief    === 頂点データバッファー作成関数 ===
    [[nodiscard]] bool create_vertex();

    //@brief    === 頂点インデックスバッファー作成関数 ===
    [[nodiscard]] bool create_index();

    //----------------------------------------------------------------------------------------------------
};