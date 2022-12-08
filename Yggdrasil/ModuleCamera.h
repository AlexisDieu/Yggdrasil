#pragma once
#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"

class ModuleCamera : public Module
{
	public:
		ModuleCamera();
		~ModuleCamera();

		bool Init();
		float4x4 GetProj();
		float4x4 GetView();
		update_status PreUpdate(float dt);
		update_status Update(float dt);
		update_status PostUpdate(float dt);
		void Translate(float3 deltaCoords,float dt);
		void SetPosition(float3 pos);
		void SetFOV();

		void ResetPos();
		void ResetCamera();
		void ResetFOV();
		void Rotate(float3 rad,float dt);

		void setCameraSpeed(float speed);
		float getCameraSpeed();
		void setRotationSpeed(float rotation);
		float getRotationSpeed();


		void setAspectRatio();
		void setAspectRatio(float aspect);
		void setNearPlane(float nearPlane);
		void setFarPlane(float farPlane);
		void setPerspectiveFOV(float horizontalFOV);
		float getAspectRatio();
		float getVerticalFOV();
		float getHorizontalFOV();
		float getNearPlane();
		float getFarPlane();

		bool CleanUp();
		void Focus(const float3& focus);
		void Orbit(const float3& focus, float3 rad, float dt);
private:
	int width, height;
	float cameraSpeed = 2.00f;
	float rotationSpeed = 1.00f;
	float aspect = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
	float verticalFOV = 4.0;
	float horizontalFOV = 4.0;
	float nearPlane = 0.1;
	float farPlane = 100.0;
	Frustum frustum;
};

