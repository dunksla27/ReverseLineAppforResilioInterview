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
		while (std::getline(*inputFile, inputLine))																		//iterate input of lines to queue until end of file
		{
			(*inputLines).push(inputLine);
		}
	}

	return;
}

static void reverseThreadFunction(bool* inProgress, std::queue<std::string>* inputLines, std::queue<std::string>* outputLines)
{
	std::string inputLine;
	while (*inProgress)																									//iterate reversal of lines in queue until previous thread is comfirmed to terminate
	{
		if (!(*inputLines).empty())
		{
			inputLine = (*inputLines).front();
			std::reverse(inputLine.begin(), inputLine.end());
			(*outputLines).push(inputLine);
			(*inputLines).pop();
		}
	}

	return;
}

static void outputThreadFunction(bool* inProgress, std::ofstream* outputFile, std::queue<std::string>* outputLines)
{
	bool firstLine = true;
	while (firstLine)																									//used separate loop for first line to prevent preceeding or trailing line break in the output file
	{
		if (!(*outputLines).empty())
		{
			*outputFile << (*outputLines).front();
			(*outputLines).pop();
			firstLine = false;
		}
	}
	while (*inProgress)																									//iterate output of lines from queue until previous thread is comfirmed to terminate
	{
		if (!(*outputLines).empty())
		{
			*outputFile << "\n";
			*outputFile << (*outputLines).front();
			(*outputLines).pop();
		}
	}
	return;
}

int main()
{
	std::ifstream inputFile;																							//declare input file variable
	inputFile.open("LinesForTesting.txt");																				//open input stream

	std::ofstream outputFile;																							//declare output file variable
	outputFile.open("ReversedText.txt");																				//open output stream

	std::queue<std::string> inputLines;																					//queues to share data between threads while maintaining proper order of text lines
	std::queue<std::string> outputLines;																				//

	bool inputInProgress = true;																						//booleans to terminate successive thread loops as they complete
	bool reverseInProgress = true;																						//

	std::thread inputThread(inputThreadFunction, &inputFile, &inputLines);												//first thread to handle input from file
	std::thread reverseThread(reverseThreadFunction, &inputInProgress, &inputLines, &outputLines);						//second thread to handle reverse string operation
	std::thread outputThread(outputThreadFunction, &reverseInProgress, &outputFile, &outputLines);						//third thread to hanle output to file

	inputThread.join();																									//wait for input thread to complete
	inputInProgress = false;																							//notify text reversal thread that input is complete
	reverseThread.join();																								//wait for reverse thread to complete
	reverseInProgress = false;																							//notify output threat that text reversal is complete
	outputThread.join();																								//wait for output thread to complete

	inputFile.close();																									//ensure file streams close before termination
	outputFile.close();																									//

	return 0;																											//terminate main
}