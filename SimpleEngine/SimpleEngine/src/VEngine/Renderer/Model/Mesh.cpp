#include "Mesh.h"
#include "VEngine/Renderer/Renderer.h"

namespace VEngine
{
	Mesh::Mesh(/*float vertices[]*/)
	{
		SetupMesh();
	}

	void Mesh::SetupMesh()
	{
		GLfloat vertices[] = {
			// Positions          // Texture Coords
			-0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  //0
			 0.5f, -0.5f, 0.5f,  1.0f,  0.0f,  //1
			 0.5f,  0.5f, 0.5f,  1.0f,  1.0f,  //2
			-0.5f,  0.5f, 0.5f,  0.0f,  1.0f,  //3
											   
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, //4
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, //5
			-0.5f,  0.5f, -0.5f,  1.0f,  1.0f, //6
			-0.5f, -0.5f, -0.5f,  1.0f,  0.0f, //7

		};
		



		//float vertices[9 * 4] = 
		//{
		//	-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.0f, 0.0f,
		//	 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, 1.0f, 0.0f,
		//	 0.5f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 1.0f, 1.0f,
		//	 -0.5f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.0f, 1.0f,
		//};

		m_VertexArray.reset(VEngine::VertexArray::Create());

		Ref<VEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
		VEngine::BufferLayout layout = 
		{
			{VEngine::ShaderDataType::Float3, "a_Position"},
			//{VEngine::ShaderDataType::Float4, "a_Color"},
			{VEngine::ShaderDataType::Float2, "a_TexCoord"},
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//创建索引缓冲对象EBO,它专门储存索引，OpenGL调用这些顶点的索引来决定该绘制哪个顶点
		unsigned int indices[36] = { 0,1,2, 0,2,3, 1,4,5, 1,5,2, 6,5,4, 4,7,6, 6,3,0, 6,0,7 , 6,3,2, 6,2,5, 7,4,1, 0,7,1};
		Ref<VEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(VEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/Texture.glsl");

		//m_Texture = VEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture = VEngine::Texture2D::Create("assets/textures/Box.jpg");
	}

	void Mesh::Draw(TimeStep ts)
	{
		auto shader = m_ShaderLibrary->Get("Texture");

		m_Texture->Bind(1);

		m_Time += ts;
		glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(5.0f * m_Time), glm::vec3(1.0, 1.0, 1.0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		Renderer::Submit(shader, m_VertexArray, transform);
	}
}