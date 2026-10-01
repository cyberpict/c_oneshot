#pragma once

#include <QString>
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

QString  wordTypeToString(WordType t);
QString  wordTypeQuestion(WordType t); // e.g. "a(n) noun"

// Renders the story template for display while playing:
// filled slots show their answer, the active (1-based) filled slot is
// highlighted with ⟨…⟩, unfilled slots show [N], invalid placeholders [?].
// activeSlot is 1-based; pass 0 for "no active slot".
QString renderTemplate(const Story &story,
                       const QStringList &answers,
                       int activeSlot);

// Renders the final story text with every placeholder replaced by its answer;
// invalid placeholders show [?].
QString fillTemplate(const Story &story, const QStringList &answers);

QVector<Story> allStories();

} // namespace madlibs
