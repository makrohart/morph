#pragma once

#include <vector>

#include "FileJournalist.h"

namespace journal
{
    class Journalists
    {
        private:
        Journalists()
        {
            m_journalists.push_back(new FileJournalist());
        }
        Journalists(const Journalists&) = delete;
        Journalists& operator=(const Journalists&) = delete;
        Journalists(Journalists&&) = delete;
        Journalists& operator=(Journalists&&) = delete;

        public:
        ~Journalists()
        {
            for (auto pJournalist : m_journalists)
            {
                if (pJournalist)
                    delete pJournalist;
            }
        }

        static Journalists& getInstance()
        {
            static Journalists s_instance;

            return s_instance;
        }

        std::vector<IJournalist*> getJournalists() const
        {
            return m_journalists;
        }

        private:
        std::vector<IJournalist*> m_journalists;
    };
}