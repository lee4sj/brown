/*!

  Main window class for CS123. See the header file for additional documentation.

  @author Evan Wallace (evan.exe@gmail.com)
  @author Ben Herila (ben@herila.net)

**/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Databinding.h"
#include "Settings.h"
#include "scenegraph/RayScene.h"
#include "scenegraph/ShapesScene.h"
#include "scenegraph/SceneviewScene.h"
#include "camera/CamtransCamera.h"
#include "CS123XmlSceneParser.h"
#include <QSettings>
#include <math.h>
#include <QFileDialog>
#include <QMessageBox>
#include <assert.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Make sure the settings are loaded before the UI
    settings.loadSettingsOrDefaults();

    ui->setupUi(this);

    // Restore the UI settings
    QSettings qtSettings("CS123");
    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());

    // Resize the window because the window is huge since all docks were visible
    resize(1000, 600);

    // Bind the UI elements to their properties in the global Settings object, using binding objects to simplify
    // the code.  Each binding object connects to its UI elements and keeps the UI and its setting in sync.

    QList<QAction*> actions;

#define SETUP_ACTION(dock, key) \
    actions.push_back(ui->dock->toggleViewAction()); \
    actions.back()->setShortcut(QKeySequence(key));

    SETUP_ACTION(brushDock,     "CTRL+1");
    SETUP_ACTION(filterDock,    "CTRL+2");
    SETUP_ACTION(shapesDock,    "CTRL+3");
    SETUP_ACTION(camtransDock,  "CTRL+4");
    SETUP_ACTION(rayDock,       "CTRL+5");

    ui->menuToolbars->addActions(actions);
#undef SETUP_ACTION

    tabifyDockWidget(ui->brushDock, ui->filterDock);
    tabifyDockWidget(ui->brushDock, ui->shapesDock);
    tabifyDockWidget(ui->brushDock, ui->camtransDock);
    tabifyDockWidget(ui->brushDock, ui->rayDock);
    ui->brushDock->raise();

    dataBind();

    // Hide the "stop rendering" button until we need it
    ui->rayStopRenderingButton->setHidden(true);


    // Reset the contents of both canvas widgets (make a new 500x500 image for the 2D one)
    fileNew();

    // Make certain radio buttons switch to the 2D canvas when clicked.
    QList<QRadioButton*> a;
    a += ui->brushTypeLinear;
    a += ui->brushTypeQuadratic;
    a += ui->brushTypeSmudge;
    a += ui->brushTypeSolid;
    a += ui->brushTypeSpecial1;
    a += ui->brushTypeSpecial2;
    a += ui->filterTypeBlur;
    a += ui->filterTypeEdgeDetect;
    a += ui->filterTypeGrayscale;
    a += ui->filterTypeInvert;
    a += ui->filterTypeRotate;
    a += ui->filterTypeScale;
    a += ui->filterTypeSpecial1;
    a += ui->filterTypeSpecial2;
    a += ui->filterTypeSpecial3;
    foreach (QRadioButton *rb, a)
        connect(rb, SIGNAL(clicked()), this, SLOT(activateCanvas2D()));

    a.clear();
    a += ui->shapeTypeCone;
    a += ui->shapeTypeCube;
    a += ui->shapeTypeCylinder;
    a += ui->shapeTypeSpecial1;
    a += ui->shapeTypeSpecial2;
    a += ui->shapeTypeSpecial3;
    foreach (QRadioButton *rb, a)
        connect(rb, SIGNAL(clicked()), this, SLOT(activateCanvas3D()));

}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings)
        delete b;
    delete ui;
}

