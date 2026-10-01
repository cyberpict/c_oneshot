// Headless test of the story data and the shared template-rendering
// functions in stories.cpp (no GUI stack needed; runs offscreen).
#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QVector>

#include "stories.h"

class TestStories : public QObject {
    Q_OBJECT

private slots:
    void slotsMatchBlanks();
    void fillProducesCorrectText();
    void fillRoundTrips();
    void renderHighlightsActiveSlot();
    void renderFlagsInvalidSlot();
};

void TestStories::slotsMatchBlanks()
{
    // Every {{N}} in a template must have a matching entry in blanks[N-1],
    // and all slot numbers must be 1..N with no gaps.
    const auto stories = madlibs::allStories();
    QVERIFY(!stories.isEmpty());

    for (const auto &s : stories) {
        int expected = 1;
        QSet<int> seen;
        int i = 0;
        while (i < s.templateText.size()) {
            const int open = s.templateText.indexOf(QStringLiteral("{{"), i);
            if (open < 0) break;
            const int close = s.templateText.indexOf(QStringLiteral("}}"), open + 2);
            QVERIFY(close > open);
            const int slot = s.templateText.mid(open + 2, close - open - 2).trimmed().toInt();
            QVERIFY(slot == expected);
            QVERIFY(!seen.contains(slot));
            QVERIFY(slot >= 1 && slot <= s.blanks.size());
            seen.insert(slot);
            i = close + 2;
            ++expected;
        }
        QCOMPARE(expected - 1, s.blanks.size());
    }
}

void TestStories::fillProducesCorrectText()
{
    // Run the shared fillTemplate() against every stock story.
    const auto stories = madlibs::allStories();
    for (const auto &s : stories) {
        QStringList answers;
        answers.reserve(s.blanks.size());
        for (int k = 0; k < s.blanks.size(); ++k)
            answers << QStringLiteral("WORD%1").arg(k + 1);
        const QString out = madlibs::fillTemplate(s, answers);
        QVERIFY(!out.contains(QStringLiteral("{{")));
        QVERIFY(!out.contains(QStringLiteral("[?")));   // nothing left unfilled
        for (int k = 0; k < s.blanks.size(); ++k)
            QVERIFY(out.contains(QStringLiteral("WORD%1").arg(k + 1)));
    }
}

void TestStories::fillRoundTrips()
{
    // Sanity: a tiny template we control.
    madlibs::Story s;
    s.title        = QStringLiteral("Round Trip");
    s.templateText = QStringLiteral("Hello {{1}}, you {{2}} the {{3}}!");
    s.blanks       = { madlibs::WordType::Person,
                       madlibs::WordType::PastTenseVerb,
                       madlibs::WordType::Noun };
    const QStringList answers{QStringLiteral("Bob"), QStringLiteral("ate"), QStringLiteral("carrot")};
    QCOMPARE(madlibs::fillTemplate(s, answers),
             QStringLiteral("Hello Bob, you ate the carrot!"));
}

void TestStories::renderHighlightsActiveSlot()
{
    madlibs::Story s;
    s.templateText = QStringLiteral("Hello {{1}}, you {{2}} the {{3}}!");
    s.blanks       = { madlibs::WordType::Person,
                       madlibs::WordType::PastTenseVerb,
                       madlibs::WordType::Noun };
    const QStringList answers{QStringLiteral("Bob"), QStringLiteral("ate")};
    // Slot 2 is filled and active -> highlighted; slot 3 unfilled -> [3].
    QCOMPARE(madlibs::renderTemplate(s, answers, 2),
             QStringLiteral("Hello Bob, you \u27e8ate\u27e9 the [3]!"));
    // fillTemplate (activeSlot = 0) never highlights
    QCOMPARE(madlibs::fillTemplate(s, answers),
             QStringLiteral("Hello Bob, you ate the [3]!"));
}

void TestStories::renderFlagsInvalidSlot()
{
    madlibs::Story s;
    s.templateText = QStringLiteral("Bad {{abc}} and out-of-range {{9}}.");
    s.blanks       = { madlibs::WordType::Noun };
    QCOMPARE(madlibs::fillTemplate(s, {QStringLiteral("x")}),
             QStringLiteral("Bad [?] and out-of-range [?]."));
}

QTEST_MAIN(TestStories)
#include "test_stories.moc"
