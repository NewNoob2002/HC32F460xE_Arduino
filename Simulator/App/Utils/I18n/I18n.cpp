#include "I18n.h"

namespace {

#if defined(APP_LANG_RU)
I18n::Language currentLanguage = I18n::Language::Russian;
#else
I18n::Language currentLanguage = I18n::Language::English;
#endif

const I18n::LanguagePack&
GetPack(const I18n::Language language) {
    switch (language) {
        case I18n::Language::Russian: return I18n::RussianPack();
        case I18n::Language::English:
        default: return I18n::EnglishPack();
    }
}

} // namespace

I18n::Language
I18n::GetLanguage() {
    return currentLanguage;
}

bool
I18n::SetLanguage(const Language language) {
#if APP_I18N_DYNAMIC
    currentLanguage = language;
    return true;
#else
    return language == currentLanguage;
#endif
}

const char*
I18n::Text(const TextId id) {
    return Text(currentLanguage, id);
}

const char*
I18n::Text(const Language language, const TextId id) {
    const size_t index = static_cast<size_t>(id);
    if (index >= static_cast<size_t>(TextId::Count)) {
        return "";
    }

    const char* text = GetPack(language).texts[index];
    if (text != nullptr) {
        return text;
    }

    text = EnglishPack().texts[index];
    return text != nullptr ? text : "";
}