void MainWindow::dataBind()
{
    // Brush dock

#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

    BIND( ChoiceBinding::bindRadioButtons(NUM_BRUSH_TYPES, settings.brushType,
                                    ui->brushTypeSolid,
                                    ui->brushTypeLinear,
                                    ui->brushTypeQuadratic,
                                    ui->brushTypeSmudge,
                                    ui->brushTypeSpecial1,
                                    ui->brushTypeSpecial2))

    BIND( IntBinding::bindSliderAndTextbox(ui->brushRadiusSlider, ui->brushRadiusTextbox, settings.brushRadius, 0, 96) )
    BIND( IntBinding::bindSliderAndTextbox(ui->brushColorSliderRed, ui->brushColorTextboxRed, settings.brushRed, 0, 255) )
    BIND( IntBinding::bindSliderAndTextbox(ui->brushColorSliderGreen, ui->brushColorTextboxGreen, settings.brushGreen, 0, 255) )
    BIND( IntBinding::bindSliderAndTextbox(ui->brushColorSliderBlue, ui->brushColorTextboxBlue, settings.brushBlue, 0, 255) )
    BIND( IntBinding::bindSliderAndTextbox(ui->brushColorSliderAlpha, ui->brushColorTextboxAlpha, settings.brushAlpha, 0, 255) )

    // Filter dock
    BIND( ChoiceBinding::bindRadioButtons(NUM_FILTER_TYPES, settings.filterType,
                                    ui->filterTypeInvert,
                                    ui->filterTypeGrayscale,
                                    ui->filterTypeEdgeDetect,
                                    ui->filterTypeBlur,
                                    ui->filterTypeScale,
                                    ui->filterTypeRotate,
                                    ui->filterTypeSpecial1,
                                    ui->filterTypeSpecial2,
                                    ui->filterTypeSpecial3) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->edgeDetectThresholdSlider, ui->edgeDetectThresholdTextbox, settings.edgeDetectThreshold, 0, 1) )
    BIND( IntBinding::bindSliderAndTextbox(ui->blurRadiusSlider, ui->blurRadiusTextbox, settings.blurRadius, 1, 200) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->scaleSliderX, ui->scaleTextboxX, settings.scaleX, 0.1, 10) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->scaleSliderY, ui->scaleTextboxY, settings.scaleY, 0.1, 10) )
    BIND( IntBinding::bindSliderAndTextbox(ui->rotateSlider, ui->rotateAngleEdit, settings.rotateAngle, -360, 360) )

    // Shapes dock
    BIND( BoolBinding::bindCheckbox(ui->showSceneviewInstead, settings.useSceneviewScene) )
    BIND( ChoiceBinding::bindRadioButtons(NUM_SHAPE_TYPES, settings.shapeType,
                                    ui->shapeTypeCube,
                                    ui->shapeTypeCone,
                                    ui->shapeTypeSphere,
                                    ui->shapeTypeCylinder,
                                    ui->shapeTypeSpecial1,
                                    ui->shapeTypeSpecial2,
                                    ui->shapeTypeSpecial3) )
    BIND( IntBinding::bindSliderAndTextbox(ui->shapeParameterSlider1, ui->shapeParameterTextbox1, settings.shapeParameter1, 1, 100) )
    BIND( IntBinding::bindSliderAndTextbox(ui->shapeParameterSlider2, ui->shapeParameterTextbox2, settings.shapeParameter2, 1, 100) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->shapeParameterSlider3, ui->shapeParameterTextbox3, settings.shapeParameter3, 1, 100) )
    BIND( BoolBinding::bindCheckbox(ui->useLightingCheckbox, settings.useLighting) )
    BIND( BoolBinding::bindCheckbox(ui->drawWireframeCheckbox, settings.drawWireframe) )
    BIND( BoolBinding::bindCheckbox(ui->drawNormalsCheckbox, settings.drawNormals) )

    // Camtrans dock
    BIND( BoolBinding::bindCheckbox(ui->cameraOrbitCheckbox, settings.useOrbitCamera) )
    BIND( FloatBinding::bindDial(ui->transX, settings.cameraPosX, -2, 2, true) )
    BIND( FloatBinding::bindDial(ui->transY, settings.cameraPosY, -2, 2, true) )
    BIND( FloatBinding::bindDial(ui->transZ, settings.cameraPosZ, -2, 2, true) )
    BIND( FloatBinding::bindDial(ui->rotU,   settings.cameraRotU, -20, 20, true) )
    BIND( FloatBinding::bindDial(ui->rotV,   settings.cameraRotV, -20, 20, true) )
    BIND( FloatBinding::bindDial(ui->rotW,   settings.cameraRotN, -180, 180, false) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->cameraFovSlider, ui->cameraFovTextbox, settings.cameraFov, 1, 179) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->cameraNearSlider, ui->cameraNearTextbox, settings.cameraNear, 0.1, 50) )
    BIND( FloatBinding::bindSliderAndTextbox(ui->cameraFarSlider, ui->cameraFarTextbox, settings.cameraFar, 0.1, 50) )

    // Ray dock
    BIND( BoolBinding::bindCheckbox(ui->raySuperSamping,          settings.useSuperSampling) )
    BIND( IntBinding::bindTextbox( ui->raySuperSamplesTextbox,   settings.numSuperSamples) )
    BIND( BoolBinding::bindCheckbox(ui->rayAntiAliasing,          settings.useAntiAliasing) )
    BIND( BoolBinding::bindCheckbox(ui->rayShadows,               settings.useShadows) )
    BIND( BoolBinding::bindCheckbox(ui->rayTextureMapping,        settings.useTextureMapping) )
    BIND( BoolBinding::bindCheckbox(ui->rayReflection,            settings.useReflection) )
    BIND( BoolBinding::bindCheckbox(ui->rayRefraction,            settings.useRefraction) )
    BIND( BoolBinding::bindCheckbox(ui->rayPointLights,           settings.usePointLights) )
    BIND( BoolBinding::bindCheckbox(ui->rayDirectionalLights,     settings.useDirectionalLights) )
    BIND( BoolBinding::bindCheckbox(ui->raySpotLights,            settings.useSpotLights) )
    BIND( BoolBinding::bindCheckbox(ui->rayMultiThreading,        settings.useMultiThreading) )

    BIND( ChoiceBinding::bindTabs(ui->tabWidget, settings.currentTab) )

