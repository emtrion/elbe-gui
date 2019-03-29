#include "messagehandler.h"

MessageHandler::MessageHandler() :
	QAbstractMessageHandler(0)
{

}

QString MessageHandler::statusMessage() const
{
	return description;
}

int MessageHandler::line() const
{
	return sourceLocation.line();
}

int MessageHandler::column() const
{
	return sourceLocation.column();
}

void MessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation)
{
	Q_UNUSED(type);
	Q_UNUSED(identifier);

	this->description = description;
	this->sourceLocation = sourceLocation;
}
