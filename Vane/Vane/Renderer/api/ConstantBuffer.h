#pragma once
#include <d3d11.h>


template <typename T>
class ConstantBuffer {
public:
	ConstantBuffer() = default;
	~ConstantBuffer()
	{
		Destroy(); 
	}

	bool Init(ID3D11Device* device)
	{
		static_assert((sizeof(T) % 16) == 0, "ConstantBuffer<T>: T must be 16-byte aligned.");

		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.Usage = D3D11_USAGE_DEFAULT;
		cbDesc.ByteWidth = sizeof(T);
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		long hr = device->CreateBuffer(&cbDesc, nullptr, &buffer);
		return SUCCEEDED(hr);
	}

	void Destroy()
	{
		if (buffer)
		{
			buffer->Release();
			buffer = nullptr;
		}
	}

	void Update(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->UpdateSubresource(buffer, 0, nullptr, &data, 0, 0);
	}

	void Update(ID3D11DeviceContext* deviceContext, const T& newData)
	{
		data = newData;
		Update(deviceContext);
	}

	// Buffer
	operator ID3D11Buffer* () const { return buffer; }
	ID3D11Buffer* const* operator&() const { return &buffer; }

	// Data
	T* operator->()
	{ 
		return &data;
	}
	operator T* ()
	{ 
		return &data;
	}
	operator void* ()
	{ 
		return (void*)&data; 
	}

private:
	T data = {};
	ID3D11Buffer* buffer = nullptr;

	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;
};