#undef BIND

    // make sure the aspect ratio updates when ui->canvas3D changes size
    connect(ui->canvas3D, SIGNAL(aspectRatioChanged()), this, SLOT(updateAspectRatio()));
}


void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e); // allow the superclass to handle this for the most part...

    switch (e->type()) {

    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;

    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("CS123");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    // Stop any raytracing, otherwise the thread will hang around until done
    ui->canvas2D->cancelRender();

    QMainWindow::closeEvent(event);
}

void MainWindow::updateAspectRatio()
{
    // Update the aspect ratio text so the aspect ratio will be correct even if the
    // 3D canvas isn't visible (the 3D canvas isn't resized when it isn't visible)
    QSize activeTabSize = ui->tabWidget->currentWidget()->size();
    float aspectRatio = (float)activeTabSize.width() / (float)activeTabSize.height();
    ui->cameraAspectRatio->setText(QString("Aspect ratio: %1").arg(aspectRatio));
}


void MainWindow::settingsChanged()
{
    ui->canvas2D->settingsChanged();
    ui->canvas3D->settingsChanged();
}

void MainWindow::setAllRayFeatures(bool checked)
{
    ui->raySuperSamping->setChecked(checked);
    ui->rayAntiAliasing->setChecked(checked);
    ui->rayShadows->setChecked(checked);
    ui->rayTextureMapping->setChecked(checked);
    ui->rayReflection->setChecked(checked);
    ui->rayRefraction->setChecked(checked);
    ui->rayMultiThreading->setChecked(checked);

    // We don't want to affect the light checkboxes...
    //ui->rayPointLights->setChecked(checked);
    //ui->rayDirectionalLights->setChecked(checked);
    //ui->raySpotLights->setChecked(checked);
}

void MainWindow::fileCopy3Dto2D()
{
    // Make sure OpenGL gets a chance to redraw
    ui->tabWidget->setCurrentIndex(TAB_3D);
    ui->canvas3D->update();
    QApplication::processEvents();

    // Resize the 2D canvas to the size of the 3D canvas and copy the pixels over
    ui->canvas2D->resize(ui->canvas3D->width(), ui->canvas3D->height());
    ui->canvas3D->copyPixels(ui->canvas2D->width(), ui->canvas2D->height(), ui->canvas2D->data());
    ui->tabWidget->setCurrentIndex(TAB_2D);
}

void MainWindow::fileNew()
{
    ui->canvas2D->newImage();
}

