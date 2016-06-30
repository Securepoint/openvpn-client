#include "framelessdialog.h"

#include <QFrame>

#include <QLabel>
#include <QPushbutton>
#include <QHBoxLayout>

#include <map>
//#include <WinSock2.h>
#include <Windows.h>
#include <mutex>



WNDPROC pfOriginalFramelessdialogProc;
std::map<HWND, WNDPROC> wndProcMap;

static const int CloseButtonWidth = 21;
static const int DragAreaHeight = 20;

float windowsDpiScale();

LRESULT FramelessDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //
    //
    //

    switch (uMsg) {
    case WM_NCHITTEST: {
        LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);
        if (hit == HTCLIENT)
        {
            POINT p;
            if (GetCursorPos(&p))
            {
                if (ScreenToClient(hwnd, &p))
                {
                    if (p.y < DragAreaHeight * windowsDpiScale())
                    {
                        hit = HTCAPTION;
                    }

                    RECT rect;
                    GetWindowRect(hwnd, &rect);

                    if(p.x > ((rect.right - rect.left) - CloseButtonWidth*windowsDpiScale()))
                    {
                        for(auto keks : wndProcMap)
                        {
                            if(keks.first == hwnd)
                            {

                                return CallWindowProc(keks.second, hwnd, uMsg, wParam, lParam);
                            }
                        }
                    }
                }
            }
        }

        return hit;
                       }

    default:
        for(auto keks : wndProcMap)
        {
            if(keks.first == hwnd)
                return CallWindowProc(keks.second, hwnd, uMsg, wParam, lParam);
        }
    }

    return 0;
}

FramelessDialog::FramelessDialog(QWidget * parent)
    : QDialog(parent),
    wrapper(nullptr),
    bIsFramelessSetup(false)
{
    pfOriginalFramelessdialogProc = (WNDPROC)SetWindowLong((HWND)winId(), GWL_WNDPROC, (long)FramelessDialogProc);

    wndProcMap.insert(std::make_pair((HWND)winId(), pfOriginalFramelessdialogProc));

    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
    DWORD style = GetWindowLong((HWND)winId(), GWL_STYLE);
    //style |= WS_CAPTION;
    //style |= WS_SYSMENU;
    SetWindowLong((HWND)winId(), GWL_STYLE, style);
};

QWidget * FramelessDialog::getWrapperWidget()
{
    if(!wrapper)
        wrapper = new QWidget(this);
    return wrapper;
}

void FramelessDialog::postSetupFrameless()
{

}

