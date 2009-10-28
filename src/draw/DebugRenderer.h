/*----------------------------------------------------------------------------*\
|
|						     AGEIA PhysX Technology
|
|							     www.ageia.com
|
\*----------------------------------------------------------------------------*/

#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

class NxDebugRenderable;

class DebugRenderer
{
public:
	void renderData(const NxDebugRenderable& data) const;

private:
	static void renderBuffer(float* pVertList, float* pColorList, int type, int num);
};

#endif // DEBUGRENDER_H