void MainWindow::fileOpen()
{
    QString file = QFileDialog::getOpenFileName(this, QString(), "/course/cs123/data/");
    if (!file.isNull())
    {
        if (file.endsWith(".xml"))
        {
            CS123XmlSceneParser parser(file.toAscii().data());
            if (parser.parse())
            {
                SceneviewScene *scene = new SceneviewScene;
                Scene::parse(scene, &parser);
                ui->canvas3D->setScene(scene); /* ownership transfer of scene pointer - the
                                                  old scene is deleted automatically */
                ui->showSceneviewInstead->setChecked(true);

                // Set the camera for the new scene
                CS123SceneCameraData camera;
                if (parser.getCameraData(camera))
                {
                    camera.pos.data[3] = 1;
                    camera.look.data[3] = 0;
                    camera.up.data[3] = 0;

                    CamtransCamera *cam = ui->canvas3D->getCamtransCamera();
                    cam->orientLook(camera.pos, camera.look, camera.up);
                    cam->setHeightAngle(camera.heightAngle);
                }

                if (settings.useOrbitCamera) {
                    ui->cameraOrbitCheckbox->setChecked(false);
                }

                activateCanvas3D();
            }
            else
            {
                QMessageBox::critical(this, "Error", "Could not load scene \"" + file + "\"");
            }
        }
        else
        {
            if (!ui->canvas2D->loadImage(file))
            {
                QMessageBox::critical(this, "Error", "Could not load image \"" + file + "\"");
            }
            else
            {
                activateCanvas2D();
            }
        }
    }
}

void MainWindow::fileSave()
{
    if (settings.currentTab == TAB_2D)
        ui->canvas2D->saveImage();
}

void MainWindow::checkAllRayFeatures()
{
    setAllRayFeatures(true);
}

void MainWindow::uncheckAllRayFeatures()
{
    setAllRayFeatures(false);
}

void MainWindow::filterImage()
{
    // Disable the UI so the user can't interfere with the filtering
    setAllEnabled(false);

    // Actually do the filter.
    ui->canvas2D->filterImage();

    // Enable the UI again
    setAllEnabled(true);
}

void MainWindow::renderImage()
{
    // Make sure OpenGL gets a chance to update the OrbitCamera, which can only be done when
    // that tab is active (because it needs the OpenGL context for its matrix transforms)
    ui->tabWidget->setCurrentIndex(TAB_3D);
    ui->canvas3D->update();
    QApplication::processEvents();

    ui->tabWidget->setCurrentIndex(TAB_2D);

    OpenGLScene *glScene = ui->canvas3D->getScene();
    if (glScene != NULL)
    {
        // TODO: Set up RayScene from glScene and call ui->canvas2D->setScene()

        // Disable the UI so the user can't interfere with the raytracing
        setAllEnabled(false);

        // Swap the "render" button for the "stop rendering" button
        ui->rayRenderButton->setHidden(true);
        ui->rayStopRenderingButton->setHidden(false);

        // Render the image
        QSize activeTabSize = ui->tabWidget->currentWidget()->size();
        ui->canvas2D->renderImage(ui->canvas3D->getCamera(), activeTabSize.width(), activeTabSize.height());

        // Swap the "stop rendering" button for the "render" button
        ui->rayRenderButton->setHidden(false);
        ui->rayStopRenderingButton->setHidden(true);

        // Enable the UI again
        setAllEnabled(true);
    }
}

void MainWindow::setAllEnabled(bool enabled)
{
    QList<QWidget *> widgets;
    widgets += ui->brushDock;
    widgets += ui->filterDock;
    widgets += ui->shapesDock;
    widgets += ui->camtransDock;
    widgets += ui->rayAllOrNone;
    widgets += ui->rayFeatures;
    widgets += ui->rayLighting;
    widgets += ui->rayRenderButton;

    QList<QAction *> actions;
    actions += ui->actionNew;
    actions += ui->actionOpen;
    actions += ui->actionSave;
    actions += ui->actionRevert;
    actions += ui->actionQuit;

    foreach (QWidget *widget, widgets)
        widget->setEnabled(enabled);
    foreach (QAction *action, actions)
        action->setEnabled(enabled);
}

void MainWindow::activateCanvas2D() {
    ui->tabWidget->setCurrentWidget(ui->tab2D);
}

void MainWindow::activateCanvas3D() {
    ui->tabWidget->setCurrentWidget(ui->tab3D);
}
