#include <QApplication>

#include "blazerwidget.h"

int main(int argc, char *argv[]) {
	QApplication app(argc,argv);


    BlazerWidget w;

    w.init();

    w.show();

	return app.exec();
}
