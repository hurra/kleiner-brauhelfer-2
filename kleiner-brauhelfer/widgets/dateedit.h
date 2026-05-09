#ifndef DATEEDIT_H
#define DATEEDIT_H

#include <QDateEdit>

class DateEdit : public QDateEdit
{
    Q_OBJECT

public:
    explicit DateEdit(QWidget *parent = nullptr);
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    void addChangeDecorator();
    void setEnabled(bool e);
    void setDisabled(bool d);
    void setReadOnly(bool r);
    bool hasFocus() const;
    void setError(bool e);
private:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void updatePalette();
private:
    bool mError;
};

#endif // DATEEDIT_H
