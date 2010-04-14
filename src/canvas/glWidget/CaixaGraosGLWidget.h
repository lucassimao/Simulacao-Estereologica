#ifndef  CAIXA_DE_GRAOS_H
#define  CAIXA_DE_GRAOS_H

#define NOMINMAX
#include <QGLWidget>
#include <QKeyEvent>
#include "RenderizacaoStrategy.h"
#include "..\..\model\SimulacaoCaixa.h"


using namespace simulacao::model;


namespace simulacao {
	namespace canvas {
		namespace glWidget{

		class CaixaGraosGLWidget : public QGLWidget {

			Q_OBJECT
		
		private slots:
			void salvarImagem();
		public:
			CaixaGraosGLWidget(QWidget *parent = 0,SimulacaoCaixa *simulacao = 0);	
			void setStrategy(RenderizacaoStrategy *strategy){ this->renderizacaoStrategy = strategy; }
			void posicionarCameraNoTopoDaCaixa();
			void posicionarCameraNoPontoInicial();

		protected:
			void initializeGL();
			void resizeGL(int width, int height);
			void paintGL();
			void mousePressEvent(QMouseEvent *event);
			void mouseMoveEvent(QMouseEvent *event);
			void mouseReleaseEvent(QMouseEvent *event);
			void keyPressEvent ( QKeyEvent * event );

		private:
			void draw();
			void SetupCamera();

			inline void drawCuboid(const NxActor * cuboid, NX_BOOL drawBothSides = NX_FALSE);
			
			NxActor* box;
			SimulacaoCaixa *simulacao;
			NxTriangleMesh * staticTriangleMesh;
			RenderizacaoStrategy *renderizacaoStrategy;

			float gCameraAspectRatio;
			NxVec3 gCameraForward;
			NxVec3 gCameraRight;
			const static NxReal gCameraSpeed;
			NxVec3 gCameraPos;
			QPoint lastPos;


		};
	}
}
}
#endif