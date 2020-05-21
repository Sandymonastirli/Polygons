//Chrysanthi Monastirli AM 1716, Miltiadis Vasileiadis AM 2944

#pragma once
class Camera3D
{
public:
	Camera3D();     //constructor
	~Camera3D();    // destructor
	void Init(float xpos, float ypos, float zpos);    //arxikopoihsh
	void RefreshCamera();                               //ananewsi kameras
	void SetCameraPosition(float x, float y, float z);     //bale thesi kameras ,sinartisi set
	void GetCameraPosition(float &x, float &y, float &z);  //pare  thesi kameras, sinartisi get
	void GetEyeDirectionVector(float &x, float &y, float &z);  //pare dieuthinsi tou dianismatos apo opou koitame
	void SetCameraYaw(float angle); //thetoume tin kinisi pou kanei i kamera girw apo enan katakorifo aksona, ayto simainei to yaw(heading),"peristrofi"/"talanteusi"
	void SetCameraPitch(float angle);      //thetoume tin kinisi pou kanei pitch, ston "plagio" aksona, ayto fainetai kalitera me sxedio

	// Navigation
	void MoveCamera(float incr);
	void StrafeCamera(float incr);  //me to strafe exoume tin kamera mas na kounietai deksia kai aristera
	void FlyCamera(float incr);                        //gia tin thesi tis kameras px apo panw kapws apenanti apo to "antikeimeno" mou,apo konta mporoume na eksigisume/deiksoume tin kinisi kalitera
	void RotateCameraYaw(float angle);                //gia talanteusi yaw(eidos kinisis kameras)
	void RotateCameraPitch(float angle);              //gia talanteusi pitch(eidos kinisis kameras)
private:
	float xPos, yPos, zPos;                         // thesi se aksones x,y,z
	float xEye, yEye, zEye;                         //i dieuthisi tou dianismatos apo opou koitame
	float m_yaw, m_pitch;                           // poikiles peristrofes gwniwn
	float m_strafe_lx, m_strafe_lz;                 //panta 90 moires stin dieuthinsi dianismatos
};
