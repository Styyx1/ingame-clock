#include "image-loader.h"
#include <extern/stb_image.h>

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTextureFromFile(ID3D11Device* device, const char* filename)
{
    int width, height, channels;
    
    stbi_uc* pixels = stbi_load(filename, &width, &height, &channels, 4);
    if (!pixels) {
        // Failed to load
        return nullptr;
    }

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels;
    initData.SysMemPitch = width * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
    stbi_image_free(pixels);

    if (FAILED(hr)) {
        return nullptr;
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    hr = device->CreateShaderResourceView(texture.Get(), nullptr, &srv);
    if (FAILED(hr)) {
        return nullptr;
    }

    return srv;
}
