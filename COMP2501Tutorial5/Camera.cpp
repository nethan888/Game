#include "Camera.h"

Camera::Camera(Vector3 pos, Vector3 lookat, Vector3 up)
{
	position = pos;
	forwardVector = lookat - pos;
	forwardVector.normalize();
	upVector = up;
	upVector.normalize();
}

void Camera::roll(float angleDeg){
	updateOrientation(forwardVector, angleDeg);
}

void Camera::pitch(float angleDeg){
	updateOrientation(Vector3::cross(forwardVector, upVector), angleDeg);
}

void Camera::yaw(float angleDeg){
	//updateOrientation(upVector, angleDeg);
	updateOrientation(Vector3(0,1,0), angleDeg);	//bonus
}

void Camera::move(float distance){
	position += forwardVector * distance;
}

void Camera::strafe(float distance){
	position += Vector3::cross(forwardVector, upVector) * distance;
}

void Camera::climb(float distance){
	position += upVector * distance;
}

void Camera::updateOrientation(Vector3 axis, float angleDeg){
	Matrix4 rotMatrix = Matrix4::IDENTITY;
	rotMatrix.rotate(axis, angleDeg);

	upVector = upVector * rotMatrix;
	forwardVector = forwardVector * rotMatrix;

	upVector.normalize();
	forwardVector.normalize();
}

Matrix4 Camera::getViewMatrix(){
	return createViewMatrix(position, forwardVector, upVector);
}