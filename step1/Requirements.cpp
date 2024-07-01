#include "pch.h"
#include "Requirements.hpp"
#include "Pilot.hpp"
#include "Programmer.hpp"
#include "Job.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

void Requirements::DynamicAllocation()
{
	std::vector<std::unique_ptr<Job>> jobs;
	jobs.push_back(std::make_unique<Pilot>());
	jobs.push_back(std::make_unique<Programmer>());

	for (const auto& jobber : jobs)
	{
		jobber->Action1();
		jobber->Action2();
		jobber->Action3();
		jobber->Action4();
	}
}

size_t Requirements::ContainsTheString(bool (*func)(const std::string&), const std::vector<std::string>& strings)
{
	auto printMatches = [&](const std::string& value)
	{
		if (func(value))
		{
			std::cout << value << std::endl;
			return true;
		}
		return false;
	};
	return std::count_if(strings.cbegin(), strings.cend(), printMatches);
}

std::vector<std::vector<std::string>> Requirements::Transpose(const std::vector<std::vector<std::string>>& values)
{
	// write the file
	std::FILE* tmpFile = std::tmpfile(); // I want a temp file for all the benefits of temp file. Plus then I don't have to fret on OS. Or clutter the disk.
	for(const std::vector<std::string>& row : values)
	{
		for (const std::string& value : row)
		{
			std::fputs(value.c_str(), tmpFile);
			std::fputs(",", tmpFile);
		}
		std::fputs("\n", tmpFile);
	}

	// read it back in (which is weird)
	std::rewind(tmpFile);
	std::vector<std::vector<std::string>> readValues;

	char buffer[1024];
	while (std::fgets(buffer, 1024, tmpFile))
	{
		std::string line = buffer;
		std::vector<std::string> row;
		size_t position_start = 0;
		size_t position_end = 0;
		size_t delimiter_length = 1;
		while ((position_end = line.find(",", position_start)) != std::string::npos)
		{
			const std::string cell = line.substr(position_start, position_end - position_start);
			row.push_back(cell);
			position_start = position_end + delimiter_length;
		}
		readValues.push_back(row);
	}

	std::fclose(tmpFile);

	// pivot the data. Hard assumptions on uniformity
	std::vector<std::vector<std::string>> pivotValues(readValues[0].size(), std::vector<std::string>(readValues.size()));
	for (std::vector<std::string>::size_type i = 0; i < readValues[0].size(); i++)
	{
		for (std::vector<std::string>::size_type j = 0; j < readValues.size(); j++)
		{
			pivotValues[i][j] = readValues[j][i];
		}
	}
	return pivotValues;
}