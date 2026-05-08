#ifndef WDGSUDSTATUS_H
#define WDGSUDSTATUS_H

#include <QWidget>
#include "sudobject.h"

class WdgSudStatus : public QWidget
{
    Q_OBJECT

public:
    explicit WdgSudStatus(QWidget *parent = nullptr);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void setSud(SudObject* sud);
    SudObject* getSud() const;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    SudObject* mSud;
};

#endif // WDGSUDSTATUS_H
