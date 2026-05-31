#pragma once
#include"Device.h"
#include <DirectXMath.h>
#include<vector>

struct Vertex {
    DirectX::XMFLOAT3 pos;
    //解決法はSV＿VertexID
};

class PolygonBase
{
public:
    //----------------------------------------------------------------------------------------------------

    //@brief    === ポリゴン作成関数 ===
    //@param    vertex_data 頂点データ配列
    //@return   ポリゴン作成の可否
    [[nodsicard]] bool create_polygon(std::vector<Vertex>& vertex_data)noexcept;

    //@brief    === ポリゴン描画関数 ===
    //@param    list    コマンドリスト
    void draw_polygon(ID3D12GraphicsCommandList* list)noexcept;

    //----------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------

    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_{};      /// 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_{};       /// インデックスバッファ
    D3D12_VERTEX_BUFFER_VIEW               vertexBufferView_{};  /// 頂点バッファビュー
    D3D12_INDEX_BUFFER_VIEW                indexBufferView_{};   /// インデックスバッファビュー

    //----------------------------------------------------------------------------------------------------

    //@brief    === 頂点データバッファー作成関数 ===
    [[nodiscard]] bool create_vertex(std::vector<Vertex>& vertex_data);

    //@brief    === 頂点インデックスバッファー作成関数 ===
    [[nodiscard]] bool create_index(std::vector<Vertex>& vertex_data);

    //----------------------------------------------------------------------------------------------------
};