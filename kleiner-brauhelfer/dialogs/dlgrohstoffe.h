#ifndef DLG_ROHSTOFFE_H
#define DLG_ROHSTOFFE_H

#include "dlgabstract.h"
#include <QTableView>

namespace Ui {
class DlgRohstoffe;
}

class DlgRohstoffe : public DlgAbstract
{
    Q_OBJECT

public:
    static QStringList HopfenTypname;
    static QStringList HefeTypname;
    static QStringList HefeTypFlTrName;
    static QStringList ZusatzTypname;
    static QStringList Einheiten;
    static QStringList list_tr(const QStringList& list);

    static DlgRohstoffe *Dialog;

public:
    explicit DlgRohstoffe(QWidget *parent = nullptr);
    virtual ~DlgRohstoffe() Q_DECL_OVERRIDE;
    void saveSettings() Q_DECL_OVERRIDE;
    void loadSettings() Q_DECL_OVERRIDE;
    static void restoreView();
    void modulesChanged(Settings::Modules modules) Q_DECL_OVERRIDE;

private slots:
    void on_tableMalz_clicked(const QModelIndex &index);
    void on_tableHopfen_clicked(const QModelIndex &index);
    void on_tableHefe_clicked(const QModelIndex &index);
    void on_tableWeitereZutaten_clicked(const QModelIndex &index);
    void buttonAdd_clicked();
    void buttonNeuVorlage_clicked();
    void buttonNeuVorlageObrama_clicked();
    void on_buttonDelete_clicked();
    void buttonCopy_clicked();
    void on_radioButtonAlle_clicked();
    void on_radioButtonVorhanden_clicked();
    void on_radioButtonInGebrauch_clicked();
    void on_lineEditFilter_textChanged(const QString &pattern);
    void on_toolBoxRohstoffe_currentChanged();

private:
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void addEntry(QTableView *table, const QMap<int, QVariant> &values);
    void updateLabelNumItems();

private:
    Ui::DlgRohstoffe *ui;
    int mRowWasser;
};

#endif // DLG_ROHSTOFFE_H