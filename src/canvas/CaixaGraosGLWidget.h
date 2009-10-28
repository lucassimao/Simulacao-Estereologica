#ifndef  CAIXA_DE_GRAOS_H
#define  CAIXA_DE_GRAOS_H

#include "..\model\SimulacaoCaixa.h"
#include "..\draw\DrawObjects.h"
#include <QGLWidget>
#include <QKeyEvent>

using namespace simulacao::model;

namespace simulacao {
	namespace canvas {

		class CaixaGraosGLWidget : public QGLWidget {

			Q_OBJECT

		public:
			CaixaGraosGLWidget(QWidget *parent = 0,SimulacaoCaixa *simulacao = 0);	

		protected:
			void initializeGL();
			void resizeGL(int width, int height);
			void paintGL();
			void mousePressEvent(QMouseEvent *event);
			void mouseMoveEvent(QMouseEvent *event);
			void keyPressEvent ( QKeyEvent * event );

		private:
			void draw();
			void SetupCamera();
			void RenderActors();
			//NxCCDSkeleton *createCCDfromActor ( NxActor *);

			void drawCuboid(const NxActor * cuboid, NX_BOOL drawBothSides = NX_FALSE);
			//NxActor * criarCaixa();
			//void CreateMeshes();
			
			NxActor* box;
			SimulacaoCaixa *simulacao;
			NxTriangleMesh * staticTriangleMesh;
			float gCameraAspectRatio;
			static NxVec3 gCameraForward;
			static NxVec3 gCameraRight;
			const static NxReal gCameraSpeed;
			NxVec3 gCameraPos;
			QPoint lastPos;


		};
	}
}
#endif