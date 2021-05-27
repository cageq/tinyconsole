#include "cmd_tree.h"

CmdNode::CmdNode(const std::string &k)
{
    key = k;
}

CmdNode::~CmdNode()
{
    for (auto node : children)
    {
        delete node;
    }
    children.clear();
}
CmdNode *CmdNode::has_child(const std::string &k)
{
    for (auto &n : children)
    {
        if (n != nullptr && n->key == k)
        {
            return n;
        }
    }
    return nullptr;
}
CmdNode *CmdNode::add_child(const std::string &k)
{
    auto n = new CmdNode(k);
    children.push_back(n);
    return n;
}

void CmdTree::add_command(const std::string &cmd, CommandHandler handler)
{
    auto cmds = split(cmd);
    CmdNode *curNode = &root_node;
    for (auto &c : cmds)
    {

        auto *node = curNode->has_child(c);
        if (node == nullptr)
        {
            curNode = curNode->add_child(c);
            curNode->handler = handler;
        }
        else
        {
            curNode = node;
        }
    }
    curNode->handler = handler;
}

std::string CmdTree::get_completion(const std::string &input)
{
    std::string hints = "";
    auto cmds = split(input);

    CmdNode *curNode = &root_node;
    for (auto &c : cmds)
    {
        auto *node = curNode->has_child(c);
        if (node == nullptr)
        {
            if (!curNode->children.empty())
            {
                for (uint32_t i = 0; i < curNode->children.size(); i++)
                {
                    if (compare_key(c, curNode->children[i]->key))
                    {
                        auto keyStr = curNode->children[i]->key;
                        hints += keyStr + " ";
                        return hints;
                    }
                }
            }
        }
        else
        {
            hints += node->key + " ";
            curNode = node;
        }
    }

    return "";
}

char *CmdTree::get_hint(const std::string &input)
{
    static char hints[128] = {0};

    auto cmds = split(input);
    CmdNode *curNode = &root_node;
    for (auto &c : cmds)
    {
        auto *node = curNode->has_child(c);
        if (node == nullptr)
        {
            if (!curNode->children.empty())
            {
                for (uint32_t i = 0; i < curNode->children.size(); i++)
                {

                    if (compare_key(c, curNode->children[i]->key))
                    {
                        auto keyStr = curNode->children[i]->key;
                        if (keyStr.length() > c.length())
                        {
                            memcpy(hints, keyStr.c_str() + c.length(), keyStr.length() - c.length());
                            hints[keyStr.length() - c.length()] = 0;
                            return hints;
                        }
                    }
                }
            }
        }
        else
        {
            curNode = node;
        }
    }

    return nullptr;
}

bool CmdTree::compare_key(const std::string &input, const std::string &key)
{

    for (uint32_t i = 0; i < input.length(); i++)
    {
        if (i < key.length() && key[i] != input[i])
        {
            return false;
        }
    }
    return true;
}

std::vector<std::string> CmdTree::split(const std::string &in, const std::string &delim)
{
    std::regex re{delim};
    return std::vector<std::string>{
        std::sregex_token_iterator(in.begin(), in.end(), re, -1),
        std::sregex_token_iterator()};
}

void CmdTree::execute(const std::string &cmd)
{

    printf("execute command :%s\n", cmd.c_str());
    auto cmds = split(cmd);
    CmdNode *curNode = &root_node;

    for (auto &c : cmds)
    {
        auto *node = curNode->has_child(c);
        if (node != nullptr)
        {
            curNode = node;
        }
    }

    if (curNode->handler)
    {
        curNode->handler();
    }
    else
    {
        perror("invalid command\n");
    }
}
