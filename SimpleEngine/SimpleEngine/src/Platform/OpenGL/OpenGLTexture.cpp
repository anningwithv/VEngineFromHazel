#include "OpenGLTexture.h"
#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include "VEngine/Core/Core.h"

namespace VEngine
{
	VEngine::OpenGLTexture::OpenGLTexture(const std::string & path) : m_Path(path)
	{
		int width, height, channels;
		//OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
		stbi_set_flip_vertically_on_load(1);
		//Load image data
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//VENGINE_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		//glTexParameter*函数为放大和缩小指定过滤方式
		//GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）OpenGL会选择中心点最接近纹理坐标的那个像素
		//GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	VEngine::OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void VEngine::OpenGLTexture::Bind(unsigned int slot) const
	{
		//绑定这个纹理到指定的纹理单元(slot)
		glBindTextureUnit(slot, m_RendererID);
	}
}