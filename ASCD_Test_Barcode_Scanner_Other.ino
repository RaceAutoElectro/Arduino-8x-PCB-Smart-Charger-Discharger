#include <hidboot.h> //Barcode Scanner
#include <usbhub.h> //Barcode Scanner
#include <hiduniversal.h>  //Barcode Scanner

// USB Host Shield - Barcode Scanner
class KbdRptParser : public KeyboardReportParser
{

protected:
	virtual void OnKeyDown  (uint8_t mod, uint8_t key);
	virtual void OnKeyPressed(uint8_t key);
};

KbdRptParser Prs;
USB     Usb;
HIDUniversal      Hid(&Usb);    
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

// Get Barcode
char barcodeString[25] = "";
bool barcodeStringCompleted = false;


void setup()
{
	Serial.begin(9600);  
	char usbInit[25];
	sprintf(usbInit, "%-20s", Usb.Init() == -1 ? "USB: Did not start" : "USB: Started");
	Serial.println(usbInit);
  Hid.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
	Usb.Task(); //Barcode Scanner
}

void processBarcode(int keyInput)
{
	//Barcode Scanner
	if (keyInput == 19) //Return Carriage ASCII Numeric 19
	{
		barcodeStringCompleted = true;
		Serial.println(barcodeString);
		strcpy(barcodeString, "");
	} else {
		sprintf(barcodeString, "%s%c", barcodeString, (char)keyInput);
		barcodeStringCompleted = false;
	}
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)  
{
	//Barcode Scanner
	uint8_t c = OemToAscii(mod, key);
	if (c) OnKeyPressed(c);
}

void KbdRptParser::OnKeyPressed(uint8_t key)  
{
	//Barcode Scanner
	processBarcode(key);
};
