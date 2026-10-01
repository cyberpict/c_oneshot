#pragma once

#include <QList>
#include <QString>
#include <QStringList>
#include <QVector>

namespace madlibs {

enum class WordType {
    Noun,
    PluralNoun,
    Verb,
    PastTenseVerb,
    Adjective,
    Adverb,
    Exclamation,
    Person,
    Place,
    Animal,
    Color,
    Food,
    Number,
};

struct Story {
    QString            title;
    QString            templateText;   // uses {{N}} placeholders (1-based)
    QVector<WordType>  blanks;         // blanks[N-1] is the word type for slot N
};

WordType wordTypeFromString(const QString &name);
QString  wordTypeToString(WordType t);
QString  wordTypeQuestion(WordType t); // e.g. "a(n) noun"

QVector<Story> allStories();

} // namespace madlibs
