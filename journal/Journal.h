#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "../aspectable/Aspectable.h"
#include "Journalists.h"
#include "Severity.h"

namespace journal
{
    template<Severity severity>
    class Journal : public aspectable::Aspectable<aspectable::Aspect>
    {
        public:
        Journal() : aspectable::Aspectable<aspectable::Aspect>(this) {}
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

        void log(const std::string& str)
        {
            // Do not invoke log function inside, it will leads to stack overflow
            Journal<severity>() << str.c_str();
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