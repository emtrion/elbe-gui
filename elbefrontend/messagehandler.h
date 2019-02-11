#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QAbstractMessageHandler>

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
		QString m_description;
		QSourceLocation m_sourceLocation;
};

#endif // MESSAGEHANDLER_H
