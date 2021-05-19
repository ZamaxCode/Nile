#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <regex>
#include "validacion.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileMenu=ui->menubar->addMenu(tr("&File"));
    openFileAction=new QAction(tr("Open File"),this);

    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    fileMenu->addAction(openFileAction);
}

MainWindow::~MainWindow()
{
    saveDB();
    delete ui;
}

void MainWindow::enableLoginPB()
{
    if(ui->loginEmailLE->text().length()>0
            && ui->passwordLE->text().length()>0)
    {
        ui->loginPB->setEnabled(true);
    }
    else
    {
        ui->loginPB->setEnabled(false);
    }
}

void MainWindow::enableCreatePB()
{
    if(ui->newUserLE->text().length()>0
            && ui->emailLE->text().length()>0
            && ui->newPasswordLE->text().length()>0)
    {
        ui->createPB->setEnabled(true);
    }
    else
    {
        ui->createPB->setEnabled(false);
    }
}

void MainWindow::validateUser()
{
    size_t i;
    QMessageBox msg;
    string correo;
    correo = ui->loginEmailLE->text().toStdString();
    if(!validarCorreo(correo))
    {
        msg.setText("Invalid email/password");
        msg.exec();
    }
    else
    {
        for(i=0; i<users.size(); i++)
        {
            if(users.at(i).getEmail() == ui->loginEmailLE->text())
            {
                if(users.at(i).getPassword() == ui->passwordLE->text())
                {
                    currentUser = users.at(i).getEmail();
                    ui->wiewsSW->setCurrentIndex(1);
                    chargeProducts();
                    showProducts();
                }
                else
                {
                    msg.setText("Invalid email/password");
                    msg.exec();
                }
                break;
            }
        }
        if(i==users.size())
        {
            msg.setText("Invalid email/password");
            msg.exec();
        }
    }
}

void MainWindow::saveDB()
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    setPurchases();

    jsonObj["users"]=database;
    jsonDoc=QJsonDocument(jsonObj);
    jsonObj["products"]=dbProducts;
    jsonDoc=QJsonDocument(jsonObj);

    dbFile.open(QIODevice::WriteOnly);
    dbFile.write(jsonDoc.toJson());
    dbFile.close();
}

void MainWindow::loadDB()
{
    User u;
    Products p;
    QJsonDocument jsonDoc;
    QJsonObject jsonObj,jsonObj2,jsonObj3;
    QByteArray data;

    dbFile.open(QIODevice::ReadOnly);
    data=dbFile.readAll();
    jsonDoc=QJsonDocument::fromJson(data);
    jsonObj=jsonDoc.object();
    database = jsonObj["users"].toArray();
    dbProducts = jsonObj["products"].toArray();

    dbFile.close();

    for(int i(0); i < database.size(); ++i)
    {
        jsonObj2 = database.at(i).toObject();
        u.setName(jsonObj2["name"].toString());
        u.setEmail(jsonObj2["email"].toString());
        u.setPassword(jsonObj2["password"].toString());
        users.push_back(u);
    }

    for(int i(0); i < dbProducts.size(); ++i)
    {
        jsonObj3 = dbProducts.at(i).toObject();
        p.setId(jsonObj3["id"].toString());
        p.setName(jsonObj3["name"].toString());
        p.setPrice(jsonObj3["price"].toDouble());
        p.setSold(jsonObj3["sold"].toInt());
        prod.push_back(p);
    }

    for(int i(0); i<5 ;++i)
    {
        Recommend rec;
        rec.setId(prod.at(rand() % 50).getId());
        rec.setPeso(0);
        recommendVec.push_back(rec);
    }
    showRecommendations();
    recommendVec.clear();
}

