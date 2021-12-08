#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace VEngine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case VEngine::ShaderDataType::Float:
		case VEngine::ShaderDataType::Float2:
		case VEngine::ShaderDataType::Float3:
		case VEngine::ShaderDataType::Float4:
		case VEngine::ShaderDataType::Mat3:
		case VEngine::ShaderDataType::Mat4:
			return GL_FLOAT;
		case VEngine::ShaderDataType::Int:
		case VEngine::ShaderDataType::Int2:
		case VEngine::ShaderDataType::Int3:
		case VEngine::ShaderDataType::Int4:
			return GL_INT;
		case VEngine::ShaderDataType::Bool:
			return GL_BOOL;
		default:
			break;
		}

		VENGINE_CORE_ASSERT(false, "Unknown shader data type!");

		return 0;
	}

	VEngine::OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VEngine::OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VEngine::OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VEngine::OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VEngine::OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		//VENGINE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size != 0, "Vertex buffer has no layout");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		unsigned int index = 0;
		for (const auto& element : vertexBuffer->GetLayout())
		{
			//glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
			//��һ������ָ������Ҫ���õĶ������ԡ����ǵ������ڶ�����ɫ����ʹ��layout(location = 0)
			//������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0��
			//��Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
			//�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
			//����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
			//���ĸ��������������Ƿ�ϣ�����ݱ���׼��(Normalize)
			//�����������������(Stride)�������������������Ķ���������֮��ļ����
			int location = index + m_VertexBufferIndexOffset;
			int count = element.GetComponentCount();
			GLenum dataType = ShaderDataTypeToOpenGLBaseType(element.Type);
			auto normalize = element.Normalized ? GL_TRUE : GL_FALSE;
			unsigned int stride = vertexBuffer->GetLayout().GetStride();
			glVertexAttribPointer(location, count, dataType, normalize, stride, (const void*)element.Offset);
			//���ö�������,Ĭ���ǽ�ֹ��
			glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += vertexBuffer->GetLayout().GetElements().size();
	}

	void VEngine::OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}