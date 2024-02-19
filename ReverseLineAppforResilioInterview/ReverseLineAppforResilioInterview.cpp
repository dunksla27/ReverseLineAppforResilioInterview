#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <thread>


static void inputThreadFunction(std::ifstream* inputFile, std::queue<std::string>* inputLines)
{
	std::string inputLine;
	if ((*inputFile).is_open())
	{
		while (std::getline(*inputFile, inputLine))
		{
			(*inputLines).push(inputLine);
		}
	}

	return;
}

static void reverseThreadFunction(bool* inProgress, std::queue<std::string>* inputLines, std::queue<std::string>* outputLines)
{
	std::string inputLine;
	while (*inProgress || !(*inputLines).empty())
	{
		if (!(*inputLines).empty())
		{
			inputLine = (*inputLines).front();
			std::reverse(inputLine.begin(), inputLine.end());
			(*outputLines).push(inputLine);
			(*inputLines).pop();
		}
	}
}

static void outputThreadFunction(bool* inProgress, std::ofstream* outputFile, std::queue<std::string>* outputLines)
{
	while (*inProgress || !(*outputLines).empty())
	{
		if (!(*outputLines).empty())
		{
			*outputFile << (*outputLines).front();
			*outputFile << "\n";
			(*outputLines).pop();
		}
	}

}

int main()
{
	std::ifstream inputFile;																							//declare input file variable
	inputFile.open("LinesForTesting.txt");																				//open input stream

	std::ofstream outputFile;																							//declare output file variable
	outputFile.open("ReversedText.txt");																				//open output stream

	std::queue<std::string> inputLines;																					//queues to share data between threads while maintaining proper order of text lines
	std::queue<std::string> outputLines;

	bool inputInProgress = true;																						//booleans to terminate thread loops as they complete
	bool reverseInProgress = true;

	std::thread inputThread(inputThreadFunction, &inputFile, &inputLines);												//first thread to handle input from file
	std::thread reverseThread(reverseThreadFunction, &inputInProgress, &inputLines, &outputLines);						//second thread to handle reverse string operation
	std::thread outputThread(outputThreadFunction, &reverseInProgress, &outputFile, &outputLines);						//third thread to hanle output to file

	inputThread.join();
	inputInProgress = false;
	reverseThread.join();
	reverseInProgress = false;
	outputThread.join();

}