#pragma once

#include <fstream>

#include "IJournalist.h"
#include "json.hpp"

namespace journal
{
    class FileJournalist : public IJournalist
    {
        public:
        FileJournalist();
        
        void write(const std::string& data) override;

        void flush() override;

        private:
        std::string m_journalPath;
        std::fstream m_journal;
    };
}