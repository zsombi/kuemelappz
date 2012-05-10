#include "globaldefs.h"
#include "applicationitem.h"
#include "applicationitem_p.h"
#include "widgetset.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeProperty>
#include "screen.h"

#ifdef MOBILE_SYSTEM
#include <QOrientationSensor>
#include <QOrientationReading>
QTM_USE_NAMESPACE
#endif

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

void ApplicationItemPrivate::initializePlatform()
{
    //QCoreApplication::setAttribute(Qt::AA_S60DontConstructApplicationPanes);
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    TRAPD(error,
        if (appUi)
            appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
    );
    Q_UNUSED(error)
    // connect scene update to catch screen size
    q_ptr->connect(appView, SIGNAL(sceneResized(QSize)), SLOT(_q_sceneUpdate(const QSize&)), Qt::QueuedConnection);

}

void ApplicationItemPrivate::platformCompleted()
{
    // do nothing
}
