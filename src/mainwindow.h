#pragma once

#include <QMainWindow>
#include <QStringList>
#include "stories.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QProgressBar;
class QAction;

namespace madlibs {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void newGame();
    void onStoryChanged();
    void advance();
    void back();
    void onPrint();
    void onCopyResult();

private:
    void buildUi();
    void buildActions();
    void finishState();
    void refreshButtons();

    QVector<Story> m_stories;
    Story          m_current;
    int            m_filledCount = 0;   // how many blanks filled so far
    QStringList    m_answers;

    QComboBox    *m_storyBox      = nullptr;
    QLabel       *m_questionLabel = nullptr;   // asks for the current word type
    QPlainTextEdit *m_templateView = nullptr;  // the template, with words already filled
    QLineEdit    *m_input         = nullptr;
    QPushButton  *m_backBtn       = nullptr;
    QPushButton  *m_nextBtn       = nullptr;
    QPushButton  *m_newGameBtn    = nullptr;
    QPushButton  *m_printBtn      = nullptr;
    QPushButton  *m_copyBtn       = nullptr;
    QProgressBar *m_progress      = nullptr;

    QAction    *m_newGameAct = nullptr;
    QAction    *m_printAct   = nullptr;
    QAction    *m_copyAct    = nullptr;
    QAction    *m_quitAct    = nullptr;
    QAction    *m_aboutAct   = nullptr;
};

} // namespace madlibs
