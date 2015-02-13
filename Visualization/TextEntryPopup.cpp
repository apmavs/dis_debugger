#include "TextEntryPopup.h"
#include "ui_TextEntryPopup.h"

TextEntryPopup::TextEntryPopup(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TextEntryPopup)
{
    ui->setupUi(this);
    setVisible(false);
    first_popup = false;
}

void TextEntryPopup::okPressed()
{
    emit confirmedEdit(ui->EditEntry->text());
    setVisible(false);
}

void TextEntryPopup::cancelPressed()
{
    setVisible(false);
}

TextEntryPopup::~TextEntryPopup()
{
    delete ui;
}

void TextEntryPopup::popup(QString request, QString defaultVal)
{
    ui->TitleLabel->setText(request);
    ui->EditEntry->setText(defaultVal);
    setVisible(true);
    if(first_popup)
    {
        first_popup = false;
        exec();
    }
}
