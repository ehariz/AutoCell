#include "ruleeditor.h"

RuleEditor::RuleEditor(QWidget *parent)
{
    selectedRule = -1;
    QHBoxLayout *hlayout = new QHBoxLayout();
    m_rulesListWidget = new QListWidget(this);
    QLabel *rulesLabel = new QLabel("Rules ",this);
    QVBoxLayout *rulesListLayout = new QVBoxLayout();
    rulesListLayout->addWidget(rulesLabel);
    rulesListLayout->addWidget(m_rulesListWidget);
    hlayout->addLayout(rulesListLayout);

    QGridLayout *rulesInputLayout = new QGridLayout();

    rulesInputLayout->addWidget(new QLabel("Current cell values :",this),0,0);
    m_currentStatesEdit = new QLineEdit(this);
    QRegExp rgx("([0-9]+,)*");
    QRegExpValidator *v = new QRegExpValidator(rgx, this);
    m_currentStatesEdit->setValidator(v);
    rulesInputLayout->addWidget(m_currentStatesEdit,0,1);

    rulesInputLayout->addWidget(new QLabel("Neighbour number lower bound :",this),1,0);
    m_lowerNeighbourBox = new QSpinBox(this);
    rulesInputLayout->addWidget(m_lowerNeighbourBox,1,1);

    rulesInputLayout->addWidget(new QLabel("Neighbour number upper bound :",this),2,0);
    m_upperNeighbourBox = new QSpinBox(this);
    rulesInputLayout->addWidget(m_upperNeighbourBox,2,1);

    rulesInputLayout->addWidget(new QLabel("Neighbour values :",this),3,0);
    m_neighbourStatesEdit = new QLineEdit(this);
    m_neighbourStatesEdit->setValidator(v);
    rulesInputLayout->addWidget(m_neighbourStatesEdit,3,1);

    rulesInputLayout->addWidget(new QLabel("Output state :",this),4,0);
    m_outputStateBox = new QSpinBox(this);
    rulesInputLayout->addWidget(m_outputStateBox,4,1);

    hlayout->addLayout(rulesInputLayout);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_addBt = new QPushButton("Add Rule",this);
    m_importBt = new QPushButton("Import Rule file",this);
    m_doneBt = new QPushButton("Done !",this);
    m_removeBt = new QPushButton("Remove Rule",this);
    buttonLayout->addWidget(m_importBt);
    buttonLayout->addWidget(m_addBt);
    buttonLayout->addWidget(m_removeBt);
    buttonLayout->addWidget(m_doneBt);

    mainLayout->addLayout(hlayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(m_addBt, SIGNAL(clicked(bool)), this, SLOT(addRule()));
    connect(m_importBt, SIGNAL(clicked(bool)), this, SLOT(importFile()));
    connect(m_doneBt, SIGNAL(clicked(bool)), this, SLOT(sendRules()));
    connect(m_removeBt, SIGNAL(clicked(bool)), this, SLOT(removeRule()));

}



void RuleEditor::addRule(){
    unsigned int outputState = m_outputStateBox->value();
    QVector<unsigned int> currentCellValues;
    QStringList valList = m_currentStatesEdit->text().split(",");
    for(int i = 0; i < valList.size(); i++) currentCellValues.append(valList.at(i).toInt());

    QPair<unsigned int, unsigned int> neighbourInterval;
    neighbourInterval.first = m_lowerNeighbourBox->value();
    neighbourInterval.second = m_upperNeighbourBox->value();

    QSet<unsigned int> neighbourValues;
    valList = m_neighbourStatesEdit->text().split(",");
    for(int i = 0; i < valList.size(); i++) neighbourValues << valList.at(i).toInt();

    m_rules.append(new NeighbourRule(outputState,currentCellValues,neighbourInterval,neighbourValues));

    QString listLabel = m_currentStatesEdit->text()+" -> "+QString::number(m_outputStateBox->value())
                        +" if "+QString::number(m_lowerNeighbourBox->value())+" to "+
                        QString::number(m_upperNeighbourBox->value())+" neighbours are in states "+
                        m_neighbourStatesEdit->text();
    m_rulesListWidget->addItem(listLabel);
}

void RuleEditor::removeRule(){
    m_rules.removeAt(m_rulesListWidget->currentRow());
    delete m_rulesListWidget->takeItem(m_rulesListWidget->currentRow());
}

void RuleEditor::sendRules(){
    emit rulesFilled(m_rules);
    this->close();
}

void RuleEditor::importFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Rule file"), ".",
                                                    tr("Automaton rule files (*.atr)"));
    if(!fileName.isEmpty()){
        emit fileImported(fileName);
        this->close();
    }
}
