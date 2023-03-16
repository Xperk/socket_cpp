#include <iostream>
#include <WinSock2.h>

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

    friend std::ostream &operator<<(std::ostream &os, const Fraction &f)
    {
        os << f.numerator << "/" << f.denominator;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Fraction &f)
    {
        is >> f.numerator >> f.denominator;
        return is;
    }
};

int main()
{
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Tạo socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Kết nối đến server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);                  // Port của server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Địa chỉ IP của server
    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Nhập phân số thứ nhất và gửi đi
    Fraction f1;
    std::cout << "Nhap phan so thu nhat: ";
    std::cin >> f1;
    send(clientSocket, (char *)&f1, sizeof(f1), 0);

    // Nhập phân số thứ hai và gửi đi
    Fraction f2;
    std::cout << "Nhap phan so thu hai: ";
    std::cin >> f2;
    send(clientSocket, (char *)&f2, sizeof(f2), 0);

    // Nhận kết quả từ server
    Fraction result1;
    recv(clientSocket, (char *)&result1, sizeof(result1), 0);
    Fraction result2;
    recv(clientSocket, (char *)&result2, sizeof(result2), 0);
    Fraction result3;
    recv(clientSocket, (char *)&result3, sizeof(result3), 0);
    Fraction result4;
    recv(clientSocket, (char *)&result4, sizeof(result4), 0);

    // In kết quả ra màn hình
    std::cout << "Ket qua: " << result1 << std::endl;
    std::cout << "Ket qua: " << result2 << std::endl;
    std::cout << "Ket qua: " << result3 << std::endl;
    std::cout << "Ket qua: " << result4 << std::endl;

    // Đóng kết nối và giải phóng Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
