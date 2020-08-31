#include "Framework.h"

map<wstring, Texture*> Texture::totalTexture;

// move => 이동생성자
Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
    :srv(srv), image(move(image))
{

}

Texture::~Texture()
{
    srv->Release();
}

Texture* Texture::Add(wstring file)
{
    if (totalTexture.count(file) > 0)
        return totalTexture[file];

    wstring extension = Utility::GetExtension(file);

    ScratchImage image;

    // Texture 위치가 Model마다 다른 위치에 Texture를 저장하기 위해 전체 경로를 이용하기로 함
    //wstring path = L"Textures/" + file;

    if (extension == L"tga")
    {
        HRESULT hr = LoadFromTGAFile(file.c_str(), nullptr, image);
        assert(SUCCEEDED(hr));
    }
    else if (extension == L"dds")
    {
        HRESULT hr = LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
        assert(SUCCEEDED(hr));
    }
    else
    {
        HRESULT hr = LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);
        assert(SUCCEEDED(hr));
    }

    ID3D11ShaderResourceView* srv;

    HRESULT hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv);
    assert(SUCCEEDED(hr));
    

    /*
    * 맵에 넣는 방법
    totalTexture[file] = new Texture(srv, image);
    totalTexture.insert(pair<wstring, Texture*>(file, new Texture(srv, image)));
    totalTexture.insert(make_pair(file, new Texture(srv, image)));
    totalTexture.insert({ file, new Texture(srv, image) });
    */
    totalTexture[file] = new Texture(srv, image);

    return totalTexture[file];
}

void Texture::Delete()
{
    for (auto texture : totalTexture)
    {
        delete texture.second;
    }

}

void Texture::PSSet(UINT slot)
{
    DC->PSSetShaderResources(slot, 1, &srv);
}

vector<XMFLOAT4> Texture::ReadPixels()
{
    vector<XMFLOAT4> result;

    uint8_t* colors = image.GetPixels();
    UINT size = image.GetPixelsSize();

    float scale = 1.0f / 255.0f;

    for (int i = 0; i < size; i += 4)
    {
        XMFLOAT4 color;

        color.x = colors[i + 0] * scale;
        color.y = colors[i + 1] * scale;
        color.z = colors[i + 2] * scale;
        color.w = colors[i + 3] * scale;

        result.emplace_back(color);
    }
    return result;
}
