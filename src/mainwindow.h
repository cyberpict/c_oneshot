// mainwindow.h — the Mad Libs game window.
//
// MainWindow is a small state machine around a single story:
//   select story -> fill blanks one by one (Next/Back to revise)
//   -> finished: copy or print the result.
//
// The game state lives in the plain data members (m_current, m_answers,
// m_filledCount); the Qt widgets are a view over that state and are
// refreshed via refreshButtons()/the question label whenever the state
// changes.

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
    Q_OBJECT   // enables signals/slots, tr() and the meta-object compiler

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Starts (or restarts) a game with the story selected in the combobox.
    void newGame();
    // Slot for QComboBox::currentIndexChanged — a changed story means a new game.
    void onStoryChanged();
    // Accepts the current input word, stores it, and moves to the next
    // slot (or into the finished state if no slots remain).
    void advance();
    // Undoes the last answered slot and puts its word back in the input box.
    void back();
    // Prints the finished story through the platform print dialog.
    void onPrint();
    // Copies the finished story text to the clipboard.
    void onCopyResult();

private:
    // Builds the central widget layout (combobox, labels, input, buttons).
    void buildUi();
    // Builds the QActions, menus and toolbar.
    void buildActions();
    // Applies the "all slots filled" UI state (disables input, shows
    // the fully filled story, enables Copy/Print).
    void finishState();
    // Enables/disables buttons and relabels Next based on current state.
    void refreshButtons();

    // All available stories, in the order shown in the story combobox.
    QVector<Story> m_stories;
    // The story currently being played (copied from m_stories in newGame()).
    Story          m_current;
    // How many blanks have been answered so far (1..m_current.blanks.size()).
    int            m_filledCount = 0;
    // The answers collected so far, in slot order (answers[i] is the word
    // chosen for slot i+1). Back() removes the last one.
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
