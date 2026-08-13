#ifndef UTILS_H
#define UTILS_H

#include "Object.h"
#include <string>

OptionButtonType strToOption(std::string str);
TitleButtonType strToTitle(std::string str);
UiItemType strToUi(std::string str);
EndItemType strToEnd(std::string str);
ItemType strToItem(std::string str);

#endif