#include "ircconnection.h"

IrcConnection::IrcConnection()
{
}

IrcConnection::IrcConnection(string _server, int _port, string _channel, void(*_callback)(string), string _nick)
{
	strcpy_s(server_addr, _server.c_str());
	port = _port;
	channel = _channel;
	nick = _nick;
	callback = _callback;
}

bool IrcConnection::connectToServer()
{
	WSADATA wsd;
	struct sockaddr_in server;
	struct hostent *host = NULL;

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "Failed to load Winsock library!" << endl;
		return false;
	}

	if (client == INVALID_SOCKET)
	{
		cout << "Creating socket has failed!" << endl;
		return false;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.S_un.S_addr = inet_addr(server_addr);

	if (server.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		host = gethostbyname(server_addr);
		if (host == NULL)
		{
			cout << "Could not resolve hostname!" << endl;
			return false;
		}
	}

	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connect(client, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		cout << "Could not connect to server!" << endl;
		return false;
	}
	sendMessage("NICK " + nick + "\r\n");
	sendMessage("USER " + nick + " " + nick + " " + nick + " " + nick + "\r\n");
	return true;
}

void IrcConnection::read()
{
	int ret;
	char receive_buffer[DEFAULT_BUFFER];

	while (true) {
		ret = recv(client, receive_buffer, DEFAULT_BUFFER, 0);
		if (ret == 0) {
			break;
		}
		else if (ret == SOCKET_ERROR) {
			cout << "Socket error!" << endl;
			break;
		}
		receive_buffer[ret] = '\0';
		callback(receive_buffer);
	}
	closesocket(client);
}

void IrcConnection::join(string channel)
{
	sendMessage("JOIN " + channel + "\r\n");
}


IrcConnection::IrcConnection(string _server, int _port, string _channel, void(*_callback)(string))
{
	IrcConnection(_server, _port, _channel, _callback, "legitsoft");
}

void IrcConnection::sendMessage(string _message)
{
	char cmessage[4096];
	strcpy_s(cmessage, _message.c_str());
	int ret = send(client, cmessage, strlen(cmessage), 0);
	if (ret == 0) {
		cout << "Sending message: " << cmessage << endl;
	}
	else if (ret == SOCKET_ERROR) {
		cout << "ERROR in sending message!" << endl;
	}
}