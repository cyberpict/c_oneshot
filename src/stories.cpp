#include "stories.h"

#include <QHash>

namespace madlibs {

WordType wordTypeFromString(const QString &name)
{
    static const QHash<QString, WordType> map = {
        {"noun",        WordType::Noun},
        {"plural_noun", WordType::PluralNoun},
        {"verb",        WordType::Verb},
        {"past_verb",   WordType::PastTenseVerb},
        {"adjective",   WordType::Adjective},
        {"adverb",      WordType::Adverb},
        {"exclamation", WordType::Exclamation},
        {"person",      WordType::Person},
        {"place",       WordType::Place},
        {"animal",      WordType::Animal},
        {"color",       WordType::Color},
        {"food",        WordType::Food},
        {"number",      WordType::Number},
    };
    return map.value(name.trimmed(), WordType::Noun);
}

QString wordTypeQuestion(WordType t)
{
    switch (t) {
    case WordType::Noun:          return QStringLiteral("a noun");
    case WordType::PluralNoun:    return QStringLiteral("a plural noun");
    case WordType::Verb:          return QStringLiteral("a verb (present tense)");
    case WordType::PastTenseVerb: return QStringLiteral("a verb (past tense)");
    case WordType::Adjective:     return QStringLiteral("an adjective");
    case WordType::Adverb:        return QStringLiteral("an adverb");
    case WordType::Exclamation:   return QStringLiteral("an exclamation");
    case WordType::Person:        return QStringLiteral("a person's name");
    case WordType::Place:         return QStringLiteral("a place");
    case WordType::Animal:        return QStringLiteral("a kind of animal");
    case WordType::Color:         return QStringLiteral("a color");
    case WordType::Food:          return QStringLiteral("a food");
    case WordType::Number:        return QStringLiteral("a number");
    }
    return QStringLiteral("a word");
}

QString wordTypeToString(WordType t)
{
    switch (t) {
    case WordType::Noun:          return QStringLiteral("noun");
    case WordType::PluralNoun:    return QStringLiteral("plural noun");
    case WordType::Verb:          return QStringLiteral("verb");
    case WordType::PastTenseVerb: return QStringLiteral("past-tense verb");
    case WordType::Adjective:     return QStringLiteral("adjective");
    case WordType::Adverb:        return QStringLiteral("adverb");
    case WordType::Exclamation:   return QStringLiteral("exclamation");
    case WordType::Person:        return QStringLiteral("person");
    case WordType::Place:         return QStringLiteral("place");
    case WordType::Animal:        return QStringLiteral("animal");
    case WordType::Color:         return QStringLiteral("color");
    case WordType::Food:          return QStringLiteral("food");
    case WordType::Number:        return QStringLiteral("number");
    }
    return QStringLiteral("word");
}

QVector<Story> allStories()
{
    // Template grammar: {{N}} marks the N-th blank (1-based).
    // blanks[N-1] is the WordType for that blank.

    QVector<Story> out;

    Story s1;
    s1.title        = QStringLiteral("The Space Hike");
    s1.templateText = QStringLiteral(
        "Last {{1}} I went hiking in a {{2}} near a {{3}}. "
        "It was {{4}}, and I brought a bag of {{5}}. "
        "Suddenly, a {{6}} {{7}} at me! "
        "\"{{8}}!\" I yelled, and then I {{9}} for {{10}} kilometers. "
        "When I finally got home, my friend {{11}} said it was the {{12}} thing ever."
    );
    s1.blanks = {
        WordType::Adverb,        // 1
        WordType::Place,         // 2
        WordType::Animal,        // 3
        WordType::Adjective,     // 4
        WordType::Food,          // 5
        WordType::Color,         // 6
        WordType::PastTenseVerb, // 7
        WordType::Exclamation,   // 8
        WordType::PastTenseVerb, // 9
        WordType::Number,        // 10
        WordType::Person,        // 11
        WordType::Adjective,     // 12
    };
    out.push_back(s1);

    Story s2;
    s2.title        = QStringLiteral("A Day at the Zoo");
    s2.templateText = QStringLiteral(
        "One {{1}} day, my {{2}} took me to a zoo in {{3}}. "
        "The first {{4}} we saw was a {{5}} — there were {{6}} of them! "
        "After that a {{7}} {{8}} {{9}} across the path. "
        "A guide in a {{10}} hat told us to {{11}} quietly. "
        "We finished with a plate of {{12}} and felt very {{13}}."
    );
    s2.blanks = {
        WordType::Adverb,        // 1
        WordType::Person,        // 2
        WordType::Place,         // 3
        WordType::Noun,          // 4
        WordType::PluralNoun,    // 5
        WordType::Number,        // 6
        WordType::Animal,        // 7
        WordType::PastTenseVerb, // 8
        WordType::Adverb,        // 9
        WordType::Color,         // 10
        WordType::PastTenseVerb, // 11
        WordType::Food,          // 12
        WordType::Adjective,     // 13
    };
    out.push_back(s2);

    Story s3;
    s3.title        = QStringLiteral("The Rude Neighbor");
    s3.templateText = QStringLiteral(
        "My {{1}} was being {{2}} today. "
        "She {{3}} at {{4}} {{5}}, then {{6}} a {{7}} {{8}} into the {{9}} pen. "
        "I told her to {{10}} gently, but she only said: "
        "\"{{11}}! {{12}}, I always {{13}} this way!\" "
        "I hope she starts {{14}} at a {{15}} soon. Until then I'll eat all the {{16}}."
    );
    s3.blanks = {
        WordType::Person,        // 1
        WordType::Adjective,     // 2
        WordType::PastTenseVerb, // 3
        WordType::Number,        // 4
        WordType::PluralNoun,    // 5
        WordType::PastTenseVerb, // 6
        WordType::Color,         // 7
        WordType::Noun,          // 8
        WordType::Animal,        // 9
        WordType::PastTenseVerb, // 10
        WordType::Exclamation,   // 11
        WordType::Adverb,        // 12
        WordType::PastTenseVerb, // 13
        WordType::Verb,          // 14
        WordType::Place,         // 15
        WordType::Food,          // 16
    };
    out.push_back(s3);

    return out;
}

} // namespace madlibs
