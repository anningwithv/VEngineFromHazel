#pragma once

#include "Model.h"

namespace VEngine
{
	class CubeModel : public Model
	{
	public:
		CubeModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Ref<Material> mat);
		virtual void Draw(TimeStep ts) override;
		virtual void SubmitMesh()  override;

	private:
		Mesh* m_Mesh;
	};
}
