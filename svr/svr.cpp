#include <iostream>
#include <WinSock2.h>
#include <numeric>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

class Fraction
{
public:
	int numerator;
	int denominator;
	// Fraction(int n = 0, int d = 1) : numerator(n), denominator(d) {}
	Fraction(int n = 0, int d = 1)
	{
		numerator = n;
		denominator = d;
	}

	friend std::ostream &operator<<(std::ostream &os, Fraction &f)
	{
		os << f.numerator << f.denominator;
		return os;
	}
	friend std::istream &operator>>(std::istream &is, Fraction &f)
	{
		is >> f.numerator >> f.denominator;
		return is;
	}

	// Fraction reduce(Fraction &f)
	// {
	// 	int a = f.numerator;
	// 	int b = f.denominator;
	// 	while (a != b)
	// 	{
	// 		if (a < b)
	// 			a -= b;
	// 		else
	// 			b -= a;
	// 	}
	// 	f.numerator /= a;
	// 	f.denominator /= a;
	// 	return f;
	// }

	Fraction operator+(const Fraction &other) const
	{
		int n = numerator * other.denominator + other.numerator * denominator;
		int d = denominator * other.denominator;

		int b = n, c = d;
		while (b != c)
		{
			if (b < c)
				c -= b;
			else
				b -= c;
		}
		int a = c;
		n /= a;
		d /= a;

		return Fraction(n, d);
	}

	Fraction operator-(const Fraction &other) const
	{
		int n = numerator * other.denominator - other.numerator * denominator;
		int d = denominator * other.denominator;

		int b = n, c = d;
		while (b != c)
		{
			if (b < c)
				c -= b;
			else
				b -= c;
		}
		int a = c;
		n /= a;
		d /= a;

		return Fraction(n, d);
	}

	Fraction operator*(const Fraction &other) const
	{
		int n = numerator * other.numerator;
		int d = denominator * other.denominator;

		int b = n, c = d;
		while (b != c)
		{
			if (b < c)
				c -= b;
			else
				b -= c;
		}
		int a = c;
		n /= a;
		d /= a;

		return Fraction(n, d);
	}

	Fraction operator/(const Fraction &other) const
	{
		int n = numerator * other.denominator;
		int d = denominator * other.numerator;

		int b = n, c = d;
		while (b != c)
		{
			if (b < c)
				c -= b;
			else
				b -= c;
		}
		int a = c;
		n /= a;
		d /= a;

		return Fraction(n, d);
	}
};

int main()
{
	do
	{
		/* code */
		// Khởi tạo Winsock
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cerr << "WSAStartup failed." << std::endl;
			return 1;
		}

		// Tạo socket
		SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listenSocket == INVALID_SOCKET)
		{
			std::cerr << "Failed to create socket." << std::endl;
			WSACleanup();
			return 1;
		}

		// Bind socket đến địa chỉ và port
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(12345); // Port để lắng nghe
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		if (bind(listenSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			std::cerr << "Failed to bind socket." << std::endl;
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		// Lắng nghe kết nối từ client
		if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			std::cerr << "Failed to listen on socket." << std::endl;
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		// Chấp nhận kết nối từ client
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "Failed to accept connection." << std::endl;
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		// Nhận phân số thứ nhất từ client
		Fraction f1;
		recv(clientSocket, (char *)&f1, sizeof(f1), 0);

		// Nhận phân số thứ hai từ client
		Fraction f2;
		recv(clientSocket, (char *)&f2, sizeof(f2), 0);

		// Thực hiện các phép tính
		Fraction result1 = f1 + f2;
		Fraction result2 = f1 - f2;
		Fraction result3 = f1 * f2;
		Fraction result4 = f1 / f2;

		// Gửi kết quả

		send(clientSocket, (char *)&result1, sizeof(result1), 0);
		send(clientSocket, (char *)&result2, sizeof(result2), 0);
		send(clientSocket, (char *)&result3, sizeof(result3), 0);
		send(clientSocket, (char *)&result4, sizeof(result4), 0);

		// Đóng kết nối
		closesocket(clientSocket);
		closesocket(listenSocket);
		WSACleanup();
	} while (1);

	return 0;
}
