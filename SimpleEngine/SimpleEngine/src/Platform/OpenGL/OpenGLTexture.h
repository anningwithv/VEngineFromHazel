#pragma once

#include "VEngine/Renderer/Texture/Texture.h"

namespace VEngine
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& path);
		virtual ~OpenGLTexture();

		virtual unsigned int GetWidth() const override { return m_Width; }
		virtual unsigned int GetHeight() const override { return m_Height; }

		virtual void Bind(unsigned int slot = 0) const override;

	private:
		std::string  m_Path;
		unsigned int m_Width;
		unsigned int m_Height;
		unsigned int m_RendererID;
	};
}
