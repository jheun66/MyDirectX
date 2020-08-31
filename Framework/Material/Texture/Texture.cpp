#include "Framework.h"

map<wstring, Texture*> Texture::totalTexture;

// move => �̵�������
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

    // Texture ��ġ�� Model���� �ٸ� ��ġ�� Texture�� �����ϱ� ���� ��ü ��θ� �̿��ϱ�� ��
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
    * �ʿ� �ִ� ���
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
