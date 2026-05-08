#include "wdgsudstatus.h"
#include <QPainter>
#include <QStaticText>
#include "settings.h"
#include "brauhelfer.h"

extern Settings* gSettings;

WdgSudStatus::WdgSudStatus(QWidget *parent) :
    QWidget(parent),
    mSud(nullptr)
{
}

QSize WdgSudStatus::minimumSizeHint() const
{
    return QSize(200, 18);
}

QSize WdgSudStatus::sizeHint() const
{
    return QSize(400, 18);
}

void WdgSudStatus::setSud(SudObject *sud)
{
    mSud = sud;
    connect(mSud, &SudObject::loadedChanged, this, [this](){update();});
    connect(mSud, &SudObject::dataChanged, this, [this](){update();});
}

SudObject* WdgSudStatus::getSud() const
{
    return mSud;
}

void WdgSudStatus::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!mSud)
        return;

    bool enabled = mSud->isLoaded();
    Brauhelfer::SudStatus status = static_cast<Brauhelfer::SudStatus>(mSud->getStatus());

    const QColor colorBase = palette().color(QPalette::ColorGroup::Disabled, QPalette::Text);
    const QColor colorText = palette().color(enabled ? QPalette::ColorGroup::Active : QPalette::ColorGroup::Disabled, QPalette::Text);

    int y = height()/2;

    QString str;
    QPainter painter(this);
    QFont font = painter.font();
    QFontMetrics fm(font);
    QLocale locale = QLocale();

    int x0 = 10;
    if (enabled)
    {
        str = mSud->getSudname();
        painter.setPen(QPen(enabled ? gSettings->colorRezept : colorBase, 2, enabled ? Qt::SolidLine : Qt::DashLine));
        painter.drawLine(10, y, 20, y);
        painter.setPen(colorText);
        font.setBold(true);
        painter.setFont(font);
        x0 += QFontMetrics(font).boundingRect(str).width() + 30;
        painter.drawStaticText(20 + 10, 0, QStaticText(str));
        font.setBold(false);
        painter.setFont(font);
    }

    int d = 14;
    int dh = d/2;
    int w = width() - x0;
    int x1 = 1*w/5 + x0;
    int x2 = 2*w/5 + x0;
    int x3 = 3*w/5 + x0;
    int x4 = 4*w/5 + x0;

    painter.setPen(QPen(enabled ? gSettings->colorRezept : colorBase, 2, enabled ? Qt::SolidLine : Qt::DashLine));
    painter.drawLine(x0, y, x1 - dh - 10, y);

    str = tr("Rezept");
    painter.setPen(QPen(enabled ? gSettings->colorGebraut : colorBase, 2, (enabled && status >= Brauhelfer::SudStatus::Gebraut) ? Qt::SolidLine : Qt::DashLine));
    painter.drawLine(x1 + fm.boundingRect(str).width() + d + 10, y, x2 - dh - 10, y);
    painter.setPen(colorBase);
    painter.setPen(colorText);
    painter.drawStaticText(x1 + d, 0, QStaticText(str));

    if (status >= Brauhelfer::SudStatus::Gebraut && w > 800)
        str = tr("Gebraut") + " " + locale.toString(mSud->getBraudatum().date(), QLocale::ShortFormat);
    else
        str = tr("Gebraut");
    painter.setPen(QPen(enabled ? gSettings->colorAbgefuellt : colorBase, 2, (enabled && status >= Brauhelfer::SudStatus::Abgefuellt) ? Qt::SolidLine : Qt::DashLine));
    painter.drawLine(x2 + fm.boundingRect(str).width() + d + 10, y, x3 - dh - 10, y);
    painter.setPen(colorText);
    painter.drawStaticText(x2 + d, 0, QStaticText(str));

    if (status >= Brauhelfer::SudStatus::Abgefuellt && w > 800)
        str = tr("Abgefüllt") + " " + locale.toString(mSud->getAbfuelldatum().date(), QLocale::ShortFormat);
    else
        str = tr("Abgefüllt");
    painter.setPen(QPen(enabled ? gSettings->colorAusgetrunken : colorBase, 2, (enabled && status >= Brauhelfer::SudStatus::Verbraucht) ? Qt::SolidLine : Qt::DashLine));
    painter.drawLine(x3 + fm.boundingRect(str).width() + d + 10, y, x4 - dh - 10, y);
    painter.setPen(colorText);
    painter.drawStaticText(x3 + d, 0, QStaticText(str));

    str = tr("Ausgetrunken");
    painter.setPen(QPen(colorBase, 2, (enabled && status >= Brauhelfer::SudStatus::Verbraucht) ? Qt::SolidLine : Qt::DashLine));
    painter.drawLine(x4 + fm.boundingRect(str).width() + d + 10, y, width()-20, y);
    painter.setPen(colorText);
    painter.drawStaticText(x4 + d, 0, QStaticText(str));

    painter.setPen(QPen(colorBase, 2));
    painter.setBrush(palette().base().color());
    painter.drawEllipse(x1-dh, y-dh, d, d);
    painter.drawEllipse(x2-dh, y-dh, d, d);
    painter.drawEllipse(x3-dh, y-dh, d, d);
    painter.drawEllipse(x4-dh, y-dh, d, d);

    if (enabled)
    {
        int d2 = d - 4;
        painter.setPen(Qt::NoPen);
        painter.setBrush(gSettings->colorRezept);
        painter.drawEllipse(x1-d2/2, y-d2/2, d2, d2);
        if (status >= Brauhelfer::SudStatus::Gebraut)
        {
            painter.setBrush(gSettings->colorGebraut);
            painter.drawEllipse(x2-d2/2, y-d2/2, d2, d2);
        }
        if (status >= Brauhelfer::SudStatus::Abgefuellt)
        {
            painter.setBrush(gSettings->colorAbgefuellt);
            painter.drawEllipse(x3-d2/2, y-d2/2, d2, d2);
        }
        if (status >= Brauhelfer::SudStatus::Verbraucht)
        {
            painter.setBrush(gSettings->colorAusgetrunken);
            painter.drawEllipse(x4-d2/2, y-d2/2, d2, d2);
        }
    }
}