void FramelessDialog::setupFrameless()
{
    /* Make the line stuff border and line below title */

    if(!bIsFramelessSetup)
    {
        auto geo = wrapper->geometry();
        geo.adjust(0, DragAreaHeight*windowsDpiScale(), 0, DragAreaHeight*windowsDpiScale());
        wrapper->setGeometry(geo);

        this->setMaximumSize(wrapper->maximumSize().width(), wrapper->maximumSize().height() + DragAreaHeight*windowsDpiScale());
        this->setMinimumSize(wrapper->minimumSize().width(), wrapper->minimumSize().height() + DragAreaHeight*windowsDpiScale());

        auto geom = geometry();
        geom.setSize(QSize(geo.width(), geo.height() + DragAreaHeight*windowsDpiScale()));
        setGeometry(geom);

        wrapper->setMaximumSize(wrapper->maximumSize().width(), wrapper->maximumSize().height() + DragAreaHeight*windowsDpiScale());
        wrapper->setMinimumSize(wrapper->minimumSize().width(), wrapper->minimumSize().height() + DragAreaHeight*windowsDpiScale());

        this->setWindowFlags(Qt::FramelessWindowHint);
        auto size = this->size();


        auto windowTitleLabel_BG = new QLabel(this);
        windowTitleLabel_BG->setObjectName(QStringLiteral("windowTitleLabel"));
        QFont font;
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(8);
        windowTitleLabel_BG->setFont(font);
        windowTitleLabel_BG->setStyleSheet(QStringLiteral(""));

        windowTitleLabel_BG->setGeometry(QRect(0, 0, size.width() - CloseButtonWidth, DragAreaHeight*windowsDpiScale()));


        auto windowTitleIcon = new QLabel(this);
        windowTitleIcon->setObjectName(QStringLiteral("windowTitleIcon"));
        windowTitleIcon->setMaximumSize(QSize(130, DragAreaHeight*windowsDpiScale()));
        windowTitleIcon->setGeometry(QRect(4, 0, 130, DragAreaHeight*windowsDpiScale()));

        windowTitleIcon->setStyleSheet(QStringLiteral(""));
        windowTitleIcon->setPixmap(QPixmap(QString::fromUtf8(":/data/images/logo.png")));

        static QImage img;

        static std::once_flag flag;
        std::call_once(flag, [this]() {
            img.load(":/data/images/logo.png", "PNG");
            img = img.scaled(QSize(122*windowsDpiScale(), 9*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        });

        windowTitleIcon->setPixmap(QPixmap::fromImage(img));
        windowTitleIcon->setMaximumSize(130*windowsDpiScale(), DragAreaHeight*windowsDpiScale());
        windowTitleIcon->setMinimumSize(130*windowsDpiScale(), DragAreaHeight*windowsDpiScale());
        windowTitleIcon->setFixedSize(130*windowsDpiScale(), DragAreaHeight*windowsDpiScale());
        auto g = windowTitleIcon->geometry();
        g.setSize(QSize(130*windowsDpiScale(), DragAreaHeight*windowsDpiScale()));
        windowTitleIcon->setGeometry(g);


        auto windowTitleLabel = new QLabel(this);
        windowTitleLabel->setObjectName(QStringLiteral("windowTitleLabel"));
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(8);
        windowTitleLabel->setFont(font);
        windowTitleLabel->setStyleSheet(QStringLiteral(""));

        windowTitleLabel->setGeometry(QRect(windowTitleIcon->width() + 4, 0, size.width() - CloseButtonWidth, DragAreaHeight*windowsDpiScale()));

        windowTitleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        windowTitleLabel->setText("SSL VPN v2" /*this->windowTitle()*/);

        auto line = new QFrame(this);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, DragAreaHeight*windowsDpiScale(), size.width(), 1));
        line->setAutoFillBackground(false);
        line->setStyleSheet(QStringLiteral(""));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        auto line_2 = new QFrame(this);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 0, size.width() + 1, 1));
        line_2->setAutoFillBackground(false);
        line_2->setStyleSheet(QStringLiteral("color: gray;"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setFrameShape(QFrame::HLine);

        auto line_3 = new QFrame(this);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(0, size.height() - 1, size.width() + 1, 1));
        line_3->setAutoFillBackground(false);
        line_3->setStyleSheet(QStringLiteral("color: gray;"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setFrameShape(QFrame::HLine);

        auto line_4 = new QFrame(this);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(0, 0, 1, size.height() + 1));
        line_4->setStyleSheet(QStringLiteral("color: gray;"));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setFrameShape(QFrame::VLine);

        auto line_5 = new QFrame(this);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(size.width() - 1, 0, 1, size.height() + 1));
        line_5->setStyleSheet(QStringLiteral("color: gray;"));
        line_5->setFrameShadow(QFrame::Plain);
        line_5->setFrameShape(QFrame::VLine);

       /* auto lbWindowTitle = new QLabel(this);
        lbWindowTitle->setObjectName(QStringLiteral("lbWindowTitle"));
        lbWindowTitle->setGeometry(QRect(4, 0, size.width() - CloseButtonWidth, DragAreaHeight));
        lbWindowTitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        lbWindowTitle->setText(this->windowTitle());
*/


     /*  auto windowTitleIcon = new QLabel(this);
       auto windowTitleLabel = new QLabel(this);
       auto horizontalSpacer = new QSpacerItem(40, 20);

       windowTitleIcon->setPixmap(QPixmap(":/data/images/logo.png"));
       windowTitleLabel->setText("SSL VPN v2");
       hHeader->addWidget(windowTitleIcon);
       hHeader->addWidget(windowTitleLabel);*/

        auto cmdClose_2 = new QPushButton(this);
        cmdClose_2->setObjectName(QStringLiteral("close_dat_shit"));
        cmdClose_2->setGeometry(QRect(size.width() - DragAreaHeight*windowsDpiScale(), 2, 16*windowsDpiScale(), 16*windowsDpiScale()));

        connectionClose = QObject::connect(cmdClose_2, &QPushButton::clicked, [&]() {
            this->close();
        });

        {
            QImage img(":/data/images/close.png");
            img = img.scaled(QSize(16*windowsDpiScale(), 16*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QIcon icon1;
            icon1.addPixmap(QPixmap::fromImage(img));

            cmdClose_2->setIcon(icon1);
            cmdClose_2->setIconSize(QSize(16*windowsDpiScale(), 16*windowsDpiScale()));
            cmdClose_2->setFlat(true);
        }
        bIsFramelessSetup = true;
    }
}

FramelessDialog::~FramelessDialog()
{
    for(auto keks : wndProcMap)
    {
        if(keks.first == (HWND)winId())
        {
            wndProcMap.erase(keks.first);
            break;
        }
    }

    QObject::disconnect(connectionClose);
}