void MainWindow::chargeProducts()
{
    if(ui->comboBox->currentText()=="Todos los departamentos")
    {
        prodMod.clear();
        for(unsigned i(0); i<prod.size(); ++i)
        {
            prodMod.push_back(prod.at(i));
        }
    }

    if(ui->comboBox->currentText()=="Alimentos y Bebidas")
    {
        prodMod.clear();
        for(unsigned i(0); i<=9; ++i)
        {
            prodMod.push_back(prod.at(i));
        }
    }

    if(ui->comboBox->currentText()=="Libros")
    {
        prodMod.clear();
        for(unsigned i(10); i<=19; ++i)
        {
            prodMod.push_back(prod.at(i));
        }
    }

    if(ui->comboBox->currentText()=="Electrónicos")
    {
        prodMod.clear();
        for(unsigned i(20); i<=29; ++i)
        {
            prodMod.push_back(prod.at(i));
        }
    }

    if(ui->comboBox->currentText()=="Hogar Y Cocina")
    {
        prodMod.clear();
        for(unsigned i(30); i<=39; ++i)
        {
            prodMod.push_back(prod.at(i));
        }
    }

    if(ui->comboBox->currentText()=="Deporte y Aire Libre")
    {
        prodMod.clear();
        for(unsigned i(40); i<=49; ++i)
        {
            prodMod.push_back(prod.at(i));
        }
    }
}

void MainWindow::showProducts()
{
    QString id, name, price;
    float pri;
    layoutProducts* lop;
    for(unsigned i(0); i<prodMod.size(); ++i)
    {
        lop = new layoutProducts();
        id=prodMod.at(i).getId();
        name=prodMod.at(i).getName();
        pri=prodMod.at(i).getPrice();
        price=QString::number(pri);
        lop->setProduct(id,name,price);
        lop->setId(prodMod.at(i).getId());
        connect(lop, SIGNAL(added(QString, int)), this, SLOT(addToPurchase(QString, int)));
        ui->productsGL->addWidget(lop, i/5, i%5,Qt::AlignCenter);
    }
}

void MainWindow::minPriceProducts()
{
    srchProducts();
    Products temp;
    for(unsigned i(0); i < prodMod.size()-1; ++i){
        for(unsigned j(0); j < prodMod.size()-1; ++j){
            if(prodMod.at(j+1).getPrice() < prodMod[j].getPrice()){
                temp = prodMod.at(j);
                prodMod.at(j) = prodMod.at(j+1);
                prodMod.at(j+1) = temp;
            }
        }
    }
}

void MainWindow::maxPriceProducts()
{
    srchProducts();
    Products temp;
    for(unsigned i(0); i < prodMod.size()-1; ++i){
        for(unsigned j(0); j < prodMod.size()-1; ++j){
            if(prodMod.at(j+1).getPrice() > prodMod[j].getPrice()){
                temp = prodMod.at(j);
                prodMod.at(j) = prodMod.at(j+1);
                prodMod.at(j+1) = temp;
            }
        }
    }
}

void MainWindow::srchProducts()
{
    vector<Products> temp;
    for(unsigned i(0); i < prodMod.size(); ++i){
        if(prodMod.at(i).getName().toUpper().contains(ui->srchLE->text().toUpper()))
        {
            temp.push_back(prodMod.at(i));
        }
    }
    prodMod.clear();
    prodMod=temp;

}

void MainWindow::enableSrchPB()
{
    if(ui->srchLE->text().length()>0)
    {
        ui->srchPB->setEnabled(true);
    }
    else
    {
        ui->srchPB->setEnabled(false);
    }
}

void MainWindow::setPurchases()
{
    QJsonObject objUser, objDate, objPurch, objFinal;
    QJsonArray purch, date;
    QDateTime time;

    if(purchases.size()>0)
    {
        for(int i(0); i<database.size(); ++i)
        {
            objUser=database.at(i).toObject();
            if(currentUser==objUser["email"].toString())
            {
                purch=objUser["purchases"].toArray();
                for(unsigned i(0); i<purchases.size(); ++i)
                {
                    objPurch["id"]=purchases.at(i);
                    date.append(objPurch);
                }
                objDate[(time.currentDateTime().toString("dd/MM/yy hh:mm:ss"))]=date;
                purch.append(objDate);
                objUser["purchases"]=purch;

                objFinal["name"] = objUser["name"].toString();
                objFinal["email"] = objUser["email"].toString();
                objFinal["password"] = objUser["password"].toString();
                objFinal["purchases"] = objUser["purchases"].toArray();

                database.replace(i,objFinal);
            }
        }
    }
}

