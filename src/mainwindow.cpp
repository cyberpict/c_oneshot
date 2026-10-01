#include "mainwindow.h"

#include <QApplication>
#include <QAction>
#include <QClipboard>
#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPrintDialog>
#include <QPrinter>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

namespace madlibs {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stories(allStories())
    , m_current(m_stories.isEmpty() ? Story{} : m_stories.first())
{
    buildUi();
    buildActions();

    connect(m_storyBox,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onStoryChanged);
    connect(m_nextBtn,    &QPushButton::clicked,                              this, &MainWindow::advance);
    connect(m_input,      &QLineEdit::returnPressed,                          this, &MainWindow::advance);
    connect(m_backBtn,    &QPushButton::clicked,                              this, &MainWindow::back);
    connect(m_newGameBtn, &QPushButton::clicked,                              this, &MainWindow::newGame);
    connect(m_printBtn,   &QPushButton::clicked,                              this, &MainWindow::onPrint);
    connect(m_copyBtn,    &QPushButton::clicked,                              this, &MainWindow::onCopyResult);

    connect(m_newGameAct, &QAction::triggered, this, &MainWindow::newGame);
    connect(m_printAct,   &QAction::triggered, this, &MainWindow::onPrint);
    connect(m_copyAct,    &QAction::triggered, this, &MainWindow::onCopyResult);
    connect(m_quitAct,    &QAction::triggered, this, &MainWindow::close);
    connect(m_aboutAct,   &QAction::triggered, [this]() {
        QMessageBox::about(this, tr("About Mad Libs"),
                           tr("<b>Mad Libs</b><br/>A silly word-filling game.<br/><br/>"
                              "Built with Qt 6 and GCC."));
    });

    newGame();
    resize(760, 560);
}

MainWindow::~MainWindow() = default;

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    auto *v       = new QVBoxLayout(central);

    auto *storyRow = new QWidget(central);
    auto *storyLay = new QHBoxLayout(storyRow);
    storyLay->setContentsMargins(0, 0, 0, 0);
    m_storyBox = new QComboBox(storyRow);
    for (const auto &s : m_stories)
        m_storyBox->addItem(s.title);
    storyLay->addWidget(new QLabel(tr("Story:"), storyRow));
    storyLay->addWidget(m_storyBox);
    storyLay->addStretch(1);
    v->addWidget(storyRow);

    v->addSpacing(4);
    m_questionLabel = new QLabel(tr("Pick a story and press New Game."), central);
    m_questionLabel->setWordWrap(true);
    v->addWidget(m_questionLabel);

    v->addWidget(new QLabel(tr("Template so far:"), central));
    m_templateView = new QPlainTextEdit(central);
    m_templateView->setReadOnly(true);
    m_templateView->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    v->addWidget(m_templateView, 1);

    v->addSpacing(4);
    auto *inputRow = new QWidget(central);
    auto *inputLay = new QHBoxLayout(inputRow);
    inputLay->setContentsMargins(0, 0, 0, 0);
    m_input = new QLineEdit(inputRow);
    m_input->setPlaceholderText(tr("Type a word and press Enter."));
    m_input->setMaxLength(200);   // keep user text bounded
    inputLay->addWidget(m_input, 1);
    v->addWidget(inputRow);

    m_progress = new QProgressBar(central);
    m_progress->setTextVisible(false);
    v->addWidget(m_progress);

    auto *btnRow = new QWidget(central);
    auto *btnLay = new QHBoxLayout(btnRow);
    btnLay->setContentsMargins(0, 0, 0, 0);
    m_backBtn   = new QPushButton(tr("Back"),    btnRow);
    m_nextBtn   = new QPushButton(tr("Next"),    btnRow);
    m_newGameBtn= new QPushButton(tr("New Game"), btnRow);
    m_printBtn  = new QPushButton(tr("Print…"),    btnRow);
    m_copyBtn   = new QPushButton(tr("Copy Result"), btnRow);
    btnLay->addWidget(m_backBtn);
    btnLay->addWidget(m_nextBtn);
    btnLay->addStretch(1);
    btnLay->addWidget(m_newGameBtn);
    btnLay->addWidget(m_printBtn);
    btnLay->addWidget(m_copyBtn);
    v->addWidget(btnRow);

    setCentralWidget(central);
    statusBar()->showMessage(tr("Ready"), 3000);

    m_input->setFocus();
}

void MainWindow::buildActions()
{
    m_newGameAct = new QAction(tr("&New Game"),      this);
    m_newGameAct->setShortcut(QKeySequence(Qt::Key_F5));

    m_printAct   = new QAction(tr("&Print…"),      this);
    m_printAct->setShortcut(QKeySequence::Print);

    m_copyAct    = new QAction(tr("&Copy Result"), this);
    m_copyAct->setShortcut(QKeySequence::Copy);

    m_quitAct    = new QAction(tr("E&xit"),        this);
    m_quitAct->setShortcut(QKeySequence::Quit);

    m_aboutAct   = new QAction(tr("About &Mad Libs"), this);

    auto *game = menuBar()->addMenu(tr("&Game"));
    game->addAction(m_newGameAct);
    game->addAction(m_aboutAct);
    game->addSeparator();
    game->addAction(m_quitAct);

    auto *doc = menuBar()->addMenu(tr("&Document"));
    doc->addAction(m_printAct);
    doc->addAction(m_copyAct);

    auto *tb = new QToolBar(tr("Toolbar"), this);
    tb->addAction(m_newGameAct);
    tb->addAction(m_printAct);
    tb->addAction(m_copyAct);
    addToolBar(tb);
}

