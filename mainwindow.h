#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <SFML/Graphics/RenderWindow.hpp>

class Grid;
class Node;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_rowsLineEdit_textChanged(const QString &arg1);

    void on_columnsLineEdit_textChanged(const QString &arg1);

    void on_DiagMovecheckBox_stateChanged(int arg1);

    void on_actionOpen_triggered();

    void on_randomRadioButton_clicked();

    void on_astarRadioButton_clicked();

    void on_bfsRadioButton_clicked();

    void on_djikstraRadioButton_clicked();

    void on_manhattanRadioButton_clicked();

    void on_chebysevRadioButton_clicked();

    void on_euclideanRadioButton_clicked();

private:
    /////////////Methods/////////////
    Node* Init_start(Grid &grid);
    Node* Init_finish(Grid &grid);
    void Random_Search(Grid &grid, Node *start, Node *finish);
    void A_Star(Grid &grid, Node *start, Node *finish, int heuristicChoice);
    void Greedy_Best_FS(Grid &grid, Node *start, Node *finish);
    void Dijkstra_Search(Grid &grid, Node *start, Node *finish);
    void Run();

    /////////////Members/////////////
    Ui::MainWindow *ui;

    sf::RenderWindow window;
    int rows{};
    int columns{};
    bool allowDiagonalMovement = false;
    bool loadFromFile = false;
    QString path{};
    int algorithmChoice = 1;
    int heuristicChoice = 1;
};
#endif // MAINWINDOW_H
