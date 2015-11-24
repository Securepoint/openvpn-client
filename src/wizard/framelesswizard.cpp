#include "framelesswizard.h"

#include <QFrame>
#include <QLabel>

#include <map>
//#include <WinSock2.h>
#include <Windows.h>

#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>

#include <mutex>

WNDPROC pfOriginalFramelessWizardProc;
std::map<HWND, WNDPROC> wndProcMapWizard;

float windowsDpiScale();

static const int CloseButtonWidthWizard = 21;
static const int DragAreaHeightWizard = 20;

LRESULT FramelessWizardProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
                    if (p.y < DragAreaHeightWizard * windowsDpiScale())
                    {
                        hit = HTCAPTION;
                    }

                    RECT rect;
                    GetWindowRect(hwnd, &rect);

                    if(p.x > ((rect.right - rect.left) - CloseButtonWidthWizard*windowsDpiScale()))
                    {
                        for(auto keks : wndProcMapWizard)
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
        for(auto keks : wndProcMapWizard)
        {
            if(keks.first == hwnd)
                return CallWindowProc(keks.second, hwnd, uMsg, wParam, lParam);
        }
    }

    return 0;
}

FramelessWizard::FramelessWizard(QWidget * parent)
    : QWizard(parent),
    bIsFramelessSetup(false),
	line(0),
	line_2(0),
	line_3(0),
	line_4(0),
	line_5(0),
	cmdClose(0)
{
    pfOriginalFramelessWizardProc = (WNDPROC)SetWindowLong((HWND)winId(), GWL_WNDPROC, (long)FramelessWizardProc);

    wndProcMapWizard.insert(std::make_pair((HWND)winId(), pfOriginalFramelessWizardProc));

    this->setWindowFlags(Qt::WindowCloseButtonHint);
    DWORD style = GetWindowLong((HWND)winId(), GWL_STYLE);
    //style |= WS_CAPTION;
    //style |= WS_SYSMENU;
    SetWindowLong((HWND)winId(), GWL_STYLE, style);
};

