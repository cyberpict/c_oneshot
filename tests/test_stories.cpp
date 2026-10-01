// Headless parser test: validates the {{N}} template -> filled-text algorithm
// without any Qt Widgets (so it runs under QtTest without a display).
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QString>
#include <QVector>

#include "stories.h"

class TestStories : public QObject {
    Q_OBJECT

private slots:
    void slotsMatchBlanks();
    void fillProducesCorrectText();
    void fillRoundTrips();
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
    // Build the same fill algorithm as mainwindow.cpp uses, independently,
    // to make sure the contract in stories.h produces the right output.
    auto fill = [](const madlibs::Story &s, const QStringList &answers) {
        QString out;
        int i = 0;
        while (i < s.templateText.size()) {
            const int open = s.templateText.indexOf(QStringLiteral("{{"), i);
            if (open < 0) { out += s.templateText.mid(i); break; }
            out += s.templateText.mid(i, open - i);
            const int close = s.templateText.indexOf(QStringLiteral("}}"), open + 2);
            if (close < 0) { out += s.templateText.mid(open); break; }
            const int slot = s.templateText.mid(open + 2, close - open - 2).trimmed().toInt();
            if (slot >= 1 && slot <= answers.size())
                out += answers.at(slot - 1);
            else
                out += QStringLiteral("[?%1?]").arg(slot);
            i = close + 2;
        }
        return out;
    };

    const auto stories = madlibs::allStories();
    for (const auto &s : stories) {
        QStringList answers;
        answers.reserve(s.blanks.size());
        for (int k = 0; k < s.blanks.size(); ++k)
            answers << QStringLiteral("WORD%1").arg(k + 1);
        const QString out = fill(s, answers);
        QVERIFY(!out.contains(QStringLiteral("{{")));
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
    auto fill = [](const madlibs::Story &s, const QStringList &answers) {
        QString out;
        int i = 0;
        while (i < s.templateText.size()) {
            const int open = s.templateText.indexOf(QStringLiteral("{{"), i);
            if (open < 0) { out += s.templateText.mid(i); break; }
            out += s.templateText.mid(i, open - i);
            const int close = s.templateText.indexOf(QStringLiteral("}}"), open + 2);
            const int slot = s.templateText.mid(open + 2, close - open - 2).trimmed().toInt();
            if (slot >= 1 && slot <= answers.size())
                out += answers.at(slot - 1);
            else
                out += QStringLiteral("[?%1?]").arg(slot);
            i = close + 2;
        }
        return out;
    };
    QCOMPARE(fill(s, answers),
             QStringLiteral("Hello Bob, you ate the carrot!"));
}

QTEST_MAIN(TestStories)
#include "test_stories.moc"
