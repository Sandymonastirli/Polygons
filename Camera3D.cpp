//Chrysanthi Monastirli AM 1716, Miltiadis Vasileiadis AM 2944

/* aksiopoiithikan osa eipwthikan mesa stin taksi ,diafaneies alla kai tipoi sxetika me tis kiniseis tis cameras
 epeita apo psaksimo */
#include "Camera3D.h"
#include <GL/glut.h>
#include "math.h"


static const float pi = 3.14159265358979323846264338327950288f; //to gnwsto "p" poy oloi kseroume
static const float pi2 = 1.57079632679489661923132169163975144f;  //to miso tou "p" pou oloi kseroume

Camera3D::Camera3D()
{
}


Camera3D::~Camera3D()
{
}

void Camera3D::Init(float xpos, float ypos, float zpos) //i init kanei tin arxikopoihsh, apo initialization
{
	m_yaw = 0.0;
	m_pitch = 0.0;

	SetCameraPosition(xpos, ypos, zpos);
}

void Camera3D::RefreshCamera()
{
	// oi parametroi tis kameras simfwna me to sistima sintetagmenwn Riegl
	// x/y gia flat, z gia ipsos
	xEye = cos(m_yaw) * cos(m_pitch);
	yEye = sin(m_pitch);
	zEye = sin(m_yaw) * cos(m_pitch);

	m_strafe_lx = cos(m_yaw - pi2);
	m_strafe_lz = sin(m_yaw - pi2);

/* to glMatrixMode kathorizei poio matrix stack einai stoxos gia tis epomenes matrix leitourgies
kai dexetai ws orisma to GL_MODELVIEW kathorizei ton tropo metamorfwsis twn antikeimenwn px
peristrofi kai klimakwsi sto plaisio twn sintetagmenwn tou kosmou mas */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();               //antikathista  ton trexon matrix me ton identity matrix

	gluLookAt(xPos, yPos, zPos, xPos + xEye, yPos + yEye, zPos + zEye, 0.0, 1.0, 0.0);
}

void Camera3D::SetCameraPosition(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;

	RefreshCamera();
}

void Camera3D::GetCameraPosition(float &x, float &y, float &z)
{
	x = xPos;
	y = yPos;
	z = zPos;
}

void Camera3D::GetEyeDirectionVector(float &x, float &y, float &z)
{
	x = xEye;
	y = yEye;
	z = zEye;
}

void Camera3D::MoveCamera(float incr)
{
	float lx = cos(m_yaw)*cos(m_pitch);
	float ly = sin(m_pitch);
	float lz = sin(m_yaw)*cos(m_pitch);

	xPos = xPos + incr * lx;
	yPos = yPos + incr * ly;
	zPos = zPos + incr * lz;

	RefreshCamera();
}

void Camera3D::StrafeCamera(float incr)
{
	xPos = xPos + incr * m_strafe_lx;
	zPos = zPos + incr * m_strafe_lz;

	RefreshCamera();
}

void Camera3D::FlyCamera(float incr)
{
	yPos = yPos + incr;

	RefreshCamera();
}

void Camera3D::RotateCameraYaw(float angle)
{
	m_yaw += angle;

	RefreshCamera();
}

void Camera3D::RotateCameraPitch(float angle)
{
	const float limit = 89.0 * pi / 180.0;

	m_pitch += angle;

	if (m_pitch < -limit)
		m_pitch = -limit;

	if (m_pitch > limit)
		m_pitch = limit;

	RefreshCamera();
}

void Camera3D::SetCameraYaw(float angle)
{
	m_yaw = angle;

	RefreshCamera();
}

void Camera3D::SetCameraPitch(float angle)
{
	m_pitch = angle;

	RefreshCamera();
}
