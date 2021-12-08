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
			//glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
			//第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)
			//定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。
			//因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
			//第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
			//第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
			//第四个参数定义我们是否希望数据被标准化(Normalize)
			//第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
			int location = index + m_VertexBufferIndexOffset;
			int count = element.GetComponentCount();
			GLenum dataType = ShaderDataTypeToOpenGLBaseType(element.Type);
			auto normalize = element.Normalized ? GL_TRUE : GL_FALSE;
			unsigned int stride = vertexBuffer->GetLayout().GetStride();
			glVertexAttribPointer(location, count, dataType, normalize, stride, (const void*)element.Offset);
			//启用顶点属性,默认是禁止的
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