void FramelessWizard::setupFrameless()
{
     /* Make the line stuff border and line below title */

    if(!bIsFramelessSetup)
    {
        auto size = this->size();


        auto windowTitleLabel_BG = new QLabel(this);
        windowTitleLabel_BG->setObjectName(QStringLiteral("windowTitleLabel"));
        QFont font;
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(8);
        windowTitleLabel_BG->setFont(font);
        windowTitleLabel_BG->setStyleSheet(QStringLiteral(""));

        windowTitleLabel_BG->setGeometry(QRect(0, 0, size.width() - CloseButtonWidthWizard, DragAreaHeightWizard * windowsDpiScale()));


        auto  windowTitleIcon = new QLabel(this);
        windowTitleIcon->setObjectName(QStringLiteral("windowTitleIcon"));
        windowTitleIcon->setMaximumSize(QSize(130, DragAreaHeightWizard * windowsDpiScale()));
        windowTitleIcon->setGeometry(QRect(4, 0, 130, DragAreaHeightWizard * windowsDpiScale()));

        windowTitleIcon->setStyleSheet(QStringLiteral(""));
        windowTitleIcon->setPixmap(QPixmap(QString::fromUtf8(":/data/images/logo.png")));

        static QImage img;

        static std::once_flag flag;
        std::call_once(flag, [this]() {
            img.load(":/data/images/logo.png", "PNG");
            img = img.scaled(QSize(122*windowsDpiScale(), 9*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);        
        });

        windowTitleIcon->setPixmap(QPixmap::fromImage(img));
        windowTitleIcon->setMaximumSize(130*windowsDpiScale(), DragAreaHeightWizard * windowsDpiScale());
        windowTitleIcon->setMinimumSize(130*windowsDpiScale(), DragAreaHeightWizard * windowsDpiScale());
        windowTitleIcon->setFixedSize(130*windowsDpiScale(), DragAreaHeightWizard * windowsDpiScale());
        auto g = windowTitleIcon->geometry();
        g.setSize(QSize(130*windowsDpiScale(), DragAreaHeightWizard * windowsDpiScale()));
        windowTitleIcon->setGeometry(g);

        auto windowTitleLabel = new QLabel(this);
        windowTitleLabel->setObjectName(QStringLiteral("windowTitleLabel"));
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(8);
        windowTitleLabel->setFont(font);
        windowTitleLabel->setStyleSheet(QStringLiteral(""));

        windowTitleLabel->setGeometry(QRect(windowTitleIcon->width() + 4, 0, size.width() - CloseButtonWidthWizard, DragAreaHeightWizard * windowsDpiScale()));

        windowTitleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        windowTitleLabel->setText("SSL VPN v2" /*this->windowTitle()*/);



        line = new QFrame(this);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, DragAreaHeightWizard * windowsDpiScale(), size.width(), 1));
        line->setAutoFillBackground(false);
        line->setStyleSheet(QStringLiteral(""));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        line_2 = new QFrame(this);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 0, size.width() + 1, 1));
        line_2->setAutoFillBackground(false);
        line_2->setStyleSheet(QStringLiteral("color: gray;"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setFrameShape(QFrame::HLine);

        line_3 = new QFrame(this);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(0, size.height() - 1, size.width() + 1, 1));
        line_3->setAutoFillBackground(false);
        line_3->setStyleSheet(QStringLiteral("color: gray;"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setFrameShape(QFrame::HLine);

        line_4 = new QFrame(this);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(0, 0, 1, size.height() + 1));
        line_4->setStyleSheet(QStringLiteral("color: gray;"));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setFrameShape(QFrame::VLine);

        line_5 = new QFrame(this);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(size.width() - 1, 0, 1, size.height() + 1));
        line_5->setStyleSheet(QStringLiteral("color: gray;"));
        line_5->setFrameShadow(QFrame::Plain);
        line_5->setFrameShape(QFrame::VLine);

        //auto lbWindowTitle = new QLabel(this);
        //lbWindowTitle->setObjectName(QStringLiteral("lbWindowTitle"));
        //lbWindowTitle->setGeometry(QRect(4, 0, size.width() - CloseButtonWidthWizard, DragAreaHeightWizard * windowsDpiScale()));
        //lbWindowTitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        //lbWindowTitle->setText(this->windowTitle());

        cmdClose = new QPushButton(this);
        cmdClose->setObjectName("close_dat_shit");
        cmdClose->setGeometry(QRect(size.width() - DragAreaHeightWizard*windowsDpiScale(), 2, 16*windowsDpiScale(), 16*windowsDpiScale()));


        {
            QImage img(":/data/images/close.png");
            img = img.scaled(QSize(16*windowsDpiScale(), 16*windowsDpiScale()), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QIcon icon1;
            icon1.addPixmap(QPixmap::fromImage(img));

            cmdClose->setIcon(icon1);
            cmdClose->setIconSize(QSize(16*windowsDpiScale(), 16*windowsDpiScale()));
            cmdClose->setFlat(true);
        }


        QObject::connect(cmdClose, SIGNAL(clicked()), this, SLOT(customCmdClose()));

        bIsFramelessSetup = true;
    }
}

FramelessWizard::~FramelessWizard()
{
    for(auto keks : wndProcMapWizard)
    {
        if(keks.first == (HWND)winId())
        {
            wndProcMapWizard.erase(keks.first);
            break;
        }
    }
}

void FramelessWizard::customCmdClose()
{
    this->close();
}


void FramelessWizard::resizeEvent(QResizeEvent * event)
{

    auto size = this->size();

	if (this->line) {
		line->setGeometry(QRect(0, DragAreaHeightWizard * windowsDpiScale(), size.width(), 1));
	}
    
	if (this->line_2) {
		line_2->setGeometry(QRect(0, 0, size.width() + 1, 1));
	}

	if (this->line_3) {
		line_3->setGeometry(QRect(0, size.height() - 1, size.width() + 1, 1));
	}

	if (this->line_4) {
		line_4->setGeometry(QRect(0, 0, 1, size.height() + 1));
	}

	if (this->line_5) {
		line_5->setGeometry(QRect(size.width() - 1, 0, 1, size.height() + 1));
	}

	if (this->cmdClose) {
		cmdClose->setGeometry(QRect(size.width() - 20 * windowsDpiScale(), 2, 16 * windowsDpiScale(), 16 * windowsDpiScale()));
	}

    QWizard::resizeEvent(event);
}