void MainWindow::createGraph()
{
    QJsonObject objUser, objDate, objPurch, objId;
    QJsonArray purch, date;
    string id1, id2;
    QStringList keys;

    for(int i(0); i<database.size(); ++i)
    {
        objUser=database.at(i).toObject();

        purch=objUser["purchases"].toArray();

        for(int j(0); j<purch.size(); ++j)
        {
            objPurch=purch.at(j).toObject();
            keys=objPurch.keys();
            date=objPurch[keys.first()].toArray();
            keys.removeFirst();
            for(int k(0); k<date.size()-1; ++k)
            {
                objId=date.at(k).toObject();
                id1=objId["id"].toString().toStdString();
                cout<<id1<<endl;
                for(int l(k+1); l<date.size(); ++l)
                {
                    objId=date.at(l).toObject();
                    id2=objId["id"].toString().toStdString();
                    cout<<"\t"<<id2<<endl;
                    if(grafo.isEdge(id1,id2))
                    {
                        grafo.createEdge(id1,id2,(grafo.getCost(id1,id2))+1);
                    }
                    else
                    {
                        grafo.createEdge(id1,id2,1);
                    }
                }
            }
        }
    }
}

void MainWindow::chargeRecommendations(const QString id)
{

    if(grafo.contains(id.toStdString()))
    {
        map<string, int> neighbors=grafo.getNeighbors(id.toStdString());
        Recommend recommendation;
        typename map<string, int>::iterator it;
        it=neighbors.begin();
        while(it!=neighbors.end())
        {
            recommendation.setId(QString::fromStdString(it->first));
            recommendation.setPeso(it->second);
            unsigned i;
            for(i=0; i<recommendVec.size(); ++i)
            {
                if(recommendVec.at(i).getId()==recommendation.getId())
                {
                    if(recommendVec.at(i).getPeso()<recommendation.getPeso())
                    {
                        recommendVec.at(i).setPeso(recommendation.getPeso());
                    }
                }
            }
            if(i==recommendVec.size())
            {
                recommendVec.push_back(recommendation);
            }

            ++it;
        }

        Recommend temp;
        for(unsigned i(0); i < recommendVec.size()-1; ++i){
            for(unsigned j(0); j < recommendVec.size()-1; ++j){
                if(recommendVec.at(j+1).getPeso() > recommendVec[j].getPeso()){
                    temp = recommendVec.at(j);
                    recommendVec.at(j) = recommendVec.at(j+1);
                    recommendVec.at(j+1) = temp;
                }
            }
        }
    }
    showRecommendations();
}

void MainWindow::showRecommendations()
{
    QString id, name, price;
    float pri;
    layoutProducts* lop;
    QLayoutItem* child;
    while((child = ui->recommendGL->takeAt(0))!=0)
    {
        delete child->widget();
    }
    for(unsigned j(0); j<recommendVec.size(); ++j)
    { 
        if(j==5)
            break;
        for(unsigned i=0; i<prod.size(); ++i)
        {
            if(recommendVec.at(j).getId()==prod.at(i).getId())
            {
                lop = new layoutProducts();
                id=prod.at(i).getId();
                name=prod.at(i).getName();
                pri=prod.at(i).getPrice();
                price=QString::number(pri);
                lop->setProduct(id,name,price);
                lop->setId(prod.at(i).getId());
                connect(lop, SIGNAL(added(QString, int)), this, SLOT(addToPurchase(QString, int)));
                ui->recommendGL->addWidget(lop, j/5, j%5,Qt::AlignCenter);
            }
        }

    }
}



//Slots
void MainWindow::on_loginEmailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableLoginPB();
}

void MainWindow::on_newUserLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableCreatePB();
}

