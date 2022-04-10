#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b*t + c*t2 + d*t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk

	m_controlPoints.push_back(glm::vec3(370, -120, 100));
	m_controlPoints.push_back(glm::vec3(270, -80, 275));
	m_controlPoints.push_back(glm::vec3(10, -50, 250));
	m_controlPoints.push_back(glm::vec3(-100, -25, 150));
	m_controlPoints.push_back(glm::vec3(-230, 103, -350));
	m_controlPoints.push_back(glm::vec3(0, -25, -415));
	m_controlPoints.push_back(glm::vec3(120, -70, -300));
	m_controlPoints.push_back(glm::vec3(290, -100, -140));
	

	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)s	
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int) m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i-1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M-1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int) m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int) (d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size()-1; i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;
	
	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j-1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	glm::vec3 T = glm::normalize(m_controlPoints[iPrev] - m_controlPoints[iCur]);
	glm::vec3 N = glm::cross(T, glm::vec3(0.f, 1, 0.f));

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::CreateCentreline(string _textureName)
{
	m_texture.Load(_textureName);

	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Call Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(500);
	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	CreateVBO(m_centrelinePoints);
}


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	for (int i = 0; i < m_centrelinePoints.size(); i++)
	{
		glm::vec3 p, pNext;
		p = m_centrelinePoints[i];
		int nextidx = (i + 1) % m_centrelinePoints.size();
		pNext = m_centrelinePoints[nextidx];
		glm::vec3 T = glm::normalize(pNext - p);
		glm::vec3 N = glm::cross(T, glm::vec3(0.f, 1, 0.f));
		float w = 30;
		glm::vec3 l = p - (w / 2) * N;
		glm::vec3 r = p + (w / 2) * N;

		m_leftOffsetPoints.push_back(l);
		m_rightOffsetPoints.push_back(r);
	}

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);

	CreateVBO(m_leftOffsetPoints);

	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);

	CreateVBO(m_rightOffsetPoints);
}

void CCatmullRom::CreateVBO(vector<glm::vec3> points)
{
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < points.size(); i++) {
		vbo.AddData(&points[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateTrack()
{
	
	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);
	vector<glm::vec3> trianglePoints;
	for (int i = 0; i < m_centrelinePoints.size(); i++)
	{
		trianglePoints.push_back(m_leftOffsetPoints[i]);
		trianglePoints.push_back(m_rightOffsetPoints[i]);
	}
	CreatetrackVBO(trianglePoints, 0.01f);
}

void CCatmullRom::CreatetrackVBO(vector<glm::vec3> points, float texdistance)
{
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float  dis =0;
	for (unsigned int i = 0; i < points.size(); i++) {
		
		glm::vec2 texCoord = glm::vec2(i % 2, dis);

		vbo.AddData(&points[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
		if(i % 2)
			dis += texdistance;
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it

	glLineWidth(5.0f);
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_LINE_STRIP, 0, 500);
}

void CCatmullRom::CreateFenceCurves(string _texturefile)
{

	m_FenceTexture.Load(_texturefile);
	m_FenceTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_FenceTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_FenceTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_FenceTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Compute the offset curves, one left, and one right.  Store the points in m_leftFencePoints and m_rightFencePoints respectively

	float w = 8; 
	for (int i = 0; i < m_leftOffsetPoints.size(); i++)
	{
		glm::vec3 Rp, RpNext,Lp,LpNext;
		Rp = m_rightOffsetPoints[i];
		Lp = m_leftOffsetPoints[i];
		int nextidx = (i + 1) % m_rightOffsetPoints.size();
		RpNext = m_rightOffsetPoints[nextidx];
		LpNext = m_leftOffsetPoints[nextidx];
		
		//Left Fence Points
		glm::vec3 T = glm::normalize(LpNext - Lp);
		glm::vec3 N = glm::cross(T, glm::vec3(0.f, 1.f, 0.f));
		glm::vec3 B = glm::normalize(glm::cross(N, T));
		glm::vec3 l = Lp + (w / 2) * B;

		//Right Fence Points 
		T = glm::normalize(RpNext - Rp);
		N = glm::cross(T, glm::vec3(0.f, 1, 0.f));
		B = glm::normalize(glm::cross(N, T));
		glm::vec3 r = Rp + (w / 2) * B;


		m_leftFencePoints.push_back(l);
		m_leftFencePoints.push_back(m_leftOffsetPoints[i]);

		m_rightFencePoints.push_back(m_rightOffsetPoints[i]);
		m_rightFencePoints.push_back(r);
	}

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	glGenVertexArrays(1, &m_vaoLeftFenceCurve);
	glBindVertexArray(m_vaoLeftFenceCurve);

	CreatetrackVBO(m_leftFencePoints, 0.1f);

	glGenVertexArrays(1, &m_vaoRightFenceCurve);
	glBindVertexArray(m_vaoRightFenceCurve);

	CreatetrackVBO(m_rightFencePoints, 0.1f);
}
void CCatmullRom::RenderFences()
{
	m_FenceTexture.Bind();
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoRightFenceCurve);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 1000);


	glLineWidth(5.0f);
	glBindVertexArray(m_vaoLeftFenceCurve);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 1000);
}
void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	glDrawArrays(GL_LINE_STRIP, 0, 500);

	// Bind the VAO m_vaoRightOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoRightOffsetCurve);
	glDrawArrays(GL_LINE_STRIP, 0, 500);
}


void CCatmullRom::RenderTrack()
{
	// Bind the VAO m_vaoTrack and render it
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_texture.Bind();
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoTrack);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 1000);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);