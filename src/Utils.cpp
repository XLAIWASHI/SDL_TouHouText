#include "Utils.h"

OptionButtonType strToOption(std::string str)
{
    if(str == "vol") return OptionButtonType::vol;
    if(str == "sevol") return OptionButtonType::sevol;
    if(str == "mode") return OptionButtonType::mode;
    if(str == "quit") return OptionButtonType::quit;
}

TitleButtonType strToTitle(std::string str)
{
    if(str == "start") return TitleButtonType::start;
    if(str == "option") return TitleButtonType::option;
    if(str == "quit") return TitleButtonType::quit;
}
