/*
 * QR Code generator demo (C++)
 */

#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "qrcodegen.hpp"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std;

struct PersonInfo
{
	string name;
	string dob;
	string address;
	string company;
	string signs;

	PersonInfo() : name{""}, dob{""}, address{""}, company{""}, signs{""} {}
	void InputData();
};

void PersonInfo::InputData()
{
	cout << "Enter name : ";
	getline(cin, this->name);
	cout << "\nEnter dob : ";
	getline(cin, this->dob);
	cout << "\nEnter address : ";
	getline(cin, this->address);
	cout << "\nEnter company : ";
	getline(cin, this->company);
	cout << "\nEnter signs : ";
	getline(cin, this->signs);
}

class HealthDeclaration
{
public:
	HealthDeclaration();
	~HealthDeclaration();
	void SetInfomation();
	void printQr(const QrCode& qr);
	void CreateQrCode();
	string toSvgString(const QrCode& qr, int border);

private:
	PersonInfo person;
};

HealthDeclaration::HealthDeclaration()
{

}

HealthDeclaration::~HealthDeclaration()
{

}

void HealthDeclaration::SetInfomation()
{
	this->person.InputData();
}


// Creates a single QR Code, then prints it to the console.
void HealthDeclaration::CreateQrCode() {
	//const char* text = "Hello,world" ;
	//text = (person.name + person.dob + person.address + person.company + person.signs).c_str();

	string text = "Name: "+this->person.name +", Dob:"+ this->person.dob + ", Address:" + this->person.address + ", Company:" + this->person.company + ", Signs:" + this->person.signs;
	const QrCode::Ecc errCorLvl = QrCode::Ecc::HIGH;

	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text.c_str(), errCorLvl);
	printQr(qr);
	cout << toSvgString(qr, 4) << endl;
}

// Prints the given QrCode object to the console.
void HealthDeclaration::printQr(const QrCode& qr) {
	int border = 4;
	for (int y = -border; y < qr.getSize() + border; y++) {
		for (int x = -border; x < qr.getSize() + border; x++) {
			std::cout << (qr.getModule(x, y) ? "##" : "  ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
string HealthDeclaration::toSvgString(const QrCode& qr, int border) {
	if (border < 0)
		throw std::domain_error("Border must be non-negative");
	if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
		throw std::overflow_error("Border too large");

	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	sb << "\t<path d=\"";
	for (int y = 0; y < qr.getSize(); y++) {
		for (int x = 0; x < qr.getSize(); x++) {
			if (qr.getModule(x, y)) {
				if (x != 0 || y != 0)
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";
	return sb.str();
}

int main()
{
	HealthDeclaration person;
	person.SetInfomation();
	person.CreateQrCode();
}
