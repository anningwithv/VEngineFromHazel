#pragma once

#include "Model.h"

namespace VEngine
{
	class SkyboxModel : public Model
	{
	public:
		SkyboxModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Ref<Material> mat);
		virtual void Draw(TimeStep ts) override;
	private:
		Mesh* m_Mesh;
	};
}

