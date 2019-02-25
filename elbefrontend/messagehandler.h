#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QAbstractMessageHandler>

/* This is the messagehandler for the xml validator.
 * It defines how the messages after a validation are formatted
 */

class MessageHandler : public QAbstractMessageHandler
{
	public:
		MessageHandler();
		QString statusMessage() const;
		int line() const;
		int column() const;

	protected:
		virtual void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

	private:
		QString description;
		QSourceLocation sourceLocation;
};

#endif // MESSAGEHANDLER_H
