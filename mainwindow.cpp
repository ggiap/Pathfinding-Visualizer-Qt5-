#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpressionValidator>

#include <fstream>
#include <sstream>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "A_star.h"
#include "Dijkstra.h"
#include "GreedyBestFS.h"
#include "Node.h"
#include "Search.h"
#include "Button.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMaximumSize(width(), height());
    setMinimumSize(width(), height());

    ui->HeuristicGroupBox->setEnabled(false);

    //https://forum.qt.io/topic/95182/qintvalidator-not-working-as-im-expecting/5
    ui->rowsLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,2}"),this));
    ui->columnsLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,2}"),this));

    ui->randomRadioButton->setChecked(true);
    ui->manhattanRadioButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->columnsLineEdit->text() == "" || ui->columnsLineEdit->text() == "1") return;

    if(ui->columnsLineEdit->text().toInt() < 2 || ui->rowsLineEdit->text().toInt() < 2)
    {
        ui->rowsLineEdit->text() = "2";
        ui->columnsLineEdit->text() = "2";
    }

    rows = ui->rowsLineEdit->text().toInt();
    columns = ui->columnsLineEdit->text().toInt();
    ui->rowsLineEdit->text().clear();
    ui->columnsLineEdit->text().clear();

    if(rows == columns)
        Run();
    else
    {
        QMessageBox* mb = new QMessageBox();
        mb->setIcon(QMessageBox::Warning);
        mb->setText("Rows and Columns must be equal!");
        mb->setStandardButtons(QMessageBox::Ok);
        mb->setDefaultButton(QMessageBox::Ok);
        mb->exec();
        delete mb;
    }
}

void MainWindow::on_rowsLineEdit_textChanged([[maybe_unused]]const QString &arg1)
{
    if (ui->rowsLineEdit->text().toInt() > 130)
        ui->rowsLineEdit->setText("130");

    ui->columnsLineEdit->setText(ui->rowsLineEdit->text());
}

void MainWindow::on_columnsLineEdit_textChanged([[maybe_unused]]const QString &arg1)
{
    if (ui->columnsLineEdit->text().toInt() > 130)
        ui->columnsLineEdit->setText("130");

    ui->rowsLineEdit->setText(ui->columnsLineEdit->text());
}

void MainWindow::on_DiagMovecheckBox_stateChanged(int arg1)
{
    allowDiagonalMovement = static_cast<bool>(arg1);
}

void MainWindow::on_actionOpen_triggered()
{
    rows = 0;
    columns = 0;
    loadFromFile = true;
    QFileDialog *qfd = new QFileDialog();
    qfd->setFileMode(QFileDialog::ExistingFile);
    qfd->setAcceptMode(QFileDialog::AcceptOpen);
    qfd->setNameFilter(tr("Text files (*.txt)"));
    qfd->setDefaultSuffix("txt");
    qfd->setDirectory("../");
    if(qfd->exec())
    {
        path = qfd->selectedFiles().front();
        std::ifstream file(path.toStdString());
        if(file.is_open())
        {
            std::vector<int> temp;
            std::string buffer, number;
            if (getline(file, buffer))
            {
                std::stringstream line(buffer);
                while (line >> number)
                {
                    temp.push_back(stoi(number));
                }
                columns = temp.size();
                ++rows;
            }
            while (getline(file, buffer))
                ++rows;

            if (rows != columns)
            {
                QMessageBox* mb = new QMessageBox();
                mb->setIcon(QMessageBox::Critical);
                mb->setText("Rows and Columns must be equal!");
                mb->setStandardButtons(QMessageBox::Ok);
                mb->setDefaultButton(QMessageBox::Ok);
                mb->exec();
                delete mb;

                ui->rowsLineEdit->setText("");
            }
            else
            {
                if (rows == 0)
                {
                    QMessageBox* mb = new QMessageBox();
                    mb->setIcon(QMessageBox::Critical);
                    mb->setText("The file is empty!");
                    mb->setStandardButtons(QMessageBox::Ok);
                    mb->setDefaultButton(QMessageBox::Ok);
                    mb->exec();
                    delete mb;

                    ui->rowsLineEdit->setText("");
                }
                else
                    ui->rowsLineEdit->setText(std::to_string(rows).c_str());
            }
        }
    }
}

