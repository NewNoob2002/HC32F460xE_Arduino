#ifndef I18N_H
#define I18N_H

#include "I18nTextId.h"

#ifndef APP_I18N_DYNAMIC
#define APP_I18N_DYNAMIC 1
#endif

#if defined(RGK_LOGO_USE)
#define APP_LANG_RU
#endif // RGK_LOGO_USE

/* Define APP_LANG_RU for a Russian-only/default build. English is the default. */

namespace I18n {

enum class Language {
    English = 0,
    Russian,
};

struct LanguagePack {
    Language language;
    const char* const* texts;
};

Language GetLanguage();
bool SetLanguage(Language language);
const char* Text(TextId id);
const char* Text(Language language, TextId id);

const LanguagePack& EnglishPack();
const LanguagePack& RussianPack();

} // namespace I18n

#endif
