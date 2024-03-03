#include <QApplication>

#include "control/bzactions.h"
#include "blazarwidget.h"

int main(int argc, char *argv[]) {
	QApplication app(argc,argv);

    BzActions actions;
    BlazarWidget w;

    w.init(actions);

    w.show();


    actions.bind(".","incTimeScale");
    actions.bind(",","decTimeScale");


    actions.onSimpleEvent("quit", &app, [&]() {
        app.quit();
    });

	return app.exec();
}
