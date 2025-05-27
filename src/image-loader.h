#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <d3d11.h>
#include <wrl/client.h>  // For Microsoft::WRL::ComPtr

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTextureFromFile(ID3D11Device* device, const char* filename);
