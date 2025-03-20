#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Token {
	std::string sourceIPAddress = "", destinationIPAddress = "", message = "Hello, world!";
	bool isOccupied = false, destinationReached = false;
};

struct Calculator {
	std::string name, IPAdress, buffer;
};

std::vector<Calculator> InitializeRing() {
	std::vector<Calculator> ring(10);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, 255);

	for (int i = 0; i < 10; i++) {
		ring[i].name = "C" + std::to_string(i);
		ring[i].buffer = "null";
		ring[i].IPAdress = "";

		for (int j = 0; j < 4; j++) {
			if (j > 0) ring[i].IPAdress += ".";
			ring[i].IPAdress += std::to_string(distrib(gen));
		}
	}

	return ring;
}

void Iteration(std::vector<Calculator>& ring, int& firstSource, Token& token) {
	int source, destination;
	std::random_device rd;
	std::mt19937 gen(rd());

	do {
		std::uniform_int_distribution<int> distrib_source(0, 9);
		source = distrib_source(gen);
		destination = distrib_source(gen);

	} while (source == destination);

	for (int i = 0; i < 10; i++) {
		std::cout << ring[i].name << " (" << ring[i].IPAdress << ") -> " << ring[i].buffer << std::endl;
	}

	if (firstSource == -1) {
		firstSource = source;
	}

	std::cout << "\nSource: " << ring[source].name << std::endl;
	std::cout << "Destination: " << ring[destination].name << std::endl;
	std::cout << std::endl;

	token.destinationIPAddress = ring[destination].IPAdress;
	token.sourceIPAddress = ring[source].IPAdress;

	if (token.isOccupied == false) {

		token.isOccupied = true;

		if (source != firstSource) {
			int i = firstSource;

			while (ring[i].IPAdress != ring[source].IPAdress) {
				std::cout << ring[i].name << ": Moving the jeton." << std::endl;
				i++;
				if (i == 10) {
					i = 0;
				}
			}

			std::cout << ring[i].name << ": The jeton is taken." << std::endl;
		}

		int j = source;
		while (ring[j].IPAdress != ring[destination].IPAdress) {
			std::cout << ring[j].name << ": Moving the jeton." << std::endl;
			j++;
			if (j == 10) {
				j = 0;
			}
		}
		if (ring[j].buffer == "null") {
			ring[j].buffer = token.message;
		}
		else {
			ring[j].buffer += "; " + token.message;
		}
		token.destinationReached = true;
		std::cout << ring[j].name << ": Destination reached. Buffer: " << ring[j].buffer << std::endl;

		while (ring[j].IPAdress != ring[source].IPAdress) {
			std::cout << ring[j].name << ": Moving the jeton." << std::endl;
			j++;
			if (j == 10) {
				j = 0;
			}
		}

		std::cout << ring[j].name << ": Back to the source. The jeton is free." << std::endl;
		token.isOccupied = false;
		token.destinationReached = false;
		token.sourceIPAddress = "";
		token.destinationIPAddress = "";
	}
	else {
		std::cout << "The jeton is occupied." << std::endl;
	}
	firstSource = source;
}

void ShowRing(const std::vector<Calculator>& ring) {
	for (int i = 0; i < 10; i++) {
		std::cout << ring[i].name << " (" << ring[i].IPAdress << ")" << std::endl;
	}
}

int main()
{
	Token token;
	std::vector<Calculator> ring(10);
	int firstSource = -1;

	ring = InitializeRing();

	std::cout << "The ring is:" << std::endl;
	ShowRing(ring);
	std::cout << std::endl;

	for (int i = 0; i < 10; i++) {
		std::string separator(30, '-');
		std::cout << separator;
		std::cout << "\nIteration " << i + 1 << ":" << std::endl;
		Iteration(ring, firstSource, token);
	}
	return 0;
}