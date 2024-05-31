#pragma once
#include <filesystem>
#include <unordered_map>
namespace SIF {

	void saveDebugData(const std::filesystem::path& dir, const std::unordered_map<std::string, int>& data, const std::string& timestamp);

}