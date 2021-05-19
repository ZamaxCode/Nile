#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdlib.h>
#include <time.h>
#include "user.h"
#include <vector>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "layoutproducts.h"
#include "products.h"
#include "Graph.h"
#include "string"
#include <QStringList>
#include <recommend.h>
#include <map>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    vector<User> getUsers() const;
    void setUsers(const vector<User> &value);

    vector<Products> getProd() const;
    void setProd(const vector<Products> &value);

private slots:
    void on_loginEmailLE_textChanged(const QString &arg1);

    void on_passwordLE_textChanged(const QString &arg1);

    void on_newUserLE_textChanged(const QString &arg1);

    void on_emailLE_textChanged(const QString &arg1);

    void on_newPasswordLE_textChanged(const QString &arg1);

    void on_createPB_clicked();

    void on_loginPB_clicked();

    void openFile();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_minPricePB_clicked();

    void on_maxPricePB_clicked();

    void on_srchPB_clicked();

    void on_srchLE_textChanged(const QString &arg1);

    void addToPurchase(QString, int);

private:
    Ui::MainWindow *ui;

    vector<User> users;
    vector<Products> prod;
    vector<Products> prodMod;
    QAction *openFileAction;
    QFile dbFile;
    QJsonArray database, dbProducts;
    QMenu *fileMenu;
    QString time;
    QString currentUser;
    vector<QString> purchases;
    Graph<string> grafo;
    vector<Recommend> recommendVec;

    void enableLoginPB();
    void enableCreatePB();
    void validateUser();
    void saveDB();
    void loadDB();
    void chargeProducts();
    void showProducts();
    void minPriceProducts();
    void maxPriceProducts();
    void srchProducts();
    void enableSrchPB();
    void setPurchases();
    void createGraph();
    void chargeRecommendations(const QString id);
    void showRecommendations();
};
#endif // MAINWINDOW_H