void MainWindow::on_randomRadioButton_clicked()
{
    algorithmChoice = visualizer::Alg_Choice::Random;
    ui->HeuristicGroupBox->setEnabled(false);
}

void MainWindow::on_astarRadioButton_clicked()
{
    algorithmChoice = visualizer::Alg_Choice::Astar;
    ui->HeuristicGroupBox->setEnabled(true);
}

void MainWindow::on_bfsRadioButton_clicked()
{
    algorithmChoice = visualizer::Alg_Choice::GreedyBFS;
    ui->HeuristicGroupBox->setEnabled(true);
}

void MainWindow::on_djikstraRadioButton_clicked()
{
    algorithmChoice = visualizer::Alg_Choice::Dijkstra;
    ui->HeuristicGroupBox->setEnabled(false);
}

void MainWindow::on_manhattanRadioButton_clicked()
{
    heuristicChoice = visualizer::Heuristic_Choice::Manhattan;
}

void MainWindow::on_chebysevRadioButton_clicked()
{
    heuristicChoice = visualizer::Heuristic_Choice::Chebyshev;
}

void MainWindow::on_euclideanRadioButton_clicked()
{
    heuristicChoice = visualizer::Heuristic_Choice::Euclidean;
}

Node* MainWindow::Init_start(Grid &grid)
{
    for (size_t i = 0; i < grid.nodes.size(); ++i)
    {
        if (grid.nodes.at(i).value == 0)
        {
            return &grid.nodes.at(i);
        }
    }
    return nullptr;
}

Node* MainWindow::Init_finish(Grid &grid)
{
    for (size_t i = grid.nodes.size() - 1; i > 0; --i)
    {
        if (grid.nodes.at(i).value == 0)
        {
            return &grid.nodes.at(i);
        }
    }
    return nullptr;
}

void MainWindow::Random_Search(Grid &grid, Node *start, Node *finish)
{
    for (auto i = grid.nodes.begin(); i != grid.nodes.end(); ++i)
    {
        if (i->nodeIndex != start->nodeIndex && i->nodeIndex != finish->nodeIndex)
            i->value == 0 ? i->cell.setFillColor(sf::Color::White) : i->cell.setFillColor(sf::Color::Magenta);
    }

    Search search(grid);
    search.Start(*start, *finish);

    while (!search.finished)
    {
        search.Step(window, allowDiagonalMovement);
    }
#ifdef _DEBUG
    cout << "Search done. Path length: " << search.iterations << endl;
#endif // _DEBUG

    for (auto &node : search.path)
    {
        if (node->value != 1 && node->nodeIndex != start->nodeIndex &&
            node->nodeIndex != finish->nodeIndex)
        {
            node->cell.setFillColor(sf::Color::Blue);
        }
    }
}

void MainWindow::A_Star(Grid &grid, Node *start, Node *finish, int heuristicChoice)
{
    for (auto i = grid.nodes.begin(); i != grid.nodes.end(); ++i)
    {
        if (i->nodeIndex != start->nodeIndex && i->nodeIndex != finish->nodeIndex)
            i->value == 0 ? i->cell.setFillColor(sf::Color::White) : i->cell.setFillColor(sf::Color::Magenta);
    }
    A_star a_star(grid, *start, *finish, heuristicChoice);
    a_star.Start_Search(window, allowDiagonalMovement);

    if (a_star.path.size() > 1)
    {
        for (auto &node : a_star.path)
            if (node->nodeIndex != start->nodeIndex &&
                node->nodeIndex != finish->nodeIndex)
                node->cell.setFillColor(sf::Color::Blue);
    }
    else
        printf("No path found!\n\n");
}

void MainWindow::Greedy_Best_FS(Grid &grid, Node *start, Node *finish)
{
    for (auto i = grid.nodes.begin(); i != grid.nodes.end(); ++i)
    {
        if (i->nodeIndex != start->nodeIndex && i->nodeIndex != finish->nodeIndex)
            i->value == 0 ? i->cell.setFillColor(sf::Color::White) : i->cell.setFillColor(sf::Color::Magenta);
    }

    GreedyBestFS bestFS(grid, *start, *finish, heuristicChoice);
    bestFS.Start_Search(window, allowDiagonalMovement);

    if (bestFS.path.size() > 1)
    {
        for (auto &node : bestFS.path)
            if (node->nodeIndex != start->nodeIndex &&
                node->nodeIndex != finish->nodeIndex)
                node->cell.setFillColor(sf::Color::Blue);
    }
    else
        printf("No path found!\n\n");
}

