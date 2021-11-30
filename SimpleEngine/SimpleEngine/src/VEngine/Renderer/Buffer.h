#pragma once

#include "VEngine/Log.h"

namespace VEngine
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case VEngine::ShaderDataType::None:
			return 0;
		case VEngine::ShaderDataType::Float:
			return 4;
		case VEngine::ShaderDataType::Float2:
			return 4*2;
		case VEngine::ShaderDataType::Float3:
			return 4*3;
		case VEngine::ShaderDataType::Float4:
			return 4*4;
		case VEngine::ShaderDataType::Mat3:
			return 4*3*3;
		case VEngine::ShaderDataType::Mat4:
			return 4*4*4;
		case VEngine::ShaderDataType::Int:
			return 4;
		case VEngine::ShaderDataType::Int2:
			return 4*2;
		case VEngine::ShaderDataType::Int3:
			return 4*3;
		case VEngine::ShaderDataType::Int4:
			return 4*4;
		case VEngine::ShaderDataType::Bool:
			return 1;
		default:
			break;
		}

		VENGINE_CORE_ASSERT(false, "Unknown shader data type!");

		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned int Size;
		unsigned int Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}

		unsigned int GetComponentCount() const
		{
			switch (Type)
			{
			case VEngine::ShaderDataType::None:
				return 0;
			case VEngine::ShaderDataType::Float:
				return 1;
			case VEngine::ShaderDataType::Float2:
				return 2;
			case VEngine::ShaderDataType::Float3:
				return 3;
			case VEngine::ShaderDataType::Float4:
				return 4;
			case VEngine::ShaderDataType::Mat3:
				return 3 * 3;
			case VEngine::ShaderDataType::Mat4:
				return 4 * 4;
			case VEngine::ShaderDataType::Int:
				return 1;
			case VEngine::ShaderDataType::Int2:
				return 2;
			case VEngine::ShaderDataType::Int3:
				return 3;
			case VEngine::ShaderDataType::Int4:
				return 4;
			case VEngine::ShaderDataType::Bool:
				return 1;
			default:
				break;
			}

			VENGINE_CORE_ASSERT(false, "Unknown shader data type!");

			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements):m_Elements(elements) 
		{
			CalculateOffsetsAndStride();
		}
	
		inline unsigned int GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, unsigned int size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual int GetCount() const = 0;

		static IndexBuffer* Create(unsigned int* indices, unsigned int count);
	};
}
