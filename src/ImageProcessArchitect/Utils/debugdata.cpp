#include "debugdata.h"
#include "SIFLog.h"
#include <fstream>
#include <format>
namespace SIF {

	void saveDebugData(const std::filesystem::path& dir, const std::unordered_map<std::string, int>& data, const std::string& timestamp) {
        // Create the directory
        if (!std::filesystem::exists(dir)) {
            std::error_code ec;
            std::filesystem::create_directories(dir, ec);
            if (ec) {
                Log::error(std::format("Failed to create directory: {}. Error: {}", dir.string(), ec.message()));
                return;
            }
            Log::info(std::format("Directory created: {}", dir.string()));
        }
        
        // Construct the filename
        //static int num{ 1 };
        const std::string filename = timestamp + ".debug";
        //const std::string filename = std::to_string(num) + ".debug";
        const std::filesystem::path filePath = dir / filename;

        // Open the file for writing
        std::ofstream outFile(filePath);
        if (!outFile.is_open()) {
            return;
        }

        // Write the data to the file
        for (const auto& [key, value] : data) {
            outFile << key << ": " << value << std::endl;
        }

        // Close the file
        outFile.close();
	}

}