#ifndef ELBEHANDLER_H
#define ELBEHANDLER_H

#include <QString>



class ElbeHandler
{
	public:
		ElbeHandler();
		QString createProjectElbeInstance();
		QString checkSystemForElbeVersion();
};

#endif // ELBEHANDLER_H
