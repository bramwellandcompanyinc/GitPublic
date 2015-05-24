#pragma once

namespace LookupTables
{

	bool Init();
	double NormaliseHeading(double dblHeading);
	float NormaliseHeading(float fltHeading);
	double AngleDelta();	//the accuracy of our lookup tables.
	double Sine(double& dblHeading);
	double Cosine(double& dblHeading);
	double Tangent(double& dblAngle);
	float Tangent(float& fltAngle);
	void SineAndCosine(double& dblHeading, double& dblSine, double& dblCosine);
	void SineAndCosine(float& fltHeading, float& fltSine, float& fltCosine);

	//table for determining the most appropriate angular separation of an ocean mesh given the height of eye

	//double OceanMeshAngularSeparation(double a_dblHeightOfEye);
};
