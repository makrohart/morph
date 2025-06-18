#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>

#include "Journalists.h"
#include "Severity.h"

namespace journal
{
    template<Severity severity>
    class Journal
    {
        public:   
        ~Journal()
        {
            std::string data = "// " + getTimeStamp() + " [" + ToStringFrom<severity>() + "] " + m_data + "\n";
            auto journalist = Journalists::getInstance().getJournalists();
            for (auto pJournalist : journalist)
            {
                if (!pJournalist)
                    continue;
                
                pJournalist->write(data);
                pJournalist->flush();
            }
        }

        Journal& operator<<(const char* pStr)
        {
            m_data += pStr;
            return  *this;
        }

        private:
        std::string getTimeStamp() const
        {
            std::time_t t = std::time(nullptr);
            std::tm localTime = *std::localtime(&t);
            std::stringstream time;
            time << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
            return time.str();
	    }

        private:
        std::string m_data;
    };
}