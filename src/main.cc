#include <QApplication>

#include "blazarwidget.h"

int main(int argc, char *argv[]) {
	QApplication app(argc,argv);


    BlazarWidget w;

    w.init();

    w.show();

	return app.exec();
}
