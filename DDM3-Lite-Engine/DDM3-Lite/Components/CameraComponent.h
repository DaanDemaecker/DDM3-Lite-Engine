#ifndef CameraComponentIncluded
#define CameraComponentIncluded

// File includes
#include "../BaseClasses/Component.h"
#include "../DataTypes/Structs.h"
#include "../Includes/VulkanIncludes.h"
#include "../Components/SkyBoxComponent.h"

namespace DDM3
{

	class CameraComponent final : public Component
	{
	public:
		CameraComponent();
		~CameraComponent() = default;

		void SetFovAngle(float angle) { m_FovAngle = angle; }
		void SetFovAngleDegrees(float angle);

		virtual void LateUpdate() override;
		

		float GetFovAngle() const { return m_FovAngle; }

		// Update uniform buffer with camera transform
		// Parameters:
		//     buffer: reference to the uniform buffer object that needs updating
		//     extent: the extent of the swapchain
		void UpdateUniformBuffer(UniformBufferObject& buffer, VkExtent2D extent);
		
		void RenderSkybox();

	private:
		glm::mat4 m_Matrix{};

		float m_FovAngle{};
		const float m_DefaultAngleDegrees{ 90.f };

		void UpdateMatrix();

		std::shared_ptr<SkyBoxComponent> m_pSkyBox{};
	};
}
#endif // !CameraComponentIncluded
