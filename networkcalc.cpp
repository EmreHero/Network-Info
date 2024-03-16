#include <iostream>
#include <cstring>

int main () {
	
	std::string ip = ""; // input ip
	
	char allowedChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '/'};
	
	int values[] = {0, 0, 0, 0, 0}; // for 192.16.87.21/24, [0] = 192, [1] = 16, [2] = 87, [3] = 21, [4] = 24
	
	unsigned int valuesIndex = 0, howManySlash = 0, howManyDot = 0, range, x, hosts = 1;
	bool isAllowed, isNetMaskEqualsZero = true;
	int netMask[4], netID[4], broadcast[4], first[4], last[4];
	
	
	std::cout << "Type in the ip address:\n>> ";
	std::cin >> ip;
	
	// detecting errors
	for (int i = 0; i < ip.length(); i++) {
		
		isAllowed = false;
		for (int j = 0; j < 12; j++) { // the input must be numbers, dot or slash
			
			if(ip.at(i) == allowedChars[j]) {
				isAllowed = true;
				break;
			}
		}
		if (!isAllowed) { // wrong input
			std::cout << "[ERROR]: Such an IP address does not exist. (\"" << ip.at(i) << "\" is a disallowed character. 0-9, \".\" and \"/\" are allowed.)\n";
			system("pause");
			return 0;
		}
		
		if (ip.at(i) == '.') { // for storing and controling dot count
			valuesIndex++;
			howManyDot++;
		}
		else if (ip.at(i) == '/') {// for storing and controling slash count
			valuesIndex++;
			howManySlash++;
		}
		else { // if current char is a number
			values[valuesIndex] = values[valuesIndex] * 10 + ip.at(i) - '0'; // converting char to int
		}
		
		if (i == ip.length() - 1) { // when full input string has controlled
			if (howManySlash != 1 || howManyDot != 3) { // wrong dot or slash count
				std::cout << "[ERROR]: Such an IP address does not exist. (";
				
				if (howManyDot != 3) { // wrong dot count
					if(howManyDot == 1) { // there "is" ...
						std::cout << "There is 1 character \".\", should be 3.";
					}
					else { // there "are" ...
						std::cout << "There are " << howManyDot << " character \".\", should be 3.";
					}
				}
				if (howManyDot != 3 && howManySlash != 1) { // if both are wrong
					std::cout << " ";
				}
				if (howManySlash != 1) { // wrong slash count
					std::cout << "There are " << howManySlash << " character \"/\", should be 1.";
				}
				std::cout << ")\n";
				system("pause");
				return 0;
			}
			for (int j = 0; j < 4; j++) { // if segments is higher than 8bits
				
				if (values[j] > 255) {
					std::cout << "[ERROR]: Such an IP address does not exist. (ip address\' values cannot be above 255)\n";
					system("pause");
					return 0;
				}	
			}
			if (values[4] > 32) { // if subnet mask is higher than 32
				std::cout << "[ERROR]: Such an IP address does not exist. (slash value cannot be above 32)\n";
				system("pause");
				return 0;
			}
		}
	}
	
	// calculating maximum host count
	for (int i = 0; i < 32 - values[4]; i++) {
		hosts *= 2;
	}
	hosts -= 2; // broadcast and network id

	// other logic part, I tried to calculate as short as possible I could
	for (int i = 0; i < 4; i++) {
		if(i < 3 - (32 - values[4])/8) {
			netMask[i] = 255;
			netID[i] = values[i];
			broadcast[i] = values[i];
		}
		else if (i == 3 - (32 - values[4])/8) {
			
			if (values[4] % 8 == 0) {
				netMask[i] = 255;
				netID[i] = values[i];
				broadcast[i] = values[i];
			}
			else {
				netMask[i] = 0;				
				for(int j = 0; j < values[4] % 8; j++) {
					x = 1;
					for(int k = 0; k < j; k++) {
						x *= 2;
					}
					netMask[i] += x;
				}
				x = 1;
				for (int j = 0; j < 8 - values[4]%8; j++) {
					x *= 2;
				}
				netMask[i] *= x;
				
				netID[i] = 0;
				range = 1; 
				for(int j = 0; j < values[4]%8; j++) {
					range *= 2;
				}
				range = 256/range;

				while (values[i] >= netID[i]) {
					netID[i] += range;
				}
				netID[i] -= range;
				broadcast[i] = netID[i] + range - 1;
			}
		}
		else {
			netMask[i] = 0;
			netID[i] = 0;
			broadcast[i] = 255;
		}
		first[i] = netID[i];
		last[i] = broadcast[i];
			
		if (i == 3) {
			first[i]++;
			last[i]--;
		}
	}
	
	// printing values
	std::cout << "\nHosts: " << hosts << "\nSubnet: ";
	for (int i = 0; i < 4; i++) {
		std::cout << netMask[i];
		if (i < 3) {
			std::cout << ".";
		}
	}
	std::cout << "\nNetwork ID: ";
	for (int i = 0; i < 4; i++) {
		std::cout << netID[i];
		if (i < 3) {
			std::cout << ".";
		}
	}
	std::cout << "\nBroadcast IP: ";
	for (int i = 0; i < 4; i++) {
		std::cout << broadcast[i];
		if (i < 3) {
			std::cout << ".";
		}
	}
	std::cout << "\nFirst IP: ";
	for (int i = 0; i < 4; i++) {
		std::cout << first[i];
		if (i < 3) {
			std::cout << ".";
		}
	}
	std::cout << "\nLast IP: ";
	for (int i = 0; i < 4; i++) {
		std::cout << last[i];
		if (i < 3) {
			std::cout << ".";
		}
	}
	std::cout << "\n";
	system("pause");
}
