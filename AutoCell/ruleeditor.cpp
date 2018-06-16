#include "ruleeditor.h"

/** \brief Constructor of the dialog for rule creation
 * \param dimensions Dimensions of the created automaton
 */
RuleEditor::RuleEditor(unsigned int dimensions, QWidget *parent) : QDialog(parent), m_dimensions(dimensions)
{
    QGridLayout *rulesInputLayout = new QGridLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();
    if (m_dimensions > 1)
    {
        m_selectedRule = -1;

        m_rulesListWidget = new QListWidget(this);
        QLabel *rulesLabel = new QLabel("Rules ",this);
        QVBoxLayout *rulesListLayout = new QVBoxLayout();
        rulesListLayout->addWidget(rulesLabel);
        rulesListLayout->addWidget(m_rulesListWidget);
        hlayout->addLayout(rulesListLayout);

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
    }
    else
    {
        rulesInputLayout->addWidget(new QLabel("Automaton number :",this),0,0);
        m_automatonNumber = new QSpinBox(this);
        m_automatonNumber->setMaximum(255);
        m_automatonNumber->setMinimum(0);
        rulesInputLayout->addWidget(m_automatonNumber,0,1);
    }

    hlayout->addLayout(rulesInputLayout);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    if (dimensions > 1)
    {
        m_addBt = new QPushButton("Add Rule",this);
        m_importBt = new QPushButton("Import Rule file",this);
        m_removeBt = new QPushButton("Remove Rule",this);
        buttonLayout->addWidget(m_importBt);
        buttonLayout->addWidget(m_addBt);
        buttonLayout->addWidget(m_removeBt);
    }
    m_doneBt = new QPushButton("Done !",this);


    buttonLayout->addWidget(m_doneBt);

    mainLayout->addLayout(hlayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    if (dimensions > 1)
    {
        connect(m_addBt, SIGNAL(clicked(bool)), this, SLOT(addRule()));
        connect(m_importBt, SIGNAL(clicked(bool)), this, SLOT(importFile()));
        connect(m_removeBt, SIGNAL(clicked(bool)), this, SLOT(removeRule()));
    }
    connect(m_doneBt, SIGNAL(clicked(bool)), this, SLOT(sendRules()));

}


/** \brief Add the rule contained in the fields
 */
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

/** \brief Remove the selected rule
 */
void RuleEditor::removeRule(){
    m_rules.removeAt(m_rulesListWidget->currentRow());
    delete m_rulesListWidget->takeItem(m_rulesListWidget->currentRow());
}

/** \brief Action when we click sur "Done"
 */
void RuleEditor::sendRules(){
    if (m_dimensions == 1)
    {
        QList<const Rule*> ruleList = generate1DRules(m_automatonNumber->value());
        for (const Rule* rule : ruleList) // C++11
        {
            m_rules.append(rule);
        }

    }
    emit rulesFilled(m_rules);
    this->close();
}

/** \brief Import a rule file
 */
void RuleEditor::importFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Rule file"), ".",
                                                    tr("Automaton rule files (*.atr)"));
    if(!fileName.isEmpty()){
        emit fileImported(fileName);
        this->close();
    }
}