void MainWindow::Dijkstra_Search(Grid &grid, Node *start, Node *finish)
{
    for (auto i = grid.nodes.begin(); i != grid.nodes.end(); ++i)
    {
        if (i->nodeIndex != start->nodeIndex && i->nodeIndex != finish->nodeIndex)
            i->value == 0 ? i->cell.setFillColor(sf::Color::White) : i->cell.setFillColor(sf::Color::Magenta);
    }

    Dijkstra dijkstra(grid, *start, *finish);
    dijkstra.Start_Search(window, allowDiagonalMovement);

    if (dijkstra.path.size() > 1)
    {
        for (auto &node : dijkstra.path)
            if (node->nodeIndex != start->nodeIndex &&
                node->nodeIndex != finish->nodeIndex)
                node->cell.setFillColor(sf::Color::Blue);
    }
    else
        printf("No path found!\n\n");
}

void MainWindow::Run()
{
    window.create(sf::VideoMode(730, 650), "SFML", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(400, 0));

    ::Button startButton;
    startButton.buttonConfig(
        sf::Vector2f(80.0f, 25.0f),
        sf::Vector2f(650.0f, 0.0f),
        sf::Color(150, 150, 150, 255),
        "Start");

    ::Button resetButton;
    resetButton.buttonConfig(
        sf::Vector2f(80.0f, 25.0f),
        sf::Vector2f(650.0f, 26.0f),
        sf::Color(150, 150, 150, 255),
        "Reset");


    vector<int> temp;
    vector<vector<int>> map;

    if (loadFromFile)
    {
        string buffer, number;
        ifstream file(path.toUtf8());
        if (file.is_open())
        {
            while (getline(file, buffer))
            {
                stringstream line(buffer);
                while (line >> number)
                {
                    temp.push_back(stoi(number));
                }
#ifdef _DEBUG
                for (auto num : temp)
                    printf("%d ", num);
                puts("");
#endif // _DEBUG

                map.push_back(temp);
                temp.clear();
            }
        }
        loadFromFile = false;
    }
    else
    {
        temp = vector<int>(rows, 0);
        map = vector<vector<int>>(columns, temp);
    }

    Grid grid(map);
    Node *start = Init_start(grid);
    Node *finish = Init_finish(grid);

    if (start == nullptr || finish == nullptr || start->nodeIndex == finish->nodeIndex)
    {
        window.close();

        QMessageBox* mb = new QMessageBox();
        mb->setIcon(QMessageBox::Critical);
        mb->setText("Cannot solve this grid!");
        mb->setStandardButtons(QMessageBox::Ok);
        mb->setDefaultButton(QMessageBox::Ok);
        mb->exec();
        delete mb;

        return;
    }

    start->cell.setFillColor(sf::Color::Green);
    finish->cell.setFillColor(sf::Color::Red);
    for (auto i = grid.nodes.begin(); i != grid.nodes.end(); ++i)
    {
        if (i->nodeIndex != start->nodeIndex && i->nodeIndex != finish->nodeIndex)
            i->value == 0 ? i->cell.setFillColor(sf::Color::White) : i->cell.setFillColor(sf::Color::Magenta);
    }

    bool middleMouseButtonPressed = false;
    sf::Event event;
    //Game loop
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosf(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            int i = -1;
            for (auto &node : grid.nodes)
                if (node.cell.getGlobalBounds().contains(mousePosf))
                    i = node.nodeIndex;

            // end the program
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            //Track mouse movement
            if (event.type == sf::Event::MouseMoved)
            {
                if (startButton.getRect().getGlobalBounds().contains(mousePosf))
                {
                    startButton.getRect().setFillColor(sf::Color(150, 150, 150, 150));
                }
                else
                {
                    startButton.getRect().setFillColor(sf::Color(150, 150, 150, 255));
                }
                if (resetButton.getRect().getGlobalBounds().contains(mousePosf))
                {
                    resetButton.getRect().setFillColor(sf::Color(150, 150, 150, 150));
                }
                else
                {
                    resetButton.getRect().setFillColor(sf::Color(150, 150, 150, 255));
                }

                // Mark the unwalkable nodes
                if (middleMouseButtonPressed && i != -1
                    && grid.nodes.at(i).nodeIndex != start->nodeIndex
                    && grid.nodes.at(i).nodeIndex != finish->nodeIndex)
                {
                    if (grid.nodes.at(i).nodeValueChanged == false)
                    {
                        if (grid.nodes.at(i).value == 0)
                        {
                            grid.nodes.at(i).nodeValueChanged = true;
                            grid.nodes.at(i).value = 1;
                            grid.nodes.at(i).cell.setFillColor(sf::Color::Magenta);
                        }
                        else
                        {
                            grid.nodes.at(i).nodeValueChanged = true;
                            grid.nodes.at(i).value = 0;
                            grid.nodes.at(i).cell.setFillColor(sf::Color::White);
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // Right mouse button click
                if (event.mouseButton.button == sf::Mouse::Right && i != -1
                    && grid.nodes.at(i).value == 0 && grid.nodes.at(i).nodeIndex != start->nodeIndex)
                {
                    // Mark the finish node
                    if (finish == nullptr) finish = &grid.nodes.at(i);
                    finish->cell.setFillColor(sf::Color::White);
                    finish = &grid.nodes.at(i);
                    finish->cell.setFillColor(sf::Color::Red);
                }

                // Middle button mouse click
                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    middleMouseButtonPressed = true;
                }

                // Left mouse button click
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Mark the start node
                    if (i != -1 && grid.nodes.at(i).value == 0
                        && grid.nodes.at(i).nodeIndex != finish->nodeIndex)
                    {
                        if (start == nullptr) start = &grid.nodes.at(i);
                        start->cell.setFillColor(sf::Color::White);
                        start = &grid.nodes.at(i);
                        start->cell.setFillColor(sf::Color::Green);
                    }

                    // START Button
                    if (startButton.getRect().getGlobalBounds().contains(mousePosf))
                    {
                        if (algorithmChoice == visualizer::Alg_Choice::Random)
                            Random_Search(grid, start, finish);
                        else if (algorithmChoice == visualizer::Alg_Choice::Astar)
                            A_Star(grid, start, finish, heuristicChoice);
                        else if (algorithmChoice == visualizer::Alg_Choice::GreedyBFS)
                            Greedy_Best_FS(grid, start, finish);
                        else if (algorithmChoice == visualizer::Alg_Choice::Dijkstra)
                            Dijkstra_Search(grid, start, finish);
                    }

                    // RESET Button
                    if (resetButton.getRect().getGlobalBounds().contains(mousePosf))
                    {
                        for (auto &node : grid.nodes)
                        {
                            node.value = 0;
                            node.Clear();
                            node.cell.setFillColor(sf::Color::White);
                        }
                        /*
                        *  Regarding reading from a file:
                        *  Must do this otherwise grid has the old
                        *  values loaded from the file
                        */

                        auto map = std::vector<std::vector<int>>(columns, std::vector<int>(rows, 0));
                        grid = Grid(map);

                        start = &grid.nodes.at(0);
                        finish = &grid.nodes.at(grid.nodes.size() - 1);
                        start->cell.setFillColor(sf::Color::Green);
                        finish->cell.setFillColor(sf::Color::Red);
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    if (i != -1 && grid.nodes.at(i).nodeValueChanged == false
                        && grid.nodes.at(i).nodeIndex != start->nodeIndex
                        && grid.nodes.at(i).nodeIndex != finish->nodeIndex)
                    {
                        if (grid.nodes.at(i).value == 0)
                        {
                            grid.nodes.at(i).nodeValueChanged = true;
                            grid.nodes.at(i).value = 1;
                            grid.nodes.at(i).cell.setFillColor(sf::Color::Magenta);
                        }
                        else
                        {
                            grid.nodes.at(i).nodeValueChanged = true;
                            grid.nodes.at(i).value = 0;
                            grid.nodes.at(i).cell.setFillColor(sf::Color::White);
                        }
                    }

                    middleMouseButtonPressed = false;
                    for (auto &node : grid.nodes)
                        if (node.nodeValueChanged == true)
                            node.nodeValueChanged = false;
                }
            }
        }

        window.clear(sf::Color::Black);
        startButton.draw(window);
        resetButton.draw(window);
        for (size_t i = 0; i < grid.nodes.size(); ++i)
            window.draw(grid.nodes.at(i).cell);
        window.display();
    }
}
