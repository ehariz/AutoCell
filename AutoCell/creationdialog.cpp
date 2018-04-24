#include "creationdialog.h"
#include <iostream>
CreationDialog::CreationDialog(QWidget *parent)
{
    QLabel *m_dimLabel= new QLabel(tr("Write your dimensions and their size, separated by a comma.\n"
                           "For instance, '25,25 ' will create a 2-dimensional 25x25 Automaton. "));
    QLabel *m_densityLabel = new QLabel(tr("Density :"));
    QLabel *m_stateMaxLabel = new QLabel(tr("Max state :"));
    m_densityBox = new QSpinBox();
    m_stateMaxBox = new QSpinBox();

    QHBoxLayout *densityLayout = new QHBoxLayout();
    densityLayout->addWidget(m_densityLabel);
    densityLayout->addWidget(m_densityBox);

    QHBoxLayout *stateMaxLayout = new QHBoxLayout();
    stateMaxLayout->addWidget(m_stateMaxLabel);
    stateMaxLayout->addWidget(m_stateMaxBox);

    m_dimensionsEdit = new QLineEdit;
    QRegExp rgx("([0-9]+,)*");
    QRegExpValidator *v = new QRegExpValidator(rgx, this);
    m_dimensionsEdit->setValidator(v);
    m_doneBt = new QPushButton(tr("Done !"));

    QVBoxLayout *layout = new QVBoxLayout;

    QGroupBox *grpBox = createGenButtons();

    layout->addWidget(m_dimLabel);
    layout->addWidget(m_dimensionsEdit);
    layout->addLayout(densityLayout);
    layout->addLayout(stateMaxLayout);
    layout->addWidget(grpBox);
    layout->addWidget(m_doneBt);
    setLayout(layout);

    connect(m_doneBt, SIGNAL(clicked(bool)), this, SLOT(processSettings()));

}

QGroupBox *CreationDialog::createGenButtons(){
    m_groupBox = new QGroupBox(tr("Cell generation settings"));
    m_empGen = new QRadioButton(tr("&Empty Board"));
    m_randGen = new QRadioButton(tr("&Random"));
    m_symGen = new QRadioButton(tr("&Symmetrical"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_empGen);
    layout->addWidget(m_randGen);
    layout->addWidget(m_symGen);

    m_groupBox->setLayout(layout);

    return m_groupBox;
}


void CreationDialog::processSettings(){
    QString dimensions = m_dimensionsEdit->text();
    if(dimensions.length() == 0){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You must specify valid dimensions !");
        messageBox.setFixedSize(500,200);
    }
    else{
        CellHandler::generationTypes genType;
        if(m_randGen == NULL)std::cout << "Radio button null line 68 \n" << std::flush;
        if(m_symGen->isChecked()) genType = CellHandler::generationTypes::symetric;
        else if(m_randGen->isChecked()) genType = CellHandler::generationTypes::random;
        else genType = CellHandler::generationTypes::empty;
        QStringList dimList = m_dimensionsEdit->text().split(",");
        QVector<unsigned int> dimensions;
        for(int i = 0; i < dimList.size(); i++) dimensions.append(dimList.at(i).toInt());

        emit settingsFilled(dimensions, genType, m_stateMaxBox->value(), m_densityBox->value());
        this->close();
    }

}

