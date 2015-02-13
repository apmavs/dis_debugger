#ifndef TEXTENTRYPOPUP_H
#define TEXTENTRYPOPUP_H

#include <QDialog>

namespace Ui {
class TextEntryPopup;
}

class TextEntryPopup : public QDialog
{
    Q_OBJECT
private:
    Ui::TextEntryPopup *ui;
    bool first_popup;

private slots:
    void okPressed();
    void cancelPressed();

signals:
    // If OK button is pushed, send this signal with what was typed in
    void confirmedEdit(QString value);

public:
    explicit TextEntryPopup(QWidget* parent = 0);
    ~TextEntryPopup();
    void popup(QString request, QString defaultVal = "");

};

#endif // TEXTENTRYPOPUP_H
