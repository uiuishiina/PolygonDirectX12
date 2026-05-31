#include"PolygonBase.h"

//@brief    === ポリゴン作成関数 ===
//@param    vertex_data 頂点データ配列
//@return   ポリゴン作成の可否
[[nodiscard]] bool PolygonBase::create_polygon(const VertexData& vertex_data)noexcept {

	polygon_ = vertex_data;

	if (polygon_.vertex_vec.empty()) {
		return false;
	}
	if (!create_vertex()) {
		return false;
	}
	if (!create_index()) {
		return false;
	}
	return true;
}

//@brief    === ポリゴン描画関数 ===
//@param    list    コマンドリスト
void PolygonBase::draw_polygon(ID3D12GraphicsCommandList* list)noexcept {

	list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
	list->IASetIndexBuffer(&index_buffer_view_);
	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	list->DrawIndexedInstanced(static_cast<UINT>(polygon_.index_vec.size()), 1, 0, 0, 0);
}

[[nodiscard]] bool PolygonBase::create_vertex() {

	const UINT vertex_buffer_size = sizeof(Vertex) * static_cast<UINT>(polygon_.vertex_vec.size());

	CD3DX12_HEAP_PROPERTIES heap_props(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resource_desc{};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Width = vertex_buffer_size;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = Device::Instance().get_device()->CreateCommittedResource(
		&heap_props,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_)
	);
	if (FAILED(hr)) {
		return false;
	}

	Vertex* vertex_data_begin;
	hr = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertex_data_begin));
	if (FAILED(hr)) {
		return false;
	}

	memcpy(vertex_data_begin, polygon_.vertex_vec.data(), vertex_buffer_size);
	vertex_buffer_->Unmap(0, nullptr);

	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.StrideInBytes = sizeof(Vertex);
	vertex_buffer_view_.SizeInBytes = vertex_buffer_size;
	return true;
}

[[nodiscard]] bool PolygonBase::create_index() {

	const UINT64 index_buffer_size = sizeof(UINT) * polygon_.index_vec.size();

	CD3DX12_HEAP_PROPERTIES heap_props(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resource_desc{};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Width = index_buffer_size;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = Device::Instance().get_device()->CreateCommittedResource(
		&heap_props,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_)
	);
	if (FAILED(hr)) {
		return false;
	}
	UINT* index_data_begin;
	hr = index_buffer_->Map(0, nullptr, reinterpret_cast<void**>(&index_data_begin));
	if (FAILED(hr)) {
		return false;
	}

	memcpy_s(index_data_begin, index_buffer_size, polygon_.index_vec.data(), index_buffer_size);
	index_buffer_->Unmap(0, nullptr);

	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;
	index_buffer_view_.SizeInBytes = index_buffer_size;
	return true;
}