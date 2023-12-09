#include <QtWidgets>
Int main(int argc, char **argv)
{
	QApplication app(argc,argv);
	QLabel *hello = new QLabel( “<font color=blue>Hello”
			“ <i>Qt World!</i></font>”, 0 );
	hello->show();
	return app.exec();
}
