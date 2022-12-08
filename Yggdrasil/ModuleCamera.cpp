#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "Math/float3x3.h"
#include "Math/Quat.h"
#include "Geometry/AABB.h"
#include "Geometry/Sphere.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetPos(float3(0.0f, 3.0f, 15.0f));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(nearPlane, farPlane);
	frustum.SetPerspective(2.f * atanf(tanf(math::pi / 4.0 * 0.5f) * aspect), math::pi / 4.0);

	return true;
}

update_status ModuleCamera::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update(float dt)
{
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	aspect = ((float)w) / ((float)h);
	setAspectRatio(aspect);

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
void ModuleCamera::Translate(float3 pos,float dt)
{
	frustum.SetPos(frustum.Pos() + float4x4(frustum.WorldMatrix()).Float3x3Part().MulDir(pos*dt));
}

void ModuleCamera::ResetPos() {
	frustum.SetPos(float3(0.0f, 3.0f, 15.0f));
}

void ModuleCamera::ResetCamera() {
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
}

void ModuleCamera::ResetFOV()
{
	frustum.SetPerspective(2.f * atanf(tanf(math::pi / 4.0 * 0.5f) * aspect), math::pi / 4.0);
}

void ModuleCamera::Rotate(float3 rad,float dt) {

	//float3x3 rotationDeltaMatrix;
	Quat quatx = quatx.RotateAxisAngle(frustum.WorldRight(),rad.x*dt);
	Quat quaty = quatx.RotateY(rad.y*dt);
	Quat quatRes = quatx.Mul(quaty);

	//Another possibility without using quaternion || also check more function in quat to optimise more ?
	//rotationDeltaMatrix = float3x3::RotateZ(rad.z)* float3x3::RotateY(rad.y)* float3x3::RotateAxisAngle(frustum.WorldRight(), rad.x);

	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(quatRes.Mul(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(quatRes.Mul(oldUp));

}
bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::Focus(const float3& focus) {

	float3 target = focus - frustum.Pos();

	float3x3 move = float3x3::LookAt(frustum.Front(), target.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(move.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(move.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::Orbit(const float3& target,float3 direction,float dt) {

	float3 vector = frustum.Pos() - target;

	vector = Quat(frustum.Up(), direction.x * dt).Transform(vector);
	vector = Quat(frustum.WorldRight(), direction.y * dt).Transform(vector);

	frustum.SetPos(vector + target);

	Focus(target);

}

//////////////////////////////////////////
//////// GETTER AND SETTER //////////////
/////////////////////////////////////////

void ModuleCamera::setCameraSpeed(float speed)
{
	this->cameraSpeed = speed;
}
float ModuleCamera::getCameraSpeed()
{
	return this->cameraSpeed;
}

void ModuleCamera::setRotationSpeed(float rotation)
{
	this->rotationSpeed = rotation;
}

float ModuleCamera::getRotationSpeed()
{
	return rotationSpeed;
}

void ModuleCamera::setAspectRatio()
{
	SDL_GetWindowSize(App->window->window, &width, &height);
	aspect = width / height;
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspect);
}

void ModuleCamera::setAspectRatio(float aspect)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspect);
}


void ModuleCamera::setNearPlane(float nearPlane)
{
	frustum.SetViewPlaneDistances(nearPlane, getFarPlane());
}

void ModuleCamera::setFarPlane(float farPlane)
{
	frustum.SetViewPlaneDistances(getNearPlane(), farPlane);
}

void ModuleCamera::setPerspectiveFOV(float horizontalFOV)
{
	frustum.SetPerspective(horizontalFOV, frustum.VerticalFov());
}

float ModuleCamera::getAspectRatio()
{
	return 	frustum.AspectRatio();
}

float ModuleCamera::getVerticalFOV()
{
	return frustum.VerticalFov();
}

float ModuleCamera::getHorizontalFOV()
{
	return frustum.HorizontalFov();
}

float ModuleCamera::getNearPlane()
{
	return frustum.NearPlaneDistance();
}

float ModuleCamera::getFarPlane()
{
	return frustum.FarPlaneDistance();
}

void ModuleCamera::SetPosition(float3 pos)
{
	frustum.SetPos(pos);
}

void ModuleCamera::SetFOV()
{
	SDL_GetWindowSize(App->window->window, &width, &height);
	aspect = width / height;
	frustum.SetHorizontalFovAndAspectRatio(getHorizontalFOV(), aspect);
}

float4x4 ModuleCamera::GetProj() {
	return frustum.ProjectionMatrix();
}
float4x4 ModuleCamera::GetView() {
	return frustum.ViewMatrix();
}