void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableCreatePB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableCreatePB();
}

void MainWindow::on_createPB_clicked()
{
    QMessageBox msg;
    User u;
    string correo;
    QJsonObject jsonObj;

    correo = ui->emailLE->text().toStdString();
    if(!validarCorreo(correo))
    {
        msg.setText("Invalid Email");
        msg.exec();
    }
    else
    {
        size_t i;
        bool b=false;
        for(i=0; i<users.size(); i++)
        {
            if(users.at(i).getEmail() == ui->emailLE->text())
            {
                msg.setText("Email already exist");
                msg.exec();
                b=true;
            }
        }
        if(!b)
        {
            u.setName(ui->newUserLE->text());
            u.setEmail(ui->emailLE->text());
            u.setPassword(ui->newPasswordLE->text());

            users.push_back(u);

            jsonObj["name"]=u.getName();
            jsonObj["email"]=u.getEmail();
            jsonObj["password"]=u.getPassword();
            database.append(jsonObj);

            msg.setText("New user has been created!");
            msg.exec();
        }
    }
    ui->newUserLE->clear();
    ui->emailLE->clear();
    ui->newPasswordLE->clear();
}

void MainWindow::on_loginPB_clicked()
{
    validateUser();
    ui->loginEmailLE->clear();
    ui->passwordLE->clear();
}

void MainWindow::openFile()
{
    QString fileName;

    fileName=QFileDialog::getOpenFileName(this, "Nile Database", "", "JSON files(*.json)");
    if(fileName.length()>0)
    {
        dbFile.setFileName(fileName);
        loadDB();
        createGraph();
        grafo.printData();
        ui->loginFrame->setEnabled(true);
    }
}

void MainWindow::addToPurchase(QString id, int ammount)
{
    QJsonObject jsonObj;

    for(unsigned i(0); i<prod.size(); ++i)
    {
        if(id==prod.at(i).getId())
        {
            prod.at(i).setSold(prod.at(i).getSold()+ammount);
            jsonObj["id"]=prod.at(i).getId();
            jsonObj["name"]=prod.at(i).getName();
            jsonObj["price"]=prod.at(i).getPrice();
            jsonObj["sold"]=prod.at(i).getSold();
            dbProducts.replace(i, jsonObj);
            break;
        }
    }
    unsigned i;
    for(i=0;i<purchases.size();++i)
    {
        if(id==purchases.at(i))
            break;
    }
    if(i==purchases.size())
        purchases.push_back(id);

    chargeRecommendations(id);

    vector<Recommend>::iterator it = recommendVec.begin();
    while(it!=recommendVec.end())
    {
        if(id==it->getId())
        {
            recommendVec.erase(it);
            showRecommendations();
            break;
        }
        ++it;
    }

}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    QLayoutItem* child;

    while((child = ui->productsGL->takeAt(0))!=0)
    {
        delete child->widget();
    }
    chargeProducts();
    showProducts();
}

void MainWindow::on_minPricePB_clicked()
{
    QLayoutItem* child;

    while((child = ui->productsGL->takeAt(0))!=0)
    {
        delete child->widget();
    }
    minPriceProducts();
    showProducts();
}

void MainWindow::on_maxPricePB_clicked()
{
    QLayoutItem* child;

    while((child = ui->productsGL->takeAt(0))!=0)
    {
        delete child->widget();
    }
    maxPriceProducts();
    showProducts();
}

void MainWindow::on_srchLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSrchPB();
}

void MainWindow::on_srchPB_clicked()
{
    QLayoutItem* child;

    while((child = ui->productsGL->takeAt(0))!=0)
    {
        delete child->widget();
    }
    srchProducts();
    showProducts();
    chargeProducts();
}



//Vector setters y getters
vector<User> MainWindow::getUsers() const
{
    return users;
}

void MainWindow::setUsers(const vector<User> &value)
{
    users = value;
}

vector<Products> MainWindow::getProd() const
{
    return prod;
}

void MainWindow::setProd(const vector<Products> &value)
{
    prod = value;
}
