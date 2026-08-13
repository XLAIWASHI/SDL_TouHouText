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

UiItemType strToUi(std::string str)
{
    if(str == "hiScore") return UiItemType::hiScore;
    if(str == "score") return UiItemType::score;
    if(str == "player") return UiItemType::player;
    if(str == "spell") return UiItemType::spell;
    if(str == "health") return UiItemType::health;
    if(str == "bomb") return UiItemType::bomb;
    if(str == "title") return UiItemType::title;
}

EndItemType strToEnd(std::string str)
{
    if(str == "finishScore") return EndItemType::finishScore;
}

ItemType strToItem(std::string str)
{
    if(str == "point") return ItemType::point;
    if(str == "power") return ItemType::power;
    if(str == "bomb") return ItemType::bomb;
    if(str == "life") return ItemType::life;
}
