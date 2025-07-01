#pragma once

#include <vector>

namespace morph
{
    struct TreeNode
    {
        TreeNode(){}
        ~TreeNode(){}

        void add(TreeNode* pChild)
        {

        }

        void  remove(TreeNode* pChild)
        {

        }

        private:
        int m_id;
        TreeNode* m_pParent;
        std::vector<TreeNode*> m_pChildren;
    }
}