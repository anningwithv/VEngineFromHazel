#pragma once

#include "Model.h"

namespace VEngine
{
	class SphereModel : public Model
	{
	public:
		SphereModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, const std::string& shaderName, const std::string& diffuseTex, const std::string& specularTex);
		virtual void Draw(TimeStep ts) override;
		virtual void SubmitMesh()  override;

	private:
		Mesh* m_Mesh;
		unsigned int sphereVAO = 0;
		unsigned int indexCount;
	};
}