void MainWindow::newGame()
{
    m_current     = m_stories.value(m_storyBox->currentIndex());
    m_answers.clear();
    m_filledCount = 0;

    if (m_current.blanks.isEmpty()) {
        m_questionLabel->setText(tr("This story has no blanks."));
        m_templateView->setPlainText(renderTemplate(m_current, m_answers, /*active*/ 0));
        m_input->clear();
        m_input->setEnabled(false);
        m_progress->setRange(0, 1);
        m_progress->setValue(1);
        refreshButtons();
        return;
    }

    m_questionLabel->setText(
        tr("%1 (slot 1 of %2): give me %3")
            .arg(m_current.title)
            .arg(m_current.blanks.size())
            .arg(wordTypeQuestion(m_current.blanks.first()))
    );
    m_templateView->setPlainText(renderTemplate(m_current, m_answers, /*active*/ 1));
    m_input->clear();
    m_input->setEnabled(true);
    m_progress->setRange(0, m_current.blanks.size());
    m_progress->setValue(0);
    refreshButtons();
    statusBar()->showMessage(tr("Fill in %1 word(s).").arg(m_current.blanks.size()), 3500);
    m_input->setFocus();
}

void MainWindow::onStoryChanged()
{
    newGame();
}

void MainWindow::advance()
{
    if (m_filledCount >= m_current.blanks.size()) {
        // Finished; drop any stale input instead of silently ignoring it.
        m_input->clear();
        return;
    }

    const QString word = m_input->text().trimmed();
    if (word.isEmpty()) {
        m_input->setFocus();
        m_input->selectAll();
        statusBar()->showMessage(tr("Type a word first."), 2000);
        return;
    }

    m_answers.append(word);
    ++m_filledCount;
    m_progress->setValue(m_filledCount);
    m_input->clear();

    if (m_filledCount >= m_current.blanks.size())
        finishState();
    else {
        const int nextSlot = m_filledCount + 1;
        m_questionLabel->setText(
            tr("Slot %1 of %2: give me %3")
                .arg(nextSlot)
                .arg(m_current.blanks.size())
                .arg(wordTypeQuestion(m_current.blanks.at(nextSlot - 1)))
        );
        m_templateView->setPlainText(renderTemplate(m_current, m_answers, nextSlot));
        m_input->setFocus();
    }
    refreshButtons();
}

void MainWindow::back()
{
    if (m_filledCount == 0)
        return;
    m_answers.removeLast();
    --m_filledCount;
    m_progress->setValue(m_filledCount);

    const int slot = m_filledCount + 1;   // the slot just un-filled
    // Restore its previous answer (if any) into the input for editing.
    m_input->setEnabled(true);
    m_input->setText(slot <= m_answers.size() ? m_answers.at(slot - 1) : QString());
    m_input->setFocus();
    m_input->selectAll();

    m_questionLabel->setText(
        tr("Slot %1 of %2: give me %3")
            .arg(slot)
            .arg(m_current.blanks.size())
            .arg(wordTypeQuestion(m_current.blanks.at(slot - 1)))
    );
    m_templateView->setPlainText(renderTemplate(m_current, m_answers, slot));
    refreshButtons();
}

void MainWindow::finishState()
{
    m_questionLabel->setText(tr("Done! Read it, then copy or print, or press Back to edit."));
    m_input->clear();
    m_input->setEnabled(false);
    m_templateView->setPlainText(fillTemplate(m_current, m_answers));
    m_progress->setValue(m_current.blanks.size());
    refreshButtons();
    statusBar()->showMessage(tr("Finished! Press Copy or Print, or Back to edit."), 5000);
}

void MainWindow::refreshButtons()
{
    const bool finished  = m_filledCount >= m_current.blanks.size();
    const bool canBack   = m_filledCount > 0;
    m_backBtn  ->setEnabled(canBack);
    m_nextBtn  ->setEnabled(!finished && !m_current.blanks.isEmpty());
    m_newGameBtn->setEnabled(true);
    m_printBtn ->setEnabled(finished);
    m_copyBtn  ->setEnabled(finished);
    m_nextBtn  ->setText(finished ? tr("Done!") : tr("Next"));
}

void MainWindow::onPrint()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted)
        m_templateView->print(&printer);
}

void MainWindow::onCopyResult()
{
    QApplication::clipboard()->setText(m_templateView->toPlainText());
    statusBar()->showMessage(tr("Story copied to the clipboard."), 2500);
}

} // namespace madlibs
