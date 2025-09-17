#include "FileJournalist.h"

namespace
{
    const char JSExtention[] = ".txt";

    const std::vector<std::string> JournalConfigPaths
	{
		"Journal.json",                        // Default location
        "../../PlayableJournal/Journal.json"   // Fallback location
	};

    nlohmann::json GetJournalConfig()
    {
        std::string json;
        for (const auto& journalConfigPath : JournalConfigPaths)
        {
            if (std::filesystem::exists(journalConfigPath))
            {
                json = journalConfigPath;
                break;
            }
        }
        std::ifstream jsonStream(json);
        return nlohmann::json::parse(jsonStream);
    }
}

journal::FileJournalist::FileJournalist()
{
	const nlohmann::json journalConfig = GetJournalConfig();
	const std::string fileName = journalConfig["Name"];
	const std::string fileDirectory = journalConfig["Path"]["Journal"];

	std::string filePath = fileDirectory + std::string(fileName) + std::string(JSExtention);
	int postfix = 1;
	while (std::filesystem::exists(filePath.c_str()))
	{
		std::ostringstream oss;
		oss << fileDirectory << fileName << "." << std::setw(4) << std::setfill('0') << postfix++ << JSExtention;
		filePath = oss.str();
	}

    m_journalPath = filePath;
    m_journal.open(m_journalPath, std::fstream::out | std::fstream::app);
}

void journal::FileJournalist::write(const std::string& data)
{
    m_journal << data.c_str();
}

void journal::FileJournalist::flush()
{
    m_journal.flush();
}